#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "StructsAndDefinations.h"

// Hash table initilization
void createHashTable(int capacity)
{
    hashTable = (HashTable *)malloc(sizeof(HashTable));
    if (hashTable == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Making its size prime for less collisions
    hashTable->HashSize = capacity * 3 + 31;
    hashTable->HashTable = (HashMapNode **)calloc(hashTable->HashSize, sizeof(HashMapNode *));
    if (hashTable->HashTable == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
}

// Cache data structure creation as a DLL
void createCache(int capacity)
{
    cache = (Cache *)malloc(sizeof(Cache));
    cache->Capacity = capacity;
    cache->Size = 0;
    cache->qHead = NULL;
    cache->qTail = NULL;
    // also initilize the hash map
    initHashTable(capacity);
}

// Adding a new node to DLL's head in cache
void addToHead(DLLQueNode *newNode)
{
    if (cache->qHead == NULL)
    {
        // only one node in cache
        cache->qHead = newNode;
        cache->qTail = newNode;
        return;
    }
    // Add this new node in the existing list's head
    newNode->Next = cache->qHead;
    newNode->Prev = NULL;
    cache->qHead->Prev = newNode;
    cache->qHead = newNode;
}

void removeNode(DLLQueNode *newNode)
{
    // check if its head or not
    if (newNode->Prev != NULL)
    {
        newNode->Prev->Next = newNode->Next;
    }
    else
    {
        cache->qHead = newNode->Next;
    }

    // Check if its tail or not
    if (newNode->Next != NULL)
    {
        newNode->Next->Prev = newNode->Prev;
    }
    else
    {
        cache->qTail = newNode->Prev;
    }
}

void moveToHead(DLLQueNode *newNode)
{
    // Used when a existing node tried to accessed, so to make it MRU
    removeNode(newNode);
    addToHead(newNode);
}