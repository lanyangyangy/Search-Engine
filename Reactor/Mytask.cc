#include "Mytask.h"


MyTask::MyTask(const string &msg , const TcpConnectionPtr &con, KeyAche& keyache)
: _msg(msg)
, _con(con)
,_keyache(keyache)
, _keyrecom(msg,con,_keyache)
{
    
}

void MyTask::process()
{
    
    _keyrecom.execute();
    // _con->sendInLoop(_msg);
}