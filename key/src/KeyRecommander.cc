#include "KeyRecommander.h"



KeyRecommander::KeyRecommander(const string&query, const TcpConnectionPtr& con,const KeyAche& keycache):_queryWord(query)
,_Dictionary(Dictionary::createInstance())
, _conn(con)
,_keyache(keycache)
,_resultQue([this](const std::pair<std::string,int>& a,
                            const std::pair<std::string,int>& b) -> bool {
              // 优先按第二项（频率）降序，频率相同时按词典中的编号（或其他）升序
              if (a.second != b.second)
                  return a.second > b.second;   // 注意：priority_queue 默认最大堆，所以返回 a < b 表示 a 优先级低
              // 使用 _Dictionary 获取更多信息
              int m = _Dictionary->getDict(a.first);
              int n = _Dictionary->getDict(b.first);
              if( m!=n)
              return m > n;                     // 编号小的优先
              else
               return a.first[0]>b.first[0];
          })
{
    
}
void KeyRecommander::execute() //执行查询
{
  
    map<string,int> cache_res = _keyache.respone(_queryWord);
    if(cache_res.empty())
    {

        _wait_words= _Dictionary->doQuery(_queryWord);

        statistic();
        response();
    }
    else
    {

        json json_array = json::array();
        auto tempQue = cache_res; 
        int count = 0;
        while (!tempQue.empty() && count < nums) 
        {
            for(const auto& [key, value] : cache_res)
            {
                if( count< nums)
                {
                    json item;
                   
                    item["word"] = key;     // 候选词
                    item["score"] = value;   // 词频 或 编辑距离

                    // 5. 放入 JSON 数组中
                    json_array.push_back(item);    
                    count++;
                }
            }
          
        }
        // 6. dump() 会把 json 对象序列化为 string 字符串返回
        _conn->sendInLoop(json_array.dump()); 
        _conn->sendInLoop("\n");

    }

 
}

void KeyRecommander::statistic() //进行计算
{
    for (auto it = _wait_words.begin(); it != _wait_words.end(); ++it) 
    {
        int n = distance(*it);
        _resultQue.push({*it,n});
    }

}
// int KeyRecommander::distance(const string & rhs) //计算最小编辑距离
// {
// /* 编辑距离：空间优化后的动态规划 */
//     int n = rhs.length(), m =  _queryWord.length();
//     vector<int> dp(m + 1, 0);
//     // 状态转移：首行
//     for (int j = 1; j <= m; j++) {
//         dp[j] = j;
//     }
//     // 状态转移：其余行
//     for (int i = 1; i <= n; i++) {
//         // 状态转移：首列
//         int leftup = dp[0]; // 暂存 dp[i-1, j-1]
//         dp[0] = i;
//         // 状态转移：其余列
//         for(int j = 1; j <= m; j++) {
//             int temp = dp[j];
//             if (rhs[i - 1] ==  _queryWord[j - 1]) {
//                 // 若两字符相等，则直接跳过此两字符
//                 dp[j] = leftup;
//             } else {
//                 // 最少编辑步数 = 插入、删除、替换这三种操作的最少编辑步数 + 1
//                 dp[j] = std::min(std::min(dp[j - 1], dp[j]), leftup) + 1;
//             }
//             leftup = temp; // 更新为下一轮的 dp[i-1, j-1]
//         }
//     }
//     return dp[m];
// }

int KeyRecommander::distance(const string & rhs) {
    size_t m = rhs.size(), n =  _queryWord.size();
    if (m == 0) return n;
    if (n == 0) return m;

    std::vector<int> dp_prev(n + 1), dp_curr(n + 1);
    // 初始化第一行：dp[0][j] = j
    for (size_t j = 0; j <= n; ++j) dp_prev[j] = j;

    for (size_t i = 1; i <= m; ++i) {
        dp_curr[0] = i;  // dp[i][0] = i
        for (size_t j = 1; j <= n; ++j) {
            if (rhs[i-1] ==  _queryWord[j-1]) {
                dp_curr[j] = dp_prev[j-1];
            } else {
                dp_curr[j] = 1 + std::min({dp_prev[j],     // 删除
                                           dp_curr[j-1],   // 插入
                                           dp_prev[j-1]}); // 替换
            }
        }
        std::swap(dp_prev, dp_curr); // 滚动数组
    }
    return dp_prev[n];
}

void KeyRecommander::response() //响应客户端的请求
{
   
    json json_array = json::array();
    auto tempQue = _resultQue; 
    int count = 0;
    std::map<std::string, int> cache;
    while (!tempQue.empty() && count < nums) 
    {
        // 获取队首元素 (最优的候选词)
        pair<string, int> top_item = tempQue.top();
        tempQue.pop(); 

        // 4. 组装成 JSON 对象
        json item;
        item["word"] = top_item.first;     // 候选词
        item["score"] = top_item.second;   // 词频 或 编辑距离
        cache[top_item.first] =  top_item.second;
        // 5. 放入 JSON 数组中 
        json_array.push_back(item);
        
        count++;
    }
    _keyache.add(std::make_pair(_queryWord,cache));
    // 6. dump() 会把 json 对象序列化为 string 字符串返回
    _conn->sendInLoop(json_array.dump()); 
    _conn->sendInLoop("\n");

}





void KeyRecommander::show()
{
    priority_queue<pair<string,int>, vector<pair<string,int>>, CompareType> pq = _resultQue;
    while (!pq.empty()) 
    {
        show_string(pq.top().first);
        cout << "=" << pq.top().second << " ";
        pq.pop();
    }
}

