#ifndef __MYTASK_H__
#define __MYTASK_H__
#include "TcpServer.h"
#include "TcpConnection.h"
#include "ThreadPool.h"
#include "Search.h"
using Search_ptr = std::shared_ptr<Search> ;
class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con,const Search_ptr& search1);
    void process();
private:
    string _msg;
    TcpConnectionPtr _con;
    Search_ptr _search;
};




#endif