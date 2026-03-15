#include <SFML/Graphics.hpp>
#include "flappybird.h"
#include "LeaderboardManager.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    LeaderboardManager leaderboard;
    leaderboard.LoadFromFile();

    string playerName;
    cout<<"\n===================================\n";
    cout<<"     WELCOME TO FLAPPY BIRD\n";
    cout<<"===================================\n";
    cout<<"Enter your player name: ";
    getline(cin, playerName);
    while(playerName.empty()){
        cout<<"Name cannot be empty. Enter your player name: ";
        getline(cin, playerName);
    }
    cout<<"Get ready, "<<playerName<<"! Launching game...\n";

    sf::RenderWindow window(sf::VideoMode(800, 600), "Gaming Hub - Flappy Bird");
    window.setFramerateLimit(60);

    FlappyBird game(&window);
    game.play();

    const char* resultStr = game.getResult();

    int finalScore = 0;
    int i = 0;
    while (resultStr[i] != '\0') {
        finalScore = finalScore * 10 + (resultStr[i] - '0');
        i++;
    }

    cout<<"\nGame Over! Your score: "<<finalScore<<"\n";

    leaderboard.AddRecord(playerName.c_str(), finalScore);
    leaderboard.SortRecords();
    leaderboard.SaveToFile();
    leaderboard.DisplayLeaderboard();

    return 0;
}
