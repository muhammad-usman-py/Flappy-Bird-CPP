#pragma once
#include <fstream>
#include <iostream>

// holds a single player's name and score
struct PlayerRecord{
    char name[50];
    int score;
};

class LeaderboardManager{
private:
    PlayerRecord records[100];
    int recordCount;
    const char* filename;

    // copies a char array without using strcpy
    void copyString(char* dest, const char* src);

public:
    LeaderboardManager();
    
    void LoadFromFile();
    void SaveToFile();
    void AddRecord(const char* playerName, int score);
    void SortRecords();
    void DisplayLeaderboard();
};