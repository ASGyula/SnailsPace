//
// Created by asgyu on 2026. márc. 7..
//
#include "game_manager.h"

#include <SDL_timer.h>
#include <stdio.h>

#include "graphics.h"
#include "ui_manager.h"

Game init_game(const int screen_width, const int screen_height, const char* player_name){
    Game game;
    SDL_Window* window = SDL_CreateWindow("Snail's Pace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    game.window = window;
    game.glContext = SDL_GL_CreateContext(window);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    game.isRunning = true;
    game.isLoading = false;

    game.player.camera.screenWidth = screen_width;
    game.player.camera.screenHeight = screen_height;
    initialize_camera(&game.player.camera);

    game.visualNovelState.currentDialogID = 0;
    game.visualNovelState.playerName = player_name;

    game.lastTime = SDL_GetTicks();

    setup_projection(screen_width, screen_height);
    game.textureAssets.mainFont = TTF_OpenFont("assets/Fonts/Roboto.ttf", 24);

    if(game.textureAssets.mainFont == NULL){
        printf("[HIBA] Nem sikerult a Roboto fontot betolteni\n");
    }

    {
        game.textureAssets.Helsie_Happy.textureID = load_texture("External/Gemini/Helsie_Happy.png");
        game.textureAssets.Helsie_Happy.w = 400;
        game.textureAssets.Helsie_Happy.h = 400;
        game.textureAssets.Helsie_Happy.x = 0;
        game.textureAssets.Helsie_Happy.y = screen_height - game.textureAssets.Helsie_Scared.h;
        game.textureAssets.Helsie_Happy.isShowing = true;

        game.textureAssets.Helsie_Scared.textureID = load_texture("External/Gemini/Helsie_Scared.png");
        game.textureAssets.Helsie_Scared.w = 400;
        game.textureAssets.Helsie_Scared.h = 400;
        game.textureAssets.Helsie_Scared.x = 0;
        game.textureAssets.Helsie_Scared.y = screen_height - game.textureAssets.Helsie_Scared.h;
        game.textureAssets.Helsie_Scared.isShowing = true;

        game.textureAssets.Gyulasz_Brave.textureID = load_texture("External/Gemini/Gyulasz_Brave.png");
        game.textureAssets.Gyulasz_Brave.w = 400;
        game.textureAssets.Gyulasz_Brave.h = 450;
        game.textureAssets.Gyulasz_Brave.x = -10;
        game.textureAssets.Gyulasz_Brave.y = screen_height - game.textureAssets.Gyulasz_Brave.h;
        game.textureAssets.Gyulasz_Brave.isShowing = true;

        game.textureAssets.Gyulasz_Determined.textureID = load_texture("External/Gemini/Gyulasz_Determined.png");
        game.textureAssets.Gyulasz_Determined.w = 400;
        game.textureAssets.Gyulasz_Determined.h = 450;
        game.textureAssets.Gyulasz_Determined.x = -10;
        game.textureAssets.Gyulasz_Determined.y = screen_height - game.textureAssets.Gyulasz_Determined.h;
        game.textureAssets.Gyulasz_Determined.isShowing = true;

        game.textureAssets.Helsie_lt3_Gyula.textureID = load_texture("External/Gemini/Helsie_lt3_Gyula.png");
        game.textureAssets.Helsie_lt3_Gyula.w = 400;
        game.textureAssets.Helsie_lt3_Gyula.h = 450;
        game.textureAssets.Helsie_lt3_Gyula.x = -10;
        game.textureAssets.Helsie_lt3_Gyula.y = screen_height - game.textureAssets.Helsie_lt3_Gyula.h;
        game.textureAssets.Helsie_lt3_Gyula.isShowing = true;

        game.textureAssets.Gyulasz_Thinking.textureID = load_texture("External/Gemini/Gyulasz_Thinking.png");
        game.textureAssets.Gyulasz_Thinking.w = 400;
        game.textureAssets.Gyulasz_Thinking.h = 450;
        game.textureAssets.Gyulasz_Thinking.x = 0;
        game.textureAssets.Gyulasz_Thinking.y = screen_height - game.textureAssets.Gyulasz_Thinking.h;
        game.textureAssets.Gyulasz_Thinking.isShowing = true;

        game.textureAssets.Gyulasz_Have_an_evil_idea.textureID = load_texture("External/Gemini/Gyulasz_Have_an_evil_idea.png");
        game.textureAssets.Gyulasz_Have_an_evil_idea.w = 400;
        game.textureAssets.Gyulasz_Have_an_evil_idea.h = 450;
        game.textureAssets.Gyulasz_Have_an_evil_idea.x = 0;
        game.textureAssets.Gyulasz_Have_an_evil_idea.y = screen_height - game.textureAssets.Gyulasz_Have_an_evil_idea.h;
        game.textureAssets.Gyulasz_Have_an_evil_idea.isShowing = true;

        game.textureAssets.Gyulasz_Suprised.textureID = load_texture("External/Gemini/Gyulasz_Suprised.png");
        game.textureAssets.Gyulasz_Suprised.w = 400;
        game.textureAssets.Gyulasz_Suprised.h = 450;
        game.textureAssets.Gyulasz_Suprised.x = 0;
        game.textureAssets.Gyulasz_Suprised.y = screen_height - game.textureAssets.Gyulasz_Suprised.h;
        game.textureAssets.Gyulasz_Suprised.isShowing = true;

        game.textureAssets.Gyulasz_Scared.textureID = load_texture("External/Gemini/Gyulasz_Scared.png");
        game.textureAssets.Gyulasz_Scared.w = 400;
        game.textureAssets.Gyulasz_Scared.h = 450;
        game.textureAssets.Gyulasz_Scared.x = -10;
        game.textureAssets.Gyulasz_Scared.y = screen_height - game.textureAssets.Gyulasz_Scared.h;
        game.textureAssets.Gyulasz_Scared.isShowing = true;

        game.textureAssets.Gyulasz_Hand_Shake.textureID = load_texture("External/Gemini/Gyulasz_Hand_Shake.png");
        game.textureAssets.Gyulasz_Hand_Shake.w = 400;
        game.textureAssets.Gyulasz_Hand_Shake.h = 450;
        game.textureAssets.Gyulasz_Hand_Shake.x = 0;
        game.textureAssets.Gyulasz_Hand_Shake.y = screen_height - game.textureAssets.Gyulasz_Hand_Shake.h;
        game.textureAssets.Gyulasz_Hand_Shake.isShowing = true;

        game.textureAssets.Dealer.textureID = load_texture("External/Gemini/Dealer.png");
        game.textureAssets.Dealer.w = 400;
        game.textureAssets.Dealer.h = 450;
        game.textureAssets.Dealer.x = screen_width-game.textureAssets.Dealer.w;
        game.textureAssets.Dealer.y = screen_height - game.textureAssets.Dealer.h;
        game.textureAssets.Dealer.isShowing = true;

        game.textureAssets.Snail.textureID = load_texture("External/Gemini/Snail.png");
        game.textureAssets.Snail.w = 400;
        game.textureAssets.Snail.h = 450;
        game.textureAssets.Snail.x = screen_width-game.textureAssets.Snail.w;
        game.textureAssets.Snail.y = screen_height - game.textureAssets.Snail.h;
        game.textureAssets.Snail.isShowing = true;
    }

    scene_switch(&game, VN_INTRO);

    return game;
}

void change_camera_input_handler(Game* game, bool is_enabled_movement, bool is_enabled_rotation){
    game->player.camera.isEnabledMovement = is_enabled_movement;
    game->player.camera.isEnabledRotation = is_enabled_rotation;
}

void scene_switch(Game* game, GameScene game_scene){
    printf("[INFO] Uj jelenet: ");
    game->scene = game_scene;
    switch(game_scene){
        case VN_INTRO:
            printf("VN_INTRO\n");
            change_camera_input_handler(game, false, false);
            break;
        case DEALER_ROOM:
            printf("DEALER_ROOM\n");
            break;
        case BAT_VISION:
            printf("BAT_VISION\n");
            break;
        case LIDAR:
            printf("LIDAR\n");
            break;
        case LAST_ROOM:
            printf("LAST_ROOM\n");
            break;
        default:
            printf("default\n");
            break;
    }
}

GameScene get_current_game_scene(Game* game){
    return game->scene;
}