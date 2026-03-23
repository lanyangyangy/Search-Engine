#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "Acceptor.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include "TcpConnection.h"
#include <unistd.h>
#include <functional>
#include <iostream>

using std::vector;
using std::map;
using std::shared_ptr;
using std::function;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void (const TcpConnectionPtr &)>;

class TcpConnection;
class EventLoop
{
    private:
    int _epollfd;
    Acceptor& _acceptor;
    map<int, shared_ptr<TcpConnection>> _conns;
    vector<struct epoll_event> _events;
    bool _isLooping;
    TcpConnectionCallback _onNewConnectionCb;
    TcpConnectionCallback _onNewCloseCb;
    TcpConnectionCallback _onMessageCb;

    int create_epoll(); //创建epoll
    void addEpollReadFd(int fd); //添加epoll_event
    void delEpollReadFd(int fd);//删除epoll_event
    void Epoll_wait(); //  封装epoll_wait
    void handle_Newconnection(); //处理新链接请求
    void handle_Message(int fd);//数据收发

   


    public:
    EventLoop(Acceptor& acceptor);
    ~EventLoop();
    void set_onNewConnectionCb(TcpConnectionCallback&& cb);
    void set_onNewCloseCb(TcpConnectionCallback&& cb);
    void set_onMessageCb(TcpConnectionCallback&& cb);
    
    void loop(); //循环
    void unloop(); //结束循环


};

#endif