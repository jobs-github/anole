#include "utils.h"
#include <list>
#include <openssl/evp.h>

namespace anole { \

static zlog_category_t * g_default_category = NULL;

void init_logger(const std::string& category_name)
{
    g_default_category = zlog_get_category(category_name.c_str());
}

zlog_category_t * cat()
{
    return g_default_category;
}

void set_mdc(const char * mdc)
{
    zlog_put_mdc(MDC_KEY, mdc);
}

struct evp_md_ctx_t
{
    EVP_MD_CTX * ctx_;

    evp_md_ctx_t();
    ~evp_md_ctx_t();

    unsigned int encode(const std::string& plain, uint8_t *digest);
};

evp_md_ctx_t::evp_md_ctx_t() : ctx_(NULL) {}

evp_md_ctx_t::~evp_md_ctx_t()
{
    if (NULL == ctx_)
    {
        return;
    }
    EVP_MD_CTX_free(ctx_);
}

unsigned int evp_md_ctx_t::encode(const std::string& plain, uint8_t *digest)
{
    ctx_ = EVP_MD_CTX_new();
    if (!ctx_)
    {
        return -1;
    }
    if (!EVP_DigestInit_ex(ctx_, EVP_sha224(), NULL))
    {
        return -1;
    }
    if (!EVP_DigestUpdate(ctx_, plain.c_str(), plain.length()))
    {
        return -1;
    }
    unsigned int digest_len;
    if (!EVP_DigestFinal_ex(ctx_, digest, &digest_len))
    {
        return -1;
    }
    return digest_len;
}

std::string sha224(const std::string& plain)
{
    evp_md_ctx_t ctx;
    uint8_t digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = ctx.encode(plain, digest);
    if (digest_len < 0)
    {
        return "";
    }
    char cipher[(EVP_MAX_MD_SIZE << 1) + 1];
    for (unsigned int i = 0; i < digest_len; i++)
    {
        sprintf(cipher + (i << 1), "%02x", (unsigned int)digest[i]);
    }
    cipher[digest_len << 1] = '\0';
    return std::string(cipher);
}

void init_config(slothjson::config_t& config)
{
    if ("server" == config.run_type)
    {
        config.rt = SERVER;
    }
    else
    {
        config.rt = CLIENT;
    }

    for (auto& pwd: config.password)
    {
        config.pwd[sha224(pwd)] = pwd;
    }

    for (auto& proto: config.ssl.alpn)
    {
        config.ssl.alpn_str += (char)((unsigned char)(proto.size()));
        config.ssl.alpn_str += proto;
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

c_str_t default_dhparam()
{
    // openssl dhparam -out dh.pem 2048
    static const char buf[] =
    "-----BEGIN DH PARAMETERS-----\n"
    "MIIBCAKCAQEA6wgkm37+IzNkyYP60B+kZFNpQqDDmz+OAidLIqmMSjssuZ8Z4ECD\n"
    "E3mhUZRNLnATjrweInkwtg7eHS5Tq2xkqhKXRxYuQYuQ262d7g0kQ6Er5dwCPJkZ\n"
    "fCPxx8C/7h81o8mlSzVJ/+XOE+bzZcR0pfWVndD/PgNduFdEGkp8S+akSsY6/Kns\n"
    "YxOonK8r50+ut2KJHv3xsn1bf20UcW0iY0KBectbaFeEoxGib4ue3YPD/ZzOR4UU\n"
    "ixN0Ztd8JpvUQRzddkToEdoNliwR1zy0ZFOPHCT1jN81WWCNZsbVyX/8QYM8aCkN\n"
    "wtaorBhBDOt8iHnwXdZ6ivwp1ScO2mkDKwIBAg==\n"
    "-----END DH PARAMETERS-----";

    static const size_t len = sizeof(buf);

    static c_str_t dhparam = {(char *) buf, len};

    return dhparam;
}

} // ssl

} // anole
