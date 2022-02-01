//
// Created by William Aey on 1/25/2022.
//

#pragma once
#define SDL_ASTRO_PLAYER_H
#include "SDL_image.h"
#include "SDL.h"
#include "textureManager.h"
#include "game.h"
class player {
public:
    player(int x, int y);

    ~player();

    void update();

    void render();

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
