// #include "DictProducer.h"
// #include "cppjieba/Jieba.hpp"
#include "SplitTool.h"
// #include "KeyRecommander.h"
// #include "Reactor/EchoServer.h"
// #include "KeyAche.h"
#include "quchong.h"
#include "xml.h"

// int main(int argc, char **argv)
// {
//     // SplitToolCppJieba* tool = new SplitToolCppJieba();
//     // DictProducer dict("./data/yuliao",tool);
//     // KeyRecommander key("hello",nullptr);
    
//     EchoServer server(4, 10, "127.0.0.1", 1234);
//     server.start();

//     return 0;
// }

int main()
{
    SplitToolCppJieba* tool = new SplitToolCppJieba();
    Xml _xml("/home/lanlan/program/data/yuliao2",tool);
    // Quchong quchong;
    // string s("我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，走上人生巅峰。");
    // quchong.compute(s);
    
}