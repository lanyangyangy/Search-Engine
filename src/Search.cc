#include "Search.h"

Search::Search( ):_quchong()
{
    read_InvertIndex();
}


void Search::read_InvertIndex()
{

    ifstream ifs;
    ifs.open("/home/lanlan/program/data/InvertIndex.dat", std::ios::in);
    if(ifs.is_open())
    {
        string myText;
        while (getline(ifs, myText)) 
        {
        // 输出文件中的文本
            if(!myText.empty())
            {
                stringstream ss(myText);
                std::vector<std::string> words;
                std::string word;
                
                while (std::getline(ss, word, ' ')) {
                words.push_back(word);
                } 
                _df[words[0]] = 0;
                for(int i=1; i<words.size(); i+=2)
                {
                
                    _InvertIndexTable[words[0]][std::stoi(words[i])]= std::stod(words[i+1]);
                    if(std::stoi(words[i])>_N)
                    {
                        _N =std::stoi(words[i]);
                    }
                    _df[words[0]]++;
                }

            }
        }
     
    }
    else 
    {
        cout<<"open InvertIndex.dat error"<<endl;
        exit;
    }

}
string Search::doing(const string& words)
{

    string _words = words;  //传进来的关键词

    vector<string> _splited_word = split_word(_words);//分完后的词
    map<string,int> _tf;
    compute_tf(_tf, _splited_word);

    map<string,double> _idf = compute_idf(_df);//逆文档频率s
    map<std::string, double> _w = compute_w( _tf, _idf);//权重
    set<int> _docid = pipei_key(_splited_word);//匹配的文章id
    vector<pair<int, map<string, double>>> _doc_w = get_w(_splited_word, _docid);
    int _id = select( _w,  _doc_w);

    pair<int,int> out =  chazhao(_id);  //存放文章的起点和大小；
    string res = fund(out);
    string json_string = to_Json(res);
   
    return json_string;
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

void Search::compute_tf(map<string,int>& _tf, const vector<string>& _splited_word)
{
    
    for(int i=0; i<_splited_word.size(); i++)
    {
        
        if(!is_fuhao(_splited_word[i].front()))
        {
            if( _tf.find(_splited_word[i]) != _tf.end())     _tf.find(_splited_word[i])->second++;
            else   _tf[_splited_word[i]] = 1;
           
        }
    }
       
}
map<string, double> Search::compute_idf( map<string,int> df)
{
    map<std::string, double> _idf;
    for (map<string, int>::iterator it = df.begin(); it != df.end(); ++it)
    {
        double num = log2(_N/(it->second +1));
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
        // cout<< _idf.find(it->first)->second;
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

vector<pair<int, map<string, double>>> Search::get_w(vector<string> word, set<int> docid)
{
    vector<pair<int, map<string, double>>> _doc_w;
    for(auto i= docid.begin(); i != docid.end(); i++)
    {
        map<std::string, double> _w;
        for(int j=0; j<word.size(); j++)
        {
            _w[word[j]] = _InvertIndexTable[word[j]][*i];
        } 
        _doc_w.push_back({*i,_w});
    }
    return _doc_w;
}
int Search::select(map<std::string, double> _w, vector<pair<int, map<string, double>>> _doc_w)
{
    // vector<int> nums;
    int id;//最大值对应的id
    for(int i=0 ; i<_doc_w.size(); i++)
    {
        auto it_a = _w.begin();
        auto it_b = _doc_w[i].second.begin(); 
        double x_y = 0;
        double sum_x = 0;
        double sum_y = 0;
        double max_cos;//最大值
        
        for(; it_a != _w.end() && it_b != _doc_w[i].second.end(); ++it_a, ++it_b)
        {
            
            x_y += it_a->second * it_b->second;
            sum_x += pow(it_a->second,2);
            sum_y += pow(it_b->second,2);
        }
        sum_x = sqrt(sum_x);
    
        sum_y = sqrt(sum_y);
        double cos = x_y/(sum_x*sum_y);

        if(cos > max_cos)
        {
            max_cos = cos;
            id = _doc_w[i].first;
        }


    }
    return id;
}

pair<int,int> Search::chazhao(int id)
{
    ifstream offset;
    offset.open("./data/offset.dat", std::ios::in);
    if(!offset.is_open())
    {
        cerr<< "Open offset.dat failure" <<endl;
    }
    else
    {
        string myText;
        while (getline(offset, myText)) 
        {
        // 输出文件中的文本
            if(!myText.empty())
            {
                stringstream ss(myText);
                std::vector<std::string> words;
                std::string word;
                
                while (std::getline(ss, word, ' '))
                {
                    words.push_back(word);
                } 
                if(std::stoi(words[0]) == id)
                {
                    return make_pair(stoi(words[1]), stoi(words[2]));
                }
                

            }
        }
    }

}

string Search::fund(pair<int,int> out)
{
    string buffer(out.second, '\0');

    ifstream ifs;
    ifs.open("./data/ripepage.dat", std::ios::in);
    if(!ifs.is_open())
    {
        cerr<< "Open offset.dat failure" <<endl;
    }
    else
    {
        
        ifs.seekg(out.first, std::ios::beg);
        if (!ifs)
        {   ifs.close();
            std::cerr << "Failed to seek to position " << out.first << std::endl;
            exit;
        }
       
        ifs.read(buffer.data(), out.second);
      
        streamsize bytes_read = ifs.gcount();

       
        if (bytes_read < out.second) {
            buffer.resize(bytes_read);
        }

    }
    return buffer;
}

string Search::to_Json(const string& words)
 {
    TiXmlDocument* tinyXmlDoc = new TiXmlDocument();
    tinyXmlDoc->Parse(words.c_str());

    
    TiXmlElement * doc = tinyXmlDoc->RootElement();
    TiXmlElement * titleNode = doc->FirstChildElement("title");
    TiXmlElement * descNode = doc->FirstChildElement("content");

    string title = (titleNode && titleNode->GetText()) ? titleNode->GetText() : "【无标题】";
    string content  = (descNode && descNode->GetText())   ? descNode->GetText()  : "【无内容】";


    json j;
    j["content"] = content;
    j["title"] = title;
    delete tinyXmlDoc;
    return j.dump();
 }

