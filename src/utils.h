#ifndef __utils_20190625132440_h__
#define __utils_20190625132440_h__

#include "config.h"
#include <string>
#include <sstream>
#include <openssl/ssl.h>
#include <zlog.h>

namespace anole { \

#define make_str(s) { (char *) s, sizeof(s) - 1 }
#define MDC_KEY  "mdc"

struct c_str_t
{
    char * data;
    size_t len;
};

template <typename T>
std::string to_string(const T& n)
{
    std::ostringstream ss ;
    ss << n ;
    return ss.str() ;
}

void init_logger(const std::string& category_name);
zlog_category_t * cat();
void set_mdc(const char * mdc);

enum run_type_t
{
    SERVER,
    CLIENT
};

void init_config(slothjson::config_t& config);

namespace ssl { \

void set_callback(SSL_CTX * ctx);
SSL_SESSION * get_session();

c_str_t default_dhparam();

} //

} // anole

#endif // __utils_20190625132440_h__
