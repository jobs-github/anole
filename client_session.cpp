#include "client_session.h"

namespace anole {\

client_session_t::client_session_t(const slothjson::config_t& config,
    boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context):
session_t(config, io_context),
in_socket_(io_context)
{
}

void client_session_t::start()
{
}

boost::asio::ip::tcp::socket& client_session_t::accept_socket()
{
    return in_socket_;
}

} // anole
