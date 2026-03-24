#include "Mytask.h"


MyTask::MyTask(const string &msg, const TcpConnectionPtr &con)
: _msg(msg)
, _con(con)
{
    
}

void MyTask::process()
{
    
    _con->sendInLoop(_msg);
}