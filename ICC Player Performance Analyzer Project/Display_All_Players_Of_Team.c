#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

void displayAllPlayersOfTeam()
{
    printf("Enter Team ID: ");
    int userInputTeamID;
    if (scanf("%d", &userInputTeamID) != 1 || userInputTeamID > 10 || userInputTeamID < 1)
    {
        printf("Team ID should be between 1 to 10.\n");
        return;
    }

    userInputTeamID--;
    printf("====================================================================================\nID\tName\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n====================================================================================\n");

    int teamNo = findTeamNo(teams[userInputTeamID]);
    PlayerInTeamNode *temp = (mainTeams + teamNo)->TeamListHead;
    while (temp != NULL)
    {
        printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n", temp->data->PlayerId, temp->data->Name, temp->data->Role, temp->data->TotalRuns, temp->data->BattingAverage, temp->data->StrikeRate, temp->data->Wickets, temp->data->EconomyRate, temp->data->PerformanceIndex);
        temp = temp->next;
    }
    printf("Total Players: %d\nAverage Batting Strike Rate: %2f\n", (mainTeams + userInputTeamID)->TotalPlayers, (mainTeams + userInputTeamID)->AverageBattingStrikerate);
}