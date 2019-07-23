#ifndef __anole_proto_20190708192825_h__
#define __anole_proto_20190708192825_h__

#include <string>
#include "utils.h"

// https://www.ietf.org/rfc/rfc1928.txt

namespace anole {\

static const uint8_t PROTO_VER             = 0x05;

static const uint8_t METHOD_NO_AUTH        = 0x00;
static const uint8_t METHOD_NO_ACCEPTABLE  = 0xff;

static const uint8_t CMD_CONNECT           = 0x01;
static const uint8_t CMD_UDP_ASSOCIATE     = 0x03;

static const uint8_t ATYP_IPV4             = 0x01;
static const uint8_t ATYP_DOMAIN           = 0x03;
static const uint8_t ATYP_IPV6             = 0x04;

static const uint8_t REP_SUCCEEDED         = 0x00;
static const uint8_t REP_CMD_NOT_SUPPORTED = 0x07;

c_str_t& no_acceptable_methods();
c_str_t& no_authentication_required();
c_str_t& command_not_supported();
c_str_t& succeeded();

struct sock5_address_t
{
    enum address_type_e
    {
        IPv4 = ATYP_IPV4,
        DOMAIN_NAME = ATYP_DOMAIN,
        IPv6 = ATYP_IPV6,
    } address_type;
    std::string address;
    uint16_t port;
    int decode(const std::string& data);
private:
    bool is_valid(char address_type);
    int decode_ipv4(const std::string& data);
    int decode_domain(const std::string& data);
    int decode_ipv6(const std::string& data);
    uint16_t decode_port(const std::string& data, int offset);
};

struct request_t
{
    std::string password;
    enum command_e
    {
        CONNECT = CMD_CONNECT,
        UDP_ASSOCIATE = CMD_UDP_ASSOCIATE
    } command;
    sock5_address_t address;
    std::string payload;
    bool decode(const std::string& data);

};

} // anole

#endif // __anole_proto_20190708192825_h__
