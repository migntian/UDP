#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        printf("usage %s [IP] [port]\n",argv[0]);//argv[0]是什么
        return 1;
    }
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock < 0)
    {
        printf("socket error\n");
        return 2;
    }
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[2]));//将char型的字符串转换为整形的
    //in_addr用来表示一个IP地址，其实就是一个32位的数
    //inet_addr功能是将一个点分十进制的IP转换为长整数型
    //且它返回的地址已经是网络字节序列了，所以不用htonl函数
    local.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0)
    {
        printf("bind error\n");
        return 3;
    }
    char buf[1024];
    struct sockaddr_in client;
    while(1)
    {
        socklen_t len = sizeof(client);
        buf[0] = 0;//初始化
        ssize_t s = recvfrom(sock,buf,sizeof(buf)-1,0,
                             (struct sockaddr *)&client,&len);
        //ssize_t recvfrom(int sockfd,void *buf,size_t len,unsigned int flag,
        //(struct sockaddr *)from,socket_t *fromlen)
        //recvfrom函数中参数
        //buf:接收缓冲区
        //len:缓冲区长度
        //flags:调用操作方式
        //from:指针，指向装有源地址的缓冲区，用(struct sockaddr*)强转
        //fromlen:指针，指向from缓冲区长度值
        if(s > 0)
        {
            buf[s] = 0;//相当于增加了一个\0，且这个数组本身就是char型
            //inet_ntoa(struct in_addr in)
            //将in_addr类型的参数转换为字符串
            printf("[%s|%d]:%s\n",inet_ntoa(client.sin_addr)
                   ,ntohs(client.sin_port),buf);
            sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&client,len);
            //int sendto(socket s,const void *msg,int len,unsigned int flags,
            //const struct sockaddr *to,int tolen) 
        }
    }
    close(sock);
    return 0;

}


