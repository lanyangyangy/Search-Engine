#include "DictProducer.h"

DictProducer::DictProducer(const string& dir)   //构造函数  处理英文
{
    doFiles(dir);
    pushStopWord();
    buildEnDdict();
    buildEnIndex();
    storeDict( getFiles()+"/data/dict.dat");
    storeIndex(getFiles()+"/data/dictIndex.dat");

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
    for (auto& i : fs::directory_iterator(dir)) {
        if(i.path().string().find("english") != std::string::npos )
        {
        _files.push_back(i.path().string());
        }
    }
}
DictProducer::DictProducer(const string& dir, SplitTool* splitTool)//构造函数,专为中文处理
{

}
void DictProducer::buildEnDdict()  //创建英文字典
{
    readfile();
   
}
void DictProducer::buildEnIndex()  //创建英文索引
{
    for(int i=0; i< _eng_dict.size(); i++)
    {
        pushIndex(_eng_dict[i].first,i);
    }  
}
void DictProducer::buildCnDict(const char* filepath)  //创建中文字典
{

}

void DictProducer::storeDict(const string filepath)  //将词典写入文件
{

    ofstream oss(filepath);
    for(int i=0; i<_eng_dict.size(); i++)
    {
        oss <<_eng_dict[i].first << " "<<_eng_dict[i].second<<"\n";
    }
    oss.close();
   
}
void DictProducer::storeIndex(const string filepath)
{
    // vector<pair<string, int>> _eng_dict;   //词典
    // map<string, set<int>> _eng_index;   //词典索引
    ofstream oss(filepath);
    cout<< filepath<<endl;
    for (const auto& [key, value] : _eng_index)
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



void DictProducer::showFiles()const  //查看文件路径，作为测试用
{
    for(auto &i : _files)
    {
        cout<<i;
    }
    
}
void DictProducer::showDict()const  //查看词典，作为测试用
{
    cout<<"showDict()"<<endl;
    for(int i=0; i<_eng_dict.size();i++)
    {
        cout<< _eng_dict[i].first <<"  " << _eng_dict[i].second<< endl;
    }
}
void DictProducer::showIndex() const
{
     cout<<"showIndex()"<<endl;
    for (const auto& [key, value] : _eng_index)
    {
        cout << '[' << key << "]  " ;
        for(auto &e: value)
        {
            cout<< e<<" ";
        }
        cout<<endl;
    }
       
}
string DictProducer::getFiles()  //获取文件的绝对路径
{
    return fs::current_path(); 
}
void DictProducer::pushDict(const string& word)//存储某个单词
{
    if(_eng_stop_worlds.find(word) == _eng_stop_worlds.end())
    {
        for(int i = 0; i< _eng_dict.size(); i++)
        {
            if(word == _eng_dict[i].first)
            {
                _eng_dict[i].second++;
                return;
            }
        }
        _eng_dict.push_back({word,1});
        return;
    }
}

void DictProducer::pushIndex(const string& word,int idx)//存储某个词典索引
{
    for(int i=0; i< word.size(); i++)  //遍历单词中的每一个元素存储位置
    {
        _eng_index[string(1,word[i])].insert(idx);
    }
}
void DictProducer::pushStopWord()
{
    string dir_stop_world = _dir_now + "/stop_words_eng.txt";
    ifstream stop_worlds_file(dir_stop_world);
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
        _eng_stop_worlds.insert(line);
    }
    }
}

void DictProducer::readfile()
{
    string words;
    ifstream ifs;
    for(int i=0; i < _files.size(); i++ )
    {
  
        std::ifstream ifs;
        ifs.open(_files[i]);
        if (!ifs.is_open()) {
            std::cerr << "Failed to open file: " << _files[i] << std::endl;
        } else {
            std::string line;
            while (std::getline(ifs, line)) {   // 每次读取一行（不含换行符）
                words.append(line);
            }
            ifs.close();
    }

        ClearWords(words);   

        istringstream iss(words);  //放入字典
        string tmp;
        while(std::getline(iss,tmp,' '))
        {
            pushDict(tmp);
        }

    }
}
void DictProducer::ClearWords(string& words) //清洗数据
{   
    words.erase(std::remove_if(words.begin(), words.end(), ispunct), words.end()); //去除标点符号
    transform(words.begin(),words.end(),words.begin(),::tolower);
}

bool isChineseChar(unsigned char c) {
return c >= 0xE0;
}