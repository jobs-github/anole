#include "server_session.h"
#include "utils.h"
#include "anole_proto.h"

namespace anole {\

server_session_t::server_session_t(const slothjson::config_t& config, boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context):
status_(HANDSHAKE),
sess_(config, io_context),
in_socket_(io_context, ssl_context)
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
        in_recv(std::string((const char *)sess_.in_read_buf, len));
    });
}

void server_session_t::in_recv(const std::string& buf)
{
    if (HANDSHAKE == status_)
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
        auto self = shared_from_this();
        sess_.resolver.async_resolve(query_addr, query_port, [this, self, query_addr, query_port](const boost::system::error_code err, boost::asio::ip::tcp::resolver::results_type rc){
            if (err || rc.size() < 1)
            {
                zlog_error(anole::cat(), "%s:%s cannot resolve remote server hostname  %s:%s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), query_addr.c_str(), query_port.c_str());
                destory();
                return;
            }
            // boost::asio::ip::basic_resolver_entry<boost::asio::ip::tcp>
            // auto it = rc.begin();
        });
    }
}

} // anole

