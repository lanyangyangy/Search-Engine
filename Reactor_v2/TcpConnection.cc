#include "TcpConnection.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;
TcpConnection::TcpConnection(int fd):_sock(fd),_sockIO(fd),_local_addr(get_localAddr()),_peer_addr(get_peerAddr())
{

}

TcpConnection::~TcpConnection()
{
}
void TcpConnection::send(const string& msg)
{
    _sockIO.writen(msg.c_str(),msg.size());
}
string TcpConnection::receive()
{
    char buff[65535] = {0};
    _sockIO.readLine(buff, sizeof(buff));

    return string(buff);

}
InetAddress TcpConnection::get_localAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    //获取本端地址的函数getsockname
    int ret = getsockname(_sock.get_fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname");
    }

    return InetAddress(addr);
}
InetAddress TcpConnection::get_peerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    //获取对端地址的函数getpeername
    int ret = getpeername(_sock.get_fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getpeername");
    }

    return InetAddress(addr);
}

string TcpConnection::toString()
{
    ostringstream oss;
    oss << _local_addr.ip() << ":"
        << _local_addr.port() << "---->"
        << _peer_addr.ip() << ":"
        << _peer_addr.port();

    return oss.str();
}

void TcpConnection::set_onNewConnectionCb(const TcpConnectionCallback& cb)
{
    _onNewConnectionCb = cb;
}
void TcpConnection::set_onNewCloseCb(const TcpConnectionCallback& cb)
{
    _onNewCloseCb = cb;
}
void TcpConnection::set_onMessageCb(const TcpConnectionCallback& cb)
{
    _onMessageCb =cb;
}
void TcpConnection::handle_onNewConnectionCb()
{
    if(_onNewConnectionCb)
    {
        _onNewConnectionCb(shared_from_this());
    }
    else
    {
       cout << "nullptr == _onNewConnection" << endl;
    }
}
void TcpConnection::handle_onNewCloseCb()
{

    if(_onNewCloseCb)
    {
        _onNewCloseCb(shared_from_this());
    }
    else
    {
       cout << "nullptr == _onNewCloseCb" << endl;
    }
}
void TcpConnection::handle_onMessageCb()
{

    if(_onMessageCb)
    {
        _onMessageCb(shared_from_this());
    }
    else
    {
        cout<< "nullptr == _onNewCloseCb" << endl;
    }
}
bool TcpConnection::is_closed()
{
    char buff[10] = {0};
    int ret = recv(_sock.get_fd(), buff, sizeof(buff), MSG_PEEK);
    return (0 == ret);

}