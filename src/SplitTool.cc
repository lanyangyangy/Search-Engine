#include "SplitTool.h"

SplitTool::SplitTool()
{}
SplitToolCppJieba::SplitToolCppJieba():_conf(config())
,_jieba(_conf[0],_conf[1],_conf[2],_conf[3],_conf[4])
{

}

vector<string> SplitToolCppJieba::config()
{
    string conf = "./conf/jieba.txt";
    vector<string> vconf;
    ifstream iss(conf);
    string line;
    if (iss.is_open()) {
        std::string line;
        while (std::getline(iss, line)) {
            line = line.substr(line.find(' '));
            line.pop_back();
            line.erase(0,4);
            vconf.push_back(line);
        }
        iss.close();
    } else {
        std::cout << "Unable to open the file." << std::endl;
    } 
    return vconf;
    
}

vector<string> SplitToolCppJieba::cut(const string &sentence)
{
    vector<string> words;
    _jieba.CutAll(sentence, words);
    return words;
}
