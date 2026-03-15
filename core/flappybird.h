#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

// ... rest of your class stays exactly the same

class FlappyBird {
private:
    sf::RenderWindow* window;
    
    sf::Texture birdTex;
    sf::Texture pipeTex;
    sf::Texture bgTex;

    sf::Sprite bird;
    sf::Sprite pipes[6];
    sf::Sprite bgSprite;

    sf::Font font;
    sf::Text scoreText;
    sf::Text controlText;
    sf::Text gameOverText;
    sf::RectangleShape topBar; // The dark background box for the text

    // We need a small helper to safely convert live score to text
    void updateScoreText();
    void writeResultFromScore(int value);
    
    // Physics
    float gravity;
    float velocity;
    float jumpStrength;
    float pipeSpeed;
    
    // Game State
    bool gameOver;
    bool isPaused;
    int score;
    int sessionBestScore;
    char resultString[50]; 
    
    void resetPositions();

public:
    FlappyBird(sf::RenderWindow* win);
    ~FlappyBird();
    
    void play();
    const char* getResult(); 
};