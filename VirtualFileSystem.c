#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Defination Part
#define NUMBER_OF_BLOCKS 10 // Change it to 5000
#define BLOCK_SIZE 16
char **virtualMemory; // Declearing it globally so that its accessible from everywhere

// Structure Creation Data
typedef struct FreeBlockNodeStruct
{
    int index;
    struct FreeBlockNodeStruct *prev;
    struct FreeBlockNodeStruct *next;
} FreeBlockNodeStruct;

typedef struct FreeBlockNodeListStruct
{
    struct FreeBlockNodeStruct *head;
    struct FreeBlockNodeStruct *tail;
    int numberOfFreeBlocks;
} FreeBlockNodeListStruct;

typedef struct FileNodeStruct
{
    char *name;
    struct FileNodeStruct *next;
    struct FileNodeStruct *parent;
    struct FileNodeStruct *child;
    int *blockPointers;
    bool file;
    int numberOfBlocks;
    int numberOfChildren;

} FileNodeStruct;

typedef struct FileSystemListStruct
{
    FileNodeStruct *root;
    FileNodeStruct *pwd;
} FileSystemListStruct;

// Declearing them globally so that they are accessible from everywhere
FreeBlockNodeListStruct *freeBlockNodeList = NULL;
FileSystemListStruct *fileSystemList = NULL;

FileNodeStruct *findChildren(char *fileName)
{
    FileNodeStruct *targetFile = fileSystemList->pwd->child;
    if (targetFile == NULL)
        return NULL;
    FileNodeStruct *start = targetFile;
    do
    {
        if (strcmp(targetFile->name, fileName) == 0)
        {
            return targetFile;
        }
        targetFile = targetFile->next;
    } while (targetFile != start);
    return NULL;
}

// This function can be used to see the free list, to see list fragmentation
void printFreeList()
{
    printf("Printing List: ");
    FreeBlockNodeStruct *temp = freeBlockNodeList->head;
    if (temp == NULL)
    {
        printf("\n");
        return;
    }
    do
    {
        printf("%d ", temp->index);
        temp = temp->next;
    } while (temp != freeBlockNodeList->head);
    printf("\n");
}

void makeFileAndDirectory(char *fileName, bool isFile)
{
    // Creating new Directory
    FileNodeStruct *pwdChild = fileSystemList->pwd->child;
    FileNodeStruct *newFolder = (FileNodeStruct *)malloc(sizeof(FileNodeStruct));
    if (newFolder == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    // Giving it name
    if (findChildren(fileName) != NULL)
    {
        printf("Already exist.\n");
        return;
    }
    newFolder->name = (char *)malloc(51 * sizeof(char));
    strcpy(newFolder->name, fileName);
    fileSystemList->pwd->numberOfChildren += 1;
    newFolder->parent = fileSystemList->pwd;
    newFolder->child = NULL;
    newFolder->numberOfChildren = 0;
    newFolder->numberOfBlocks = 0;
    if (isFile)
        newFolder->file = true;
    else
        newFolder->file = false;

    // If possible also assign some blocks to it initially with 0
    newFolder->blockPointers = NULL;

    // Adding this Node in the File system
    if (pwdChild == NULL)
    {
        fileSystemList->pwd->child = newFolder;
        newFolder->next = newFolder;
    }
    else
    {
        FileNodeStruct *tempNode = pwdChild;
        while (tempNode->next != pwdChild)
        {
            tempNode = tempNode->next;
        }
        newFolder->next = pwdChild;
        tempNode->next = newFolder;
    }
    if (isFile)
        printf("File '%s' created successfully.\n", fileName);
    else
        printf("Directory '%s' created successfully.\n", fileName);
}

void blockDeallocation(FileNodeStruct *targetFile, int iterator)
{
    FreeBlockNodeStruct *freeBlockNode = freeBlockNodeList->tail;
    // Roll back the free nodes 1 by one till we assigned.
    for (int block = 0; block < iterator; block++)
    {
        // Creation of new node
        FreeBlockNodeStruct *newFreeBlockNode = (FreeBlockNodeStruct *)malloc(sizeof(FreeBlockNodeStruct));
        if (newFreeBlockNode == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
        // putting data in it
        newFreeBlockNode->index = *(targetFile->blockPointers + block);
        if (freeBlockNode == NULL)
        {
            // If list empty then make new node head and tail.
            freeBlockNodeList->tail = newFreeBlockNode;
            freeBlockNodeList->head = newFreeBlockNode;
            newFreeBlockNode->next = newFreeBlockNode;
            newFreeBlockNode->prev = newFreeBlockNode;
        }
        else
        {
            // else just add this new node at tail
            freeBlockNodeList->tail->next = newFreeBlockNode;
            newFreeBlockNode->prev = freeBlockNodeList->tail;
            newFreeBlockNode->next = freeBlockNodeList->head;
            freeBlockNodeList->head->prev = newFreeBlockNode;
            freeBlockNodeList->tail = newFreeBlockNode;
        }
        freeBlockNodeList->numberOfFreeBlocks += 1;
    }
    // Deallocate the blockPointers list
    free(targetFile->blockPointers);
    targetFile->blockPointers = NULL;
}

bool blockAllocation(FileNodeStruct *targetFile, int numberOfBlocksRequired)
{
    if (numberOfBlocksRequired > freeBlockNodeList->numberOfFreeBlocks)
        return false;
    // Get these blocks from the FreeList in the index Array
    targetFile->blockPointers = (int *)malloc(numberOfBlocksRequired * sizeof(int));
    if (targetFile->blockPointers == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    int temp = numberOfBlocksRequired;
    FreeBlockNodeStruct *freeBlockNode = freeBlockNodeList->head;
    int iterator = 0;
    while (numberOfBlocksRequired > iterator && freeBlockNode != NULL)
    {
        // Putting the free node in to the blockPointers
        FreeBlockNodeStruct *tempFreeNode = freeBlockNode;
        *(targetFile->blockPointers + iterator) = tempFreeNode->index;
        // delete this node from list

        // FREE LIST's HEAD is cruppted.

        if (tempFreeNode != tempFreeNode->next)
        {
            tempFreeNode = tempFreeNode->next;
            tempFreeNode->prev = freeBlockNodeList->tail;
            freeBlockNodeList->tail->next = tempFreeNode;
            freeBlockNodeList->head = tempFreeNode;
        }
        else
        {
            // If only 1 element is left
            tempFreeNode = NULL;
            freeBlockNodeList->tail = NULL;
            freeBlockNodeList->head = NULL;
        }
        free(freeBlockNode);
        freeBlockNode = tempFreeNode;
        iterator++;
        // Reduce the free blocks
        freeBlockNodeList->numberOfFreeBlocks -= 1;
    }
    // check if Rollback is needed
    if (numberOfBlocksRequired == iterator)
    {
        return true; // No need
    }

    blockDeallocation(targetFile, iterator);
    return false;
}

void writeFile(char *fileName, char *fileData)
{
    // Find the file in the children list
    FileNodeStruct *targetFile = findChildren(fileName);
    if (targetFile == NULL)
    {
        printf("File Not found.\n");
        return;
    }
    // Clears the past content of file.
    if (targetFile->numberOfBlocks != 0)
    {
        blockDeallocation(targetFile, targetFile->numberOfBlocks);
        targetFile->numberOfBlocks = 0;
    }

    // Find the number of blocks required
    int fileDataLength = strlen(fileData) + 2; // adding 2 extra for \0 and then \n
    int numberOfBlocksRequired = fileDataLength / BLOCK_SIZE + (fileDataLength % BLOCK_SIZE != 0);
    targetFile->numberOfBlocks = numberOfBlocksRequired;
    // Do the block allocation
    if (!blockAllocation(targetFile, numberOfBlocksRequired))
    {
        // If rollback occurs
        targetFile->numberOfBlocks = 0;
        printf("Disk Full, not enough memory.\n");
        return;
    }
    // Populate data in it.
    int fileDataIterator = 0;
    for (int block = 0; block < numberOfBlocksRequired; block++)
    {
        bool fileEnd = false;
        // Here basically we are using the blockPointers values to find the free blocks
        char *blockRow = *(virtualMemory + *(targetFile->blockPointers + block));
        // This blockRow is 16 sized character array.
        for (int cell = 0; cell < BLOCK_SIZE; cell++)
        {
            // Enter the data in the virtual memory
            *(blockRow + cell) = *(fileData + fileDataIterator);
            fileDataIterator++;

            // If file ends then stop all the loops
            if (fileDataIterator >= fileDataLength)
            {
                fileEnd = true;
                break;
            }
        }
        if (fileEnd)
            break;
    }
    printf("Data written successfully (size=%d bytes).\n", fileDataLength);
}

void readFile(char *fileName)
{
    // Find the file in the children list
    FileNodeStruct *targetFile = findChildren(fileName);
    if (targetFile == NULL || !targetFile->file)
    {
        printf("File Not found.\n");
        return;
    }

    for (int block = 0; block < targetFile->numberOfBlocks; block++)
    {
        bool fileEnd = false;
        // Here basically we are using the blockPointers values to find the free blocks
        char *blockRow = *(virtualMemory + *(targetFile->blockPointers + block));
        // This blockRow is 16 sized character array.

        int cellIterator = 0;
        while (cellIterator < 16)
        {
            if (*(blockRow + cellIterator) == '\0')
            {
                fileEnd = true;
                break;
            }
            // Print the data in the cells
            printf("%c", *(blockRow + cellIterator));
            cellIterator++;
        }
        if (fileEnd)
            break;
    }
    printf("\n");
}

void deleteFile(char *fileName)
{
    // Find the file in the children list
    FileNodeStruct *targetFile = findChildren(fileName);
    if (targetFile == NULL)
    {
        printf("File Not found.\n");
        return;
    }

    // Free all the occupied blocks of memory, and re add them in free List
    blockDeallocation(targetFile, targetFile->numberOfBlocks);
    // Deallocate the fileName dynamic array.
    free(targetFile->name);

    // Delete the file from the File list
    FileNodeStruct *parentNode = targetFile->parent;
    FileNodeStruct *childListHead = parentNode->child;

    // Reduce the number of children in the parent directory

    // If only 1 node in the children list
    if (parentNode->numberOfChildren == 1)
    {
        parentNode->child = NULL;
    }
    else
    {
        // If our target file is the start of children list then we need to
        // update the parent's child pointer also
        if (childListHead == targetFile)
        {
            parentNode->child = targetFile->next;
        }
        while (childListHead->next != targetFile)
        {
            childListHead = childListHead->next;
        }
        childListHead->next = targetFile->next;
    }
    free(targetFile);
    parentNode->numberOfChildren -= 1;
    printf("File deleted successfully.\n");
}

void removeDirectory(char *dirName)
{
    // Find the file in the children list
    FileNodeStruct *targetFolder = findChildren(dirName);
    if (targetFolder == NULL)
    {
        printf("Directory Not found.\n");
        return;
    }
    else if (targetFolder == fileSystemList->root)
    {
        printf("Can't delete Root.\n");
        return;
    }
    else if (targetFolder->numberOfChildren != 0)
    {
        printf("Directory is not empty for deletion.\n");
        return;
    }

    // Deallocate the fileName dynamic array.
    free(targetFolder->name);

    // Delete the file from the File list
    FileNodeStruct *parentNode = targetFolder->parent;
    FileNodeStruct *childListHead = parentNode->child;

    // Reduce the number of children in the parent directory

    // If only 1 node in the children list
    if (parentNode->numberOfChildren == 1)
    {
        parentNode->child = NULL;
    }
    else
    {
        // If our target file is the start of children list then we need to
        // update the parent's child pointer also
        if (childListHead == targetFolder)
        {
            parentNode->child = targetFolder->next;
        }
        while (childListHead->next != targetFolder)
        {
            childListHead = childListHead->next;
        }
        childListHead->next = targetFolder->next;
    }
    free(targetFolder);
    parentNode->numberOfChildren -= 1;
    printf("Directory removed successfully.\n");
}

void listDirectoryContent()
{
    FileNodeStruct *childNode = fileSystemList->pwd->child;
    if (childNode == NULL)
    {
        printf("(empty)\n");
        return;
    }
    do
    {
        printf((childNode->file ? "%s " : "%s/ "), childNode->name);
        childNode = childNode->next;
    } while (childNode != fileSystemList->pwd->child);
    printf("\n");
}

void changeDirectory(char *newPath)
{
    if (strcmp(newPath, "..") == 0)
    {
        if (fileSystemList->pwd == fileSystemList->root)
        {
            printf("Can't go back from root.\n");
            return;
        }
        fileSystemList->pwd = fileSystemList->pwd->parent;
        printf("Moved to %s\n", fileSystemList->pwd->name);
        return;
    }

    FileNodeStruct *targetNode = findChildren(newPath);
    if (targetNode == NULL)
    {
        printf("Path Not found.\n");
        return;
    }
    if (targetNode->file)
    {
        printf("Can't change directory to a file.\n");
        return;
    }

    fileSystemList->pwd = targetNode;
    printf("Moved to %s\n", fileSystemList->pwd->name);
}

void presentWorkingDirectory(FileNodeStruct *directory)
{
    if (directory == fileSystemList->root)
    {

        printf("/", directory->name);
        return;
    }
    presentWorkingDirectory(directory->parent);
    printf("%s/", directory->name);
}

void diskStorageInfo()
{
    printf("Total Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\nDisk Usage: %.2f%%\n", NUMBER_OF_BLOCKS, NUMBER_OF_BLOCKS - freeBlockNodeList->numberOfFreeBlocks, freeBlockNodeList->numberOfFreeBlocks, (100.00 * (NUMBER_OF_BLOCKS - freeBlockNodeList->numberOfFreeBlocks) / (double)NUMBER_OF_BLOCKS));
}

void clearFileStructure(FileNodeStruct *fileNode)
{
    if (fileNode == NULL)
        return;
    FileNodeStruct *tempFileNode = fileNode;
    do
    {
        printf("Deleting %s...\n", tempFileNode->name);
        if (tempFileNode->file)
            blockDeallocation(tempFileNode, tempFileNode->numberOfBlocks);
        else
            clearFileStructure(tempFileNode->child);
        FileNodeStruct *freeThisNode = tempFileNode;
        tempFileNode = tempFileNode->next;
        free(freeThisNode->name);
        free(freeThisNode);
    } while (tempFileNode != fileNode);

    fileNode->child = NULL;
}

void clearFreeNodeList()
{
    FreeBlockNodeStruct *freeBlockNode = freeBlockNodeList->head;
    while (freeBlockNode != NULL)
    {
        // Putting the free node in to the blockPointers
        FreeBlockNodeStruct *tempFreeNode = freeBlockNode;
        // delete this node from list
        if (tempFreeNode != tempFreeNode->next)
        {
            tempFreeNode = tempFreeNode->next;
            tempFreeNode->prev = freeBlockNodeList->tail;
            freeBlockNodeList->tail->next = tempFreeNode;
        }
        else
        {
            // If only 1 element is left
            tempFreeNode = NULL;
            freeBlockNodeList->tail = NULL;
            freeBlockNodeList->head = NULL;
        }
        free(freeBlockNode);
        freeBlockNode = tempFreeNode;
        freeBlockNodeList->numberOfFreeBlocks -= 1;
    }
}

void exitVFS()
{
    for (int blocks = 1; blocks < NUMBER_OF_BLOCKS; blocks++)
    {
        free(*(virtualMemory + blocks));
    }
    clearFileStructure(fileSystemList->root);
    free(fileSystemList->root);
    clearFreeNodeList();
    free(fileSystemList);
    free(freeBlockNodeList);
    free(virtualMemory);
    printf("Memory released. Exiting program...\n");
    return;
}

void enterVFS()
{
    // Allocating the virtual memory
    virtualMemory = (char **)malloc(NUMBER_OF_BLOCKS * sizeof(char *));
    if (virtualMemory == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    *virtualMemory = (char *)malloc(BLOCK_SIZE * sizeof(char));
    if (*virtualMemory == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    // Add error handling for malloc fail at all of its used locations

    // Virtual Storage creation
    freeBlockNodeList = (FreeBlockNodeListStruct *)malloc(sizeof(FreeBlockNodeListStruct));
    if (freeBlockNodeList == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    freeBlockNodeList->numberOfFreeBlocks = NUMBER_OF_BLOCKS;
    FreeBlockNodeStruct *freeBlockNode = (FreeBlockNodeStruct *)malloc(sizeof(FreeBlockNodeStruct));
    if (freeBlockNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    freeBlockNode->index = 0;
    freeBlockNode->next = freeBlockNode;
    freeBlockNode->prev = freeBlockNode;
    freeBlockNodeList->head = freeBlockNode;
    freeBlockNodeList->tail = freeBlockNode;

    for (int blocks = 1; blocks < NUMBER_OF_BLOCKS; blocks++)
    {
        *(virtualMemory + blocks) = (char *)malloc(BLOCK_SIZE * sizeof(char));
        if (*(virtualMemory + blocks) == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
        freeBlockNode = (FreeBlockNodeStruct *)malloc(sizeof(FreeBlockNodeStruct));
        if (freeBlockNode == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
        freeBlockNode->index = blocks;
        freeBlockNode->next = freeBlockNodeList->head;
        freeBlockNode->prev = freeBlockNodeList->tail;
        freeBlockNodeList->tail->next = freeBlockNode;
        freeBlockNodeList->head->prev = freeBlockNode;
        freeBlockNodeList->tail = freeBlockNode;
    }

    // Root Node creation
    fileSystemList = (FileSystemListStruct *)malloc(sizeof(FileSystemListStruct));
    if (fileSystemList == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    fileSystemList->root = (FileNodeStruct *)malloc(sizeof(FileNodeStruct));
    if (fileSystemList->root == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    fileSystemList->root->child = NULL;
    fileSystemList->root->next = fileSystemList->root;
    fileSystemList->root->parent = NULL;
    fileSystemList->root->file = false;
    fileSystemList->root->numberOfChildren = 0;
    fileSystemList->root->name = (char *)malloc(2 * sizeof(char));
    if (fileSystemList->root->name == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strcpy(fileSystemList->root->name, "/");
    fileSystemList->pwd = fileSystemList->root;
}

void userCommandProcessing()
{
    printf("$ ./vfs\nCompact VFS - ready. Type 'exit' to quit.\n");
    do
    {
        char *userInput = (char *)malloc(1000 * sizeof(char));
        if (userInput == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }
        printf("%s > ", fileSystemList->pwd->name);
        fgets(userInput, 1000, stdin);
        userInput[strcspn(userInput, "\n")] = 0;

        int userInputItr = 0, commandItr = 0;
        int userInputLength = strlen(userInput);

        char *command = strtok(userInput, " ");
        char *fileName = strtok(NULL, " ");

        if (strcmp("mkdir", command) == 0)
        {
            makeFileAndDirectory(fileName, false);
        }
        else if (strcmp("create", command) == 0)
        {
            makeFileAndDirectory(fileName, true);
        }
        else if (strcmp("write", command) == 0)
        {

            // All this processing to extract the string in the quotes
            char *fileData = strtok(NULL, "");
            while (*fileData != '\0' && *fileData != '\n')
            {
                // for finding 1st quote
                if (*fileData == '"')
                {
                    fileData++;
                    break;
                }
                fileData++;
            }
            // for updating 2nd quote to '\0'
            fileData[strcspn(fileData, "\"")] = 0;

            writeFile(fileName, fileData);
        }
        else if (strcmp("read", command) == 0)
        {
            readFile(fileName);
        }
        else if (strcmp("delete", command) == 0)
        {
            deleteFile(fileName);
        }
        else if (strcmp("rmdir", command) == 0)
        {
            removeDirectory(fileName);
        }
        else if (strcmp("ls", command) == 0)
        {
            listDirectoryContent();
        }
        else if (strcmp("cd", command) == 0)
        {
            changeDirectory(fileName);
        }
        else if (strcmp("pwd", command) == 0)
        {
            presentWorkingDirectory(fileSystemList->pwd);
            printf("\n");
        }
        else if (strcmp("df", command) == 0)
        {
            diskStorageInfo();
        }
        else if (strcmp("exit", command) == 0)
        {
            return;
        }
        else
        {
            printf("INVALID COMMAND\n");
        }
        free(userInput);
        userInput = NULL;
    } while (true);
}

int main(int arg, char **argv)
{
    // Start the VFS and allocate the memory
    enterVFS();

    // User Command handling
    userCommandProcessing();

    // Free all the pointers
    exitVFS();
    return 0;
}