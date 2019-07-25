#include "client_session.h"
#include "anole_proto.h"

// +----+----------+----------+
// |VER | NMETHODS | METHODS  |
// +----+----------+----------+
// | 1  |    1     | 1 to 255 |
// +----+----------+----------+

// X'00' NO AUTHENTICATION REQUIRED
// X'01' GSSAPI
// X'02' USERNAME/PASSWORD
// X'03' to X'7F' IANA ASSIGNED
// X'80' to X'FE' RESERVED FOR PRIVATE METHODS
// X'FF' NO ACCEPTABLE METHODS

// request
// +----+-----+-------+------+----------+----------+
// |VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
// +----+-----+-------+------+----------+----------+
// | 1  |  1  | X'00' |  1   | Variable |    2     |
// +----+-----+-------+------+----------+----------+
// * CMD
//     * CONNECT X'01'
//     * BIND X'02'
//     * UDP ASSOCIATE X'03'
// reply
// +----+-----+-------+------+----------+----------+
// |VER | REP |  RSV  | ATYP | BND.ADDR | BND.PORT |
// +----+-----+-------+------+----------+----------+
// | 1  |  1  | X'00' |  1   | Variable |    2     |
// +----+-----+-------+------+----------+----------+
// * REP    Reply field:
//     * X'00' succeeded
//     * X'01' general SOCKS server failure
//     * X'02' connection not allowed by ruleset
//     * X'03' Network unreachable
//     * X'04' Host unreachable
//     * X'05' Connection refused
//     * X'06' TTL expired
//     * X'07' Command not supported
//     * X'08' Address type not supported
//     * X'09' to X'FF' unassigned

// UDP request
//
// +----+------+------+----------+----------+----------+
// |RSV | FRAG | ATYP | DST.ADDR | DST.PORT |   DATA   |
// +----+------+------+----------+----------+----------+
// | 2  |  1   |  1   | Variable |    2     | Variable |
// +----+------+------+----------+----------+----------+
// * RSV  Reserved X'0000'
// * FRAG    Current fragment number
// * ATYP    address type of following addresses:
//     * IP V4 address: X'01'
//     * DOMAINNAME: X'03'
//     * IP V6 address: X'04'
// * DST.ADDR       desired destination address
// * DST.PORT       desired destination port
// * DATA     user data

namespace anole {\

client_session_t::client_session_t(const slothjson::config_t& config,
    boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context):
status_(HANDSHAKE),
is_udp_(false),
first_packet_recv_(false),
sess_(config, io_context),
in_socket_(io_context),
out_socket_(io_context, ssl_context)
{
}

void client_session_t::start()
{
    boost::system::error_code err;
    sess_.start_time = time(NULL);
    sess_.in_endpoint = in_socket_.remote_endpoint(err);
    if (err)
    {
        destory();
        return;
    }
    auto ssl = out_socket_.native_handle();
    if (sess_.config.ssl.sni != "")
    {
        SSL_set_tlsext_host_name(ssl, sess_.config.ssl.sni.c_str());
    }
    if (sess_.config.ssl.reuse_session)
    {
        auto session = anole::ssl::get_session();
        if (session)
        {
            SSL_set_session(ssl, session);
        }
    }
    in_async_read();
}

boost::asio::ip::tcp::socket& client_session_t::accept_socket()
{
    return in_socket_;
}

// in_async_read <-> in_async_write
void client_session_t::in_async_read()
{
    auto self = shared_from_this();
    in_socket_.async_read_some(boost::asio::buffer(sess_.in_read_buf, BUF_SIZE), [this, self](const boost::system::error_code err, size_t sz){
        if (boost::asio::error::operation_aborted == err)
        {
            return;
        }
        if (err)
        {
            destory();
            return;
        }
        std::string buf((const char *)sess_.in_read_buf, sz);
        if (HANDSHAKE == status_)
        {
            on_handshake(buf);
        }
        else if (REQUEST == status_)
        {
            on_request(buf);
        }
        else if (CONNECT == status_)
        {
            sess_.sent_len += buf.size();
            first_packet_recv_ = true;
            sess_.out_write_buf.append(buf);
        }
        else if (FORWARD == status_)
        {
            sess_.sent_len += buf.size();
            out_async_write(buf);
        }
        else if (UDP_FORWARD == status_)
        {
            // TODO
        }
    });
}

void client_session_t::on_handshake(const std::string& buf)
{
    if (buf.size() < 2 || buf[0] != PROTO_VER || buf.size() != (size_t)(buf[1] + 2))
    {
        zlog_error(anole::cat(), "%s:%d unknown protocol", SESS_ADDR, SESS_PORT);
        destory();
        return;
    }
    bool acceptable = false;
    for (int i = 2; i < buf[1] + 2; i++)
    {
        if (METHOD_NO_AUTH == buf[i])
        {
            acceptable = true;
            break;
        }
    }
    if (!acceptable)
    {
        zlog_error(anole::cat(), "%s:%d method not support", SESS_ADDR, SESS_PORT);
        in_async_write(anole::no_acceptable_methods());
        status_ = INVALID;
        return;
    }

    in_async_write(anole::no_authentication_required());
}


void client_session_t::on_request(const std::string& buf)
{
    if (buf.size() < 7 || buf[0] != PROTO_VER || buf[2] != 0)
    {
        zlog_error(anole::cat(), "%s:%d bad request", SESS_ADDR, SESS_PORT);
        destory();
        return;
    }

    sess_.out_write_buf = sess_.config.pwd.cbegin()->first + "\r\n" + buf[1] + buf.substr(3) + "\r\n";
    request_t req;
    if (!req.decode(sess_.out_write_buf))
    {
        zlog_error(anole::cat(), "%s:%d command not supported", SESS_ADDR, SESS_PORT);
        in_async_write(anole::command_not_supported());
        status_ = INVALID;
        return;
    }
    is_udp_ = (req.command == request_t::UDP_ASSOCIATE);
    if (is_udp_)
    {
        boost::asio::ip::udp::endpoint bindpoint(in_socket_.local_endpoint().address(), 0);
        boost::system::error_code err;
        sess_.udp_socket.open(bindpoint.protocol(), err);
        if (err)
        {
            destory();
            return;
        }
        sess_.udp_socket.bind(bindpoint);
        zlog_debug(anole::cat(), "%s:%d requested udp associate to %s:%d, open udp socket %s:%d", SESS_ADDR, SESS_PORT, req.address.address.c_str(), req.address.port, sess_.udp_socket.local_endpoint().address().to_string().c_str(), sess_.udp_socket.local_endpoint().port());

        auto succ = anole::make_succeeded();
        std::string resp(succ.data, succ.len);
        resp.append(sock5_address_t::encode(sess_.udp_socket.local_endpoint()));
        in_async_write(resp);
    }
    else
    {
        zlog_debug(anole::cat(), "%s:%d requested connection to: %s:%d", SESS_ADDR, SESS_PORT, req.address.address.c_str(), req.address.port);
        in_async_write(anole::succeeded());
    }
}

void client_session_t::in_async_write(c_str_t& buf)
{
    in_async_write(std::string(buf.data, buf.len));
}

void client_session_t::in_async_write(const std::string& buf)
{
    auto self = shared_from_this();
    auto data = std::make_shared<std::string>(buf);
    boost::asio::async_write(in_socket_, boost::asio::buffer(*data), [this, self, data](const boost::system::error_code err, size_t sz){
        if (err)
        {
            destory();
            return;
        }
        if (HANDSHAKE == status_)
        {
            status_ = REQUEST;
            in_async_read();
        }
        else if (REQUEST == status_)
        {
            status_ = CONNECT;
            establish_tunnel();
        }
        else if (FORWARD == status_)
        {
            // continue forward
            out_async_read();
        }
        else if (INVALID == status_)
        {
            destory();
        }
    });
}

void client_session_t::establish_tunnel()
{
    in_async_read();
    if (is_udp_)
    {
        udp_async_read();
    }
    auto self = shared_from_this();
    sess_.resolver.async_resolve(sess_.config.remote_addr, to_string(sess_.config.remote_port), [this, self](const boost::system::error_code err, boost::asio::ip::tcp::resolver::results_type rc){
        on_resolve(err, rc);
    });
}

void client_session_t::on_resolve(const boost::system::error_code err, boost::asio::ip::tcp::resolver::results_type rc)
{
    if (err)
    {
        zlog_error(anole::cat(), "%s:%d could not resolve remote server %s, %s", SESS_ADDR, SESS_PORT, sess_.config.remote_addr.c_str(), err.message().c_str());
        destory();
        return;
    }
    auto it = rc.begin();
    boost::system::error_code code;
    out_socket_.next_layer().open(it->endpoint().protocol(), code);
    if (code)
    {
        destory();
        return;
    }
    if (sess_.config.tcp.no_delay)
    {
        out_socket_.next_layer().set_option(boost::asio::ip::tcp::no_delay(true));
    }
    if (sess_.config.tcp.keep_alive)
    {
        out_socket_.next_layer().set_option(boost::asio::socket_base::keep_alive(true));
    }
    auto self = shared_from_this();
    out_socket_.next_layer().async_connect(*it, [this, self](const boost::system::error_code err){
        on_connect(err);
    });
}

void client_session_t::on_connect(const boost::system::error_code err)
{
    if (err)
    {
        zlog_error(anole::cat(), "%s:%d could not connect to remote server %s:%d, %s", SESS_ADDR, SESS_PORT, sess_.config.remote_addr.c_str(), sess_.config.remote_port, err.message().c_str());
        destory();
        return;
    }
    auto self = shared_from_this();
    out_socket_.async_handshake(boost::asio::ssl::stream_base::client, [this, self](const boost::system::error_code err){
        if (err)
        {
            zlog_error(anole::cat(), "%s:%d ssl handshake fail %s:%d, %s", SESS_ADDR, SESS_PORT, sess_.config.remote_addr.c_str(), sess_.config.remote_port, err.message().c_str());
            destory();
            return;
        }
        zlog_debug(anole::cat(), "%s:%d tunnel established", SESS_ADDR, SESS_PORT);
        if (sess_.config.ssl.reuse_session)
        {
            auto ssl = out_socket_.native_handle();
            if (!SSL_session_reused(ssl))
            {
                zlog_debug(anole::cat(), "%s:%d ssl session not reused", SESS_ADDR, SESS_PORT);
            }
            else
            {
                zlog_debug(anole::cat(), "%s:%d ssl session reused", SESS_ADDR, SESS_PORT);
            }
        }
        boost::system::error_code code;
        if (is_udp_)
        {
            // TODO
        }
        else
        {
            if (!first_packet_recv_)
            {
                in_socket_.cancel(code);
            }
            status_ = FORWARD;
        }
        out_async_read();
        out_async_write(sess_.out_write_buf);
    });
}

void client_session_t::out_async_read()
{
    auto self = shared_from_this();
    out_socket_.async_read_some(boost::asio::buffer(sess_.out_read_buf, BUF_SIZE), [this, self](const boost::system::error_code err, size_t sz){
        if (err)
        {
            destory();
            return;
        }
        std::string buf((const char *)sess_.out_read_buf, sz);
        if (FORWARD == status_)
        {
            sess_.recv_len += buf.size();
            in_async_write(buf);
        }
        else if (UDP_FORWARD == status_)
        {
            // TODO
        }
    });
}

void client_session_t::out_async_write(const std::string& buf)
{
    auto self = shared_from_this();
    auto data = std::make_shared<std::string>(buf);
    boost::asio::async_write(out_socket_, boost::asio::buffer(*data), [this, self, data](const boost::system::error_code err, size_t sz){
        if (err)
        {
            destory();
            return;
        }
        if (FORWARD == status_)
        {
            in_async_read();
        }
        else if (UDP_FORWARD == status_)
        {
            // TODO
        }
    });
}

void client_session_t::udp_async_read()
{
    auto self = shared_from_this();
    sess_.udp_socket.async_receive_from(boost::asio::buffer(sess_.udp_read_buf, BUF_SIZE), sess_.udp_recv_endpoint, [this, self](const boost::system::error_code err, size_t sz){
        if (boost::asio::error::operation_aborted == err)
        {
            return;
        }
        if (err)
        {
            destory();
            return;
        }
        std::string buf((const char *)sess_.udp_read_buf, sz);
        if (buf.size() < 1)
        {
            return;
        }
        if (buf.size() < 3 || buf[0] || buf[1] || buf[2])
        {
            zlog_error(anole::cat(), "%s:%d bad udp packet", SESS_ADDR, SESS_PORT);
            destory();
            return;
        }
        sock5_address_t addr;
        int addr_len = addr.decode(buf.substr(3));
        if (-1 == addr_len)
        {
            zlog_error(anole::cat(), "%s:%d bad udp packet", SESS_ADDR, SESS_PORT);
            destory();
            return;
        }
        size_t data_len = buf.size() - 3 - addr_len;
        zlog_debug(anole::cat(), "%s:%d sent udp packet (%d bytes) to %s:%d", SESS_ADDR, SESS_PORT, data_len, addr.address.c_str(), addr.port);
    });
}

void client_session_t::destory()
{
    if (DESTORY == status_)
    {
        return;
    }
    status_ = DESTORY;
    zlog_debug(anole::cat(), "%s:%d disconnected, %d bytes received, %d bytes sent, lasted for %s sec", SESS_ADDR, SESS_PORT, sess_.recv_len, sess_.sent_len, time(NULL) - sess_.start_time);
    boost::system::error_code err;
    sess_.resolver.cancel();
    if (in_socket_.is_open())
    {
        in_socket_.cancel(err);
        in_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
        in_socket_.close();
    }
    // TODO udp
    if (out_socket_.next_layer().is_open())
    {
        auto self = shared_from_this();
        auto ssl_shutdown_cb = [this, self](const boost::system::error_code err){
            if (boost::asio::error::operation_aborted == err)
            {
                return;
            }
            boost::system::error_code code;
            sess_.ssl_shutdown_timer.cancel(code);
            out_socket_.next_layer().cancel(code);
            out_socket_.next_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, code);
            out_socket_.next_layer().close();
        };
        out_socket_.next_layer().cancel(err);
        out_socket_.async_shutdown(ssl_shutdown_cb);
        sess_.ssl_shutdown_timer.expires_after(std::chrono::seconds(SSL_SHUTDOWN_TIMEOUT));
        sess_.ssl_shutdown_timer.async_wait(ssl_shutdown_cb);
    }
}

} // anole
