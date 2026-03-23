#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include "cppjieba/Jieba.hpp"
#include <vector>
#include <string>
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::endl;
using std::cout;

using std::vector;
using std::string;

using Configuration = string;

class SplitTool
{
    public:

    SplitTool();

    virtual vector<string> cut(const string &sentence)=0;
  
};

class SplitToolCppJieba:public SplitTool
{
    private:
    vector<Configuration> _conf;  //配置文件路径
    cppjieba::Jieba _jieba;

    public:
    SplitToolCppJieba();
    vector<string> config();

    vector<string> cut(const string &sentence) override;//分词函数，纯虚函数，提供接口
};

#endif