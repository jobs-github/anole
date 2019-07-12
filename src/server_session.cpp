#include "server_session.h"
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
            zlog_error(anole::cat(), "%s:%s SSL handshake failed: %s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), err.message().c_str());
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
            zlog_warn(anole::cat(), "%s:%s valid anole request but incorrect password %s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), req.password.c_str());
        }
        else
        {
            zlog_error(anole::cat(), "%s:%s authenticated as %s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), it->second.c_str());
        }
    }
    // set query args
    std::string query_addr = ok ? req.address.address : sess_.config.remote_addr;
    std::string query_port = anole::to_string((ok ? req.address.port : sess_.config.remote_port));
    if (ok)
    {
        sess_.out_write_buf = req.payload;
        zlog_debug(anole::cat(), "%s:%s requested connection to %s:%s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), query_addr.c_str(), query_port.c_str());
    }
    else
    {
        sess_.out_write_buf = buf;
        zlog_warn(anole::cat(), "%s:%s not anole request, connecting to  %s:%s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), query_addr.c_str(), query_port.c_str());
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
        zlog_error(anole::cat(), "%s:%s cannot resolve remote server hostname  %s:%s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), query_addr.c_str(), query_port.c_str());
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

void server_session_t::on_connect(const std::string& query_addr, const std::string& query_port, boost::system::error_code err)
{
    if (err)
    {
        zlog_error(anole::cat(), "%s:%s cannot establish connection to remote server %s:%s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), query_addr.c_str(), query_port.c_str());
        destory();
        return;
    }
    zlog_error(anole::cat(), "%s:%s tunnel established %s:%s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), query_addr.c_str(), query_port.c_str());
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

} // anole

