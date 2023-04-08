#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>

#define SERV_PORT 1234

void* Send( void* conn){
    int fd = *(int*)conn;
    char buf[100];
    while(1){

        int ret = recv(fd, buf, sizeof(buf),0);

        if(ret <= 0)
            break;
        else{
            printf("server receive: %s\n", buf); 
            send(fd, buf, sizeof(buf),0);
            printf("server send: %s\n", buf); 
            bzero(buf, sizeof(buf));
        }
    
    }
}

int main(){
    int sock = socket(AF_INET,SOCK_STREAM,0);
    {if(sock < 0) perror("sock()");}

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERV_PORT);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    int ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    {if(ret < 0) perror("bind()");}

    listen(sock, 10);
    
    socklen_t socklen = sizeof(struct sockaddr_in);

    for(int i = 0; i < 10; i++){
        struct sockaddr_in client_addr;
        pthread_t tid;
        int conn = accept(sock, (struct sockaddr*)&client_addr, &socklen);
        {if(conn < 0) perror("sccept()");}
        pthread_create(&tid, NULL, Send, (void*)&conn);
        pthread_detach(tid);
    }

    return 0;
}