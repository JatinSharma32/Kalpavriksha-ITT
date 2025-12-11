#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
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

    if (queue->length == 0)
    {
        return NULL;
    }
    ListNode *tempNode = queue->Front;
    PCB *temp = tempNode->ProcessControlBlock;
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
    free(tempNode);
    return temp;
}

void deleteFromQueue(Queue *queue, int PID)
{
    // Remove the process from the queue.
    ListNode *tempNode = queue->Front;
    ListNode *prevNode = NULL;
    while (tempNode != NULL && tempNode->ProcessControlBlock->PID != PID)
    {
        prevNode = tempNode;
        tempNode = tempNode->Next;
    }
    if (tempNode == NULL || tempNode->ProcessControlBlock->PID != PID)
        return;

    if (prevNode != NULL)
    {
        prevNode->Next = tempNode->Next;
        if (tempNode->Next == NULL)
            queue->Rear = prevNode;
    }
    else
    {
        queue->Front = tempNode->Next;
        if (queue->length == 1)
        {
            queue->Rear = NULL;
        }
    }
    free(tempNode);
    queue->length--;
}

void checkKillCommands()
{
    KillNode *tempNode = KillListHead;
    while (tempNode != NULL)
    {
        if (tempNode->KillTime == SystemClock)
        {
            // this PID need to be Killed
            PCB *process = getFromHashMap(tempNode->PID);
            if (process->State == READY)
                deleteFromQueue(ReadyQueue, process->PID);
            else if (process->State == WAITING)
                deleteFromQueue(WaitingQueue, process->PID);

            // Update the process properties according to Killed
            process->State = KILLED;
            process->CompletionTime = SystemClock;

            ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
            newNode->ProcessControlBlock = process;
            newNode->Next = NULL;
            // Add in terminated queue
            enqueue(TerminatedQueue, &newNode);
        }
        tempNode = tempNode->Next;
    }
}

void startScheduling()
{
    printf("\n--- Starting Simulation ---\n");
    PCB *runningProcess = NULL;
    // A while loop that will simulate the system clock
    while (ReadyQueue->length > 0 || WaitingQueue->length > 0)
    {
        // check for kill commands
        checkKillCommands();

        // check if curretn process is terminated
        if (runningProcess != NULL && runningProcess->State == KILLED)
        {
            runningProcess = NULL;
        }

        // Handling the waiting Queue and I/O operations
        if (WaitingQueue->length > 0)
        {
            ListNode *curr = WaitingQueue->Front;
            ListNode *prev = NULL;
            int initialLength = WaitingQueue->length;
            int count = 0;

            // Iterate carefully as we might remove nodes
            while (count < initialLength && curr != NULL)
            {
                ListNode *nextNode = curr->Next;
                PCB *process = curr->ProcessControlBlock;

                process->RemainingIOTime--;
                if (process->RemainingIOTime <= 0)
                {
                    // if the I/o Completes then move it to ready queue
                    process->State = READY;

                    // delete it from Waiting queue
                    if (prev == NULL)
                        WaitingQueue->Front = nextNode;
                    else
                        prev->Next = nextNode;

                    if (nextNode == NULL)
                        WaitingQueue->Rear = prev;

                    WaitingQueue->length--;
                    free(curr);

                    // add it to Ready queue again for CPU
                    ListNode *newReadyNode = (ListNode *)malloc(sizeof(ListNode));
                    newReadyNode->ProcessControlBlock = process;
                    newReadyNode->Next = NULL;
                    enqueue(ReadyQueue, &newReadyNode);

                    curr = nextNode;
                }
                else
                {
                    prev = curr;
                    curr = nextNode;
                }
                count++;
            }
        }

        // suppose if there's no process in CPU, its IDLE then assign a process
        if (runningProcess == NULL && ReadyQueue->length > 0)
        {
            runningProcess = dequeue(ReadyQueue);
            runningProcess->State = RUNNING;
        }

        // IF there's any process in the CUP them execute it for unit time
        if (runningProcess != NULL)
        {
            // find the time process was in CPU
            int timeExecuted = runningProcess->CPUBurst - runningProcess->RemainingCPUBurstTime;

            // find if I/O is needed by the process as current timestamp.
            if (runningProcess->IODuration > 0 && timeExecuted == runningProcess->IOStartTime)
            {
                runningProcess->State = WAITING;
                ListNode *ioNode = (ListNode *)malloc(sizeof(ListNode));
                ioNode->ProcessControlBlock = runningProcess;
                ioNode->Next = NULL;
                enqueue(WaitingQueue, &ioNode);
                runningProcess = NULL;
            }
            else
            {
                runningProcess->RemainingCPUBurstTime--;

                // check if the process is completed and needs to be terminated
                if (runningProcess->RemainingCPUBurstTime <= 0)
                {
                    runningProcess->State = TERMINATED;
                    runningProcess->CompletionTime = SystemClock + 1;

                    ListNode *termNode = (ListNode *)malloc(sizeof(ListNode));
                    termNode->ProcessControlBlock = runningProcess;
                    termNode->Next = NULL;
                    enqueue(TerminatedQueue, &termNode);

                    runningProcess = NULL;
                }
            }
        }

        // Increase the simulated system clock
        SystemClock++;
        sleep(1);
    }
}
