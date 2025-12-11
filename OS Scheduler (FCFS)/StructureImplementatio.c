#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "Implementations.h"

int hash(int pid)
{
    return pid % MAP_SIZE;
}

void addToHashMap(PCB *process)
{
    int index = hash(process->PID);
    ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
    newNode->ProcessControlBlock = process;
    newNode->Next = PCBHashMap->buckets[index];
    PCBHashMap->buckets[index] = newNode;
}

PCB *getFromHashMap(int pid)
{
    int index = hash(pid);
    ListNode *curr = PCBHashMap->buckets[index];
    while (curr != NULL)
    {
        if (curr->ProcessControlBlock->PID == pid)
        {
            return curr->ProcessControlBlock;
        }
        curr = curr->Next;
    }
    return NULL;
}

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
    queue->length--;
    free(temp);
    return temp;
}
