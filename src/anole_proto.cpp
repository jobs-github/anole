#include "anole_proto.h"

namespace anole {\

#define CRLF  "\r\n"

uint16_t decode_uint16(const std::string& data, int offset)
{
    return (uint8_t(data[offset]) << 8) | uint8_t(data[offset + 1]);
}

c_str_t& no_acceptable_methods()
{
    static const uint8_t buf[] = { PROTO_VER, METHOD_NO_ACCEPTABLE };
    static const size_t len = sizeof(buf) / sizeof(uint8_t);
    static c_str_t resp = { (char *)buf, len };
    return resp;
}

c_str_t& no_authentication_required()
{
    static const uint8_t buf[] = { PROTO_VER, METHOD_NO_AUTH };
    static const size_t len = sizeof(buf) / sizeof(uint8_t);
    static c_str_t resp = { (char *)buf, len };
    return resp;
}

c_str_t& command_not_supported()
{
    static const uint8_t buf[] = { PROTO_VER, REP_CMD_NOT_SUPPORTED, 0, ATYP_IPV4, 0, 0, 0, 0, 0, 0 };
    static const size_t len = sizeof(buf) / sizeof(uint8_t);
    static c_str_t resp = { (char *)buf, len };
    return resp;
}

c_str_t& succeeded()
{
    static const uint8_t buf[] = { PROTO_VER, REP_SUCCEEDED, 0, ATYP_IPV4, 0, 0, 0, 0, 0, 0 };
    static const size_t len = sizeof(buf) / sizeof(uint8_t);
    static c_str_t resp = { (char *)buf, len };
    return resp;
}

c_str_t& make_succeeded()
{
    static const uint8_t buf[] = { PROTO_VER, REP_SUCCEEDED, 0 };
    static const size_t len = sizeof(buf) / sizeof(uint8_t);
    static c_str_t resp = { (char *)buf, len };
    return resp;
}

c_str_t& zero_sock5_address()
{
    static const uint8_t buf[] = { ATYP_IPV4, 0, 0, 0, 0, 0, 0 };
    static const size_t len = sizeof(buf) / sizeof(uint8_t);
    static c_str_t resp = { (char *)buf, len };
    return resp;
}

std::string sock5_address_t::encode(const boost::asio::ip::udp::endpoint& endpoint)
{
    if (endpoint.address().is_unspecified())
    {
        auto addr = zero_sock5_address();
        return std::string(addr.data, addr.len);
    }
    std::string rc;
    if (endpoint.address().is_v4())
    {
        rc += ATYP_IPV4;
        auto ip = endpoint.address().to_v4().to_bytes();
        for (int i = 0 ; i < 4; i++)
        {
            rc += (char)ip[i];
        }
    }
    else if (endpoint.address().is_v6())
    {
        rc += ATYP_IPV6;
        auto ip = endpoint.address().to_v6().to_bytes();
        for (int i = 0 ; i < 16; i++)
        {
            rc += (char)ip[i];
        }
    }
    rc += (char)((uint8_t)(endpoint.port() >> 8));
    rc += (char)((uint8_t)(endpoint.port() & 0xFF));
    return rc;
}

int sock5_address_t::decode(const std::string& data)
{
    if (data.size() < 1 || !is_valid(data[0]))
    {
        return -1;
    }
    address_type = static_cast<address_type_e>(data[0]);
    if (IPv4 == address_type)
    {
        return decode_ipv4(data);
    }
    else if (DOMAIN_NAME == address_type)
    {
        return decode_domain(data);
    }
    else if (IPv6 == address_type)
    {
        return decode_ipv6(data);
    }
    return -1;
}

bool sock5_address_t::is_valid(char address_type)
{
    return IPv4 == address_type || DOMAIN_NAME == address_type || IPv6 == address_type;
}

int sock5_address_t::decode_ipv4(const std::string& data)
{
    if (data.size() >= 7)
    {
        char buf[16];
        sprintf(buf, "%d.%d.%d.%d", uint8_t(data[1]), uint8_t(data[2]), uint8_t(data[3]), uint8_t(data[4]));
        address = buf;
        port = decode_port(data, 5);
        return 7;
    }
    return -1;
}

int sock5_address_t::decode_domain(const std::string& data)
{
    size_t len = size_t(data[1] + 4);
    if (data.size() >= len)
    {
        address = data.substr(2, data[1]);
        port = decode_port(data, data[1] + 2);
        return len;
    }
    return -1;
}

int sock5_address_t::decode_ipv6(const std::string& data)
{
    if (data.size() >= 19)
    {
        char buf[40];
        sprintf(buf, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
            uint8_t(data[1]), uint8_t(data[2]), uint8_t(data[3]), uint8_t(data[4]),
            uint8_t(data[5]), uint8_t(data[6]), uint8_t(data[7]), uint8_t(data[8]),
            uint8_t(data[9]), uint8_t(data[10]), uint8_t(data[11]), uint8_t(data[12]),
            uint8_t(data[13]), uint8_t(data[14]), uint8_t(data[15]), uint8_t(data[16]));
        address = buf;
        port = decode_port(data, 17);
        return 19;
    }
    return -1;
}

uint16_t sock5_address_t::decode_port(const std::string& data, int offset)
{
    return decode_uint16(data, offset);
}

//---------|----|-------|------------|-------|----|----|--------
// n1      | 2  | 1     |     1      | n2    | 2  | 2  | n3
//---------|----|-------|------------|-------|----|----|--------
// password|CRLF|command|address_type|address|port|CRLF|payload
//---------|----|-------|------------|-------|----|----|--------
bool request_t::decode(const std::string& data)
{
    size_t pos = data.find(CRLF);
    if (std::string::npos == pos)
    {
        return false;
    }
    password = data.substr(0, pos);
    payload = data.substr(pos + 2);
    if (payload.size() < 1 || (payload[0] != CONNECT))
    {
        return false;
    }
    command = static_cast<command_e>(payload[0]);
    int address_len = address.decode(data.substr(1));
    if (-1 == address_len || payload.size() < size_t(address_len) + 3 || payload.substr(address_len + 1, 2) != CRLF)
    {
        return false;
    }
    payload = payload.substr(address_len + 3);
    return true;
}

int udp_packet_t::decode(const std::string& data)
{
    int addr_len = address.decode(data);
    if (addr_len < 0 || data.size() < (size_t)addr_len + 2)
    {
        return -1;
    }
    length = decode_uint16(data, addr_len);
    if (data.size() < (size_t)addr_len + 4 + length || data.substr(addr_len + 2, 2) != "\r\n")
    {
        return -1;
    }
    payload = data.substr(addr_len + 4, length);
    return addr_len + 4 + length;
}

} // anole
