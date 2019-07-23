#include "server_session.h"
#include <chrono>
#include "utils.h"
#include "anole_proto.h"

namespace anole {\

server_session_t::server_session_t(const slothjson::config_t& config, boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context):
status_(HANDSHAKE),
sess_(config, io_context),
in_socket_(io_context, ssl_context),
out_socket_(io_context)
{
}

void server_session_t::start()
{
    boost::system::error_code ec;
    sess_.start_time = time(NULL);
    sess_.in_endpoint = in_socket_.next_layer().remote_endpoint(ec);
    if (ec)
    {
        destory();
        return;
    }
    auto self = shared_from_this();
    in_socket_.async_handshake(boost::asio::ssl::stream_base::server, [this, self](const boost::system::error_code err){
        if (err)
        {
            zlog_error(anole::cat(), "%s:%s SSL handshake failed: %s", SESS_ADDR, SESS_PORT, err.message().c_str());
            destory();
            return;
        }
        in_async_read();
    });
}

boost::asio::ip::tcp::socket& server_session_t::accept_socket()
{
    return (boost::asio::ip::tcp::socket&) in_socket_.next_layer();
}

void server_session_t::destory()
{
    if (DESTORY == status_)
    {
        return;
    }
    status_ = DESTORY;
    zlog_debug(anole::cat(), "%s:%s disconnected, %d bytes received, %d bytes sent, lasted for %s sec", SESS_ADDR, SESS_PORT, sess_.recv_len, sess_.sent_len, time(NULL) - sess_.start_time);
    boost::system::error_code err;
    sess_.resolver.cancel();
    if (out_socket_.is_open())
    {
        out_socket_.cancel(err);
        out_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
        out_socket_.close(err);
    }
    if (in_socket_.next_layer().is_open())
    {
        auto self = shared_from_this();
        auto ssl_shutdown_cb = [this, self](const boost::system::error_code code){
            if (boost::asio::error::operation_aborted == code)
            {
                boost::system::error_code err;
                sess_.ssl_shutdown_timer.cancel();
                in_socket_.next_layer().cancel(err);
                in_socket_.next_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
                in_socket_.next_layer().close(err);
            }
        };
        in_socket_.next_layer().cancel(err);
        in_socket_.async_shutdown(ssl_shutdown_cb);
        sess_.ssl_shutdown_timer.expires_after(std::chrono::seconds(SSL_SHUTDOWN_TIMEOUT));
        sess_.ssl_shutdown_timer.async_wait(ssl_shutdown_cb);
    }
}

void server_session_t::in_async_read()
{
    auto self = shared_from_this();
    in_socket_.async_read_some(boost::asio::buffer(sess_.in_read_buf, BUF_SIZE), [this, self](boost::system::error_code err, size_t len){
        if (err)
        {
            destory();
            return;
        }
        std::string buf((const char *)sess_.in_read_buf, len);
        if (HANDSHAKE == status_)
        {
            on_handshake(buf);
        }
        else if (FORWARD == status_)
        {
            sess_.sent_len += buf.size();
            out_async_write(buf);
        }
    });
}

void server_session_t::on_handshake(const std::string& buf)
{
    anole::request_t req;
    bool ok = req.decode(buf);
    // check password
    if (ok)
    {
        auto it = sess_.config.pwd.find(req.password);
        if (sess_.config.pwd.end() == it)
        {
            ok = false;
            zlog_warn(anole::cat(), "%s:%s valid anole request but incorrect password %s", SESS_ADDR, SESS_PORT, req.password.c_str());
        }
        else
        {
            zlog_error(anole::cat(), "%s:%s authenticated as %s", SESS_ADDR, SESS_PORT, it->second.c_str());
        }
    }
    // set query args
    std::string query_addr = ok ? req.address.address : sess_.config.remote_addr;
    std::string query_port = anole::to_string((ok ? req.address.port : sess_.config.remote_port));
    if (ok)
    {
        sess_.out_write_buf = req.payload;
        zlog_debug(anole::cat(), "%s:%s requested connection to %s:%s", SESS_ADDR, SESS_PORT, query_addr.c_str(), query_port.c_str());
    }
    else
    {
        sess_.out_write_buf = buf;
        zlog_warn(anole::cat(), "%s:%s not anole request, connecting to  %s:%s", SESS_ADDR, SESS_PORT, query_addr.c_str(), query_port.c_str());
    }
    sess_.sent_len = sess_.out_write_buf.size();

    // resolve remote
    auto self = shared_from_this();
    sess_.resolver.async_resolve(query_addr, query_port, [this, self, query_addr, query_port](const boost::system::error_code err, boost::asio::ip::tcp::resolver::results_type rc){
        on_resolve(query_addr, query_port, err, rc);
    });
}

void server_session_t::on_resolve(const std::string& query_addr, const std::string& query_port, const boost::system::error_code err, boost::asio::ip::tcp::resolver::results_type rc)
{
    if (err || rc.size() < 1)
    {
        zlog_error(anole::cat(), "%s:%s cannot resolve remote server hostname  %s:%s", SESS_ADDR, SESS_PORT, query_addr.c_str(), query_port.c_str());
        destory();
        return;
    }
    auto iter = rc.begin();
    for (auto it = rc.begin(); it != rc.end(); ++it)
    {
        if (it->endpoint().address().is_v4())
        {
            iter = it;
            break;
        }
    }
    boost::system::error_code error;
    out_socket_.open(iter->endpoint().protocol(), error);
    if (error)
    {
        destory();
        return;
    }
    if (sess_.config.tcp.no_delay)
    {
        out_socket_.set_option(boost::asio::ip::tcp::no_delay(true));
    }
    if (sess_.config.tcp.keep_alive)
    {
        out_socket_.set_option(boost::asio::socket_base::keep_alive(true));
    }
    // connect remote
    auto self = shared_from_this();
    out_socket_.async_connect(*iter, [this, self, query_addr, query_port](boost::system::error_code err) {
        on_connect(query_addr, query_port, err);
    });
}

// out_async_read | in_async_read
void server_session_t::on_connect(const std::string& query_addr, const std::string& query_port, boost::system::error_code err)
{
    if (err)
    {
        zlog_error(anole::cat(), "%s:%s cannot establish connection to remote server %s:%s", SESS_ADDR, SESS_PORT, query_addr.c_str(), query_port.c_str());
        destory();
        return;
    }
    zlog_error(anole::cat(), "%s:%s tunnel established %s:%s", SESS_ADDR, SESS_PORT, query_addr.c_str(), query_port.c_str());
    status_ = FORWARD;
    // case out
    out_async_read();
    // case in
    if (sess_.out_write_buf != "")
    {
        out_async_write(sess_.out_write_buf);
    }
    else
    {
        in_async_read();
    }
}

void server_session_t::out_async_read()
{
    auto self = shared_from_this();
    out_socket_.async_read_some(boost::asio::buffer(sess_.out_read_buf, BUF_SIZE), [this, self](const boost::system::error_code err, size_t sz){
        if (err)
        {
            destory();
            return;
        }
        if (FORWARD == status_)
        {
            std::string buf((const char *)sess_.out_read_buf, sz);
            sess_.recv_len += buf.size();
            in_async_write(buf);
        }
    });
}

void server_session_t::out_async_write(const std::string& buf)
{
    auto self = shared_from_this();
    auto data = std::make_shared<std::string>(buf);
    boost::asio::async_write(out_socket_, boost::asio::buffer(*data), [this, self, data](const boost::system::error_code err, size_t sz){
        if (err)
        {
            destory();
            return;
        }
        if (FORWARD == status_)
        {
            // continue FORWARD
            in_async_read();
        }
    });
}

void server_session_t::in_async_write(const std::string& buf)
{
    auto self = shared_from_this();
    auto data = std::make_shared<std::string>(buf);
    boost::asio::async_write(in_socket_, boost::asio::buffer(*data), [this, self, data](const boost::system::error_code err, size_t sz){
        if (err)
        {
            destory();
            return;
        }
        if (FORWARD == status_)
        {
            // continue FORWARD
            out_async_read();
        }
    });
}

} // anole

