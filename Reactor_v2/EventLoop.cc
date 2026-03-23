#include "EventLoop.h"
using std::cout;
using std::cerr;
using std::endl;


EventLoop::EventLoop(Acceptor& acceptor)
:_acceptor(acceptor),
_epollfd(create_epoll()),
_conns(),
_events(1024),
_isLooping(false)
{
    int listenfd = _acceptor.get_fd();
    addEpollReadFd(listenfd);
}
EventLoop::~EventLoop()
{
    close(_epollfd);
}
int EventLoop::create_epoll()
{
    int epollfd = epoll_create(1);
    if(-1 ==  epollfd)
    {
        perror("epoll_create error");
        exit(-1);
    }
    return epollfd;
    
}
void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;
    int ret = epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &evt);
    if(-1 == ret)
    {
        perror("epoll_ctl error");
        close(fd);
        exit(-1);
    }


}
void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret < 0)
    {
        perror("delEpollReadFd");
        return;
    }

}
void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        Epoll_wait();
    }
}
void EventLoop::unloop() 
{
    _isLooping = false;
}
void EventLoop::Epoll_wait() 
{
    int nready=0;
    do
    {
      nready = epoll_wait(_epollfd,  &*_events.begin(), _events.size(), 3000);
    } while (-1 == nready && errno == EINTR);
   
    if(-1 == nready)
    {
        cerr << "-1 == nready" << endl;
        return;
    }
    else if(0 == nready)
    {
        cout << ">>epoll_wait timeout!!!" << endl;
    }
    else
    {
                //需要将vector手动扩容
        if(nready == (int)_events.size())
        {
           _events.resize(2 * nready);
        }
        for(int idx=0; idx < nready; idx++)
        {
            int fd = _events[idx].data.fd;
            int listenfd = _acceptor.get_fd();
            if(fd == listenfd)
            {
                if(_events[idx].events & EPOLLIN)
                {
                    handle_Newconnection();
                }
            }
            else
            {
               if(_events[idx].events & EPOLLIN)
                {
                    handle_Message(fd);
                }
            }
        }
    }
}
void EventLoop::handle_Newconnection() 
{
    struct sockaddr_in clie_addr;
    socklen_t  clie_addr_len = sizeof(clie_addr);
    int connfd = _acceptor.accept();
    if(connfd < 0)
    {
        perror("handleNewConnection");
        return;
    }
    addEpollReadFd(connfd);
    shared_ptr<TcpConnection> con(new TcpConnection(connfd) );
    _conns.insert({connfd,con});
    con->set_onNewConnectionCb(_onNewConnectionCb);//连接建立的注册
    con->set_onMessageCb(_onMessageCb);//消息达到的注册
    con->set_onNewCloseCb(_onNewCloseCb);//连接断开的注册
    con->handle_onNewConnectionCb();
}
void EventLoop::handle_Message(int fd)
{
    auto it = _conns.find(fd);
    if(it != _conns.end())
    {
        bool flag = it->second->is_closed();
        if(flag)
        {
            it->second->handle_onNewCloseCb();
            //将文件描述符从红黑树上删除掉
            delEpollReadFd(fd);
            //还需要从map数据结构中删除
            _conns.erase(it);
        }
        else
        {
            it->second->handle_onMessageCb();
        }
    }
    else
    {
        cout << "该连接不存在" << endl;
        return;
    }
}

void EventLoop::set_onNewConnectionCb(TcpConnectionCallback&& cb)
{
    _onNewConnectionCb = std::move(cb);
}
void EventLoop::set_onNewCloseCb(TcpConnectionCallback&& cb)
{
    _onNewCloseCb = std::move(cb);
}
void EventLoop::set_onMessageCb(TcpConnectionCallback&& cb)
{
    _onMessageCb = std::move(cb);
}