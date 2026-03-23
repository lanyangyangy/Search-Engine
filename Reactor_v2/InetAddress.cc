#include "InetAddress.h"

InetAddress::InetAddress(const string &ip, unsigned short port)
{
    struct in_addr inAddr;
    inet_aton(ip.c_str(), &inAddr);

    int sourcePortNet = htons(port);
    _addr.sin_family = AF_INET;
    _addr.sin_addr = inAddr;
    _addr.sin_port = sourcePortNet;

}
InetAddress::InetAddress(const struct sockaddr_in &addr): _addr(addr)
{
   
}
InetAddress::~InetAddress()
{

}
string InetAddress::ip() const
{
    return string(inet_ntoa(_addr.sin_addr));
}
unsigned short InetAddress:: port() const
{
    return ntohs(_addr.sin_port);
}
const struct sockaddr_in * InetAddress::getInetAddrPtr() const
{
    return &_addr;
}