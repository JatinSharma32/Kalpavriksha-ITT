#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "Implementations.h"

void initilization()
{
    // All 3 queues implementation
    ReadyQueue = (Queue *)malloc(sizeof(Queue));
    WaitingQueue = (Queue *)malloc(sizeof(Queue));
    TerminatedQueue = (Queue *)malloc(sizeof(Queue));
    ReadyQueue->Front = NULL;
    ReadyQueue->Rear = NULL;
    WaitingQueue->Front = NULL;
    WaitingQueue->Rear = NULL;
    TerminatedQueue->Front = NULL;
    TerminatedQueue->Rear = NULL;

    // Hashmap Implementation
    PCBHashMap = (HashMap *)malloc(sizeof(HashMap));
    PCBHashMap->buckets = (ListNode **)calloc(MAP_SIZE, sizeof(ListNode *));
    PCBHashMap->MapSize = MAP_SIZE;
}

void userInput()
{
    char command[50];
    while (true)
    {
        command
    }
}