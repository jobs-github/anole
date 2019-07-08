#include "server_session.h"
#include "utils.h"

namespace anole {\

server_session_t::server_session_t(const slothjson::config_t& config, boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context):
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
            printf("%s:%s SSL handshake failed: %s", sess_.in_endpoint.address().to_string().c_str(), anole::to_string(sess_.in_endpoint.port()).c_str(), err.message().c_str());
        }
    });
}

boost::asio::ip::tcp::socket& server_session_t::accept_socket()
{
    return (boost::asio::ip::tcp::socket&) in_socket_.next_layer();
}

void server_session_t::destory()
{

}


} // anole

