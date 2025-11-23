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
    createHashTable(capacity);
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

// when the chache is full we need to remove the last node from DLL
DLLQueNode *removeTail()
{
    DLLQueNode *deleteNode = cache->qTail;
    if (deleteNode != NULL)
    {
        removeNode(deleteNode);
    }
    return deleteNode;
}

DLLQueNode *createDLLNode(int key, char *value)
{
    DLLQueNode *newNode = (DLLQueNode *)malloc(sizeof(DLLQueNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->Key = key;
    int valueLen = strlen(value);
    newNode->Value = (char *)malloc((valueLen + 1) * sizeof(char));
    if (newNode->Value == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strcpy(newNode->Value, value);
    newNode->Prev = NULL;
    newNode->Next = NULL;
    return newNode;
}

void putInHashMap(int key, DLLQueNode *node)
{
    // find the index in hash table usign hash funcion
    int index = hashFunction(key, hashTable->HashSize);
    HashMapNode *newNode = (HashMapNode *)malloc(sizeof(HashMapNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    // put the new node in the hash table
    newNode->Key = key;
    newNode->QueueNode = node;
    newNode->Next = *(hashTable->HashTable + index);
    *(hashTable->HashTable + index) = newNode;
}

// Remove a node from the hash table
void mapRemove(int key)
{
    int index = hashFunction(key, hashTable->HashSize);
    // Getting the linked list head of the bucket
    HashMapNode *temp = *(hashTable->HashTable + index);
    HashMapNode *prev = NULL;

    while (temp != NULL)
    {
        if (temp->Key == key)
        {
            // If key in the list then remove it
            if (prev == NULL)
            {
                *(hashTable->HashTable + index) = temp->Next;
            }
            else
            {
                prev->Next = temp->Next;
            }
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->Next;
    }
}

// find the node in hash map
DLLQueNode *mapGet(int key)
{
    int index = hashFunction(key, hashTable->HashSize);
    // Get the index in hash table by hashing the key.
    HashMapNode *temp = *(hashTable->HashTable + index);

    while (temp != NULL)
    {
        if (temp->Key == key)
        {
            // key found so return the node associated with it
            return temp->QueueNode;
        }
        temp = temp->Next;
    }

    return NULL;
}