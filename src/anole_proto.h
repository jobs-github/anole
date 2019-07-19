#ifndef __anole_proto_20190708192825_h__
#define __anole_proto_20190708192825_h__

#include <string>

// +----+-----+-------+------+----------+----------+
// |VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
// +----+-----+-------+------+----------+----------+
// | 1  |  1  | X'00' |  1   | Variable |    2     |
// +----+-----+-------+------+----------+----------+
// * VER protocol version: X'05'
// * CMD
//     * CONNECT X'01'
//     * BIND X'02'
//     * UDP ASSOCIATE X'03'
// * RSV RESERVED
// * ATYP address type of following address
//     * IP V4 address: X'01'
//     * DOMAINNAME: X'03'
//     * IP V6 address: X'04'
// * DST.ADDR desired destination address
// * DST.PORT desired destination port in network octet

namespace anole {\

static const uint8_t CMD_CONNECT       = 0x01;

static const uint8_t SOCK5_ADDR_IPV4   = 0x01;
static const uint8_t SOCK5_ADDR_DOMAIN = 0x03;
static const uint8_t SOCK5_ADDR_IPV6   = 0x04;

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
