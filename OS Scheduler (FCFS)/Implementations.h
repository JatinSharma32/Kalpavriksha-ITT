#ifndef IMPLEMENTATIONS
#define IMPLEMENTATIONS

// Process Control Block
typedef struct PCB
{
    int PID;
    char *Name;
    int CPUBurst;
    int IOStartTime;
    int IODuration;
    int TurnAroundTime;
    int WaitingTime;
    int RemainingCPUBurstTime;
} PCB;

// Wrapper Node of PCB for making list
typedef struct ListNode
{
    PCB *ProcessControlBlock;
    struct ListNode *Next;
} ListNode;

// Hashmap Implementation
typedef struct HashMapNode
{
    int MapSize;
    ListNode **buckets;
} HashMap;

// Various Queues Implementation
typedef struct Queue
{
    ListNode *Front;
    ListNode *Rear;
} Queue;

extern Queue *ReadyQueue;
extern Queue *WaitingQueue;
extern Queue *TerminatedQueue;
extern int SystemClock;

#endif