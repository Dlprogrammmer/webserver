#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H
#include<sys/epoll.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include"locker.h"
#include<sys/uio.h>
class http_conn
{
private:
    int  m_sockfd;//http连接的socket
    sockaddr_in m_address;//通信的socket地址
public:
static int m_epollfd;//所有socket上事件注册到同一个epoll对象
static int m_user_count;//统计用户数量
    http_conn();
    ~http_conn();
    void process();//处理客户端请求
    void init(int sockfd,const sockaddr_in &addr);//初始化新连接
    void close_conn();//关闭连接
     bool read();
     bool write();
};


#endif
