#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <utility> 
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <cstring>


namespace fs = std::filesystem;
using std::endl;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::set;
using std::map;
using std::pair;
using std::transform;
using std::istringstream;
using std::vector;
using std::string;
using std::getline;

void Split(string str, char separator, vector<string> &output);
class Dictionary
{
    private:
    vector<pair<string, int>> _dict;   //词典
    map<string, set<int>> _index;   //词典索引
    void init(); //通过词典文件路径初始化词典
    void readfiles();
    void readdict();
    void readindex();

    public:

    static Dictionary* _pInstance;
    vector<pair<string, int> > & getDict(); //获取词典
    map<string, set<int> > & getIndexTable(); //获取索引表
    set<string> doQuery(string word); //执行查询
    static Dictionary * createInstance();
    static void destory();//销毁单例
    Dictionary();
    void showDict(const vector<pair<string, int>> dict)const;  //查看词典，作为测试用
    void showIndex(const map<string, set<int>> index)const;//查看索引
};


#endif