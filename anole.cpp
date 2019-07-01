#include "anole.h"
#include "utils.h"
#include <fstream>
#include <openssl/opensslv.h>

anole_t::anole_t(slothjson::config_t& config):
config_(config),
socket_acceptor_(io_context_),
ssl_context_(boost::asio::ssl::context::sslv23)
{
    boost::asio::ip::tcp::resolver resolver(this->io_context_);
    boost::asio::ip::tcp::endpoint listen_endpoint = *resolver.resolve(config.local_addr, anole::to_string(config.local_port)).begin();
    socket_acceptor_.open(listen_endpoint.protocol());
    socket_acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    socket_acceptor_.bind(listen_endpoint);
    socket_acceptor_.listen();

    SSL_CTX * native_handle = ssl_context_.native_handle();
    ssl_context_.set_options(boost::asio::ssl::context::default_workarounds
        | boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::no_sslv3
        | boost::asio::ssl::context::single_dh_use);
    if (config.ssl.curves != "")
    {
        SSL_CTX_set1_curves_list(native_handle, config.ssl.curves.c_str());
    }
    if (anole::SERVER == config.rt)
    {
        if (!init_server(config, native_handle))
        {
            exit(1);
        }
    }
    else if (anole::CLIENT == config.rt)
    {
        init_client(config, native_handle);
    }
    if (config.ssl.cipher != "")
    {
        SSL_CTX_set_cipher_list(native_handle, config.ssl.cipher.c_str());
    }
    init_tcp(config);
}


bool anole_t::init_server(slothjson::config_t& config, SSL_CTX * native_handle)
{
    ssl_context_.use_certificate_chain_file(config.ssl.cert);
    ssl_context_.set_password_callback([this](size_t, boost::asio::ssl::context_base::password_purpose){
        return this->config_.ssl.key_password;
    });
    ssl_context_.use_private_key_file(config.ssl.key, boost::asio::ssl::context::pem);
    if (config.ssl.prefer_server_cipher)
    {
        SSL_CTX_set_options(native_handle, SSL_OP_CIPHER_SERVER_PREFERENCE);
    }
    if (config.ssl.alpn != "")
    {
        SSL_CTX_set_alpn_select_cb(native_handle, [](SSL*, const unsigned char **out, unsigned char *outlen, const unsigned char *in, unsigned int inlen, void *config) -> int {
            auto alpn = ((slothjson::config_t *)config)->ssl.alpn.c_str();
            auto alpn_len = ((slothjson::config_t *)config)->ssl.alpn.length();
            if (OPENSSL_NPN_NEGOTIATED != SSL_select_next_proto((unsigned char **)out, outlen, (const unsigned char *)alpn, alpn_len, in, inlen))
            {
                return SSL_TLSEXT_ERR_NOACK;
            }
            return SSL_TLSEXT_ERR_OK;
        }, &config);
    }
    if (config.ssl.reuse_session)
    {
        SSL_CTX_set_timeout(native_handle, config.ssl.session_timeout);
        if (!config.ssl.session_ticket)
        {
            SSL_CTX_set_options(native_handle, SSL_OP_NO_TICKET);
        }
    }
    else
    {
        SSL_CTX_set_session_cache_mode(native_handle, SSL_SESS_CACHE_OFF);
        SSL_CTX_set_options(native_handle, SSL_OP_NO_TICKET);
    }
    if (config.ssl.plain_http_response != "")
    {
        std::ifstream ifs(config.ssl.plain_http_response, std::ios::binary);
        if (!ifs.is_open())
        {
            return false;
        }
        this->plain_http_response_ = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    }
    if (config.ssl.dhparam != "")
    {
        ssl_context_.use_tmp_dh_file(config.ssl.dhparam);
    }
    else
    {
        ssl_context_.use_tmp_dh(boost::asio::const_buffer());
    }
    SSL_CTX_set_ecdh_auto(native_handle, 1);
    return true;
}

void anole_t::init_client(slothjson::config_t& config, SSL_CTX * native_handle)
{
    if (config.ssl.sni == "")
    {
        config.ssl.sni = config.remote_addr;
    }
    if (config.ssl.verify)
    {
        ssl_context_.set_verify_mode(SSL_VERIFY_PEER);
        if (config.ssl.cert == "")
        {
            ssl_context_.set_default_verify_paths();
        }
        else
        {
            ssl_context_.load_verify_file(config.ssl.cert);
        }
        if (config.ssl.verify_hostname)
        {
            ssl_context_.set_verify_callback(boost::asio::ssl::rfc2818_verification(config.ssl.sni));
        }
        X509_VERIFY_PARAM * param = X509_VERIFY_PARAM_new();
        X509_VERIFY_PARAM_set_flags(param, X509_V_FLAG_PARTIAL_CHAIN);
        SSL_CTX_set1_param(native_handle, param);
        X509_VERIFY_PARAM_free(param);
    }
    else
    {
        ssl_context_.set_verify_mode(SSL_VERIFY_NONE);
    }
    if (config.ssl.alpn != "")
    {
        SSL_CTX_set_alpn_protos(native_handle, (unsigned char*)config.ssl.alpn.c_str(), config.ssl.alpn.length());
    }
    if (config.ssl.reuse_session)
    {
        SSL_CTX_set_session_cache_mode(native_handle, SSL_SESS_CACHE_CLIENT);
        anole::ssl::set_callback(native_handle);
        if (!config.ssl.session_ticket)
        {
            SSL_CTX_set_options(native_handle, SSL_OP_NO_TICKET);
        }
    }
    else
    {
        SSL_CTX_set_options(native_handle, SSL_OP_NO_TICKET);
    }
}

void anole_t::init_tcp(slothjson::config_t& config)
{
    if (config.tcp.no_delay)
    {
        socket_acceptor_.set_option(boost::asio::ip::tcp::no_delay(true));
    }
    if (config.tcp.keep_alive)
    {
        socket_acceptor_.set_option(boost::asio::socket_base::keep_alive(true));
    }
    if (config.tcp.fast_open)
    {
        boost::system::error_code err;
        socket_acceptor_.set_option(boost::asio::detail::socket_option::integer<IPPROTO_TCP, TCP_FASTOPEN>(config.tcp.fast_open_qlen));
    }
}

void anole_t::run()
{

}

