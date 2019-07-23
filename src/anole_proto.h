#ifndef __anole_proto_20190708192825_h__
#define __anole_proto_20190708192825_h__

#include <string>
#include "utils.h"

// https://www.ietf.org/rfc/rfc1928.txt

namespace anole {\

static const uint8_t PROTO_VER             = 0x05;

static const uint8_t METHOD_NO_AUTH        = 0x00;
static const uint8_t NO_ACCEPTABLE_METHODS = 0xff;

static const uint8_t CMD_CONNECT           = 0x01;
static const uint8_t CMD_UDP_ASSOCIATE     = 0x03;

static const uint8_t SOCK5_ADDR_IPV4       = 0x01;
static const uint8_t SOCK5_ADDR_DOMAIN     = 0x03;
static const uint8_t SOCK5_ADDR_IPV6       = 0x04;

c_str_t no_acceptable_methods();
c_str_t no_authentication_required();

struct sock5_address_t
{
    enum address_type_e
    {
        IPv4 = SOCK5_ADDR_IPV4,
        DOMAIN_NAME = SOCK5_ADDR_DOMAIN,
        IPv6 = SOCK5_ADDR_IPV6,
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
        CONNECT = CMD_CONNECT
    } command;
    sock5_address_t address;
    std::string payload;
    bool decode(const std::string& data);

};

} // anole

#endif // __anole_proto_20190708192825_h__
