#ifndef __SEARCH_H__
#define __SEARCH_H__
#include "xml.h"
#include "quchong.h"
#include <sstream>
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
    std::unordered_map<std::string, std::vector<std::pair<int, double>>> _InvertIndexTable;
    std::unordered_map<std::string, std::vector<std::pair<int, double>>> read_InvertIndex();
public:
    Search();
    vector<string> split_word(const string & word);
    void doing(const string& words );

    std::map<std::string, double> compute_w(map<string,int> _tf, map<string,double> _idf);
    void compute_tdf(map<string,int>& _tf, map<string,int>& _df,const vector<string>& _splited_word);
    map<std::string, double> compute_idf( map<string, int> _df);
    
    set<int> pipei_key(vector<string> _splited_word);
    ~Search();
};





#endif