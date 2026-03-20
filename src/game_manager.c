//
// Created by asgyu on 2026. márc. 7..
//
#include "game_manager.h"

#include <SDL_timer.h>
#include <stdio.h>

#include "dialogue_data.h"
#include "graphics.h"
#include "ui_manager.h"
#include "world_builder.h"

Game init_game(const int screen_width, const int screen_height, const char* player_name){
    Game game = {0};

    SDL_Window* window = SDL_CreateWindow("Snail's Pace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    game.window = window;
    game.glContext = SDL_GL_CreateContext(window);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    game.screen.screenWidth = screen_width;
    game.screen.screenHeight = screen_height;

    game.isRunning = true;
    game.isLoading = true;

    game.lastTime = SDL_GetTicks();

    world_initialization(&game, player_name);

    setup_projection(screen_width, screen_height);

    game.scene = MAIN_MENU;
    game.lastCheckpoint = VN_INTRO;
    scene_switch(&game, MAIN_MENU);
    return game;
}

void change_camera_input_handler(Game* game, bool is_enabled_movement, bool is_enabled_rotation){
    game->player.camera.isEnabledMovement = is_enabled_movement;
    game->player.camera.isEnabledRotation = is_enabled_rotation;
}

void scene_switch(Game* game, GameScene game_scene){
    printf("[INFO] Uj jelenet: ");

    if(game->scene != DEAD_ROOM && game->scene != MAIN_MENU){
        game->lastCheckpoint = game->scene;
    }

    game->scene = game_scene;

    build_scene(game, game_scene);
}

void load_last_checkpoint(Game* game){
    printf("[INFO] Elozo Ellenorzopont betoltese: %d\n", game->lastCheckpoint);
    scene_switch(game, game->lastCheckpoint);
}

GameScene get_current_game_scene(Game* game){
    return game->scene;
}

void handle_mouse_input_main_menu(SDL_Event* event, Game* game){
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        if(is_mouse_over_ui(&game->mainMenuUI.startButton, mouseX, mouseY)){
            scene_switch(game, VN_INTRO);
        }else if(is_mouse_over_ui(&game->mainMenuUI.invertYButton, mouseX, mouseY)){
            game->player.camera.isInvertedMouseY = !game->player.camera.isInvertedMouseY;
            const char* invert_text = game->player.camera.isInvertedMouseY ? "Inverz Y-tengely: BEKAPCSOLVA" : "Inverz Y-tengely: KIKAPCSOLVA";
            update_text_ui_element(&game->mainMenuUI.invertYButton, game->textureAssets.mainFont, invert_text, (SDL_Color){255, 255, 255, 255});
            game->mainMenuUI.invertYButton.x = (game->screen.screenWidth / 2) - (game->mainMenuUI.invertYButton.w / 2);
        }
    }
}