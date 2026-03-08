#include <math.h>

#include "include/obj_loader.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <stdio.h>

#include "camera.h"
#include "dialogue_data.h"
#include "game_manager.h"
#include "graphics.h"
#include "player.h"
#include "ui_manager.h"
#ifdef _WIN32
    #include <windows.h>
    #include <lmcons.h>
    #define PLATFORM_GET_USER(buf, size) GetUserNameA(buf, &size)
    #define MAX_USER_LEN (UNLEN + 1)
#else
    #include <unistd.h>
    #include <limits.h>
    #define PLATFORM_GET_USER(buf, size) getlogin_r(buf, size)
    #define MAX_USER_LEN (LOGIN_NAME_MAX + 1)
#endif

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
        printf("[HIBA] SDL Init hiba: %s\n", SDL_GetError());
        return 1;
    }

    if(TTF_Init() == -1){
        printf("[HIBA] SDL_ttf nem indult el: %s\n", TTF_GetError());
        return 1;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("[HIBA] Nem sikerult inincializalni a Mixert: %s\n", Mix_GetError());
    }

    char winName[UNLEN + 1];
    DWORD winSize = sizeof(winName);
    GetUserName(winName, &winSize);
    Game game = init_game(SCREEN_WIDTH, SCREEN_HEIGHT, winName);
    Screen screen = {
        .screenWidth = SCREEN_WIDTH,
        .screenHeight = SCREEN_HEIGHT
    };

    Dialogue dialogue = create_dialogue_from_id(DLG_INTRO, winName, &game.textureAssets.Gyulasz_Scared);

    SDL_Event event;
    while(game.isRunning){
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime-game.lastTime)/1000.0f;
        game.lastTime = currentTime;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) game.isRunning = false;
            switch(game.scene){
                case VN_INTRO:
                case DEALER_ROOM:
                case PRE_BAT_VISION:
                    handle_mouse_input_visual_novel(&event, &game.visualNovelState, &dialogue, &game.textureAssets);
                    break;
                case BAT_VISION:
                    // handle_wasd_input(&event, &game.player.camera, &game.isRunning, deltaTime, game.sounds);
                case LIDAR:
                    handle_mouse_input(&event, &game.player.camera);
                    break;
                case LAST_ROOM:
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        switch(game.scene){
            case VN_INTRO:
                if(game.visualNovelState.currentDialogID == DLG_MONSTER_APPEARS){
                    setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
                    scene_switch(&game, DEALER_ROOM);
                }

                render_dialogue_box(SCREEN_WIDTH, SCREEN_HEIGHT, &dialogue);
                update_dialogue(&dialogue, currentTime);
                render_dialogue_name(&dialogue, game.textureAssets.mainFont);
                render_dialogue_text(&dialogue, game.textureAssets.mainFont);
                render_ui_texture(dialogue.speaker, screen);
                break;
            case DEALER_ROOM:
                setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
                update_camera_view(&game.player.camera);
                update_moveable_model_position(&game.gameObjects.Dealer, deltaTime);
                render_moveable_model(&game.gameObjects.Dealer);
                render_dialogue_box(SCREEN_WIDTH, SCREEN_HEIGHT, &dialogue);
                update_dialogue(&dialogue, currentTime);
                render_dialogue_text(&dialogue, game.textureAssets.mainFont);

                if(game.visualNovelState.currentDialogID == DLG_SCENE_SWITCH_BAT_VISION){
                    scene_switch(&game, PRE_BAT_VISION);
                }else if(game.visualNovelState.currentDialogID == DLG_GYULASZ_SIGN_THE_CONTRACT || game.visualNovelState.currentDialogID == DLG_GYULASZ_SIGN_THE_CONTRACT2){
                    render_dialogue_name(&dialogue, game.textureAssets.mainFont);
                }
                break;
            case PRE_BAT_VISION:
                if(game.visualNovelState.currentDialogID == DLG_GYULASZ_SEE_NOTHING4){
                    scene_switch(&game, BAT_VISION);
                }

                update_camera_view(&game.player.camera);

                render_bat_vision(&game.gameObjects.BatVisionMap, currentTime);

                render_dialogue_box(SCREEN_WIDTH, SCREEN_HEIGHT, &dialogue);
                update_dialogue(&dialogue, currentTime);
                render_dialogue_name(&dialogue, game.textureAssets.mainFont);
                render_dialogue_text(&dialogue, game.textureAssets.mainFont);
                render_ui_texture(dialogue.speaker, screen);
                break;
            case BAT_VISION:
                update_camera_view(&game.player.camera);
                render_bat_vision(&game.gameObjects.BatVisionMap, currentTime);
                check_player_collision(&game.player.camera, &game.gameObjects.BatVisionMap, 0.5f);
                break;
            case LIDAR:
                update_camera_view(&game.player.camera);
                render_lidar(game.gameObjects.LidarMap, &game.player.camera);
                break;
            case LAST_ROOM:

                break;
            default:
                break;
        }

        handle_wasd_input(&event, &game.player.camera, &game.isRunning, deltaTime, game.sounds);

        SDL_GL_SwapWindow(game.window);
    }

    SDL_GL_DeleteContext(game.glContext);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
    return 0;
}