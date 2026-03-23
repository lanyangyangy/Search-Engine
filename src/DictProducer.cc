#include "DictProducer.h"

DictProducer::DictProducer(const string& dir,SplitTool* SplitTool):_tool(SplitTool)   //构造函数  处理英文
{
    doFiles(dir);

    pushStopWord(_dir_now + "/stop_words_eng.txt",0);
    pushStopWord(_dir_now + "/stop_words_zh.txt",true);
    readfile();
    deal_eng_words();
    deal_ch_words();

    
    cout<<endl;

}


void DictProducer::readfile()
{
    ifstream ifs;
    for(int i=0; i < _files.size(); i++ )
    {
        
        std::ifstream ifs;
        ifs.open(_files[i]);
        if (!ifs.is_open()) 
        {
            std::cerr << "Failed to open file: " << _files[i] << std::endl;
        } 
        else 
        {
            std::string line;
            while (std::getline(ifs, line)) {   // 每次读取一行（不含换行符）
                for(int i=0; i< line.size(); )
                {
                    if(isChineseChar(line[i]))
                    {
                        
                        _ch_words.append(line.substr(i,3));
                        i +=3;
                    }
                    else
                    {
                        _eng_words.push_back(line[i++]);
                    }
                }
            }
            ifs.close();
        }
    } 
}
void DictProducer::deal_eng_words() //处理英文文章
{
    ClearEngWords(_eng_words);   
    buildEnDdict();
    buildEnIndex();
    storeDict("./data/dict.dat",_eng_dict);
    storeIndex("./data/dictIndex.dat",_eng_index);

}
void DictProducer::deal_ch_words()  //处理中文文章
{
    buildCnDict();
    buildChIndex();
    storeDict("./data/dict.dat",_ch_dict);
    storeIndex("./data/dictIndex.dat",_ch_index);
}

void DictProducer::ClearEngWords(string& words) //清洗数据
{   
    words.erase(std::remove_if(words.begin(), words.end(), ispunct), words.end()); //去除标点符号
    transform(words.begin(),words.end(),words.begin(),::tolower);
}
vector<string> DictProducer::ClearChWords() //清洗数据
{
    vector<string> tmp = _tool->cut(_ch_words);
    for(std::vector<string>::iterator words = tmp.begin(); words != tmp.end();)
    {
        if(!isChineseWord((*words)[0]))
        {
            words = tmp.erase(words);
        }
        else{
            ++words;
        }
    }
    return tmp;
}

void DictProducer::buildEnDdict()  //创建英文字典
{   
    istringstream iss(_eng_words);  //放入字典
    string tmp;
    while(std::getline(iss,tmp,' '))
    {
        pushDict(tmp,_eng_stop_worlds, _eng_dict);
    }
   
}
void DictProducer::buildCnDict( )  //创建中文字典
{
    vector<string> words= ClearChWords();
    
    for(auto word:words )
    {
        pushDict(word,_ch_stop_worlds,_ch_dict);
    }
    
}
void DictProducer::buildChIndex()  //创建中文索引
{
    for(int i=0; i< _ch_dict.size(); i++)
    {
        pushIndex(_ch_dict[i].first,_ch_index,i);
    }  
}
void DictProducer::buildEnIndex()  //创建英文索引
{
    for(int i=0; i< _eng_dict.size(); i++)
    {
        pushIndex(_eng_dict[i].first,_eng_index,i);
    }  
}


void DictProducer::storeDict(const string filepath, const vector<pair<string, int>> dict)  //将词典写入文件
{

    ofstream oss;
    oss.open(filepath,std::ios::out|std::ios::app);
    for(int i=0; i<dict.size(); i++)
    {
        oss <<dict[i].first << " "<<dict[i].second<<"\n";
    }
    oss.close();
   
}
void DictProducer::storeIndex(const string filepath,const map<std::string, std::set<int>> index)
{
    ofstream oss;
    oss.open(filepath,std::ios::out|std::ios::app);
    cout<< filepath<<endl;
    for (const auto& [key, value] : index)
    {
        oss << key << " " ;
        for(auto &e: value)
        {
            oss<< e<<" ";
        }
        oss<<endl;
    }
    oss.close();
}

void DictProducer::doFiles(const string& dir)//处理文件路径
{
    if(dir.front()=='.')
    {
        _dir_now = getFiles()+dir.substr(1);
    }
    else{
        _dir_now =dir;
    }
    for (auto& i : fs::directory_iterator(_dir_now)) {
        if(i.path().string().find("english") != std::string::npos )
        {
        _files.push_back(i.path().string());
        }
        if(i.path().string().find("chinese") != std::string::npos )
        {
        _files.push_back(i.path().string());
        }
    }
}

string DictProducer::getFiles()  //获取文件的绝对路径
{
    return fs::current_path(); 
}
void DictProducer::pushDict(const string& word,const set<string> stop_worlds, vector<pair<string, int>>& dict)//存储某个单词
{
    if(stop_worlds.find(word) == stop_worlds.end())
    {
        for(int i = 0; i< dict.size(); i++)
        {
            if(word == dict[i].first)
            {
                dict[i].second++;
                return;
            }
        }
        dict.push_back({word,1});
        return;
    }
}

void DictProducer::pushIndex(const string& word,map<string, set<int>>& index,const int idx)//存储某个词典索引
{
    for(int i=0; i< word.size(); i++)  //遍历单词中的每一个元素存储位置
    {
        if(isChineseWord(word[i]))
        {
        
            index[word.substr(i,3)].insert(idx);
            i+=2;
        }
        else
        {
            index[string(1,word[i])].insert(idx);
        }
       
    }
}
void DictProducer::pushStopWord(const string& stop_world_dir,bool is_ch)  //存放暂停词
{
    set<string>* stop_worlds;
    if(is_ch)
    {
        stop_worlds = &this->_ch_stop_worlds;
    }
    else
    {
        stop_worlds = &this->_eng_stop_worlds;
    }
    ifstream stop_worlds_file(stop_world_dir);
    if (! stop_worlds_file.is_open())
        { cout << "Error opening file"; exit (1); }

    string line;
    while (std::getline(stop_worlds_file, line)) {
        // 去除行末尾的 '\r'（如果存在）
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();   // 或者 line.erase(line.size() - 1);
    }
    // 跳过空行
    if (!line.empty()) {
        stop_worlds->insert(line);   
    }
    }
}

bool isChineseChar(unsigned char c) {
return (c >= 0xE3 && c <= 0xE9);

}
bool isChineseWord(unsigned char c) {
return (c >= 0xE4 && c <= 0xE9);

}

void show_string(const string& words)
{
  for(int i=0; i< words.size();)
  {     if(isChineseChar(words[i]))
        {
            cout<< words.substr(i,3);
            i+=3;
        }
        else{
            cout<< words[i];
            i++;
        } 

  }

}

void DictProducer::showFiles()const  //查看文件路径，作为测试用
{
    for(auto &i : _files)
    {
        cout<<i <<endl ;
    }

}
void DictProducer::showDict(const vector<pair<string, int>> dict)const  //查看词典，作为测试用
{
    cout<<"showDict()"<<endl;
    for(int i=0; i<dict.size();i++)
    {
        cout<< dict[i].first <<"  " << dict[i].second<< endl;
    }
}
void DictProducer::showIndex(const map<string, set<int>> index) const
{
     cout<<"showIndex()"<<endl;
    for (const auto& [key, value] : index)
    {
        cout << '[' << key << "]  " ;
        for(auto &e: value)
        {
            cout<< e<<" ";
        }
    
        
    }
       
}
