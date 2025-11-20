#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

//          POINTERS DECLARATION
MainPlayerList *mainPlayerList = NULL;
MainTeam *mainTeams = NULL;

int main(int argc, char **argv)
{
    mainTeams = (MainTeam *)malloc(INITIAL_TEAM_COUNT * sizeof(MainTeam));
    if (mainTeams == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    mainPlayerList = (MainPlayerList *)malloc(sizeof(MainPlayerList));
    if (mainPlayerList == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    mainPlayerList->head = NULL;
    mainPlayerList->tail = NULL;
    initilizeTeams();
    initilizePlayers();

    userMenuDisplay();

    deallocation();
}