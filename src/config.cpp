////////////////////////////////////////////////////////////////////////////////
// NOTE : Generated by slothjson. It is NOT supposed to modify this file.
////////////////////////////////////////////////////////////////////////////////
#include "config.h"

namespace slothjson {

tcp_config_t::tcp_config_t()
{
    no_delay = true;
    __skip_no_delay = false;
    __json_has_no_delay = false;

    keep_alive = true;
    __skip_keep_alive = false;
    __json_has_keep_alive = false;

    reuse_port = false;
    __skip_reuse_port = false;
    __json_has_reuse_port = false;
}

tcp_config_t& tcp_config_t::operator=(const tcp_config_t& obj_val)
{
    this->no_delay = obj_val.no_delay;
    this->keep_alive = obj_val.keep_alive;
    this->reuse_port = obj_val.reuse_port;
    return *this;
}

bool tcp_config_t::operator==(const tcp_config_t& obj_val) const
{
    if (!(this->no_delay == obj_val.no_delay)) return false;
    if (!(this->keep_alive == obj_val.keep_alive)) return false;
    if (!(this->reuse_port == obj_val.reuse_port)) return false;
    return true;
}

bool tcp_config_t::encode(allocator_t& alloc, rapidjson::Value& json_val) const
{
    do
    {
        json_val.SetObject();
        if (!__skip_no_delay && !encode_field(no_delay, "no_delay", alloc, json_val)) break;
        if (!__skip_keep_alive && !encode_field(keep_alive, "keep_alive", alloc, json_val)) break;
        if (!__skip_reuse_port && !encode_field(reuse_port, "reuse_port", alloc, json_val)) break;

        return true;
    } while (0);

    return false;
}

bool tcp_config_t::decode(const rapidjson::Value& json_val)
{
    do
    {
        if (!decode_field(json_val, "no_delay", no_delay, __json_has_no_delay)) break;
        if (!decode_field(json_val, "keep_alive", keep_alive, __json_has_keep_alive)) break;
        if (!decode_field(json_val, "reuse_port", reuse_port, __json_has_reuse_port)) break;

        return true;
    } while (0);

    return false;
}

bool encode(const tcp_config_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
{
    return obj_val.encode(alloc, json_val);
}

bool decode(const rapidjson::Value& json_val, tcp_config_t& obj_val)
{
    return obj_val.decode(json_val);
}

ssl_config_t::ssl_config_t()
{
    verify = true;
    __skip_verify = false;
    __json_has_verify = false;

    verify_hostname = true;
    __skip_verify_hostname = false;
    __json_has_verify_hostname = false;

    __skip_cert = false;
    __json_has_cert = false;

    __skip_key = false;
    __json_has_key = false;

    __skip_key_password = false;
    __json_has_key_password = false;

    __skip_cipher = false;
    __json_has_cipher = false;

    __skip_cipher_tls13 = false;
    __json_has_cipher_tls13 = false;

    prefer_server_cipher = true;
    __skip_prefer_server_cipher = false;
    __json_has_prefer_server_cipher = false;

    __skip_sni = false;
    __json_has_sni = false;

    __skip_alpn = false;
    __json_has_alpn = false;

    __skip_alpn_port_override = false;
    __json_has_alpn_port_override = false;

    __skip_alpn_str = false;
    __json_has_alpn_str = false;

    reuse_session = true;
    __skip_reuse_session = false;
    __json_has_reuse_session = false;

    session_timeout = 600;
    __skip_session_timeout = false;
    __json_has_session_timeout = false;

    __skip_curves = false;
    __json_has_curves = false;
}

ssl_config_t& ssl_config_t::operator=(const ssl_config_t& obj_val)
{
    this->verify = obj_val.verify;
    this->verify_hostname = obj_val.verify_hostname;
    this->cert = obj_val.cert;
    this->key = obj_val.key;
    this->key_password = obj_val.key_password;
    this->cipher = obj_val.cipher;
    this->cipher_tls13 = obj_val.cipher_tls13;
    this->prefer_server_cipher = obj_val.prefer_server_cipher;
    this->sni = obj_val.sni;
    this->alpn = obj_val.alpn;
    this->alpn_port_override = obj_val.alpn_port_override;
    this->alpn_str = obj_val.alpn_str;
    this->reuse_session = obj_val.reuse_session;
    this->session_timeout = obj_val.session_timeout;
    this->curves = obj_val.curves;
    return *this;
}

bool ssl_config_t::operator==(const ssl_config_t& obj_val) const
{
    if (!(this->verify == obj_val.verify)) return false;
    if (!(this->verify_hostname == obj_val.verify_hostname)) return false;
    if (!(this->cert == obj_val.cert)) return false;
    if (!(this->key == obj_val.key)) return false;
    if (!(this->key_password == obj_val.key_password)) return false;
    if (!(this->cipher == obj_val.cipher)) return false;
    if (!(this->cipher_tls13 == obj_val.cipher_tls13)) return false;
    if (!(this->prefer_server_cipher == obj_val.prefer_server_cipher)) return false;
    if (!(this->sni == obj_val.sni)) return false;
    if (!(this->alpn == obj_val.alpn)) return false;
    if (!(this->alpn_port_override == obj_val.alpn_port_override)) return false;
    if (!(this->alpn_str == obj_val.alpn_str)) return false;
    if (!(this->reuse_session == obj_val.reuse_session)) return false;
    if (!(this->session_timeout == obj_val.session_timeout)) return false;
    if (!(this->curves == obj_val.curves)) return false;
    return true;
}

bool ssl_config_t::encode(allocator_t& alloc, rapidjson::Value& json_val) const
{
    do
    {
        json_val.SetObject();
        if (!__skip_verify && !encode_field(verify, "verify", alloc, json_val)) break;
        if (!__skip_verify_hostname && !encode_field(verify_hostname, "verify_hostname", alloc, json_val)) break;
        if (!__skip_cert && !encode_field(cert, "cert", alloc, json_val)) break;
        if (!__skip_key && !encode_field(key, "key", alloc, json_val)) break;
        if (!__skip_key_password && !encode_field(key_password, "key_password", alloc, json_val)) break;
        if (!__skip_cipher && !encode_field(cipher, "cipher", alloc, json_val)) break;
        if (!__skip_cipher_tls13 && !encode_field(cipher_tls13, "cipher_tls13", alloc, json_val)) break;
        if (!__skip_prefer_server_cipher && !encode_field(prefer_server_cipher, "prefer_server_cipher", alloc, json_val)) break;
        if (!__skip_sni && !encode_field(sni, "sni", alloc, json_val)) break;
        if (!__skip_alpn && !encode_field(alpn, "alpn", alloc, json_val)) break;
        if (!__skip_alpn_port_override && !encode_field(alpn_port_override, "alpn_port_override", alloc, json_val)) break;
        if (!__skip_alpn_str && !encode_field(alpn_str, "alpn_str", alloc, json_val)) break;
        if (!__skip_reuse_session && !encode_field(reuse_session, "reuse_session", alloc, json_val)) break;
        if (!__skip_session_timeout && !encode_field(session_timeout, "session_timeout", alloc, json_val)) break;
        if (!__skip_curves && !encode_field(curves, "curves", alloc, json_val)) break;

        return true;
    } while (0);

    return false;
}

bool ssl_config_t::decode(const rapidjson::Value& json_val)
{
    do
    {
        if (!decode_field(json_val, "verify", verify, __json_has_verify)) break;
        if (!decode_field(json_val, "verify_hostname", verify_hostname, __json_has_verify_hostname)) break;
        if (!decode_field(json_val, "cert", cert, __json_has_cert)) break;
        if (!decode_field(json_val, "key", key, __json_has_key)) break;
        if (!decode_field(json_val, "key_password", key_password, __json_has_key_password)) break;
        if (!decode_field(json_val, "cipher", cipher, __json_has_cipher)) break;
        if (!decode_field(json_val, "cipher_tls13", cipher_tls13, __json_has_cipher_tls13)) break;
        if (!decode_field(json_val, "prefer_server_cipher", prefer_server_cipher, __json_has_prefer_server_cipher)) break;
        if (!decode_field(json_val, "sni", sni, __json_has_sni)) break;
        if (!decode_field(json_val, "alpn", alpn, __json_has_alpn)) break;
        if (!decode_field(json_val, "alpn_port_override", alpn_port_override, __json_has_alpn_port_override)) break;
        if (!decode_field(json_val, "alpn_str", alpn_str, __json_has_alpn_str)) break;
        if (!decode_field(json_val, "reuse_session", reuse_session, __json_has_reuse_session)) break;
        if (!decode_field(json_val, "session_timeout", session_timeout, __json_has_session_timeout)) break;
        if (!decode_field(json_val, "curves", curves, __json_has_curves)) break;

        return true;
    } while (0);

    return false;
}

bool encode(const ssl_config_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
{
    return obj_val.encode(alloc, json_val);
}

bool decode(const rapidjson::Value& json_val, ssl_config_t& obj_val)
{
    return obj_val.decode(json_val);
}

zlog_t::zlog_t()
{
    __skip_conf = false;
    __json_has_conf = false;

    __skip_default_category = false;
    __json_has_default_category = false;
}

zlog_t& zlog_t::operator=(const zlog_t& obj_val)
{
    this->conf = obj_val.conf;
    this->default_category = obj_val.default_category;
    return *this;
}

bool zlog_t::operator==(const zlog_t& obj_val) const
{
    if (!(this->conf == obj_val.conf)) return false;
    if (!(this->default_category == obj_val.default_category)) return false;
    return true;
}

bool zlog_t::encode(allocator_t& alloc, rapidjson::Value& json_val) const
{
    do
    {
        json_val.SetObject();
        if (!__skip_conf && !encode_field(conf, "conf", alloc, json_val)) break;
        if (!__skip_default_category && !encode_field(default_category, "default_category", alloc, json_val)) break;

        return true;
    } while (0);

    return false;
}

bool zlog_t::decode(const rapidjson::Value& json_val)
{
    do
    {
        if (!decode_field(json_val, "conf", conf, __json_has_conf)) break;
        if (!decode_field(json_val, "default_category", default_category, __json_has_default_category)) break;

        return true;
    } while (0);

    return false;
}

bool encode(const zlog_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
{
    return obj_val.encode(alloc, json_val);
}

bool decode(const rapidjson::Value& json_val, zlog_t& obj_val)
{
    return obj_val.decode(json_val);
}

config_t::config_t()
{
    __skip_logger = false;
    __json_has_logger = false;

    __skip_local_addr = false;
    __json_has_local_addr = false;

    local_port = 0;
    __skip_local_port = false;
    __json_has_local_port = false;

    __skip_remote_addr = false;
    __json_has_remote_addr = false;

    remote_port = 0;
    __skip_remote_port = false;
    __json_has_remote_port = false;

    __skip_password = false;
    __json_has_password = false;

    __skip_pwd = false;
    __json_has_pwd = false;

    run_type = "client";
    __skip_run_type = false;
    __json_has_run_type = false;

    rt = 0;
    __skip_rt = false;
    __json_has_rt = false;

    __skip_ssl = false;
    __json_has_ssl = false;

    __skip_tcp = false;
    __json_has_tcp = false;
}

config_t& config_t::operator=(const config_t& obj_val)
{
    this->logger = obj_val.logger;
    this->local_addr = obj_val.local_addr;
    this->local_port = obj_val.local_port;
    this->remote_addr = obj_val.remote_addr;
    this->remote_port = obj_val.remote_port;
    this->password = obj_val.password;
    this->pwd = obj_val.pwd;
    this->run_type = obj_val.run_type;
    this->rt = obj_val.rt;
    this->ssl = obj_val.ssl;
    this->tcp = obj_val.tcp;
    return *this;
}

bool config_t::operator==(const config_t& obj_val) const
{
    if (!(this->logger == obj_val.logger)) return false;
    if (!(this->local_addr == obj_val.local_addr)) return false;
    if (!(this->local_port == obj_val.local_port)) return false;
    if (!(this->remote_addr == obj_val.remote_addr)) return false;
    if (!(this->remote_port == obj_val.remote_port)) return false;
    if (!(this->password == obj_val.password)) return false;
    if (!(this->pwd == obj_val.pwd)) return false;
    if (!(this->run_type == obj_val.run_type)) return false;
    if (!(this->rt == obj_val.rt)) return false;
    if (!(this->ssl == obj_val.ssl)) return false;
    if (!(this->tcp == obj_val.tcp)) return false;
    return true;
}

bool config_t::encode(allocator_t& alloc, rapidjson::Value& json_val) const
{
    do
    {
        json_val.SetObject();
        if (!__skip_logger && !encode_field(logger, "logger", alloc, json_val)) break;
        if (!__skip_local_addr && !encode_field(local_addr, "local_addr", alloc, json_val)) break;
        if (!__skip_local_port && !encode_field(local_port, "local_port", alloc, json_val)) break;
        if (!__skip_remote_addr && !encode_field(remote_addr, "remote_addr", alloc, json_val)) break;
        if (!__skip_remote_port && !encode_field(remote_port, "remote_port", alloc, json_val)) break;
        if (!__skip_password && !encode_field(password, "password", alloc, json_val)) break;
        if (!__skip_pwd && !encode_field(pwd, "pwd", alloc, json_val)) break;
        if (!__skip_run_type && !encode_field(run_type, "run_type", alloc, json_val)) break;
        if (!__skip_rt && !encode_field(rt, "rt", alloc, json_val)) break;
        if (!__skip_ssl && !encode_field(ssl, "ssl", alloc, json_val)) break;
        if (!__skip_tcp && !encode_field(tcp, "tcp", alloc, json_val)) break;

        return true;
    } while (0);

    return false;
}

bool config_t::decode(const rapidjson::Value& json_val)
{
    do
    {
        if (!decode_field(json_val, "logger", logger, __json_has_logger)) break;
        if (!decode_field(json_val, "local_addr", local_addr, __json_has_local_addr)) break;
        if (!decode_field(json_val, "local_port", local_port, __json_has_local_port)) break;
        if (!decode_field(json_val, "remote_addr", remote_addr, __json_has_remote_addr)) break;
        if (!decode_field(json_val, "remote_port", remote_port, __json_has_remote_port)) break;
        if (!decode_field(json_val, "password", password, __json_has_password)) break;
        if (!decode_field(json_val, "pwd", pwd, __json_has_pwd)) break;
        if (!decode_field(json_val, "run_type", run_type, __json_has_run_type)) break;
        if (!decode_field(json_val, "rt", rt, __json_has_rt)) break;
        if (!decode_field(json_val, "ssl", ssl, __json_has_ssl)) break;
        if (!decode_field(json_val, "tcp", tcp, __json_has_tcp)) break;

        return true;
    } while (0);

    return false;
}

bool encode(const config_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val)
{
    return obj_val.encode(alloc, json_val);
}

bool decode(const rapidjson::Value& json_val, config_t& obj_val)
{
    return obj_val.decode(json_val);
}

} // namespace slothjson