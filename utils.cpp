#include "utils.h"
#include <list>

namespace anole { \

void init_config(slothjson::config_t& config)
{
    if ("server" == config.run_type)
    {
        config.rt = SERVER;
    }
    else
    {
        config.rt = SERVER;
    }
}

namespace ssl { \

static std::list<SSL_SESSION *> g_sessions;

int new_session(SSL * s, SSL_SESSION * sess)
{
    g_sessions.push_front(sess);
    return 0;
}

void remove_session(SSL_CTX * s, SSL_SESSION * sess)
{
    g_sessions.remove(sess);
}

void set_callback(SSL_CTX * ctx)
{
    SSL_CTX_sess_set_new_cb(ctx, new_session);
    SSL_CTX_sess_set_remove_cb(ctx, remove_session);
}

SSL_SESSION * get_session()
{
    if (g_sessions.size() < 1)
    {
        return NULL;
    }
    return g_sessions.front();
}

} // ssl

} // anole
