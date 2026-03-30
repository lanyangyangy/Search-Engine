#include "Search.h"

Search::Search( ):_quchong(),_InvertIndexTable(read_InvertIndex())
{

}


std::unordered_map<std::string, std::vector<std::pair<int, double>>> Search::read_InvertIndex()
{
    std::unordered_map<std::string, std::vector<std::pair<int, double>>>  InvertIndexTable;
    ifstream ifs;
    ifs.open("./data/InvertIndex.dat", std::ios::in);
    if(ifs.is_open())
    {
        string myText;
        while (getline(ifs, myText)) {
        // 输出文件中的文本
        stringstream ss(myText);
        std::vector<std::string> words;
        std::string word;

        while (std::getline(ss, word, ' ')) {
        words.push_back(word);
        } 
        for(int i=1; i<words.size(); i+=2)
        {
            InvertIndexTable[words[0]].push_back({std::stoi(words[i]),std::stod(words[i+1])});
        }

        }
    }
    else 
    {
        cout<<"open InvertIndex.dat error"<<endl;
        exit;
    }
    return  InvertIndexTable;
}
void Search::doing(const string& words)
{
    string _words = words;  //传进来的关键词
    vector<string> _splited_word = split_word(_words);//分完后的词
    map<string,int> _tf;
    map<string,int> _df;
    compute_tdf(_tf,_df, _splited_word);
    map<string,double> _idf = compute_idf(_df);//逆文档频率s
    map<std::string, double> _w = compute_w( _tf, _idf);//权重
    set<int> _docid = pipei_key(_splited_word);//
}
vector<string> Search::split_word(const string & word)
{
    std::vector<std::string> splited_word;
    vector<pair<string, double>> key = _quchong.get_key(word); 
    for(int i=0; i< key.size(); i++)
    {
        splited_word.push_back(key[i].first);
    }
    for(auto i=splited_word.begin(); i!= splited_word.end(); )
        {
            if(i->size() == 1&&  'a' < (*i)[0] && 'z'>( *i)[0])
            {
                i = splited_word.erase(i);
            }
            else
            {
                i++;
            }
        }
    return splited_word;
}

void Search::compute_tdf(map<string,int>& _tf, map<string,int>& _df,const vector<string>& _splited_word)
{
    
    for(int i=0; i<_splited_word.size(); i++)
    {
        
        if(!is_fuhao(_splited_word[i].front()))
        {
            if( _tf.find(_splited_word[i]) != _tf.end())     _tf.find(_splited_word[i])->second++;
            else   _tf[_splited_word[i]] = 1;
        
            _df.insert({_splited_word[i], 1});
            
        }
    }
       
}
map<string, double> Search::compute_idf( map<string,int> _df)
{
    map<std::string, double> _idf;
    for (map<string, int>::iterator it = _df.begin(); it != _df.end(); ++it)
    {
        double num = log2(_words.size()/(it->second +1));
   
        if(num>1)
        {
           _idf[it->first] =num;
        }
        
    }
    return _idf;
}
std::map<std::string, double> Search::compute_w(map<string,int> _tf, map<string,double> _idf)
{
    std::map<std::string, double> _w;
    double all_w =0 ; //存储一篇文章的所有w，方便归一化 
    for(map<string, int>::iterator it = _tf.begin(); it != _tf.end(); ++it)
    { 
        _w[it->first] = it->second * _idf.find(it->first)->second;
        all_w += pow(_w[it->first],2);
    }
    
    for(map<string, double>::iterator it = _w.begin(); it != _w.end(); ++it)
    { 
        _w[it->first] = _w[it->first]/sqrt(all_w);
    }
    return _w;    

}


set<int> Search::pipei_key(vector<string> _splited_word)
{
    // 1. 防御性编程：如果没传词，直接返回空
    if (_splited_word.empty()) {
        return {};
    }

    // 2. 提取第一个词的文章集合，作为交集的“底子”
    auto it0 = _InvertIndexTable.find(_splited_word[0]);
    if (it0 == _InvertIndexTable.end()) {
        // 因为必须包含【所有】词，只要第一个词没找到，后续直接不用算了！
        cout << "没有查询的信息: " << _splited_word[0] << endl;
        return {}; 
    }

    // 将第一个词的文章 ID（tmp->second[j].first）存入 set 中自动去重排序
    set<int> current_ans;
    for (const auto& item : it0->second) {
        current_ans.insert(item.first);
    }

    // 3. 从第二个词开始，逐个与 current_ans 求交集
    for (size_t i = 1; i < _splited_word.size(); ++i) 
    {
        auto tmp = _InvertIndexTable.find(_splited_word[i]);
        if (tmp == _InvertIndexTable.end()) {
            cout << "没有查询的信息: " << _splited_word[i] << endl;
            return {}; // 只要有一个词断层，整个交集直接为空，光速下班！
        }

        // 把当前词的文章 ID 提取出来
        set<int> next_word_docs;
        for (const auto& item : tmp->second) {
            next_word_docs.insert(item.first);
        }

        // 准备一个临时集合存放这一轮的交集结果
        set<int> intersection_result;
        std::set_intersection(
            current_ans.begin(), current_ans.end(),
            next_word_docs.begin(), next_word_docs.end(),
            std::inserter(intersection_result, intersection_result.begin())
        );

        // 把交集结果更新给 current_ans，继续和下一个词比较
        current_ans = intersection_result;

        //如果在中途交集已经变成 0 篇文章了，直接跳出循环
        if (current_ans.empty()) {
            break;
        }
    }

    
    return current_ans;
}