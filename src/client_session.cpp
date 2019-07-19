#include "client_session.h"
#include "utils.h"

namespace anole {\

client_session_t::client_session_t(const slothjson::config_t& config,
    boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context):
sess_(config, io_context),
in_socket_(io_context),
out_socket_(io_context, ssl_context)
{
}

void client_session_t::start()
{
    boost::system::error_code err;
    sess_.start_time = time(NULL);
    sess_.in_endpoint = in_socket_.remote_endpoint(err);
    if (err)
    {
        destory();
        return;
    }
    auto ssl = out_socket_.native_handle();
    if (sess_.config.ssl.sni != "")
    {
        SSL_set_tlsext_host_name(ssl, sess_.config.ssl.sni.c_str());
    }
    if (sess_.config.ssl.reuse_session)
    {
        auto session = anole::ssl::get_session();
        if (session)
        {
            SSL_set_session(ssl, session);
        }
    }
    in_async_read();
}

boost::asio::ip::tcp::socket& client_session_t::accept_socket()
{
    return in_socket_;
}

void client_session_t::in_async_read()
{

}

void client_session_t::destory()
{

}

} // anole
