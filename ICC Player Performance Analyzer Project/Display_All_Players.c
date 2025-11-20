#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

void displayAllPlayers()
{
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int userInputRoleID;
    if (scanf("%d", &userInputRoleID) != 1 || userInputRoleID > 3 || userInputRoleID < 1)
    {
        printf("Role should be between 1 to 3.\n");
        return;
    }

    const char *roleStr;
    if (userInputRoleID == 1)
        roleStr = "Batsman";
    else if (userInputRoleID == 2)
        roleStr = "Bowler";
    else
        roleStr = "All-rounder";

    printf("%s of all teams:\n", roleStr);
    printf("======================================================================================\nID\tName\tTeam\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n======================================================================================\n");

    MainPlayer *temp = mainPlayerList->head;
    while (temp != NULL)
    {
        if (strcmp(temp->Role, roleStr) == 0)
        {
            printf("%d\t%s\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
                   temp->PlayerId,
                   temp->Name,
                   temp->TeamName,
                   temp->Role,
                   temp->TotalRuns,
                   temp->BattingAverage,
                   temp->StrikeRate,
                   temp->Wickets,
                   temp->EconomyRate,
                   temp->PerformanceIndex);
        }
        temp = temp->next;
    }
    printf("======================================================================================\n");
}