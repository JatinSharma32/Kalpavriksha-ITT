#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "Implementations.h"

void enqueue(Queue *queue, ListNode **newNode)
{
    // We need to pass the double pointer as we are storing original pointer.
    if (queue->length == 0)
    {
        queue->Front = *newNode;
        queue->Rear = *newNode;
    }
    else
    {
        queue->Rear->Next = *newNode;
        queue->Rear = *newNode;
    }
    queue->length++;
}

PCB *dequeue(Queue *queue)
{
    // We need to pass the double pointer as we are storing original pointer.
    PCB *temp;
    if (queue->length == 0)
    {
        return NULL;
    }
    temp = queue->Front->ProcessControlBlock;
    if (queue->length == 1)
    {
        queue->Front = NULL;
        queue->Rear = NULL;
    }
    else
    {
        queue->Front = queue->Front->Next;
    }
    return temp;
    queue->length--;
}