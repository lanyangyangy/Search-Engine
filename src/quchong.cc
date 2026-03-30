#include "quchong.h"

Quchong::Quchong(/* args */):_conf(config()),
                             _simhasher(_conf[0],_conf[1], _conf[2], _conf[3])
{

}

uint64_t Quchong::compute(const string& words)
{
    string word = words;
    while(1)
    {   
    // 建议用 size_t 接收 find 的返回值，这是最规范的写法
    size_t m = word.find('<'); 
    if(m != std::string::npos)
    {
        // 关键修复 1：从 m 的位置开始往后找 '>'
        size_t n = word.find('>', m); 
            
            if(n != std::string::npos)
            {
                // 关键修复 2：长度加 1，把 '>' 一并抹除！
                word.erase(m, n - m + 1); 

            }
            else 
            {
                // 如果只有 '<' 没有 '>'，直接跳出，防止死循环
                break; 
            }
        }
        else 
        {
            break;
        }
    }        
        // cout<< word<<endl;
    uint64_t u64;
    vector<pair<string ,double> > res;
    bool ok = _simhasher.extract(word, res, _topN);
    _simhasher.make(word, _topN, u64);
    return u64;
}

vector<string> Quchong::config()
{
    string conf = "./conf/simhash.txt";
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
