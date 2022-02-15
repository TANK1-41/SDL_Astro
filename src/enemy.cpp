//
// Created by William Aey on 1/31/2022.
//

#include "Astro/enemy.h"
#include "Astro/game.h"
#include "Astro/randomness.h"
#include "Astro/textureManager.h"


void generateRandomXCord(double &randomXSpawn, double &randomYSpawn, const int &windHight, const int &windWidth);


astroidMovment::astroidMovment(astroidMovment &&other) noexcept : instance(other.instance), slope(other.slope), direct(other.direct) {
    other.instance = nullptr;
}
astroidMovment &astroidMovment::operator=(astroidMovment &&other) noexcept {
    if (&other != this) {
        delete instance;
        instance = other.instance;
        slope = other.slope;
        direct = other.direct;
        other.instance = nullptr;
    }

    return *this;
}
astroidMovment::astroidMovment(astroids *instance, double slope, direction direct) : instance(instance), slope(slope), direct(direct) {
}
astroidMovment::~astroidMovment() {
    delete instance;
}

astroidManager::astroidManager(Game *game, SDL_Window *window) : game(game) {
    SDL_GetWindowSize(window, &windowWidth, &windowHight);
}

void astroidManager::createAstroid() {
    static int frameCounter = 50;
    if (frameCounter >= 50) {

        double randomXSpawn{0};
        double randomYSpawn{0};

        generateRandomXCord(randomXSpawn, randomYSpawn, windowHight, windowWidth);

        double slopeY = (windowHight / 2.f) - (randomYSpawn);
        double slopeX = (windowWidth / 2.f) - (randomXSpawn);
        double slope = slopeY / slopeX;


        auto *ast = new astroids(game, randomXSpawn, randomYSpawn);

        if (randomXSpawn > windowWidth) {
            movementSlope.emplace_back(ast, slope, backwords);
        } else {
            movementSlope.emplace_back(ast, slope, forward);
        }
        frameCounter = 0;
    } else {
        frameCounter++;
    }
}

void astroidManager::update() {
    for (auto &as: movementSlope) {
        as.instance->update(as.slope, as.direct);
    }
    /*
   * deletes out of range astroids and astroids with to greate of slope
   * */
    int x, y;
    SDL_GetWindowSize(game->getWindow(), &x, &y);
    for (size_t i = 0; i < movementSlope.size(); ++i) {
        SDL_Rect *location = movementSlope.at(i).instance->getDestRect();
        if (location->x > x + 2000 || location->x < x - 2000 || movementSlope.at(i).slope > 5) {
            movementSlope.erase(movementSlope.begin() + i);
            i--;
        } else if (location->y > y + 2000 || location->y < y - 2000 || movementSlope.at(i).slope < -5) {
            movementSlope.erase(movementSlope.begin() + i);
            i--;
        }
    }
}

void astroidManager::render() {
    for (auto &as: movementSlope) {
        as.instance->render();
    }
}

std::vector<astroidMovment> *astroidManager::getMovementSlope() {
    std::vector<astroidMovment> *movementTemp = &movementSlope;
    return movementTemp;
}

astroids::astroids(Game *game, int x, int y) : game(game) {
    astroidTex = game->textures.loadTexture("assets/astroid.png");
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = destRect.w = 64;
    srcRect.h = destRect.h = 64;
    destRect.x = x;
    destRect.y = y;
}

void astroids::update(const double &slope, const direction &direct) {
    double nextY{0};
    //find next point on line, point slope form
    if (direct == forward) {
        if (slope > 2 || slope < -2) {
            nextY = -1 * (slope * ((destRect.x - 0.5f) - destRect.x)) + destRect.y;
            destRect.x += 0.5f;
        } else {
            nextY = -1 * (slope * ((destRect.x - 2.f) - destRect.x)) + destRect.y;
            destRect.x += 2.f;
        }
    } else if (direct == backwords) {
        if (slope > 2 || slope < -2) {
            nextY = -1 * (slope * ((destRect.x + 0.5f) - destRect.x)) + destRect.y;
            destRect.x -= 0.5f;
        } else {
            nextY = -1 * (slope * ((destRect.x + 2.f) - destRect.x)) + destRect.y;
            destRect.x -= 2.f;
        }

    } else {
        throw "Astroid movement error";
    }
    double movementAmountY = nextY - destRect.y;
    destRect.y += movementAmountY;
}

void astroids::render() {
    SDL_RenderCopy(game->renderer, astroidTex, &srcRect, &destRect);
}

SDL_Rect *astroids::getDestRect() {
    SDL_Rect *temp = &destRect;
    return temp;
}

bool coinFlip() {
    return std::uniform_int_distribution<int>{0, 1}(randomness::engine) == 1;
}

void generateRandomXCord(double &randomXSpawn, double &randomYSpawn, const int &windHight, const int &windWidth) {
    randomXSpawn = std::uniform_real_distribution<double>{-1200.0, 1200.0}(randomness::engine);
    //uses equation of a circle to find spawn locations
    // y = k + sqrt(r^2 - (x-u)^2)
    if (randomXSpawn > 0) {
        randomYSpawn = (windWidth / 2.f) +
                       std::sqrt(pow(windHight, 2) -
                                 pow((randomXSpawn - (windHight)), 2));
    } else {
        // makes it possible to have a negative y cord if x is neg
        randomYSpawn = (windWidth / 2.f) -
                       std::sqrt(pow(windHight, 2) -
                                 pow(((randomXSpawn * -1.f) - (windHight)), 2));
        //y = (800/2) - sqrt((600^2) - (1293 *-1) - 600)^2) will cause an error because of imaginary num
        randomYSpawn *= -1.f;
    }
    //gives random chance of flipping asteroid
    if (coinFlip() && randomXSpawn < -100) {
        randomXSpawn *= -1.f;
        randomYSpawn *= -1.f;
    }
}
