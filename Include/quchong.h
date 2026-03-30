#ifndef __QUCHONG_H__
#define __QUCHONG_H__
#include <iostream>
#include "simhash/Simhasher.hpp"
#include <vector>
#include <string>
#include <utility>
using std::pair;
using std::vector;
using std::string;
// using std::

using namespace simhash;
class Quchong
{
private:
    vector<string> _conf;
    Simhasher _simhasher;
    size_t _topN = 5;
public:
    Quchong( );
    vector<string> config();//读取配置信息
    uint64_t compute(const string& words);
    vector<pair<string ,double>> get_key(const string& words);
    
};








#endif