#include <stdlib.h>
#include <stdio.h>
#include "Implementations.h"

Queue *ReadyQueue = NULL;
Queue *WaitingQueue = NULL;
Queue *TerminatedQueue = NULL;
HashMap *PCBHashMap = NULL;
int SystemClock = 0;
KillNode *KillListHead = NULL;

int main(int argc, char **argv)
{
    initilization();
    userInput();

    startScheduling();
}