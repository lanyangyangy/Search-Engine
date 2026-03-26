#include "DictProducer.h"
#include "cppjieba/Jieba.hpp"
#include "SplitTool.h"
#include "KeyRecommander.h"
#include "Reactor/EchoServer.h"
#include "KeyAche.h"

int main(int argc, char **argv)
{
    // SplitToolCppJieba* tool = new SplitToolCppJieba();
    // DictProducer dict("./data/yuliao",tool);
    // KeyRecommander key("hello",nullptr);
    
    EchoServer server(4, 10, "127.0.0.1", 1234);
    server.start();

    return 0;
}

// int main(int argc, char **argv)
// {
//     SplitToolCppJieba* tool = new SplitToolCppJieba();
//     DictProducer dict("./data/yuliao",tool);
//     KeyAche ke();
//     KeyRecommander key("hello",nullptr,ke);
    
//     // EchoServer server(4, 10, "127.0.0.1", 1234);
//     // server.start();

//     return 0;
// }

// #include <iostream>
// #include <vector>
// #include <string>
// #include <sw/redis++/redis++.h>
// #include <nlohmann/json.hpp> // 引入 json 库

// using namespace std;
// using namespace sw::redis;
// using json = nlohmann::json;

// int main() {
//    KeyAche ke;
//    map<string,int> word = {{"hh",10},{"h",11}};
//    ke.add(make_pair("hello",word));
//     return 0;
// }