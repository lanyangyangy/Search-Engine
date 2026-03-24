#include "Dictionary.h"

Dictionary*  Dictionary::_pInstance = nullptr;

Dictionary::Dictionary() //通过词典文件路径初始化词典
{
    readfiles();
}
vector<pair<string, int> > & Dictionary::getDict() //获取词典
{
        return this->_dict;
}
map<string, set<int> > & Dictionary::getIndexTable() //获取索引表
{
        return this->_index;
}
set<string> Dictionary::doQuery(string word) //执行查询
{
    
    set<string> ans;

    for(auto &i : word)
    {
        auto search1=_index.find(string(1,i));
        if(  search1 != _index.end() )
        { 
            for(auto &j: _index[string(1,i)])
            {
                ans.insert(_dict[j].first);
            }

        }
    }

       
    
    return ans;
}
Dictionary * Dictionary::createInstance()
{
    if(nullptr == _pInstance)
    {
        _pInstance = new Dictionary();
    }
    return _pInstance;
}

void Dictionary::destory()
{
    if(_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
        cout<< "delete Dictionary"<<endl;
    }
}

void Dictionary::readfiles()
{
    readdict();
    readindex();
    
}
void Dictionary::readdict()
{
    ifstream ifs;
    ifs.open("./data/dict.dat");
    if (!ifs.is_open()) 
    {
        std::cerr << "Failed to open file: " << std::endl;
    } 
    else 
    {
        std::string line;
        while (std::getline(ifs, line)) {   // 每次读取一行（不含换行符）
            istringstream ss(line);
            vector<string> tmp;
            Split(line,' ', tmp);

            try {
                int num = std::stoi(tmp[1]);
                // std::cout << "转换后的数字是：" << num << std::endl;
                 _dict.push_back({tmp[0],num});
                } catch (const std::invalid_argument& e) {
                std::cerr << "无效的输入字符串: " << e.what() << std::endl;
                }
           
        }
        ifs.close();
    }
}
void Dictionary::readindex()
{
    ifstream ifs;
    ifs.open("./data/dictIndex.dat");
    if (!ifs.is_open()) 
    {
        std::cerr << "Failed to open file: " << std::endl;
    } 
    else 
    {
        std::string line;
        while (std::getline(ifs, line)) {   // 每次读取一行（不含换行符）
            istringstream ss(line);
            vector<string> tmp;
            Split(line,' ', tmp);
            set<int> set;
            
            for(int i=1; i<tmp.size()-1;i++)
            {
                
                try {
                int num = std::stoi(tmp[i]);
                // std::cout << "转换后的数字是：" << num << std::endl;
                 set.insert(num);
                } catch (const std::invalid_argument& e) {
                   
                std::cerr << "无效的输入字符串: " << e.what() << std::endl;
                }
           
                // set.insert(std::stoi(tmp[i], 0, 2));
            }
            _index[tmp[0]] = set;
        }
        ifs.close();
    }
}

void Dictionary::showDict(const vector<pair<string, int>> dict)const  //查看词典，作为测试用
{
    cout<<"showDict()"<<endl;
    for(int i=0; i<dict.size();i++)
    {
        cout<< dict[i].first <<"  " << dict[i].second<< endl;
    }
}
void Dictionary::showIndex(const map<string, set<int>> index) const
{
     cout<<"showIndex()"<<endl;
    for (const auto& [key, value] : index)
    {
        cout << '[' << key << "]  " ;
        for(auto &e: value)
        {
            cout<< e<<" ";
        }
    
        cout<< endl;
    }
       
}

void Split(string str, char separator, vector<string> &output)
{
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++)
    {
        // If we reached the end of the word or the end of the input.
        if (str[i] == separator || i == str.size())
        {
            endIndex = i;
            string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            output.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
}

