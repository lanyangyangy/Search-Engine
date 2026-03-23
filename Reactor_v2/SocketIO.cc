#include "SocketIO.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
 SocketIO::SocketIO(int fd):_fd(fd)
{

}
SocketIO::~SocketIO()
{
    close(_fd);
}
int SocketIO::readn(char *buf, int len)
{
    int left = len;
    char* arr = buf;
    int ret = 0;
    while(left > 0)
    {
        ret = read(_fd,arr,left);
        if(-1 == ret && EINTR == errno)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("read error -1");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            left -= ret;
            arr -= ret;
        }

    }
    return len - left;
}
int SocketIO::readLine(char *buf, int len)
{
    int left = len-1;
    char* arr = buf;
    int ret = 0, total = 0;
    while(left > 0)
    {
        ret = recv(_fd,arr,left,MSG_PEEK);
        if(-1 == ret && EINTR == errno)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("read error -1");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
           for(int i=0; i< ret; ++i)
           {
                if(*(arr+i) == '\n')
                {
                    int sz = i + 1;
                    readn(arr, sz);
                    arr += sz;
                    *arr = '\0';//C风格字符串以'\0'结尾

                    return total + sz;
                }
           }
            readn(arr, ret);
            left -= ret;
            arr += ret;
            total += ret;
        }
   
    }
    *arr = '\0';
    return total - left;

}
int SocketIO::writen(const char *buf, int len)
{

    int left = len;
    const  char* arr = buf;
    int ret = 0;
    while(left > 0)
    {
        ret = write(_fd,arr,left);
        if(-1 == ret && EINTR == errno)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("write error -1");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            left -= ret;
            arr -= ret;
        }

    }
    return len - left;

}