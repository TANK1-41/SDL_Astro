//
// Created by TANK1_41 on 2/1/2022.
//

#pragma once

#include "enemy.h"
#include "player.h"
#include "projectiles.h"

class collision {
public:
    static bool checkCollision(std::vector<astroidMovment> *movementSlope,
                               player *pla);

    static int checkCollision(std::vector<astroidMovment> *movementSlope,
                              std::vector<projectile *> *projectiles);
};
