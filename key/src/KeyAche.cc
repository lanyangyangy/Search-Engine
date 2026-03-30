#include "KeyAche.h"

KeyAche::KeyAche():_redis(new Redis("tcp://127.0.0.1:6380"))
{

}

KeyAche::~KeyAche()
{
}

void KeyAche::add(pair<string,map<string,int>> word)
{
        string query_word = word.first;
        map<string,int> candidates = word.second;
        json j = candidates;
        string json_str = j.dump();
        _redis->set(query_word, json_str, std::chrono::minutes(3));
    
        cout << "已存入缓存: " << query_word << " -> " << json_str << endl;
}


map<string,int> KeyAche::respone(const string& word)
{
      
        auto val = _redis->get(word);
        if (val) {
        // 从 Redis 拿到 JSON 字符串后，反序列化回 C++ 的 vector
        json j_recv = json::parse(*val);
        map<string, int> cached_candidates = j_recv.get<map<string, int>>();
        
        cout << "命中缓存，解析成功!"<< endl;
        return cached_candidates;
        // cout << "命中缓存，解析成功！第一个候选词是: " << cached_candidates[0].first << endl;
        }
        return map<string, int>();
}