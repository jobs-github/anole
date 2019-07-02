#ifndef __anole_20190621154321_h__
#define __anole_20190621154321_h__

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>

#include "config.h"

class anole_t
{
public:
    anole_t(slothjson::config_t& config);
    void run();
private:
    bool init_server(slothjson::config_t& config, SSL_CTX * native_handle);
    void init_client(slothjson::config_t& config, SSL_CTX * native_handle);
    void init_tcp(slothjson::config_t& config);
    void async_accept();
private:
    const slothjson::config_t& config_;
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor socket_acceptor_;
    boost::asio::ssl::context ssl_context_;
    std::string plain_http_response_;
};

#endif // __anole_20190621154321_h__
