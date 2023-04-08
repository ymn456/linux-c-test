#include <sys/bitypes.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT 1234

int main(){

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    {if(sock < 0) perror("sock()");}

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    int ret = connect(sock, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr));
    {if(sock < 0) perror("connect()");}

    char buf_send[100];
    char buf_recv[100];

    while(1){

        printf("client input: ");
        fflush(stdin);
        scanf("%s",buf_send);
        send(sock, buf_send, sizeof(buf_send),0);
        printf("client send: %s\n", buf_send); 

        sleep(3);

        ret = recv(sock, buf_recv, sizeof(buf_recv),0);
        if(ret <= 0)
        {exit(0);}
        
        printf("client receive: %s\n", buf_recv);
        bzero(buf_recv, sizeof(buf_recv));
    }

    return 0;
}