#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "TcpServer.h"
#include "ThreadPool.h"
#include "Mytask.h"
#include "Search.h"
using Search_ptr = std::shared_ptr<Search> ;
class EchoServer
{
public:
    EchoServer(size_t threadNum, size_t queSize
               , const string &ip
               , unsigned short port);
    ~EchoServer();

    //服务器的启动与停止
    void start();
    void stop();

    //三个回调
    void onNewConnection(const TcpConnectionPtr &con);
    void onMessage(const TcpConnectionPtr &con);
    void onClose(const TcpConnectionPtr &con);

private:
    ThreadPool _pool;//线程池对象
    TcpServer _server;//TcpServer对象
    Search_ptr _search;

};

#endif
