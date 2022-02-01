//
// Created by William Aey on 2/1/2022.
//

#include "Check_collision.h"


void collision::checkCollision(std::vector<astroidMovment> &movementSlope, const player &pla) {
    leftA = pla.getDestRect().x;
    rightA = pla.getDestRect().x + pla.getDestRect().w;
    topA = pla.getDestRect().y;
    bottomA = pla.getDestRect().y + pla.getDestRect().h;

    for (size_t i{0}; i < movementSlope.size(); i++) {


        //SDL_bool collision = SDL_HasIntersection(movementSlope.at(i).instance->getDestRect(), pla.getDestRect());


        leftB = movementSlope.at(i).instance->getDestRect().x;
        rightB = movementSlope.at(i).instance->getDestRect().x + movementSlope.at(i).instance->getDestRect().w;
        topB = movementSlope.at(i).instance->getDestRect().y;
        bottomB = movementSlope.at(i).instance->getDestRect().y + movementSlope.at(i).instance->getDestRect().h;
        if (bottomA <= topB) {
            movementSlope.erase(movementSlope.begin() + i);
            i--;
            break;
        }

        if (topA >= bottomB) {
            movementSlope.erase(movementSlope.begin() + i);
            i--;
            break;
        }

        if (rightA <= leftB) {
            movementSlope.erase(movementSlope.begin() + i);
            i--;
            break;
        }

        if (leftA >= rightB) {
            movementSlope.erase(movementSlope.begin() + i);
            i--;
            break;
        }
    }

}


