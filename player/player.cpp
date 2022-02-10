//
// Created by TANK1_41 on 1/25/2022.
//

#include <Astro/player.h>
#include <Astro/textureManager.h>

projectileManager *proMan;

player::player(int x, int y) {
  xpos = x;
  ypos = y;
  playertex = textureManger::loadTexture("assets/player.png");
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = 64;
  srcRect.h = 64;
  destRect.w = 64;
  destRect.h = 64;
  spriteCenter = {28, 0};
  proMan = new projectileManager();
}

player::~player() {}

void player::update() {
  // gets mouse state and sets the angle that the spite should be angled at
  int mouseX;
  int mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  spriteAngle = atan2(ypos - mouseY, xpos - mouseX);
  spriteAngle = (spriteAngle * 180.f) / 3.141f;
  spriteAngle -= 90;
  /*
   * spawns new projectiles and updates them
   * */
  // TODO: Only shoot when player clicks left mouse button
  static int spawnRate{120};
  if (spawnRate >= 120) {
    projectile *shoot = new projectile(destRect, spriteAngle, mouseX, mouseY);
    proMan->projectiles.push_back(shoot);
    spawnRate = 0;
  } else {
    spawnRate++;
  }
  for (auto inst : proMan->projectiles) {
    inst->update();
  }
  /*
   * checks for keyboard input and applies directional movement
   * */
  if (Game::event.type == SDL_KEYDOWN) {
    switch (Game::event.key.keysym.sym) {
    case SDLK_w:
      yVelocity = -1;
      break;
    case SDLK_s:
      yVelocity = 1;
      break;
    case SDLK_a:
      xVelocity = -1;
      break;
    case SDLK_d:
      xVelocity = 1;
      break;
    }
  }

  if (Game::event.type == SDL_KEYUP) {
    switch (Game::event.key.keysym.sym) {
    case SDLK_w:
      yVelocity = 0;
      break;
    case SDLK_s:
      yVelocity = 0;
      break;
    case SDLK_a:
      xVelocity = 0;
      break;
    case SDLK_d:
      xVelocity = 0;
      break;
    }
  }

  if (yVelocity == 1) {
    ypos++;
  } else if (yVelocity == -1) {
    ypos--;
  }

  if (xVelocity == 1) {
    xpos++;
  } else if (xVelocity == -1) {
    xpos--;
  }
  // sets active position
  destRect.x = xpos;
  destRect.y = ypos;
}

void player::render() {
  SDL_RenderCopyEx(Game::renderer, playertex, &srcRect, &destRect, spriteAngle,
                   &spriteCenter, SDL_FLIP_NONE);
  // renders all projectiles
  for (auto instRender : proMan->projectiles) {
    instRender->render();
  }
}

SDL_Rect *player::getDestRect() {
  SDL_Rect *temp = &destRect;
  return temp;
}

std::vector<projectile *> *player::getproMan() {
  // returns a pointer to the vector
  return proMan->getProjectiles();
}
