#include "xml.h"

Xml::Xml(const string& path, SplitTool* tool):_path(path),_all(),_quchong(),_tool(tool)
{

    jiexi(_path);
    write();
    bianli();
    cout<<11111;
    word_freq();
    compute_idf();
    compute_w();
    compute_invertindex();

}
void Xml::jiexi(const string& dir)
{
    string curr;
    if(dir.front()=='.')
    {
        curr = fs::current_path();
        curr = curr + dir.substr(1);
    }
    else
    {
        curr = dir;
    }
    for (auto& i : fs::directory_iterator(curr)) {
        jiexi_single(i.path().string());
    }

}
void Xml::jiexi_single(const string & path)
{
    //新建一个xml文件
	// 定义一个TiXmlDocument类指针
	TiXmlDocument* tinyXmlDoc = new TiXmlDocument();
    if (!tinyXmlDoc->LoadFile(path.c_str()))
	{
		cout << "无法加载xml文件！" << endl;

		exit;
	}

    TiXmlElement * rss = tinyXmlDoc->RootElement();
    TiXmlElement * channel = rss->FirstChildElement("channel");
    TiXmlElement*  pItem = channel->FirstChildElement("item");
    vector<map<string,string>> out;
    if(pItem)
    {
        
        for (; pItem != NULL; pItem = pItem->NextSiblingElement("item")) {

		// 解析属性
        TiXmlElement* titleNode = pItem->FirstChildElement("title");
        TiXmlElement* linkNode  = pItem->FirstChildElement("link");
        TiXmlElement* descNode  = pItem->FirstChildElement("description");
        string title = (titleNode && titleNode->GetText()) ? titleNode->GetText() : descNode->GetText();
        string url  = (linkNode && linkNode->GetText())   ? linkNode->GetText()  : "【无链接】";
        string content  = (descNode && descNode->GetText())   ? descNode->GetText()  : "【无内容】";
        
        while(1)          //删除<p>等格式化文字
        {   
        // 建议用 size_t 接收 find 的返回值，这是最规范的写法
        size_t m = content.find('<'); 
        if(m != std::string::npos)
        {
            // 关键修复 1：从 m 的位置开始往后找 '>'
            size_t n = content.find('>', m); 
                
                if(n != std::string::npos)
                {
                    // 关键修复 2：长度加 1，把 '>' 一并抹除！
                    content.erase(m, n - m + 1); 
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
        map<string,string> doc;
        doc["url"] = url;
        doc["title"] = title;
        doc["content"] = content;

        _split_words.push_back( {content, _tool->cut(content)} );  //进行分词存储为后续倒排做准备
        
         _all.push_back(doc);
	    }
    }


}



void Xml::write()
{
    // ofstream offset;
    // ofstream outfile;
    // offset.open("./data/offset.dat", std::ios::out);
    // outfile.open("./data/ripepage.dat", std::ios::out);

    for(int i=0; i< _all.size();i++)
    {
        
        // offset << i <<" "<< _nums <<" ";
        int docid = i+1;
        string title = _all[i]["title"];
        string url = _all[i]["url"];
        string content= _all[i]["content"];
        
        _fmtTxt.push_back( "<doc><docid>" + std::to_string(docid) +
                        "</docid><url>" + url+
                        "</url><title>" + title +
                        "</title><content>" + content +
                        "</content></doc>"   );
        
        // outfile<<_fmtTxt[i];
        
        // _nums += _fmtTxt[i].size();
        // offset<< _fmtTxt[i].size()<<endl;
            
    }
    // offset.close();
    // outfile.close();
        
}

uint64_t  Xml::get_simhash(const string& word)
{
    return _quchong.compute(word);
}
void Xml::bianli()
{
    cout<<"bianli"<<endl;
    for(int i =0; i< _fmtTxt.size(); i++)
    {
        _simhash_all[_fmtTxt[i]] = get_simhash(_fmtTxt[i]);
        auto  _find = _sim_str.find(_simhash_all[_fmtTxt[i]]);
        if(_find != _sim_str.end())
        {
            if( _find->second.size() < _fmtTxt[i].size())
            {
                _find->second = _fmtTxt[i];
            }
        }
        else{
            _sim_str[_simhash_all[_fmtTxt[i]]] = _fmtTxt[i];
        }
    }
    ofstream offset;
    ofstream outfile;
    offset.open("./data/offset.dat", std::ios::out);
    outfile.open("./data/ripepage.dat", std::ios::out);
    _nums=0;
    int i=0;
    for (map<u_int64_t,string>::iterator it = _sim_str.begin(); it != _sim_str.end(); ++it) {
        _str_xu[it->second] = i;
        outfile<<it->second;
        offset << i++ <<" "<< _nums <<" ";
        _nums += it->second.size();
        offset<< it->second.size()<< endl;
    } 
    offset.close();
    outfile.close();
}

void Xml::word_freq()
{
    for (map<u_int64_t,string>::iterator it = _sim_str.begin(); it != _sim_str.end(); ++it)
    {
        set<string> tmp;
        vector<string> split = _tool->cut(it->second); 
        for(auto i=split.begin(); i!= split.end(); )
        {
            if(i->size() == 1&&  'a' < (*i)[0] && 'z'>( *i)[0])
            {
                i = split.erase(i);
            }
            else
            {
                i++;
            }
        }
        map<string,int>  freq;
        for(int i=0; i<split.size(); i++)
        {
            
            if(!is_fuhao(split[i].front()))
            {
                if( freq.find(split[i]) != freq.end())     freq.find(split[i])->second++;
                else   freq[split[i]] = 1;
            
                tmp.insert(split[i]);
                
            }
        }
        for (set<string>::iterator qa = tmp.begin(); qa != tmp.end(); ++qa)
        {
            if( _df.find(*qa) != _df.end())  _df.find(*qa)->second++;
            else                                   _df[*qa] = 1;
        }
        _tf.push_back({ it->first, freq});    
    } 
  
    _N = _sim_str.size();

    ofstream outfile;
    outfile.open("./data/count.dat", std::ios::out);
    for(map<string,int>::iterator qa = _df.begin(); qa != _df.end(); ++qa)
    {
        outfile<< qa->first<<" "
                << qa->second<<endl;
    }
    outfile.close();

} 

void Xml::compute_idf()
{
    cout<<"compute_idf()"<<endl;
    for (map<string, int>::iterator it = _df.begin(); it != _df.end(); ++it)
    {
        double num = log2(_N/(it->second +1));
        // _idf[it->first] = log2(_N/(it->second +1));
        if(num>1)
        {
           _idf[it->first] =num;
        }
        
    }
}

void Xml::compute_w()
{
    cout<<"compute_w()"<<endl;
    for(int i=0; i<_tf.size(); i++)
    {
        double all_w =0 ; //存储一篇文章的所有w，方便归一化 
        std::map<std::string, double> count;
        for(map<string, int>::iterator it = _tf[i].second.begin(); it != _tf[i].second.end(); ++it)
        {
           
            count[it->first] = it->second * _idf.find(it->first)->second;
            all_w += pow(count[it->first],2);
       
        }
       
        for(map<string, double>::iterator it = count.begin(); it != count.end(); ++it)
        {
           
            count[it->first] = count[it->first]/sqrt(all_w);

        }

        _w.push_back({_str_xu[_sim_str[_tf[i].first] ], count});    
    }
   

}
void Xml::compute_invertindex()
{
     cout<<"compute_invertindex"<<endl;
    for(int i=0; i< _w.size(); i++)
    {
        // int xuhao =  _str_xu[_sim_str[_w[i].first] ];
        std::vector<std::pair<int, double>> tmp;
        for(auto it = _w[i].second.begin(); it != _w[i].second.end(); ++it)
        {
          
                InvertIndexTable[it->first].push_back({_w[i].first,it->second});
        }
                
    }
    ofstream outfile;
    outfile.open("./data/InvertIndex.dat", std::ios::out);
    for(auto it = InvertIndexTable.begin(); it != InvertIndexTable.end(); it++)
    {

        outfile << it->first<<' ';
        for(int i=0; i< it->second.size(); i++)
        {
           
            outfile<< it->second[i].first<<' '
                    << it->second[i].second;
        }
          outfile<<endl;
        
    }
    outfile.close();
}   



bool is_fuhao(unsigned char c) {
return c <= 0xE4&&c >= 0xE3;
}