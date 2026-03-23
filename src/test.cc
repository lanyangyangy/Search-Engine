#include "DictProducer.h"
#include "cppjieba/Jieba.hpp"
#include "SplitTool.h"
string getFiles()  //获取文件的绝对路径
{
    return fs::current_path(); 
}
int main()
{
    SplitToolCppJieba tool;
    DictProducer dict("./data/yuliao",&tool);
//     SplitTool* tool = new SplitToolCppJieba();
//     string aaa= "我们采用简单的方式通过软连接使用";
//     vector<string> bbb;
// // git submodule init会将.git的配置进行初始化，会把 .gitmodules 文件中子模块的配置信息写入本地 .git/config后续就可以直接将库拉取下来
//     bbb = tool->cut(aaa);
//     for(int i=0;i<bbb.size();i++)
//     {
//         cout<<bbb[i]<<endl;
//     }



    return 0;
}


