#ifndef IMPLEMENTATIONS
#define IMPLEMENTATIONS
#define MAP_SIZE 100

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
    int RemainingIOTime;
    int CompletionTime;
} PCB;

// Kill Node
typedef struct KillNode
{
    int PID;
    int KillTime;
    struct KillNode *Next;
} KillNode;

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
    int length;
} Queue;

extern Queue *ReadyQueue;
extern Queue *WaitingQueue;
extern Queue *TerminatedQueue;
extern HashMap *PCBHashMap;
extern int SystemClock;
extern KillNode *KillListHead;

void initilization();
void userInput();

// Queue Functions
void enqueue(Queue *queue, ListNode **newNode);
PCB *dequeue(Queue *queue);
void addToHashMap(PCB *process);
PCB *getFromHashMap(int pid);

#endif