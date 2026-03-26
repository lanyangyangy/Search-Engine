#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__
#include <string>
#include <functional>
#include <nlohmann/json.hpp>

#include "DictProducer.h"
#include "Reactor/TcpServer.h"
#include "Reactor/TcpConnection.h"
#include "Dictionary.h"
#include "KeyAche.h"
using json = nlohmann::json;

using std::string;
using std::priority_queue;

void iteratePriorityQueue(priority_queue<int> pq) ;
using CompareType = std::function<bool(const std::pair<std::string,int>&,
                                           const std::pair<std::string,int>&)>;
class KeyRecommander
{

    private:
    struct compare;
    string _queryWord;
    TcpConnectionPtr _conn;
    Dictionary* _Dictionary;
    set<string> _wait_words;
    priority_queue<pair<string,int>, vector<pair<string,int>>, CompareType> _resultQue;
    int nums = 5; //返回字符个数
    KeyAche _keyache; 
    public:
    KeyRecommander(const string&query, const TcpConnectionPtr& con, const KeyAche& keycache);
    // KeyRecommander(const string&query);
    void execute(); //执行查询
    void queryIndexTable();//查询索引
    void statistic(); //进行计算
    int distance(const string & rhs); //计算最小编辑距离
    void response(); //响应客户端的请求

    void show();

};


#endif