#include "DictProducer.h"
#include "cppjieba/Jieba.hpp"
#include "SplitTool.h"
#include "KeyRecommander.h"
string getFiles()  //获取文件的绝对路径
{
    return fs::current_path(); 
}
int main()
{
    
    // SplitToolCppJieba* tool = new SplitToolCppJieba();
    // DictProducer dict("./data/yuliao",tool);
    KeyRecommander key("hello");
    key.show();


    return 0;
}
// #include "DictProducer.h"
// #include "cppjieba/Jieba.hpp"
// #include "SplitTool.h"
// #include "KeyRecommander.h"
// string getFiles()  //获取文件的绝对路径
// {
//     return fs::current_path(); 
// }
// int main()
// {
//     SplitToolCppJieba _tool();
//     DictProducer _dict("../data/yuliao",&_tool);
//     KeyRecommander key("hello");
//     key.show();
// //     }



//     return 0;
// }




