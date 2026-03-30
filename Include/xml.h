#ifndef __XML_H__
#define __XML_H__

#include "tinyxml.h"
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <math.h>
#include <cctype>
using std::ofstream;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::string;
namespace fs = std::filesystem;
#include <quchong.h>
#include<SplitTool.h>

bool is_fuhao(unsigned char c);

class Xml
{
    public:
   

    Xml(const string & path,SplitTool* tool);
    private:
    vector<map<string,string>> _all;  
    vector<string> _fmtTxt;  //存放组装后的字符串
    string _path;  
    map<string, uint64_t > _simhash_all;  //一篇文章的simhash值
    map<uint64_t, string> _sim_str;   //倒排
    map<string,int> _str_xu;//存放字符串的序号
    unordered_map <string, vector<pair<int, double>>> InvertIndexTable;

    long long _nums = 0;    //字符数
    Quchong _quchong; 
    int _N; //文章数目


    SplitTool* _tool;
    vector< pair<string,vector<string>>> _split_words;

    map<string,int> _df;//全文中的词频统计
    vector<pair<uint64_t,map<string,int>>> _tf; //在文章中的词频统计
    map<string,double> _idf;//逆文档频率

    vector<pair<int,map<string,double>>> _w;
    void jiexi_single(const string & path);  //解析单个xml
    void jiexi(const string& dir);  //解析所有xml
    void write(); //写ripepage.data
    void bianli();
    
    uint64_t get_simhash(const string& word);
    void word_freq();//分词频率
    void compute_idf();  //计算IDF
    void compute_w(); //计算权重
    void compute_invertindex();
    
   
};

#endif