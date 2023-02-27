#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>


#define SERV_PORT 1234
char buf[BUFSIZ];
void *new_connect(void *arg)
{
    int sock_conn = *(int *)arg;
    while(1)
    {    
        recv(sock_conn,buf,sizeof(buf),0);
        printf(buf);
        
        if(strncmp(buf,"stop",4) == 0)
        {
            close(sock_conn);
            break;
        }
        fflush(stdout);
        bzero(buf,sizeof(buf));
            
    }
}

int main()
{
    int sock_s = socket(AF_INET,SOCK_STREAM,0);
    if(sock_s == -1)
        perror("socket()");
    
    struct sockaddr_in serv_addr,client_addr;

    serv_addr.sin_family = AF_INET;

    serv_addr.sin_port = htons(SERV_PORT);

    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret;

    ret = bind(sock_s,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(ret == -1)
        perror("bind()");

    ret = listen(sock_s,10);
    if(ret == -1)
        perror("listen()");

    int addr_len = sizeof(client_addr);

    while(1)
    {
        int sock_conn = accept(sock_s,(struct sockaddr *)&client_addr,&addr_len);
        if(sock_conn == -1)
            perror("accept()");
        else
            printf("有新连接");

        pthread_t pdt;
        ret = pthread_create(&pdt,NULL,new_connect,(void*)&sock_conn);
        if(ret != 0)
            printf("error:new_connect()");
    }
    
    close(sock_s);
    return 0;
}