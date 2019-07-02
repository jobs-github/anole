#ifndef __session_20190702160348_h__
#define __session_20190702160348_h__

#include <memory>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "config.h"

namespace anole { \

class session_t : public std::enable_shared_from_this<session_t>
{
public:
    session_t(const slothjson::config_t& config, boost::asio::io_context& io_context);
    virtual ~session_t();

    virtual void start() = 0;
    virtual boost::asio::ip::tcp::socket& accept_socket() = 0;
};

} // anole

#endif // __session_20190702160348_h__
