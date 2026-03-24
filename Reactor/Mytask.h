#ifndef __MYTASK_H__
#define __MYTASK_H__
#include "TcpServer.h"
#include "TcpConnection.h"
#include "ThreadPool.h"

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con);
    void process();
private:
    string _msg;
    TcpConnectionPtr _con;
};




#endif