#include "Mytask.h"


MyTask::MyTask(const string &msg, const TcpConnectionPtr &con,const Search_ptr& search1)
: _msg(msg)
, _con(con)
, _search(search1)
{
    
}

void MyTask::process()
{


    string reslut = _search->doing(_msg);

    _con->sendInLoop(reslut);
}