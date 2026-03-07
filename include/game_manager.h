//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_GAME_MANAGER_H
#define SNAILSPACE_GAME_MANAGER_H
#include "player.h"
#include "game_types.h"
#include <SDL_ttf.h>

#include "visual_novel.h"

typedef struct{
    bool isRunning;
    bool isLoading;
    Player player;
    Uint32 lastTime;
    SDL_Window* window;
    SDL_GLContext* glContext;
    GameScene scene;
    TextureAssets textureAssets;
    VisualNovelState visualNovelState;
} Game;

Game init_game(int screen_width, int screen_height, const char* player_name);

void scene_switch(Game* game, GameScene game_scene);

GameScene get_current_game_scene(Game* game);


#endif //SNAILSPACE_GAME_MANAGER_H