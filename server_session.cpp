#include "server_session.h"

namespace anole {\

server_session_t::server_session_t(
    const slothjson::config_t& config,
    boost::asio::io_context& io_context,
    boost::asio::ssl::context& ssl_context,
    const std::string& plain_http_response) :
session_t(config, io_context),
in_socket_(io_context, ssl_context)
{
}

void server_session_t::start()
{
}

boost::asio::ip::tcp::socket& server_session_t::accept_socket()
{
    return (boost::asio::ip::tcp::socket&) in_socket_.next_layer();
}


} // anole

