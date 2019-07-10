#ifndef __session_20190702160348_h__
#define __session_20190702160348_h__

#include <stdint.h>
#include <memory>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "config.h"

namespace anole { \

enum
{
    BUF_SIZE = 8192,
};

struct session_data_t
{
    const slothjson::config_t& config;
    uint8_t in_read_buf[BUF_SIZE];
    uint64_t recv_len;
    time_t start_time;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::endpoint in_endpoint;

    session_data_t(const slothjson::config_t& _config, boost::asio::io_context& _io_context);
};

class session_t : public std::enable_shared_from_this<session_t>
{
public:
    virtual ~session_t() {}

    virtual void start() = 0;
    virtual boost::asio::ip::tcp::socket& accept_socket() = 0;
};

} // anole

#endif // __session_20190702160348_h__
