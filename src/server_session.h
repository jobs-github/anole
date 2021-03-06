#ifndef __server_session_20190702163126_h__
#define __server_session_20190702163126_h__

#include "session.h"
#include "anole_proto.h"
#include <boost/asio/ssl.hpp>

namespace anole {\

class server_session_t : public session_t
{
public:
    server_session_t(const slothjson::config_t& config, boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context);
    ~server_session_t() {}

    void start();
    boost::asio::ip::tcp::socket& accept_socket();
private:
    void destory();

    void in_async_read();
    void out_async_read();

    void in_async_write(const std::string& buf);
    void out_async_write(const std::string& buf);

    void on_handshake(const std::string& buf);
    void on_resolve(const std::string& query_addr, uint16_t query_port, const boost::system::error_code err, const boost::asio::ip::tcp::resolver::results_type& rc);
    void on_connect(const std::string& query_addr, uint16_t query_port, boost::system::error_code err);

    void udp_sent();
    void udp_async_read();
    void udp_async_write(const std::string& buf, const boost::asio::ip::udp::endpoint& endpoint);
    uint16_t get_query_port(bool decode_ok, const anole::request_t& req);
private:
    enum status_e
    {
        HANDSHAKE,
        FORWARD,
        UDP_FORWARD,
        DESTORY
    } status_;
    session_data_t sess_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> in_socket_;
    boost::asio::ip::tcp::socket out_socket_;
    boost::asio::ip::udp::resolver udp_resolver_;
};

} // anole

#endif // __server_session_20190702163126_h__
