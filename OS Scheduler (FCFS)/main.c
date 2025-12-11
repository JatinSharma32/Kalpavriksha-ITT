#include <stdlib.h>
#include <stdio.h>
#include "Implementations.h"

Queue *ReadyQueue = NULL;
Queue *WaitingQueue = NULL;
Queue *TerminatedQueue = NULL;
HashMap *PCBHashMap = NULL;
int SystemClock = 0;
KillNode *KillListHead = NULL;

void printReadyQueue()
{
    printf("Ready Queue:\n");
    ListNode *curr = ReadyQueue->Front;
    while (curr != NULL)
    {
        PCB *pcb = curr->ProcessControlBlock;
        printf("Name: %s, PID: %d, CPU Burst: %d, IO Start: %d, IO Duration: %d\n",
               pcb->Name, pcb->PID, pcb->CPUBurst, pcb->IOStartTime, pcb->IODuration);
        curr = curr->Next;
    }
}

void printKillList()
{
    printf("Kill List:\n");
    KillNode *curr = KillListHead;
    while (curr != NULL)
    {
        printf("PID: %d, Kill Time: %d\n", curr->PID, curr->KillTime);
        curr = curr->Next;
    }
}

int main(int argc, char **argv)
{
    initilization();
    userInput();

    printReadyQueue();
    printKillList();
}