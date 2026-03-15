#include "flappybird.h"
#include<iostream>
using namespace std;

FlappyBird::FlappyBird(sf::RenderWindow* win){
    window = win;
    gravity = 0.5f;
    jumpStrength = -8.0f;
    pipeSpeed = 3.0f;

    srand(static_cast<unsigned>(time(0)));

    // scale bg to fill the window
    bgTex.loadFromFile("assets/bg.png");
    bgSprite.setTexture(bgTex);
    bgSprite.setScale(800.f / bgTex.getSize().x, 600.f / bgTex.getSize().y);

    // scale bird sprite to a good PC size
    birdTex.loadFromFile("assets/bird.png");
    bird.setTexture(birdTex);
    bird.setScale(40.f / birdTex.getSize().x, 30.f / birdTex.getSize().y);

    // scale pipe sprite to match original pipe dimensions
    pipeTex.loadFromFile("assets/pipe.png");
    for(int i = 0; i < 6; i++){
        pipes[i].setTexture(pipeTex);
        pipes[i].setScale(60.f / pipeTex.getSize().x, 500.f / pipeTex.getSize().y);
    }

    // load font from assets
    if(!font.loadFromFile("assets/font.ttf")){
        cout<<"WARNING: Missing font.ttf in assets folder!\n";
    }

    // semi-transparent bar at the top
    topBar.setSize(sf::Vector2f(800.f, 40.f));
    topBar.setFillColor(sf::Color(0, 0, 0, 150));
    topBar.setPosition(0.f, 0.f);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 5.f);

    controlText.setFont(font);
    controlText.setCharacterSize(20);
    controlText.setFillColor(sf::Color::Yellow);
    controlText.setPosition(400.f, 8.f);
    controlText.setString("Controls: [W]/[Space] Jump | [P] Pause");

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(120.f, 260.f);
    gameOverText.setString("Game Over! [R] Restart  [Q] Quit");

    resultString[0] = '\0';
    updateScoreText();
    resetPositions();
}

FlappyBird::~FlappyBird(){}

void FlappyBird::resetPositions(){
    bird.setPosition(100.f, 300.f);
    isPaused = false;
    velocity = 0;
    score = 0;
    gameOver = false;
    controlText.setString("Controls: [W]/[Space] Jump | [P] Pause");

    // place pipes in pairs, spaced out horizontally
    for(int i = 0; i < 6; i += 2){
        float startX = 500.f + (i * 150.f);
        float gapY = (rand() % 250) + 150.f;
        float gapSize = 150.f;

        pipes[i].setPosition(startX, gapY - 500.f);
        pipes[i+1].setPosition(startX, gapY + gapSize);
    }
}

void FlappyBird::writeResultFromScore(int value){
    int tempScore = value;
    int idx = 0;
    char tempBuffer[10];

    if(tempScore <= 0){
        resultString[0] = '0';
        resultString[1] = '\0';
        return;
    }

    while(tempScore > 0){
        tempBuffer[idx++] = (tempScore % 10) + '0';
        tempScore /= 10;
    }

    int j = 0;
    while(idx > 0){
        resultString[j++] = tempBuffer[--idx];
    }
    resultString[j] = '\0';
}

void FlappyBird::updateScoreText(){
    char liveStr[20] = "Score: ";
    int tempScore = score;
    int idx = 7;

    if(tempScore == 0){
        liveStr[idx++] = '0';
    } else {
        char numBuf[10];
        int nIdx = 0;
        while(tempScore > 0){
            numBuf[nIdx++] = (tempScore % 10) + '0';
            tempScore /= 10;
        }
        while(nIdx > 0){
            liveStr[idx++] = numBuf[--nIdx];
        }
    }
    liveStr[idx] = '\0';
    scoreText.setString(liveStr);
}

void FlappyBird::play(){
    resetPositions();
    sessionBestScore = 0;

    while(window->isOpen()){
        sf::Event event;
        while(window->pollEvent(event)){
            if(event.type == sf::Event::Closed){
                if(score > sessionBestScore){
                    sessionBestScore = score;
                }
                writeResultFromScore(sessionBestScore);
                window->close();
            }

            if(event.type == sf::Event::KeyPressed){
                if(gameOver){
                    if(event.key.code == sf::Keyboard::R){
                        resetPositions();
                        updateScoreText();
                    } else if(event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Escape){
                        if(score > sessionBestScore){
                            sessionBestScore = score;
                        }
                        writeResultFromScore(sessionBestScore);
                        window->close();
                    }
                    continue;
                }

                if(event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Space){
                    if(!isPaused){
                        velocity = jumpStrength;
                    }
                }
                if(event.key.code == sf::Keyboard::P){
                    isPaused = !isPaused;
                }
            }
        }

        if(!isPaused && !gameOver){
            velocity += gravity;
            bird.move(0, velocity);

            for(int i = 0; i < 6; i += 2){
                pipes[i].move(-pipeSpeed, 0);
                pipes[i+1].move(-pipeSpeed, 0);

                if(pipes[i].getPosition().x < -50.f){
                    float newX = 850.f;
                    float gapY = (rand() % 250) + 150.f;
                    float gapSize = 150.f;

                    pipes[i].setPosition(newX, gapY - 500.f);
                    pipes[i+1].setPosition(newX, gapY + gapSize);

                    score++;
                    if(score > sessionBestScore){
                        sessionBestScore = score;
                    }
                    updateScoreText();
                }
            }

            for(int i = 0; i < 6; i++){
                if(bird.getGlobalBounds().intersects(pipes[i].getGlobalBounds()) || bird.getPosition().y > 600.f || bird.getPosition().y < 0.f){
                    gameOver = true;
                    isPaused = true;
                    controlText.setString("Paused - [R] Restart | [Q] Quit");
                    if(score > sessionBestScore){
                        sessionBestScore = score;
                    }
                    writeResultFromScore(sessionBestScore);
                    break;
                }
            }
        }

        window->clear();
        window->draw(bgSprite);

        for(int i = 0; i < 6; i++){
            window->draw(pipes[i]);
        }

        window->draw(bird);
        window->draw(topBar);
        window->draw(scoreText);
        window->draw(controlText);
        if(gameOver){
            window->draw(gameOverText);
        }

        window->display();
    }
}

const char* FlappyBird::getResult(){
    return resultString;
}