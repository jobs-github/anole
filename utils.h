#ifndef __utils_20190625132440_h__
#define __utils_20190625132440_h__

#include "config.h"
#include <string>
#include <sstream>
#include <openssl/ssl.h>

namespace anole { \

template <typename T>
std::string to_string(const T& n)
{
    std::ostringstream ss ;
    ss << n ;
    return ss.str() ;
}

enum run_type_t
{
    SERVER,
    CLIENT
};

void init_config(slothjson::config_t& config);

namespace ssl { \

void set_callback(SSL_CTX * ctx);
SSL_SESSION * get_session();

} //

} // anole

#endif // __utils_20190625132440_h__
