#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__
#include <string>
#include "DictProducer.h"
#include "Reactor/TcpServer.h"
#include "Reactor/TcpConnection.h"
#include "Dictionary.h"

using std::string;
using std::priority_queue;
struct compare
{
    bool operator()(const pair<string,int> s, const pair<string,int> r) const { return s.second < r.second; }
};
void iteratePriorityQueue(priority_queue<int> pq) ;

class KeyRecommander
{

    private:
    string _queryWord;
    TcpConnectionPtr _conn;
    Dictionary* _Dictionary;
    set<string> _wait_words;
    priority_queue<pair<string,int>, vector<pair<string,int>>, compare> _resultQue;

    public:
    KeyRecommander(const string&query, const TcpConnectionPtr& con);
    KeyRecommander(const string&query);
    void execute(); //执行查询
    void queryIndexTable();//查询索引
    void statistic(); //进行计算
    int distance(const string & rhs); //计算最小编辑距离
    void response(); //响应客户端的请求
    void show();

};


#endif