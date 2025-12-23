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
    ReadyQueue->length = 0;
    WaitingQueue->Front = NULL;
    WaitingQueue->Rear = NULL;
    WaitingQueue->length = 0;
    TerminatedQueue->Front = NULL;
    TerminatedQueue->Rear = NULL;
    TerminatedQueue->length = 0;

    // Hashmap Implementation
    PCBHashMap = (HashMap *)malloc(sizeof(HashMap));
    PCBHashMap->buckets = (ListNode **)calloc(MAP_SIZE, sizeof(ListNode *));
    PCBHashMap->MapSize = MAP_SIZE;
    printf("Initilization Complete.\n");
}

void userInput()
{
    char command[100];
    while (fgets(command, 100, stdin))
    {
        if (command == NULL || strcmp(command, "\n") == 0)
        {
            // Found the newline Kill Commands start
            break;
        }
        command[strcspn(command, "\n")] = '\0';
        PCB *newProcess = (PCB *)malloc(sizeof(PCB));
        newProcess->Name = (char *)malloc(50 * sizeof(char));
        if (sscanf(command, "%s %d %d %d %d", newProcess->Name, &newProcess->PID, &newProcess->CPUBurst, &newProcess->IOStartTime, &newProcess->IODuration) != 5)
        {
            // Invalid input
            free(newProcess->Name);
            free(newProcess);
            continue;
        }
        newProcess->RemainingCPUBurstTime = newProcess->CPUBurst;
        newProcess->RemainingIOTime = newProcess->IODuration;
        newProcess->CompletionTime = -1;
        newProcess->State = READY;
        newProcess->TurnAroundTime = -1;
        newProcess->WaitingTime = -1;

        // Add this node in Ready queue.
        ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
        newNode->ProcessControlBlock = newProcess;
        newNode->Next = NULL;

        // Now add it in the Ready queue
        enqueue(ReadyQueue, &newNode);

        // Add to HashMap so Kill commands can find it
        addToHashMap(newProcess);
    }

    // Now the Kill Commands Part
    while (fgets(command, 100, stdin))
    {
        if (command == NULL || strcmp(command, "\n") == 0)
        {
            // Found the newline Kill Commands start
            break;
        }
        command[strcspn(command, "\n")] = '\0';

        KillNode *newKillNode = (KillNode *)malloc(sizeof(KillNode));
        char dump[100];
        if (sscanf(command, "%s %d %d", dump, &newKillNode->PID, &newKillNode->KillTime) != 3)
        {
            // Invalid input
            free(newKillNode);
            continue;
        }

        // Add this node in Kill List.
        newKillNode->Next = KillListHead;
        KillListHead = newKillNode;
    }
}

void printSimulationResults()
{
    printf("| %-10s | %-5s | %-10s | %-8s | %-8s | %-8s | %-8s |\n",
           "Name", "PID", "State", "Burst", "IO Dur", "TAT", "Wait");
    printf("--------------------------------------------------------------------------\n");

    if (TerminatedQueue == NULL || TerminatedQueue->Front == NULL)
        return;

    ListNode *current = TerminatedQueue->Front;
    double totalTAT = 0;
    double totalWT = 0;
    int count = 0;

    while (current != NULL)
    {
        PCB *p = current->ProcessControlBlock;
        p->TurnAroundTime = p->CompletionTime;
        p->WaitingTime = p->TurnAroundTime - p->CPUBurst;

        if (p->WaitingTime < 0)
            p->WaitingTime = 0;

        char stateStr[20];
        if (p->State == TERMINATED)
            strcpy(stateStr, "DONE");
        else if (p->State == KILLED)
            strcpy(stateStr, "KILLED");
        else
            strcpy(stateStr, "OTHER");

        printf("| %-10s | %-5d | %-10s | %-8d | %-8d | %-8d | %-8d |\n",
               p->Name, p->PID, stateStr, p->CPUBurst, p->IODuration, p->TurnAroundTime, p->WaitingTime);

        totalTAT += p->TurnAroundTime;
        totalWT += p->WaitingTime;
        count++;

        current = current->Next;
    }
}