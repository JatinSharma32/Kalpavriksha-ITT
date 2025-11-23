#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct DLLQueNode
{
    int Key;
    char *Value;
    struct DLLQueNode *Prev;
    struct DLLQueNode *Next;
} DLLQueNode;

typedef struct HashMapNode
{
    int Key;
    DLLQueNode *QueueNode;
    struct HashMapNode *Next;
} HashMapNode;

typedef struct HashTable
{
    HashMapNode **HashTable;
    int HashSize;
} HashTable;

typedef struct Cache
{
    int Capacity;
    int Size;
    DLLQueNode *qHead;
    DLLQueNode *qTail;

} Cache;

int main()
{
    // Main code here
    return 0;
}