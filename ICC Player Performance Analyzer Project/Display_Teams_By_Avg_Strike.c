#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

void displayTeamsByAvgStr()
{
    printf("Teams Sorted by Average Batting Strike Rate\n=========================================================\nID\tTeam Name\tAvg Bat SR\tTotal Players\n=========================================================\n");

    for (int i = 0; i < INITIAL_TEAM_COUNT; i++)
    {
        printf("%d\t%s\t%.2f\t\t%d\n",
               (mainTeams + i)->TeamId,
               (mainTeams + i)->Name,
               (mainTeams + i)->AverageBattingStrikerate,
               (mainTeams + i)->TotalPlayers);
    }

    printf("=========================================================\n");
}