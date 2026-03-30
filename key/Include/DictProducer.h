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

bool isChineseChar(unsigned char c);  //区分中文英文，但中文里仍有符号
bool isChineseWord(unsigned char c); //区分中文和中文符号
void show_string(const string& words); //输出字符
bool isChinesepunct(int c);
void replaceAll(string& str, char oldChar, char newChar);//替换字符串的元素
class DictProducer
{
private:
    vector<string> _files;  //语料库文件的绝对路径集合
    set<string> _eng_stop_worlds; //英文停止词

    vector<pair<string, int>> _dict;    //词典
    map<string, set<int>> _index;   //索引


    set<string> _ch_stop_worlds; //中文停止词
    SplitTool* _cuttor = nullptr;   //分词工具
    
    string _dir_now;  //绝对路径
    string _eng_words; //存放英文文章
    string _ch_words; //存放中文文章
    SplitTool* _tool;

public:
    DictProducer(const string& dir, SplitTool* splitTool);   //构造函数
    
    void readfile();//读取文本
    void doFiles(const string& dir);//处理文件路径
    void pushStopWord(const string& stop_world_dir,bool is_ch);//存放stopword
    void pushDict(string& word,const set<string> stop_worlds, vector<pair<string, int>>& dict);//存储某个单词
    void pushIndex(const string& word,map<string, set<int>>& index,const int idx);//存储词典索引
    void storeDict(const string filepath, const vector<pair<string, int>> dict);  //将词典写入文件
    void storeIndex(const string filepath, const map<std::string, std::set<int>> index);  //将索引写入文件


    void ClearEngWords(); //清洗数据
    void buildEnDdict();  //创建英文字典
    void buildCnDict( );



    vector<string> ClearChWords(); //清洗数据


    void bulidDict();//创建字典
    void buildIndex();//

    void showFiles()const;  //查看文件路径，作为测试用
    void showDict(const vector<pair<string, int>> dict)const;  //查看词典，作为测试用
    void showIndex(const map<string, set<int>> index)const;//查看索引
    string getFiles();  //获取文件的绝对路径
    

    
    
    
    
    




   

};




#endif