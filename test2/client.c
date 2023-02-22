#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

int client_port = 1234;

int main(int argc,char **argv)
{
    int opt;

    char buf[BUFSIZ];

    int sock_c = socket(AF_INET,SOCK_STREAM,0);
    if(sock_c == -1)
        perror("socket()");

    struct sockaddr_in client_addr;
    
    client_addr.sin_family = AF_INET;

    client_addr.sin_port = htons(client_port);

    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret;

    ret= connect(sock_c,(struct sockaddr *)&client_addr,sizeof(client_addr));
    if(ret == -1)
        perror("connect()");

    sprintf(buf,"当前连接客户端为：%d",1);
    printf(buf);
    fflush(stdout);
    send(sock_c,buf,sizeof(buf),0);
    bzero(buf,sizeof(buf));

    while(1)
    {
        ret = read(fileno(stdin),buf,sizeof(buf));
        if(ret < 0)
            perror("read");

        send(sock_c,buf,sizeof(buf),0);
        fflush(stdout);
        if(strncmp(buf,"stop",4) == 0)
        {
            printf("client closed");
            break;
        }       
        
    }
    shutdown(sock_c,SHUT_WR);
    
    return 0;
}