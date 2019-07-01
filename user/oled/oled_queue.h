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
 
#ifndef __OLED_QUEUE_H
#define __OLED_QUEUE_H

#include "stm32f4xx.h"



/**
 * @brief 节点结构
 */
typedef struct OLED_Node{
    char* item;//数据域
    struct OLED_Node * Next;
}node, *pNode;

/**
 * @brief 队列结构体
 */
typedef struct {
    pNode front;//队列头
    pNode rear;//队列尾
}queue, *pQueue;



uint8_t OLED_InitQueue(pQueue);
inline uint8_t OLED_IsEmptyQueue(pQueue);
uint8_t OLED_InsertQueueItem(pQueue queue, char* item);
uint8_t OLED_DeleteQueueItem(pQueue queue, __IO char* item);
void OLED_DestroyQueue(pQueue);
uint8_t OLED_TraverseQueue(pQueue queue, void (*callback)(char *item));
void OLED_ClearQueue(pQueue);


#endif
