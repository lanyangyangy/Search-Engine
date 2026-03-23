#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Acceptor.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <unistd.h>
#include <functional>
#include <iostream>
#include <memory>

using std::shared_ptr;
using std::function;


class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void (const TcpConnectionPtr &)>;

using std::enable_shared_from_this;
class TcpConnection:
public enable_shared_from_this<TcpConnection>
{
private:
    SocketIO _sockIO;
    Socket _sock;
    InetAddress _local_addr;
    InetAddress _peer_addr;

    TcpConnectionCallback _onNewConnectionCb;
    TcpConnectionCallback _onNewCloseCb;
    TcpConnectionCallback _onMessageCb;

public:
    explicit TcpConnection(int fd);
    ~TcpConnection();
    void send(const string& msg);
    string receive();
    InetAddress get_localAddr();
    InetAddress get_peerAddr();
    string toString();
    void set_onNewConnectionCb(const TcpConnectionCallback& cb);
    void set_onNewCloseCb(const TcpConnectionCallback& cb);
    void set_onMessageCb(const TcpConnectionCallback& cb);
    void handle_onNewConnectionCb();
    void handle_onNewCloseCb();
    void handle_onMessageCb();
    bool is_closed();
};





#endif