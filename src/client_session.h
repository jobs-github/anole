#ifndef __client_session_20190702163658_h__
#define __client_session_20190702163658_h__

#include <boost/asio/ssl.hpp>
#include "session.h"
#include "utils.h"

namespace anole {\

class client_session_t : public session_t
{
public:
    client_session_t(const slothjson::config_t& config, boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context);
    ~client_session_t() {}

    void start();
    boost::asio::ip::tcp::socket& accept_socket();
private:
    void in_async_read();
    void on_handshake(const std::string& buf);
    void on_request(const std::string& buf);
    void in_async_write(c_str_t& buf);
    void in_async_write(const std::string& buf);
    void establish_tunnel();
    void on_resolve(const boost::system::error_code err, boost::asio::ip::tcp::resolver::results_type rc);
    void on_connect(const boost::system::error_code err);
    void out_async_read();
    void out_async_write(const std::string& buf);
    void udp_async_read();
    void destory();
private:
    enum status_e
    {
        HANDSHAKE,
        REQUEST,
        CONNECT,
        FORWARD,
        UDP_FORWARD,
        INVALID,
        DESTORY
    } status_;
    bool is_udp_;
    bool first_packet_recv_;
    session_data_t sess_;
    boost::asio::ip::tcp::socket in_socket_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> out_socket_;
};

} // anole

#endif // __client_session_20190702163658_h__
