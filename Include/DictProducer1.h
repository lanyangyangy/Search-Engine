#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "SplitTool.h"
#include <utility> 
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sstream>


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
class DictProducer
{
private:
    vector<string> _files;  //语料库文件的绝对路径集合
    vector<pair<string, int>> _eng_dict;   //英文词典
    map<string, set<int>> _eng_index;   //英文词典索引
    set<string> _eng_stop_worlds; //英文停止词

    vector<pair<string, int>> _cn_dict;   //中文词典
    map<string, set<int>> _cn_index;   //中文词典索引
    set<string> _cn_stop_worlds; //中文停止词
    SplitTool* _cuttor = nullptr;   //分词工具
    
    string _dir_now;  //绝对路径


public:
    DictProducer(const string& dir);   //构造函数
    DictProducer(const string& dir, SplitTool* splitTool);//构造函数,专为中文处理
    void buildEnDdict();  //创建英文字典
    void buildCnDict(const char* filepath);  //创建中文字典
    void storeDict(const string filepath);  //将词典写入文件
    void storeIndex(const string filepath);  //将索引写入文件
    void showFiles()const;  //查看文件路径，作为测试用
    void showDict()const;  //查看词典，作为测试用
    void showIndex()const;//查看索引
    string getFiles();  //获取文件的绝对路径
    void pushDict(const string& word);//存储某个单词
    void pushIndex(const string& word,int idx);//存储词典索引
    void pushStopWord();//存放stopword
    void readfile();//读取文本
    void ClearWords(string& words); //清洗数据
    void buildEnIndex();  //创建英文索引
    void doFiles(const string& dir);//处理文件路径
   

};




#endif