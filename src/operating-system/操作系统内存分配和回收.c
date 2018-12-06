//
//  main.c
//  memory
//
//  Created by BUSE on 2016/11/25.
//  Copyright © 2016年 BUSE. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define NUMBER 128  //内存单元的最大数
#define MINLENGTH 3 //碎片化内存的最大单元数

int  startID = 0;   //进程id起始为0
struct memoryEmploy* tail = NULL;   //进程结构体尾
struct memoryLeisure* head = NULL;    //内存单元块结构体头
struct memoryCount* memory = NULL;        //检测内存变化的结构体

//检测内存变化的结构体
struct memoryCount {
    int memoryQuantity;      //内存总数
    int memoryResidue;       //剩余内存数
    int memoryFragment;      //内存碎片数
    int searchLeisureTimes;  //搜索空闲区次数
};

//进程结构体。因为是双向链表，进程链表从最后一个往前遍历直到遇到NULL为止。
struct memoryEmploy {
    int id;                     //进程id
    int start;                  //内存单元开始下标
    int length;                 //占用的内存单元长度
    int end;                    //内存单元结束下标
    struct memoryEmploy* next;  //下一个进程
    struct memoryEmploy* prev;  //前一个进程
};

//内存单元块结构体。因为是双向链表，内存单元块链表从第一个开始遍历直到遇到NULL为止，并且start递增。
struct memoryLeisure {
    int start;                  //此内存块开始下标
    int length;                 //此内存块长度
    int end;                    //此内存块结束下标
    struct memoryLeisure* next; //下一个没有被占用的内存块
    struct memoryLeisure* prev; //前一个没有被占用的内存块
};

//新加入一个进程
int createThread(int size) {
    
    if(size > memory->memoryResidue)
        return 0;
    else{
        //遍历空闲的内存单元块
        struct memoryLeisure* leisure = head;
        
        while (leisure != NULL) {
            memory->searchLeisureTimes++;
            int blockLength = leisure->length;
            
            if(blockLength >= size){
                //创建进程链表尾指针
                if(tail == NULL){
                    tail = malloc(sizeof(struct memoryEmploy));
                    tail->prev = NULL;
                    tail->next = NULL;
                }
                else{
                    struct memoryEmploy* tailPrev = tail;
                    tail = malloc(sizeof(struct memoryEmploy));
                    tailPrev->next = tail;
                    tail->prev = tailPrev;
                    tail->next = NULL;
                }
                
                //缩小空闲内存单元的大小并把进程载入应用程序
                if(blockLength == size){
                    tail->start = leisure->start;
                    tail->length = leisure->length;
                    tail->end = leisure->end;
                    tail->id = startID++;
                    
                    if(leisure->prev != NULL){
                        leisure->prev->next = leisure->next;
                        if(leisure->next != NULL)
                            leisure->next->prev = leisure->prev;
                    }else{
                        if(leisure->next != NULL){
                            leisure->next->prev = NULL;
                        }else{
                            head = NULL;
                        }
                    }
                    free(leisure);
                    leisure = NULL;
                }else{
                    tail->start = leisure->start;
                    tail->length = size;
                    tail->end = tail->start + tail->length - 1;
                    tail->id = startID++;
                    
                    leisure->start = tail->end + 1;
                    leisure->length = leisure->length - tail->length;
                    if(leisure->length < 3)
                        memory->memoryFragment++;
                }
                memory->memoryResidue = memory->memoryResidue - tail->length;
                return 2;
            }else{
                leisure = leisure->next;
            }
        }
        
        return 1;
    }
    
}

//销毁一个进程
int destoryThread(int number) {
    struct memoryEmploy* thread = tail;
    
    //遍历进程链表
    while (thread != NULL) {
        if(thread->id == number){
            memory->memoryResidue = memory->memoryResidue + thread->length;
            
            //遍历空闲内存块链表，查找要销毁的进程所占据的内存块在的范围
            struct memoryLeisure* leisure = head;
            int connect = -1;      //判断要销毁的进程占据的内存块左右两边哪边可以和空闲内存块合并 1代表左边 2代表右边 0代表自己单独作为一个空闲内存块不合并 -1表示head为空
            while (leisure != NULL) {
                
                //在右边作为最后一个空闲块
                if(thread->start - 1 > leisure->end && leisure->next == NULL){
                    connect = 0;
                    struct memoryLeisure* newLeisure = malloc(sizeof(struct memoryLeisure));
                    newLeisure->prev = leisure;
                    leisure->next = newLeisure;
                    newLeisure->next = NULL;
                    newLeisure->start = thread->start;
                    newLeisure->length = thread->length;
                    newLeisure->end = thread->end;
                    break;
                }
                
                //在左边作为第一个空闲块
                if(thread->end + 1 < leisure->start && leisure->prev == NULL){
                    connect = 0;
                    struct memoryLeisure* newLeisure = malloc(sizeof(struct memoryLeisure));
                    newLeisure->prev = NULL;
                    newLeisure->next = leisure;
                    leisure->prev = newLeisure;
                    newLeisure->start = thread->start;
                    newLeisure->length = thread->length;
                    newLeisure->end = thread->end;
                    head = newLeisure;
                    break;
                }
                
                //因为空闲内存块不为空但是自己左右两边都没有可以合并的空闲内存块，所以单独作为一个空闲快
                if(thread->start - 1 > leisure->end && leisure->next != NULL && thread->end + 1 < leisure->next->start){
                    connect = 0;
                    struct memoryLeisure* newLeisure = malloc(sizeof(struct memoryLeisure));
                    newLeisure->prev = leisure;
                    newLeisure->next = leisure->next;
                    newLeisure->start = thread->start;
                    newLeisure->length = thread->length;
                    newLeisure->end = thread->end;
                    leisure->next = newLeisure;
                    newLeisure->next->prev = newLeisure;
                    break;
                }
                
                //要销毁的内存块左边和空闲内存块合并(或者左右两边一起合并)
                if(leisure->end == thread->start - 1){
                    connect = 1;
                    leisure->length = leisure->length + thread->length;
                    leisure->end = thread->end;
                    if(leisure->next != NULL && leisure->next->start - 1 == leisure->end){
                        leisure->end = leisure->next->end;
                        leisure->length = leisure->length + leisure->next->length;
                        struct memoryLeisure* newLeisure = leisure->next;
                        leisure->next = leisure->next->next;
                        if(leisure->next != NULL){
                            leisure->next->prev = leisure;
                        }
                        free(newLeisure);
                        newLeisure = NULL;
                    }
                    break;
                }
                
                //要销毁的内存块右边和空闲内存块合并
                if(leisure->start == thread->end + 1){
                    connect = 2;
                    leisure->start = thread->start;
                    leisure->length = leisure->length + thread->length;
                    break;
                }
                
                leisure = leisure->next;
            }
            
            //因为head为空所以自己是唯一一个空闲内存块
            if(connect == -1){
                head = malloc(sizeof(struct memoryLeisure*));
                head->prev = NULL;
                head->next = NULL;
                head->start = thread->start;
                head->length = thread->length;
                head->end = thread->end;
            }
            
            if(thread->prev != NULL){
                thread->prev->next = thread->next;
                if(thread->next != NULL)
                    thread->next->prev = thread->prev;
            }else if(thread->next != NULL){
                thread->next->prev = NULL;
            }else{
                tail = NULL;
            }
            return 1;
        }else{
            thread = thread->prev;
        }
    }
    
    return 0;
}

//查看进程和内存单元的详细信息
int showMessage(){
    printf("\n现在内存情况如下\n");
    printf("总空间 %d 剩余空间 %d 内存碎片化个数 %d 搜索空闲空间总共次数 %d\n", memory->memoryQuantity, memory->memoryResidue,memory->memoryFragment, memory->searchLeisureTimes);
    
    printf("\n空闲内存单元格信息如下\n");
    int i = 1;
    struct memoryLeisure* leisure = head;
    if(head == NULL)
        printf("没有空闲内存单元格\n");
    while (leisure != NULL) {
        printf("第%d块空闲内存单元格起始下标为 %d 单元格长度为 %d 单元格结束下标为 %d\n", i++, leisure->start, leisure->length, leisure->end);
        leisure = leisure->next;
    }
    
    i = 1;
    printf("\n现在进程相关信息如下\n");
    struct memoryEmploy*  employ = tail;
    if(tail == NULL)
        printf("没有进程启动\n");
    while (employ != NULL) {
        printf("第%d个进程id为 %d 起始内存单元格下标为 %d 单元格长度为 %d 单元格结束下标为 %d\n", i++, employ->id, employ->start, employ->length, employ->end);
        employ = employ->prev;
    }
    return 1;
}

//退出应用程序
int exitApplication(){
    free(memory);
    
    struct memoryEmploy* employ = tail;
    while (tail != NULL) {
        tail = employ->prev;
        free(employ);
        employ = tail;
    }
    
    struct memoryLeisure* leisure = head;
    while (head != NULL) {
        head = leisure->next;
        free(leisure);
        leisure = head;
    }
    
    return 1;
}
//主函数开始的地方
int main(int argc, char const *argv[]) {
    
    //type判断要执行的操作，size代表输入的内存空间大小, number代表要销毁的进程id数
    int size, type, number;
    
    type = 0;                          //默认先添加进程
    
    memory = malloc(sizeof(struct memoryCount)); //初始化检测内存变化的结构体
    memory->memoryQuantity = NUMBER;
    memory->memoryResidue = NUMBER;
    memory->memoryFragment = 0;
    memory->searchLeisureTimes = 0;
    
    head = malloc(sizeof(struct memoryLeisure));//初始化内存单元结构体
    head->start = 0;
    head->length = 128;
    head->end = head->length - 1;
    head->next = NULL;
    head->prev = NULL;
    
    int returnType = -1;            //用于判断进程创建成功与否的状态
    int destoryType = -1;           //用于判断进程销毁成功与否的状态
    
    //无限循环
    while (1) {
        
        switch (type) {
                //创建进程
            case 0:
                printf("输入要创建的进程内存空间大小: ");
                scanf("%d", &size);
                if(size < 1){
                    printf("内存空间太小，运行不了程序");
                    break;
                }
                returnType = createThread(size);
                
                if(returnType == -1)
                    printf("出现未知错误,方法调用失败。\n");
                else if(returnType == 0)
                    printf("需要创建的进程内存大于剩余的内存数。\n");
                else if(returnType == 1)
                    printf("找不到连续的内存空间来放置此进程。\n");
                else {
                    printf("进程创建成功!\n");
                    printf("进程id为%d， 进程内存单元开始下标为%d，进程内存单元结束下标为%d\n", tail->id, tail->start, tail->end);
                }
                
                break;
                
                //销毁进程
            case 1:
                printf("输入要销毁的进程id: ");
                scanf("%d", &number);
                
                destoryType = destoryThread(number);
                if(destoryType == -1)
                    printf("出现未知错误，方法调用失败。\n");
                else if(destoryType == 0)
                    printf("进程id不存在，请核实后在销毁。\n");
                else
                    printf("进程销毁成功!\n");
                break;
                
                //显示进程和内存单元的所有信息
            case 2:
                if(showMessage()){}
                else
                    printf("出现未知错误，无法查看。\n");
                
                break;
                
                //退出
            default:
                if(exitApplication()){
                    printf("退出应用程序成功!\n");
                    return 0;
                }
                else
                    printf("出现未知错误!!!\n");
                break;
        }
        
        printf("\n请输入要执行的操作代码(0 -> 创建进程； 1 -> 销毁进程； 2 ->显示进程和内存单元的所有信息; 其他数字 -> 退出应用程序): ");
        scanf("%d", &type);
    }
    
    return 0;
}
