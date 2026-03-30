#ifndef __KEYCAHE_H__
#define __KEYCAHE_H__

#include <sw/redis++/redis++.h>
#include <iostream>
#include <unordered_set>

#include <algorithm>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;
using namespace sw::redis;

//= new Redis("tcp://127.0.0.1:6379");
class KeyAche
{
private:
    Redis* _redis;

   
public:
    KeyAche();
    
    ~KeyAche();
    void Search(const string& word);
    void add(pair<string,map<string,int>> word);
    map<string,int> respone(const string& word);
    
};




#endif