#ifndef __client_session_20190702163658_h__
#define __client_session_20190702163658_h__

#include "session.h"
#include <boost/asio/ssl.hpp>

namespace anole {\

class client_session_t : public session_t
{
public:
    client_session_t(
        const slothjson::config_t& config,
        boost::asio::io_context& io_context,
        boost::asio::ssl::context& ssl_context);

    void start();
    boost::asio::ip::tcp::socket& accept_socket();
private:
    boost::asio::ip::tcp::socket in_socket_;
};

} // anole

#endif // __client_session_20190702163658_h__
