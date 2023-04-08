#include "pthread_com.h"


int fd0_0[2];
int fd0_1[2];
int fd1_0[2];
int fd1_1[2];
int fd2_0[2];
int fd2_1[2];


pthread_mutex_t mutex;

pthread_cond_t cond0_0;
pthread_cond_t cond0_1;
pthread_cond_t cond1_0;
pthread_cond_t cond1_1;
pthread_cond_t cond2_0;
pthread_cond_t cond2_1;

void* work_T0(void* arg);
void* work_T1(void* arg);
void* work_T2(void* arg);
void* work_M0(void* arg);
void* work_M1(void* arg);
void* work_M2(void* arg);


int main(){

    // 构建msg ，设计这样一个传输过程：
    // 该msg首先被ISTransManager 发送给 TaskPerformer_0，
    // 然后被TaskPerformer_0 发给 TaskPerformer_1，再被TaskPerformer_1 发给 TaskPerformer_2，
    // 最后被TaskPerformer_2 发给 ISTransManager
    msg message;
    message.sender = -1;
    message.receiver = 0;
    message.data_type = 2;
    strcpy(message.data, "./test.txt");
        

    // ISTransManager使用匿名管道与TaskPerformer通信
    // ISTransManager与TaskPerformer_0 通信的管道和信号量
    mutex = PTHREAD_MUTEX_INITIALIZER;

    pipe(fd0_0);
    pipe(fd0_1);
    cond0_0 = PTHREAD_COND_INITIALIZER;
    cond0_1 = PTHREAD_COND_INITIALIZER;


    // ISTransManager与TaskPerformer_1 通信的管道和信号量
    pipe(fd1_0);
    pipe(fd1_1);
    cond1_0 = PTHREAD_COND_INITIALIZER;
    cond1_1 = PTHREAD_COND_INITIALIZER;

    // ISTransManager与TaskPerformer_2 通信的管道和信号量

    pipe(fd2_0);
    pipe(fd2_1);
    cond2_0 = PTHREAD_COND_INITIALIZER;
    cond2_1 = PTHREAD_COND_INITIALIZER;

    pthread_t* thread = new pthread_t [6];

    // 子模块收到msg，执行工作逻辑，将msg发出
    pthread_create(thread, NULL, work_T0, NULL);
    pthread_detach(thread[0]);
    pthread_create(thread+1, NULL, work_T1, NULL);
    pthread_detach(thread[1]);
    pthread_create(thread+2, NULL, work_T2, NULL);

    // 主模块收到来自不同子模块的msg，选择转发或不转发
    pthread_create(thread+3, NULL, work_M0, NULL);
    pthread_detach(thread[3]);
    pthread_create(thread+4, NULL, work_M1, NULL);
    pthread_detach(thread[4]);
    pthread_create(thread+5, NULL, work_M2, NULL);
    pthread_detach(thread[5]);

    // ISTransManager（以下简称 M） 收到外部发来的msg， 将msg发给 TaskPerformer_0（以下简称 T0）
    write(fd0_1[1], &message, sizeof(message));
    pthread_cond_signal(&cond0_1);
    pthread_join(thread[2],NULL);
    return 0;
}


// M 发给 T0 ,T0 收到信息，然后发给 T1
void* work_T0(void* arg)
    {
        msg* msg0 = new msg;

        while(1){
        pthread_cond_wait(&cond0_1, &mutex);

        printf("T0 收到信息\n");
        read(fd0_1[0], msg0, sizeof(msg0));

        printf("信息将发给 T1\n");
        msg0->sender = 0;
        msg0->receiver = 1;
        write(fd0_0[1], msg0, sizeof(msg0));

        printf("通知 M 转发信息\n");

        pthread_cond_signal(&cond0_0);
        }
        
    }

// T1 收到信息，然后发给 T2
void* work_T1(void* arg)
    {
        msg* msg0 = new msg;

        while(1){
        pthread_cond_wait(&cond1_1, &mutex);

        printf("T1 收到信息\n");
        read(fd1_1[0], msg0, sizeof(msg0));

        printf("信息将发给 T2\n");
        msg0->sender = 1;
        msg0->receiver = 2;
        write(fd1_0[1], msg0, sizeof(msg0));

        printf("通知 M 转发信息\n");
        pthread_cond_signal(&cond1_0);
        }

        
    }

// T2 收到信息，然后发给 M
void* work_T2(void* arg)
    {
        msg* msg0 = new msg;

        pthread_cond_wait(&cond2_1, &mutex);

        printf("T2 收到信息\n");
        read(fd2_1[0], msg0, sizeof(msg0));

        printf("信息将发给 M\n");
        msg0->sender = 2;
        msg0->receiver = -1;
        write(fd2_0[1], msg0, sizeof(msg0));
        printf("程序结束\n");
        
    }

// M对收到的来自 T0 的信息接收或转发
void* work_M0(void* arg)
    {
        msg* msg0 = new msg;

        while(1){
        pthread_cond_wait(&cond0_0, &mutex);

        printf("M 收到信息\n");
        read(fd0_0[0], msg0, sizeof(msg0));
            
        if(msg0->receiver == -1){
            printf("M 接收信息\n");
        }else if(msg0->receiver == 1){
            printf("M 转发信息，转发给 T1\n");
            write(fd1_1[1], msg0, sizeof(msg0));
            pthread_cond_signal(&cond1_1);

        }else if(msg0->receiver == 2){
            printf("M 转发信息，转发给 T2\n");
            write(fd2_1[1], msg0, sizeof(msg0));
            pthread_cond_signal(&cond2_1);
        }
        }

        
    }

// M对收到的来自 T1 的信息接收或转发
void* work_M1(void* arg)
    {
        msg* msg0 = new msg;

        while(1){
        pthread_cond_wait(&cond1_0, &mutex);

        printf("M 收到信息\n");
        read(fd1_0[0], msg0, sizeof(msg0));
            
        if(msg0->receiver == -1){
            printf("M 接收信息\n");
        }else if(msg0->receiver == 0){
            printf("M 转发信息，转发给 T0\n");
            write(fd0_1[1], msg0, sizeof(msg0));
            pthread_cond_signal(&cond0_1);

        }else if(msg0->receiver == 2){
            printf("M 转发信息，转发给 T2\n");
            write(fd2_1[1], msg0, sizeof(msg0));
            pthread_cond_signal(&cond2_1);
        }
        }
        
    }

// M对收到的来自 T2 的信息接收或转发
void* work_M2(void* arg)
    {
        msg* msg0 = new msg;

        while(1){
        pthread_cond_wait(&cond2_0, &mutex);

        printf("M 收到信息\n");
        read(fd0_0[0], msg0, sizeof(msg0));
            
        if(msg0->receiver == -1){
            printf("M 接收信息\n");
        }else if(msg0->receiver == 1){
            printf("M 转发信息，转发给 T1\n");
            write(fd1_1[1], msg0, sizeof(msg0));
            pthread_cond_signal(&cond1_1);

        }else if(msg0->receiver == 2){
            printf("M 转发信息，转发给 T0\n");
            write(fd0_1[1], msg0, sizeof(msg0));
            pthread_cond_signal(&cond0_1);
        }
        }
    }
