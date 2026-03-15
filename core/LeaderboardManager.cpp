#include "LeaderboardManager.h"
#include <string>
using namespace std;

LeaderboardManager::LeaderboardManager(){
    recordCount = 0;
    filename = "scores.txt";
}

void LeaderboardManager::copyString(char* dest, const char* src){
    int i = 0;
    while(src[i] != '\0' && i < 49){
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void LeaderboardManager::LoadFromFile(){
    recordCount = 0;
    ifstream inFile(filename);
    if(inFile.is_open()){
        string line;
        while(getline(inFile, line)){
            if(line.empty()){
                continue;
            }

            int separatorIndex = -1;
            for(int i = 0; i < static_cast<int>(line.size()); i++){
                if(line[i] == '|'){
                    separatorIndex = i;
                    break;
                }
            }

            if(separatorIndex == -1){
                // Backward compatibility: old format was "name score".
                for(int i = static_cast<int>(line.size()) - 1; i >= 0; i--){
                    if(line[i] == ' '){
                        separatorIndex = i;
                        break;
                    }
                }
            }

            if(separatorIndex <= 0 || separatorIndex >= static_cast<int>(line.size()) - 1){
                continue;
            }

            string namePart = line.substr(0, separatorIndex);
            string scorePart = line.substr(separatorIndex + 1);

            int parsedScore = 0;
            bool validScore = true;
            for(int i = 0; i < static_cast<int>(scorePart.size()); i++){
                if(scorePart[i] < '0' || scorePart[i] > '9'){
                    validScore = false;
                    break;
                }
                parsedScore = parsedScore * 10 + (scorePart[i] - '0');
            }

            if(!validScore){
                continue;
            }

            copyString(records[recordCount].name, namePart.c_str());
            records[recordCount].score = parsedScore;
            recordCount++;
            if(recordCount >= 100) break;
        }
        inFile.close();
    }
}

void LeaderboardManager::SaveToFile(){
    ofstream outFile(filename);
    if(outFile.is_open()){
        for(int i = 0; i < recordCount; i++){
            outFile<<records[i].name<<"|"<<records[i].score<<"\n";
        }
        outFile.close();
    }
}

void LeaderboardManager::AddRecord(const char* playerName, int score){
    if(recordCount < 100){
        copyString(records[recordCount].name, playerName);
        records[recordCount].score = score;
        recordCount++;
    }
}

void LeaderboardManager::SortRecords(){
    for(int i = 0; i < recordCount - 1; i++){
        for(int j = 0; j < recordCount - i - 1; j++){
            if(records[j].score < records[j+1].score){
                PlayerRecord temp = records[j];
                records[j] = records[j+1];
                records[j+1] = temp;
            }
        }
    }
}

void LeaderboardManager::DisplayLeaderboard(){
    cout<<"\n=== TOP SCORES ===\n";
    if(recordCount == 0){
        cout<<"No records yet!\n";
    } else {
        for(int i = 0; i < recordCount && i < 5; i++){
            cout<<i+1<<". "<<records[i].name<<" - "<<records[i].score<<"\n";
        }
    }
    cout<<"==================\n";
}