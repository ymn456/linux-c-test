#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

#define SERV_PORT 1234

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

    for(int i = 0; i < 10 ; i++ ){

        int pid = fork();
        if(pid < 0){
            perror("fork()");
        }
        else if(pid == 0){
            struct sockaddr_in client_addr;

            int conn = accept(sock, (struct sockaddr*)&client_addr, &socklen);
            {if(conn < 0) perror("sccept()");}

            char buf[100];
            while(1){

                int ret = recv(conn, buf, sizeof(buf),0);

                if(ret <= 0)
                    exit(-1);
                else{

                    printf("server receive: %s\n", buf); 
                    send(conn, buf, sizeof(buf),0);
                    printf("server send: %s\n", buf); 

                }
        
            }

        }
        
        else if(pid > 0){
            continue;
        }

    }

    ret = wait(NULL);
    while(ret != -1){
        ret = wait(NULL);
    }

    return 0;
}