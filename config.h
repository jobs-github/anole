////////////////////////////////////////////////////////////////////////////////
// NOTE : Generated by slothjson. It is NOT supposed to modify this file.
////////////////////////////////////////////////////////////////////////////////
#ifndef __config_20190701185832_h__
#define __config_20190701185832_h__

#include "slothjson.h"

namespace slothjson {

struct tcp_config_t
{
    bool no_delay;
    bool keep_alive;
    bool fast_open;
    int fast_open_qlen;

    void skip_no_delay() { __skip_no_delay = true; }
    void skip_keep_alive() { __skip_keep_alive = true; }
    void skip_fast_open() { __skip_fast_open = true; }
    void skip_fast_open_qlen() { __skip_fast_open_qlen = true; }

    bool json_has_no_delay() const { return __json_has_no_delay; }
    bool json_has_keep_alive() const { return __json_has_keep_alive; }
    bool json_has_fast_open() const { return __json_has_fast_open; }
    bool json_has_fast_open_qlen() const { return __json_has_fast_open_qlen; }

    tcp_config_t();
    tcp_config_t& operator=(const tcp_config_t& obj_val);
    bool operator==(const tcp_config_t& obj_val) const;
    bool encode(allocator_t& alloc, rapidjson::Value& json_val) const;
    bool decode(const rapidjson::Value& json_val);

private:
    bool __skip_no_delay;
    bool __skip_keep_alive;
    bool __skip_fast_open;
    bool __skip_fast_open_qlen;

    bool __json_has_no_delay;
    bool __json_has_keep_alive;
    bool __json_has_fast_open;
    bool __json_has_fast_open_qlen;
};

bool encode(const tcp_config_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
bool decode(const rapidjson::Value& json_val, tcp_config_t& obj_val);

struct ssl_config_t
{
    bool verify;
    bool verify_hostname;
    std::string cert;
    std::string key;
    std::string key_password;
    std::string cipher;
    bool prefer_server_cipher;
    std::string sni;
    std::string alpn;
    bool reuse_session;
    bool session_ticket;
    int session_timeout;
    std::string plain_http_response;
    std::string curves;
    std::string dhparam;

    void skip_verify() { __skip_verify = true; }
    void skip_verify_hostname() { __skip_verify_hostname = true; }
    void skip_cert() { __skip_cert = true; }
    void skip_key() { __skip_key = true; }
    void skip_key_password() { __skip_key_password = true; }
    void skip_cipher() { __skip_cipher = true; }
    void skip_prefer_server_cipher() { __skip_prefer_server_cipher = true; }
    void skip_sni() { __skip_sni = true; }
    void skip_alpn() { __skip_alpn = true; }
    void skip_reuse_session() { __skip_reuse_session = true; }
    void skip_session_ticket() { __skip_session_ticket = true; }
    void skip_session_timeout() { __skip_session_timeout = true; }
    void skip_plain_http_response() { __skip_plain_http_response = true; }
    void skip_curves() { __skip_curves = true; }
    void skip_dhparam() { __skip_dhparam = true; }

    bool json_has_verify() const { return __json_has_verify; }
    bool json_has_verify_hostname() const { return __json_has_verify_hostname; }
    bool json_has_cert() const { return __json_has_cert; }
    bool json_has_key() const { return __json_has_key; }
    bool json_has_key_password() const { return __json_has_key_password; }
    bool json_has_cipher() const { return __json_has_cipher; }
    bool json_has_prefer_server_cipher() const { return __json_has_prefer_server_cipher; }
    bool json_has_sni() const { return __json_has_sni; }
    bool json_has_alpn() const { return __json_has_alpn; }
    bool json_has_reuse_session() const { return __json_has_reuse_session; }
    bool json_has_session_ticket() const { return __json_has_session_ticket; }
    bool json_has_session_timeout() const { return __json_has_session_timeout; }
    bool json_has_plain_http_response() const { return __json_has_plain_http_response; }
    bool json_has_curves() const { return __json_has_curves; }
    bool json_has_dhparam() const { return __json_has_dhparam; }

    ssl_config_t();
    ssl_config_t& operator=(const ssl_config_t& obj_val);
    bool operator==(const ssl_config_t& obj_val) const;
    bool encode(allocator_t& alloc, rapidjson::Value& json_val) const;
    bool decode(const rapidjson::Value& json_val);

private:
    bool __skip_verify;
    bool __skip_verify_hostname;
    bool __skip_cert;
    bool __skip_key;
    bool __skip_key_password;
    bool __skip_cipher;
    bool __skip_prefer_server_cipher;
    bool __skip_sni;
    bool __skip_alpn;
    bool __skip_reuse_session;
    bool __skip_session_ticket;
    bool __skip_session_timeout;
    bool __skip_plain_http_response;
    bool __skip_curves;
    bool __skip_dhparam;

    bool __json_has_verify;
    bool __json_has_verify_hostname;
    bool __json_has_cert;
    bool __json_has_key;
    bool __json_has_key_password;
    bool __json_has_cipher;
    bool __json_has_prefer_server_cipher;
    bool __json_has_sni;
    bool __json_has_alpn;
    bool __json_has_reuse_session;
    bool __json_has_session_ticket;
    bool __json_has_session_timeout;
    bool __json_has_plain_http_response;
    bool __json_has_curves;
    bool __json_has_dhparam;
};

bool encode(const ssl_config_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
bool decode(const rapidjson::Value& json_val, ssl_config_t& obj_val);

struct config_t
{
    std::string local_addr;
    uint16_t local_port;
    std::string remote_addr;
    uint16_t remote_port;
    std::string run_type;
    int rt;
    ssl_config_t ssl;
    tcp_config_t tcp;

    void skip_local_addr() { __skip_local_addr = true; }
    void skip_local_port() { __skip_local_port = true; }
    void skip_remote_addr() { __skip_remote_addr = true; }
    void skip_remote_port() { __skip_remote_port = true; }
    void skip_run_type() { __skip_run_type = true; }
    void skip_rt() { __skip_rt = true; }
    void skip_ssl() { __skip_ssl = true; }
    void skip_tcp() { __skip_tcp = true; }

    bool json_has_local_addr() const { return __json_has_local_addr; }
    bool json_has_local_port() const { return __json_has_local_port; }
    bool json_has_remote_addr() const { return __json_has_remote_addr; }
    bool json_has_remote_port() const { return __json_has_remote_port; }
    bool json_has_run_type() const { return __json_has_run_type; }
    bool json_has_rt() const { return __json_has_rt; }
    bool json_has_ssl() const { return __json_has_ssl; }
    bool json_has_tcp() const { return __json_has_tcp; }

    config_t();
    config_t& operator=(const config_t& obj_val);
    bool operator==(const config_t& obj_val) const;
    bool encode(allocator_t& alloc, rapidjson::Value& json_val) const;
    bool decode(const rapidjson::Value& json_val);

private:
    bool __skip_local_addr;
    bool __skip_local_port;
    bool __skip_remote_addr;
    bool __skip_remote_port;
    bool __skip_run_type;
    bool __skip_rt;
    bool __skip_ssl;
    bool __skip_tcp;

    bool __json_has_local_addr;
    bool __json_has_local_port;
    bool __json_has_remote_addr;
    bool __json_has_remote_port;
    bool __json_has_run_type;
    bool __json_has_rt;
    bool __json_has_ssl;
    bool __json_has_tcp;
};

bool encode(const config_t& obj_val, allocator_t& alloc, rapidjson::Value& json_val);
bool decode(const rapidjson::Value& json_val, config_t& obj_val);

} // namespace slothjson

#endif // __config_20190701185832_h__