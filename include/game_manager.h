//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_GAME_MANAGER_H
#define SNAILSPACE_GAME_MANAGER_H
#include "player.h"
#include "game_types.h"
#include <SDL_ttf.h>

#include "obj_loader.h"
#include "visual_novel.h"
#include "sounds.h"

typedef struct{
    UIElement title;
    UIElement warning;
    UIElement startButton;
    UIElement invertYButton;
} MainMenuUI;

typedef struct{
    bool isRunning;
    bool isLoading;
    Player player;
    Uint32 lastTime;
    SDL_Window* window;
    SDL_GLContext* glContext;
    GameScene lastCheckpoint;
    GameScene scene;
    Sounds sounds;
    Screen screen;
    GameObjects gameObjects;
    TextureAssets textureAssets;
    VisualNovelState visualNovelState;
    GameTriggerZones triggerZones;
    MainMenuUI mainMenuUI;
} Game;

Game init_game(int screen_width, int screen_height, const char* player_name);

void scene_switch(Game* game, GameScene game_scene);

void load_last_checkpoint(Game* game);

GameScene get_current_game_scene(Game* game);

void change_camera_input_handler(Game* game, bool is_enabled_movement, bool is_enabled_rotation);

void handle_mouse_input_main_menu(SDL_Event* event, Game* game);

#endif //SNAILSPACE_GAME_MANAGER_H