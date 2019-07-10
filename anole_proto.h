#ifndef __anole_proto_20190708192825_h__
#define __anole_proto_20190708192825_h__

#include <string>

namespace anole {\

static const uint8_t CMD_CONNECT       = 0xef;

static const uint8_t SOCK5_ADDR_IPV4   = 0xfd;
static const uint8_t SOCK5_ADDR_DOMAIN = 0xfe;
static const uint8_t SOCK5_ADDR_IPV6   = 0xff;

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
