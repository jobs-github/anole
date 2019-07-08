#include "session.h"

namespace anole {\

session_data_t::session_data_t(const slothjson::config_t& _config, boost::asio::io_context& _io_context):
config(_config),
recv_len(0),
start_time(0),
resolver(_io_context)
{
}

} // anole
