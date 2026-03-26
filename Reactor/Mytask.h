#ifndef __MYTASK_H__
#define __MYTASK_H__
#include "TcpServer.h"
#include "TcpConnection.h"
#include "ThreadPool.h"
#include "KeyRecommander.h"
#include "KeyAche.h"


class MyTask
{
public:
    MyTask(const string &msg,  const TcpConnectionPtr &con,KeyAche& keyache);
    void process();
private:
    string _msg;
    TcpConnectionPtr _con;
    KeyAche _keyache;
    KeyRecommander _keyrecom;

};




#endif