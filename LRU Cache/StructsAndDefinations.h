#ifndef STRUCTSANDDEFINITIONS_H
#define STRUCTSANDDEFINITIONS_H

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

extern Cache *cache;
extern HashTable *hashTable;

int hashFunction(int key, int hashSize);

#endif