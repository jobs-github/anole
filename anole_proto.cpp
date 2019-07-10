#include "anole_proto.h"
#include "utils.h"

namespace anole {\

static c_str_t CRLF = make_str("\r\n");

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
    else if (DOMAIN == address_type)
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
    return IPv4 == address_type || DOMAIN == address_type || IPv6 == address_type;
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
    return (uint8_t(data[offset]) << 8) | uint8_t(data[offset + 1]);
}

//---------|----|-------|------------|-------|----|----|--------
// n1      | 2  | 1     |     1      | n2    | 2  | 2  | n3
//---------|----|-------|------------|-------|----|----|--------
// password|CRLF|command|address_type|address|port|CRLF|payload
//---------|----|-------|------------|-------|----|----|--------
int request_t::decode(const std::string& data)
{
    size_t pos = data.find(CRLF.data);
    if (std::string::npos == pos)
    {
        return -1;
    }
    password = data.substr(0, pos);
    payload = data.substr(pos + CRLF.len);
    if (payload.size() < 1 || (payload[0] != CONNECT))
    {
        return -1;
    }
    command = static_cast<command_e>(payload[0]);
    int address_len = address.decode(data.substr(1));
    if (-1 == address_len || payload.size() < size_t(address_len) + 3 || payload.substr(address_len + 1, 2) != CRLF.data)
    {
        return -1;
    }
    payload = payload.substr(address_len + 3);
    return data.size();
}

} // anole
