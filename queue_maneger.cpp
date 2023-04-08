//采用泛型编程实现一个队列管理器，要求支持 uint8_t, uint16_t, uint32_t 等类型，维护空、满队列，实现数据的交换

#include<iostream>
#include<stdio.h>
using namespace std;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

template<class T>
class queue_maneger{
    public:
        // 初始化队列，length为队列长度
        queue_maneger(int length){
            ptr = new T [length];
            this->length = length;
            size= 0;
        }

        ~queue_maneger(){
            delete [] ptr;
        }
        //从队头出
        bool pop(){
            if(size == 0) return 0;
            
            else if(size == 1){
                size = 0;
                return 1;
            }
            else{
                for(int i = 1; i < size; i++){
                ptr[i-1] =ptr[i]; 
                }
                size--;
                return 1;
            }
        }

        // 从队尾进
        bool push(T data){
            if(size >= length) return 0;

            else{
                ptr[size] = data;
                size++;
                return 1;
            }
        }

        // 交换第n个和第m个元素的值
        bool swap(int n, int m){
            if( n >= length || n < 0 || m >= length || m < 0){
                return 0;
            }

            else{
                T temp;
                temp = ptr[n];
                ptr[n] = ptr[m];
                ptr[m] = temp;
                return 1;
            }
        }

        // 查看第n个元素的值
        T find(int n){
            return ptr[n];
        }

        // 判空
        bool isempty(){
            if(size == 0)
                return 1;
            else return 0;
        }

        // 判满
        bool isfull(){
            if(size == length)
                return 1;
            else return 0;
        }

        // 返回当前元素个数
        int show_size(){
            return size;
        }

    private:
        T* ptr = NULL;

        int length; // 队列总长

        int size; // 当前元素个数

};

int main(){
    // 队列初始化长度为10
    queue_maneger<uint8_t> q(10);

    // 判空
    printf("队列是否空: %s\n",(q.isempty() == 1)? "true" : "false");

    // 插入数字0到9
    for( int i = 0; i < 10; i ++){
        q.push(i);
        cout<<"插入: "<<i<<endl;
    }

    // 判满
    printf("队列是否满: %s\n",(q.isfull() == 1)? "true" : "false");

    // 将第0个与第9个元素的值交换
    printf("交换第0个和第9个元素的值\n");
    q.swap(0, 9);

    // 输出所有元素
    int size = q.show_size();
    for( int i = 0; i < size; i ++){
        cout<<"输出："<<+q.find(i)<<endl;
    }

    return 0;
}