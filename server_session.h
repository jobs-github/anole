#ifndef __server_session_20190702163126_h__
#define __server_session_20190702163126_h__

#include "session.h"
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
private:
    session_data_t sess_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> in_socket_;
};

} // anole

#endif // __server_session_20190702163126_h__
