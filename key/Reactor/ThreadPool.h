#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.h"
#include <vector>
#include <thread>
#include <functional>
#include <utility> 
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <cstring>


namespace fs = std::filesystem;
using std::endl;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::set;
using std::map;
using std::pair;
using std::transform;
using std::istringstream;
using std::vector;
using std::string;
using std::getline;
using std::thread;
using std::function;

using Task = function<void()>;

class ThreadPool
{
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    //线程池的启动与停止
    void start();
    void stop();

    //任务的添加与获取
    void addTask(Task &&task);

private:
    Task getTask();
    //线程池交给工作线程执行的任务，也就是线程入口函数
    void doTask();

private:
    size_t _threadNum;//子线程的数目
    vector<thread> _threads;//存放子线程的容器
    size_t _queSize;//任务队列的大小
    TaskQueue _taskQue;//任务队列
    bool _isExit;//标识线程池是否退出的标志位

    vector<pair<string, int>> _dict;   //英文词典
    map<string, set<int>> _index;   //英文词典索引
    // vector<pair<string, int>> _ch_dict;   //中文词典
    // map<string, set<int>> _ch_index;   //中文词典索引

    // void readfiles();
    // void readdict();
    // void readindex();
    // void showDict(const vector<pair<string, int>> dict)const;  //查看词典，作为测试用
    // void showIndex(const map<string, set<int>> index)const;//查看索引
};

#endif
