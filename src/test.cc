// #include "DictProducer.h"
// #include "cppjieba/Jieba.hpp"
// #include "SplitTool.h"
// #include "KeyRecommander.h"
// #include "Reactor/EchoServer.h"
// #include "KeyAche.h"
#include "quchong.h"
#include "xml.h"
#include "Search.h"
#include "Reactor/EchoServer.h"
// int main(int argc, char **argv)
// {
//     // SplitToolCppJieba* tool = new SplitToolCppJieba();
//     // DictProducer dict("./data/yuliao",tool);
//     // KeyRecommander key("hello",nullptr);
    
//     EchoServer server(4, 10, "127.0.0.1", 1234);
//     server.start();

//     return 0;
// }
void string_json(const string& json_str)
{
    json j = json::parse(json_str);
    
    cout<< j["title"]<<endl;
    cout<< j["content"]<< endl;

}
using std::cin;
int main()
{

    // Search* search1 = new Search();
    // string word;
    // while(cin>>word)
    // {
    //     string wwww = search1->doing(word);
    //     string_json(wwww);
    // }
    EchoServer server(4, 10, "127.0.0.1", 1234);
    server.start();

}