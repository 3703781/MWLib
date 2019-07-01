/**
 * @file    bsp_iic.c
 * @author  Miaow
 * @version 0.1.0
 * @date    2018/10/04
 * @brief   
 *          This file provides functions to manage a queue implemented by
 *          the single linked list:
 *              1. Initialization & deinitialization
 *              2. Insert & delete
 *              3. Traverse
 * @note
 *          Minimum version of header file:
 *              0.1.0
 *
 *          The source code repository is not available on GitHub now:
 *              https://github.com/3703781
 *          Your pull requests will be welcome.
 *          Here are the guidelines for your pull requests:
 *              1. Respect my coding style.
 *              2. Avoid to commit several features in one commit.
 *              3. Make your modification compact - don't reformat source code in your request.
 */
//    队列的单链表实现
//    头节点：哨兵作用，不存放数据，用来初始化队列时使队头队尾指向的地方
//    首节点：头节点后第一个节点，存放数据

#include "stdlib.h"
#include "stm32f4xx.h"
#include "oled_queue.h"


////    主函数
//int main() {
//    int val = 0;    //    定义临时变量
//    queue queue;    //    创建队列变量
//    InitQueue(&queue);    //    调用初始化队列函数
//    IsEmptyQueue(&queue);    //    调用判断队列是否为空函数
//    InsertQueue(&queue, 100);    //调用入队函数
//    InsertQueue(&queue, 200);
//    InsertQueue(&queue, 300);
//    InsertQueue(&queue, 400);
//    InsertQueue(&queue, 500);
//    InsertQueue(&queue, 600);
//    IsEmptyQueue(&queue);
//    TraverseQueue(&queue);    //    调用队列遍历函数
//    DeleteQueue(&queue, &val);    //    调用出队函数
//    TraverseQueue(&queue);
//    ClearQueue(&queue);        //    调用清空队列函数
//    IsEmptyQueue(&queue);
//    DestroyQueue(&queue);    //    调用摧毁队列函数

//    return 0;
//}

/**
 * @brief 初始化
 * @param queue 队列变量
 * @return 0-初始化成功, 1-初始化失败
 */
uint8_t OLED_InitQueue(pQueue queue)
{
    queue->front = queue->rear = (pNode)malloc(sizeof(node));//动态创建头节点，使队头，队尾指向该节点
    //头节点相当于哨兵节点的作用，不存储数据（区别于首节点）
    if (queue->front == NULL)
        return 1;//内存分配失败
    queue->front->Next = NULL;//初始队列头节点指针指向为空
    return 0;
}

/**
 * @brief 判断队列是否为空
 * @param queue 队列变量
 * @return 0-不为空, 1-为空
 */
inline uint8_t OLED_IsEmptyQueue(pQueue queue)
{
    return queue->front == queue->rear;
}

/**
 * @brief 入队列
 * @param queue 指定队列
 * @param item 入队元素
 * @return 0-入队成功, 1-入队失败
 */
uint8_t OLED_InsertQueueItem(pQueue queue, char* item)
{
    pNode P = (pNode)malloc(sizeof(node));//创建一个新节点用于存放插入的元素
    if (P == NULL) 
        return 1;//内存分配失败
    P->item = item;//把要插入的数据放到节点数据域
    P->Next = NULL;//新节点指针指向为空
    queue->rear->Next = P;//使上一个队列尾部的节点指针指向新建的节点
    queue->rear = P;//更新队尾指针，使其指向队列最后的节点
    return 0;
}

/**
 * @brief 出队列
 * @param queue 指定队列
 * @param item 输出出队元素
 * @return 0-出队成功, 1-出队失败
 * @note 从队列的首节点开始出队
 */
uint8_t OLED_DeleteQueueItem(pQueue queue, __IO char* item)
{
    if (OLED_IsEmptyQueue(queue))
        return 1;//队列空,无法出队
    pNode  P = queue->front->Next;//临时指针
    item = P->item;//保存其值
    P->item = NULL;
    queue->front->Next = P->Next;//更新头节点
    if (queue->rear == P)
        queue->rear = queue->front;
    free(P);//释放头队列
    P = NULL;//防止产生野指针
    return 0;
}

/**
 * @brief 遍历队列
 * @param queue 指定队列
 * @param callback 回调函数
 * @return 0-成功, 1-失败
 */
uint8_t OLED_TraverseQueue(pQueue queue, void (* callback)(char *item))
{
    
    if (OLED_IsEmptyQueue(queue))
        return 1;
    pNode P = queue->front->Next;//从队列首节点开始遍历(非头节点,注意区分)
    while (P != NULL)
    {
        callback(P->item);
        P = P->Next;
    }
    return 0;
}

/**
 * @brief 删除整个队列
 * @param queue 指定队列
 * @note 释放整个队列,包括头节点
 */
void OLED_DestroyQueue(pQueue queue)
{
    while (queue->front != NULL)
    {
        queue->rear = queue->front->Next;
        free(queue->front);
        queue->front = queue->rear;
    }
}

/**
 * @brief 清空队列
 * @param queue 指定队列
 */
void OLED_ClearQueue(pQueue queue)
{
    pNode P = queue->front->Next;//临时指针
    pNode Q = NULL;//临时指针
    queue->rear = queue->front;//使队尾指针指向头节点
    queue->front->Next = NULL;
    //从首节点开始清空
    while (P != NULL)
    {
        Q = P;
        P = P->Next;
        free(Q);
    }
}
