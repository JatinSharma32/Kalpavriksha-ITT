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

void createHashTable(int capacity);
void createCache(int capacity);
void userCommand();
void addToHead(DLLQueNode *newNode);
void removeNode(DLLQueNode *newNode);
void moveToHead(DLLQueNode *newNode);
DLLQueNode *removeTail();
DLLQueNode *createDLLNode(int key, char *value);
void putInHashMap(int key, DLLQueNode *node);
void mapRemove(int key);
void putCommand(int key, char *value);
// get the node from the hashtable
DLLQueNode *mapGet(int key);
char *getCommand(int key);

#endif