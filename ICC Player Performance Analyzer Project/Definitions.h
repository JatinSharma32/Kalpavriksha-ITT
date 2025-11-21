#ifndef DEFINITIONS_H
#define DEFINITIONS_H
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

//          POINTERS DECLARATION
extern MainPlayerList *mainPlayerList;
extern MainTeam *mainTeams;

//          FUNCTIONS DECLARATION
int findTeamNo(const char *teamName);

void insertInPlayerList(MainPlayer *newPlayer);

void fixTeamOrder(int updatedIndex);

void insertInTeamList(MainTeam *team, PlayerInTeamNode *newTeamMember);

void initilizePlayers();

void initilizeTeams();

void addNewPlayer();

void displayAllPlayersOfTeam();

void displayTeamsByAvgStr();

void displayTopKPlayers();

void displayAllPlayers();

void userMenuDisplay();

void deallocation();

#endif