//
// Created by William Aey on 1/25/2022.
//

#pragma once

#include "projectiles.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <valarray>
#include <vector>

class Game;

class player {
    Game *game;

public:
    explicit player(Game *game, int x, int y);

    ~player();

    void update();

    void render();

    SDL_Rect *getDestRect();

private:
    SDL_Point spriteCenter;
    double spriteAngle;
    int xVelocity;
    int yVelocity;
    int xpos;
    int ypos;
    SDL_Rect srcRect, destRect;
    SDL_Texture *playertex;
};
