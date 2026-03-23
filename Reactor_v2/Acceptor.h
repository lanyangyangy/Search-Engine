#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "Socket.h"
#include "InetAddress.h"

class Acceptor
{
private:
    Socket _sock;
    InetAddress _addr;
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
public:
    Acceptor(const string& ip, unsigned int port);
    ~Acceptor();
    void ready();
    int accept();
    int get_fd();
};



#endif