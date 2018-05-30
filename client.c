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
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));//将char型的字符串转换为整形的
    //in_addr用来表示一个IP地址，其实就是一个32位的数
    //inet_addr功能是将一个点分十进制的IP转换为长整数型
    //且它返回的地址已经是网络字节序列了，所以不用htonl函数
    server.sin_addr.s_addr = inet_addr(argv[1]);

    char buf[1024];
    struct sockaddr_in peer;
    while(1)
    {
        socklen_t len = sizeof(peer);
        buf[0] = 0;//初始化
        printf("please enter#");
        fflush(stdout);
        ssize_t s = read(0,buf,sizeof(buf)-1);
        if(s < 0)
        {
            printf("read error\n");
            return 3;
        }
        buf[s-1] = 0;
        sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&server,sizeof(server));
        //int sendto(socket s,const void *msg,int len,unsigned int flags,
        //const struct sockaddr *to,int tolen) 
        ssize_t q = recvfrom(sock,buf,sizeof(buf)-1,0,
                             (struct sockaddr *)&peer,&len);
        if(q > 0)
        {
            buf[q] = 0;
            printf("perr return %s\n",buf);
        }
    }
    close(sock);
    return 0;

}


