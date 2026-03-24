#include "KeyRecommander.h"

// Simhash::Simhash()
// {
//     simhash::Simhasher _simhasher("../dict/jieba.dict.utf8", 
//                                 "../dict/hmm_model.utf8", 
//                                 "../dict/idf.utf8", 
//                                 "../dict/stop_words.utf8");
// }
// int Simhash::compute(const string& str1, const string& str2)
// {
    
// }

KeyRecommander::KeyRecommander(const string&query):_queryWord(query),_Dictionary(Dictionary::createInstance())
{
    execute();
}

KeyRecommander::KeyRecommander(const string&query, const TcpConnectionPtr& con):_queryWord(query), _conn(con)
{
    
}
void KeyRecommander::execute() //执行查询
{
    _wait_words= _Dictionary->doQuery(_queryWord);
    statistic();
}
// void KeyRecommander::queryIndexTable()//查询索引
// {

// }
void KeyRecommander::statistic() //进行计算
{
    for (auto it = _wait_words.begin(); it != _wait_words.end(); ++it) 
    {
        int n = distance(*it);
        _resultQue.push({*it,n});
    }
}
int KeyRecommander::distance(const string & rhs) //计算最小编辑距离
{
/* 编辑距离：空间优化后的动态规划 */
    int n = rhs.length(), m =  _queryWord.length();
    vector<int> dp(m + 1, 0);
    // 状态转移：首行
    for (int j = 1; j <= m; j++) {
        dp[j] = j;
    }
    // 状态转移：其余行
    for (int i = 1; i <= n; i++) {
        // 状态转移：首列
        int leftup = dp[0]; // 暂存 dp[i-1, j-1]
        dp[0] = i;
        // 状态转移：其余列
        for(int j = 1; j <= m; j++) {
            int temp = dp[j];
            if (rhs[i - 1] ==  _queryWord[j - 1]) {
                // 若两字符相等，则直接跳过此两字符
                dp[j] = leftup;
            } else {
                // 最少编辑步数 = 插入、删除、替换这三种操作的最少编辑步数 + 1
                dp[j] = std::min(std::min(dp[j - 1], dp[j]), leftup) + 1;
            }
            leftup = temp; // 更新为下一轮的 dp[i-1, j-1]
        }
    }
    return dp[m];
}

void KeyRecommander::response() //响应客户端的请求
{

}

void KeyRecommander::show()
{
    priority_queue<pair<string,int>, vector<pair<string,int>>, compare> pq = _resultQue;
    while (!pq.empty()) 
    {
        show_string(pq.top().first);
        cout << "=" << pq.top().second << " ";
        pq.pop();
    }
}

