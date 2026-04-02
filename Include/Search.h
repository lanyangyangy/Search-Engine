#ifndef __SEARCH_H__
#define __SEARCH_H__
#include "xml.h"
#include "quchong.h"
#include <sstream>

#include "nlohmann/json.hpp"
#include "tinyxml.h"
using json = nlohmann::json;

using std::stringstream;
using std::getline;
class Search
{
private:
   
    Quchong _quchong;
    string _words;  //传进来的关键词
    // vector<string> _splited_word;//分完后的词
    // map<string,int> _tf;
    // map<string,int> _df;
    // map<string,double> _idf;//逆文档频率s
    // map<std::string, double> _w;//权重
    // map<string,set<int>> _Invert_word;
    std::unordered_map<std::string, map<int, double>> _InvertIndexTable;
    void read_InvertIndex();
    map<string,int> _df;
    int _N = 0;
private:
   
    vector<string> split_word(const string & word);

    std::map<std::string, double> compute_w(map<string,int> _tf, map<string,double> _idf);
    void compute_tf(map<string,int>& _tf,const vector<string>& _splited_word);//计算_tf
    map<std::string, double> compute_idf( map<string, int> _df);//计算_idf
    set<int> pipei_key(vector<string> _splited_word);  //查询到文章
    vector<pair<int, map<string, double>>> get_w(vector<string> word, set<int> docid); //得到所有文章的权重
    int select(map<std::string, double> _w, vector<pair<int, map<string, double>>> _doc_w);//选择最佳doc
    pair<int,int> chazhao(int id);//查找文章的偏移和大小
    string fund(pair<int,int> out);
    string to_Json(const string& words);//转字符串为json
public:
    Search();
    string doing(const string& words );
    
};





#endif