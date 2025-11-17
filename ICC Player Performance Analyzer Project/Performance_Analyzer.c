#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"

#define INITIAL_PLAYER_COUNT sizeof(players) / sizeof(players[0])
#define INITIAL_TEAM_COUNT 10

// For player node
typedef struct MainPlayer
{
    int PlayerId;
    char *Name;
    char *TeamName;
    char *TeamNo;
    char *Role;
    int TotalRuns;
    float BattingAverage;
    float StrikeRate;
    int Wickets;
    float EconomyRate;
    float PerformanceIndex;
    struct MainPlayer *prev;
    struct MainPlayer *next;
} MainPlayer;

// For player list in teams
typedef struct PlayerInTeamNode
{
    MainPlayer *data;
    struct PlayerInTeamNode *next;
    struct PlayerInTeamNode *prev;
} PlayerInTeamNode;

// For Team node
typedef struct MainTeam
{
    int TeamId;
    char *Name;
    int TotalPlayers;
    float AverageBattingStrikerate;
    PlayerInTeamNode *TeamListHead;
    PlayerInTeamNode *TeamListTail;
} MainTeam;

// For main global player list.
typedef struct MainPlayerList
{
    MainPlayer *head;
    MainPlayer *tail;
} MainPlayerList;

MainPlayerList *mainPlayerList = NULL;
MainTeam *mainTeams = NULL;

int findTeamNo(const char *teamName)
{
    for (int teamNo = 0; teamNo < INITIAL_TEAM_COUNT; teamNo++)
    {
        if (strcmp(teamName, (mainTeams + teamNo)->Name) == 0)
        {
            return teamNo;
        }
    }
    return -1;
}

// Add player to global player list in decending order of performance index
void insertInPlayerList(MainPlayer *newPlayer)
{
    // If the list is empty, insert as the first node
    if (mainPlayerList->head == NULL)
    {
        mainPlayerList->head = newPlayer;
        mainPlayerList->tail = newPlayer;
        newPlayer->prev = NULL;
        newPlayer->next = NULL;
        return;
    }

    MainPlayer *curr = mainPlayerList->head;

    // the correct position in descending order
    while (curr != NULL && curr->PerformanceIndex > newPlayer->PerformanceIndex)
    {
        curr = curr->next;
    }

    if (curr == mainPlayerList->head)
    {
        // Insert at the start
        newPlayer->next = mainPlayerList->head;
        newPlayer->prev = NULL;
        mainPlayerList->head->prev = newPlayer;
        mainPlayerList->head = newPlayer;
    }
    else if (curr == NULL)
    {
        // Insert at the last
        newPlayer->next = NULL;
        newPlayer->prev = mainPlayerList->tail;
        mainPlayerList->tail->next = newPlayer;
        mainPlayerList->tail = newPlayer;
    }
    else
    {
        // Insert in the middle
        newPlayer->next = curr;
        newPlayer->prev = curr->prev;
        curr->prev->next = newPlayer;
        curr->prev = newPlayer;
    }
}

void fixTeamOrder(int updatedIndex)
{
    // fixing the order by performing left or rigth swap like insertion sort
    while (updatedIndex < INITIAL_TEAM_COUNT - 1 &&
           mainTeams[updatedIndex].AverageBattingStrikerate < mainTeams[updatedIndex + 1].AverageBattingStrikerate)
    {
        MainTeam temp = mainTeams[updatedIndex];
        mainTeams[updatedIndex] = mainTeams[updatedIndex + 1];
        mainTeams[updatedIndex + 1] = temp;
        updatedIndex++;
    }

    while (updatedIndex > 0 &&
           mainTeams[updatedIndex].AverageBattingStrikerate > mainTeams[updatedIndex - 1].AverageBattingStrikerate)
    {
        MainTeam temp = mainTeams[updatedIndex];
        mainTeams[updatedIndex] = mainTeams[updatedIndex - 1];
        mainTeams[updatedIndex - 1] = temp;
        updatedIndex--;
    }
}

void insertInTeamList(MainTeam *team, PlayerInTeamNode *newTeamMember)
{
    // If the team list is empty
    if (team->TeamListHead == NULL)
    {
        team->TeamListHead = newTeamMember;
        team->TeamListTail = newTeamMember;
    }
    else
    {
        PlayerInTeamNode *curr = team->TeamListHead;
        // Find the correct position (descending order)
        while (curr != NULL && curr->data->PerformanceIndex >= newTeamMember->data->PerformanceIndex)
        {
            curr = curr->next;
        }

        if (curr == team->TeamListHead)
        {
            // Insert at the beginning
            newTeamMember->next = team->TeamListHead;
            team->TeamListHead->prev = newTeamMember;
            team->TeamListHead = newTeamMember;
        }
        else if (curr == NULL)
        {
            // Insert at the end
            newTeamMember->prev = team->TeamListTail;
            team->TeamListTail->next = newTeamMember;
            team->TeamListTail = newTeamMember;
        }
        else
        {
            // Insert in the middle
            newTeamMember->next = curr;
            newTeamMember->prev = curr->prev;
            curr->prev->next = newTeamMember;
            curr->prev = newTeamMember;
        }
    }

    // run onyl for allrounder or Batsman
    if (strcmp(newTeamMember->data->Role, "Batsman") == 0 || strcmp(newTeamMember->data->Role, "All-rounder") == 0)
    {
        team->AverageBattingStrikerate =
            ((team->AverageBattingStrikerate * team->TotalPlayers) + newTeamMember->data->StrikeRate) / (team->TotalPlayers + 1);
    }
    team->TotalPlayers += 1;
}

void initilizePlayers()
{
    for (int playerNo = 0; playerNo < INITIAL_PLAYER_COUNT; playerNo++)
    {
        // Adding new player to list
        MainPlayer *newPlayer = (MainPlayer *)malloc(sizeof(MainPlayer));
        if (newPlayer == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        newPlayer->PlayerId = (players + playerNo)->id;
        newPlayer->Name = (char *)malloc(51 * sizeof(char));
        if (newPlayer->Name == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        strcpy(newPlayer->Name, (players + playerNo)->name);
        newPlayer->TeamName = (char *)malloc(51 * sizeof(char));
        if (newPlayer->TeamName == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        strcpy(newPlayer->TeamName, (players + playerNo)->team);
        newPlayer->Role = (char *)malloc(51 * sizeof(char));
        if (newPlayer->Role == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        strcpy(newPlayer->Role, (players + playerNo)->role);
        newPlayer->TotalRuns = (players + playerNo)->totalRuns;
        newPlayer->BattingAverage = (players + playerNo)->battingAverage;
        newPlayer->StrikeRate = (players + playerNo)->strikeRate;
        newPlayer->Wickets = (players + playerNo)->wickets;
        newPlayer->EconomyRate = (players + playerNo)->economyRate;

        // Add the performance matrix
        if (strcmp(newPlayer->Role, "Bowler") == 0)
        {
            newPlayer->PerformanceIndex = ((double)newPlayer->Wickets * 2) + (100 - newPlayer->EconomyRate);
        }
        else if (strcmp(newPlayer->Role, "Batsman") == 0)
        {
            newPlayer->PerformanceIndex = ((double)newPlayer->BattingAverage * newPlayer->StrikeRate) / 100;
        }
        else if (strcmp(newPlayer->Role, "All-rounder") == 0)
        {
            newPlayer->PerformanceIndex = (((double)newPlayer->BattingAverage * newPlayer->StrikeRate) / 100) + (newPlayer->Wickets * 2);
        }

        insertInPlayerList(newPlayer);

        // Adding new player to team.
        PlayerInTeamNode *newTeamMember = (PlayerInTeamNode *)malloc(sizeof(PlayerInTeamNode));
        if (newTeamMember == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        newTeamMember->data = newPlayer;
        newTeamMember->next = NULL;
        newTeamMember->prev = NULL;
        int teamNo = findTeamNo(newPlayer->TeamName);
        MainTeam *team = mainTeams + teamNo;
        insertInTeamList(team, newTeamMember);

        // Fixing the team order after nw player added
        fixTeamOrder(teamNo);
    }
}

void initilizeTeams()
{
    for (int teamNo = 0; teamNo < INITIAL_TEAM_COUNT; teamNo++)
    {
        MainTeam *newTeam = (MainTeam *)malloc(sizeof(MainTeam));
        if (newTeam == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        newTeam->Name = (char *)malloc(51 * sizeof(char));
        if (newTeam->Name == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }

        strcpy(newTeam->Name, teams[teamNo]);
        newTeam->TeamId = teamNo + 1;
        newTeam->AverageBattingStrikerate = 0;
        newTeam->TeamListHead = NULL;
        newTeam->TeamListTail = NULL;
        newTeam->TotalPlayers = 0;
        *(mainTeams + teamNo) = *newTeam;
    }
}

void addNewPlayer()
{
    // User input
    printf("Enter Team ID to add player: ");
    int userInputTeamID;
    if (scanf("%d", &userInputTeamID) != 1 || userInputTeamID > 10 || userInputTeamID < 1)
    {
        printf("Team ID should be between 1 to 10.\n");
        return;
    }
    // Zero based indexing
    userInputTeamID--;

    printf("Enter Player Details:\nPlayer ID: ");
    int userInputPlayerID;
    if (scanf("%d", &userInputPlayerID) != 1 || userInputPlayerID > 1500 || userInputPlayerID < 1)
    {
        printf("User ID should be between 1 to 1500.\n");
        return;
    }

    printf("Name: ");
    char *userInputPlayerName = (char *)malloc(51 * sizeof(char));
    if (userInputPlayerName == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    fgets(userInputPlayerName, 50, stdin);
    if (strlen(userInputPlayerName) == 0)
    {
        printf("Invalid Player Name.\n");
        return;
    }

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int userInputRoleID;
    if (scanf("%d", &userInputRoleID) != 1 || userInputRoleID > 3 || userInputRoleID < 1)
    {
        printf("Role should be between 1 to 3.\n");
        return;
    }

    printf("Total Runs: ");
    int userInputTotalRuns;
    if (scanf("%d", &userInputTotalRuns) != 1)
    {
        printf("Invalid Runs.\n");
        return;
    }

    printf("Batting Average: ");
    float userInputBattingAverage;
    if (scanf("%f", &userInputBattingAverage) != 1)
    {
        printf("Invalid Batting Average.\n");
        return;
    }

    printf("Strike Rate: ");
    float userInputStrikeRate;
    if (scanf("%f", &userInputStrikeRate) != 1)
    {
        printf("Invalid Strike Rate.\n");
        return;
    }

    printf("Wickets: ");
    int userInputWickets;
    if (scanf("%d", &userInputWickets) != 1)
    {
        printf("Invalid Wickets.\n");
        return;
    }

    printf("Economy Rate: ");
    float userInputEconomyRate;
    if (scanf("%f", &userInputEconomyRate) != 1)
    {
        printf("Invalid Economy Rate.\n");
        return;
    }

    // Addition part
    MainPlayer *newPlayer = (MainPlayer *)malloc(sizeof(MainPlayer));
    if (newPlayer == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    newPlayer->PlayerId = userInputPlayerID;
    newPlayer->Name = (char *)malloc(51 * sizeof(char));
    if (newPlayer->Name == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    strcpy(newPlayer->Name, userInputPlayerName);
    newPlayer->TeamName = (char *)malloc(51 * sizeof(char));
    if (newPlayer->TeamName == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    strcpy(newPlayer->TeamName, (mainTeams + userInputTeamID)->Name);
    newPlayer->TotalRuns = userInputTotalRuns;
    newPlayer->BattingAverage = userInputBattingAverage;
    newPlayer->StrikeRate = userInputStrikeRate;
    newPlayer->Wickets = userInputWickets;
    newPlayer->EconomyRate = userInputEconomyRate;
    newPlayer->Role = (char *)malloc(51 * sizeof(char));
    if (newPlayer->Role == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }

    // Add the performance matrix
    if (userInputRoleID == 2)
    {
        strcpy(newPlayer->Role, "Bowler");
        newPlayer->PerformanceIndex = ((double)newPlayer->Wickets * 2) + (100 - newPlayer->EconomyRate);
    }
    else if (userInputRoleID == 1)
    {
        strcpy(newPlayer->Role, "Batsman");
        newPlayer->PerformanceIndex = ((double)newPlayer->BattingAverage * newPlayer->StrikeRate) / 100;
    }
    else if (userInputRoleID == 3)
    {
        strcpy(newPlayer->Role, "All-rounder");
        newPlayer->PerformanceIndex = (((double)newPlayer->BattingAverage * newPlayer->StrikeRate) / 100) + (newPlayer->Wickets * 2);
    }

    insertInPlayerList(newPlayer);

    // Adding new player to team.
    PlayerInTeamNode *newTeamMember = (PlayerInTeamNode *)malloc(sizeof(PlayerInTeamNode));
    if (newTeamMember == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    newTeamMember->data = newPlayer;
    newTeamMember->next = NULL;
    newTeamMember->prev = NULL;

    int teamNo = userInputTeamID;
    MainTeam *team = mainTeams + teamNo;
    insertInTeamList(team, newTeamMember);

    // Fixing the team order after nw player added
    fixTeamOrder(teamNo);
    printf("Player added successfully to Team %s!", (mainTeams + userInputTeamID)->Name);
}

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

void userMenuDisplay()
{
    int userChoice = 0;
    do
    {
        printf("==============================================================================\n\tICC ODI Player Performance Analyzer\n==============================================================================\n1. Add Player to Team\n2. Display Players of a Specific Team\n3. Display Teams by Average Batting Strike Rate\n4. Display Top K Players of a Specific Team by Role\n5. Display all Players of specific role Across All Teams by performance index\n6. Exit\n==============================================================================\nEnter your choice: ");
        if (scanf("%d", &userChoice) != 1 || userChoice > 6 || userChoice < 1)
        {
            userChoice = -1;
            printf("Invalid choice please choose a number from 1 to 6.\n");
            continue;
        }

        if (userChoice == 1)
        {
            addNewPlayer();
        }
        else if (userChoice == 2)
        {
            displayAllPlayersOfTeam();
        }
        else if (userChoice == 3)
        {
            displayTeamsByAvgStr();
        }
        else if (userChoice == 4)
        {
            displayTopKPlayers();
        }
        else if (userChoice == 5)
        {
            displayAllPlayers();
        }
    } while (userChoice != 6);
}

void deallocation()
{
    // first free the Doubly list of players
    MainPlayer *temp = mainPlayerList->head;
    while (temp != NULL)
    {
        MainPlayer *nextPlayer = temp->next;
        // freeing the strings
        free(temp->Name);
        free(temp->TeamName);
        free(temp->Role);
        free(temp);

        temp = nextPlayer;
    }
    free(mainPlayerList);

    // Free all the teams and their associated list of players in them
    for (int i = 0; i < INITIAL_TEAM_COUNT; i++)
    {
        PlayerInTeamNode *currNode = (mainTeams + i)->TeamListHead;
        while (currNode != NULL)
        {
            PlayerInTeamNode *nextNode = currNode->next;
            free(currNode);
            currNode = nextNode;
        }
        free((mainTeams + i)->Name);
    }
    free(mainTeams);
}

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