#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

void displayTopKPlayers()
{
    printf("Enter Team ID: ");
    int userInputTeamID;
    if (scanf("%d", &userInputTeamID) != 1 || userInputTeamID > 10 || userInputTeamID < 1)
    {
        printf("Team ID should be between 1 to 10.\n");
        return;
    }
    userInputTeamID--;

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int userInputRoleID;
    if (scanf("%d", &userInputRoleID) != 1 || userInputRoleID > 3 || userInputRoleID < 1)
    {
        printf("Role should be between 1 to 3.\n");
        return;
    }

    printf("Enter number of players: ");
    int k;
    if (scanf("%d", &k) != 1 || k < 1)
    {
        printf("Number of players should be positive.\n");
        return;
    }

    const char *roleStr;
    if (userInputRoleID == 1)
        roleStr = "Batsman";
    else if (userInputRoleID == 2)
        roleStr = "Bowler";
    else
        roleStr = "All-rounder";

    printf("Top %d %s of Team %s:\n", k, roleStr, (mainTeams + userInputTeamID)->Name);
    printf("====================================================================================\nID\tName\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n====================================================================================\n");

    int count = 0;
    int teamNo = findTeamNo(teams[userInputTeamID]);
    PlayerInTeamNode *temp = (mainTeams + teamNo)->TeamListHead;
    while (temp != NULL && count < k)
    {
        if (strcmp(temp->data->Role, roleStr) == 0)
        {
            printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
                   temp->data->PlayerId,
                   temp->data->Name,
                   temp->data->Role,
                   temp->data->TotalRuns,
                   temp->data->BattingAverage,
                   temp->data->StrikeRate,
                   temp->data->Wickets,
                   temp->data->EconomyRate,
                   temp->data->PerformanceIndex);
            count++;
        }
        temp = temp->next;
    }
    printf("====================================================================================\n");
}