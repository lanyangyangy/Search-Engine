#include "Acceptor.h"

Acceptor::Acceptor(const string& ip, unsigned int port):_addr(ip,port),_sock()
{
}

Acceptor::~Acceptor()
{
}
void Acceptor::setReuseAddr()
{
    int on = 1;
    int ret = setsockopt(_sock.get_fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(ret)
    {
        perror("setsockopt");
        return;
    }
}
void Acceptor::setReusePort()
{
    int on = 1;
    int ret = setsockopt(_sock.get_fd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    if(ret)
    {
        perror("setsockopt");
        return;
    }

}
void Acceptor::bind()
{
    int ret = ::bind(_sock.get_fd(),  (struct sockaddr *)_addr.getInetAddrPtr(),
                    sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("bind");
        return;
    }

}
void Acceptor::listen()
{
    int ret = ::listen(_sock.get_fd(), 128);
    if(-1 == ret)
    {
        perror("listen");
        return;
    }
}
void Acceptor::ready()
{   
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}
int Acceptor::accept()
{
    int connect_fd = ::accept(_sock.get_fd(),NULL,NULL);
    if(-1 == connect_fd)
    {
        perror("accept");
        return -1;
    }
    return connect_fd;
}
int Acceptor::get_fd(){return _sock.get_fd();}