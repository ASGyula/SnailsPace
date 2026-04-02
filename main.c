#include <math.h>

#include "include/obj_loader.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <stdio.h>

#include "ai.h"
#include "camera.h"
#include "dialogue_data.h"
#include "game_manager.h"
#include "graphics.h"
#include "player.h"
#include "triggers.h"
#include "ui_manager.h"
#include "input_manager.h"
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

SDL_Color yellow = {255, 255, 0, 255};
SDL_Color white = {255, 255, 255, 255};

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

    SDL_Event event;
    while(game.isRunning){
        Uint32 currentTime = SDL_GetTicks();
        const float deltaTime = (currentTime - game.lastTime) / 1000.0f;
        game.lastTime = currentTime;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) game.isRunning = false;
            switch(game.scene){
                case MAIN_MENU:
                    handle_mouse_input_main_menu(&event, &game);
                    break;
                case VN_INTRO:
                case DEALER_ROOM:
                case PRE_BAT_VISION:
                    handle_mouse_input_visual_novel(&event, &game.visualNovelState, &game.visualNovelState.dialogue, &game.textureAssets);
                    break;
                case BAT_VISION:
                    if(game.visualNovelState.isShowingUI){
                        handle_mouse_input_visual_novel(&event, &game.visualNovelState, &game.visualNovelState.dialogue, &game.textureAssets);
                    }else{
                        handle_mouse_input(&event, &game.player.camera);
                    }
                    break;
                case MITA_SAVES_PLAYER:
                    if(game.visualNovelState.isShowingUI){
                        handle_mouse_input_visual_novel(&event, &game.visualNovelState, &game.visualNovelState.dialogue, &game.textureAssets);
                    }else{
                        handle_mouse_input(&event, &game.player.camera);
                    }
                    break;
                case PRE_LIDAR:
                    handle_mouse_invert_input(&event, &game.player.camera);
                    break;
                case LIDAR:
                    handle_mouse_input(&event, &game.player.camera);
                    break;
                case LAST_ROOM:
                    break;
                case DEAD_ROOM:
                    if(event.type == SDL_KEYDOWN){
                        if(event.key.keysym.sym == SDLK_r || event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN){
                            load_last_checkpoint(&game);
                        }
                    }
                    break;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        switch(game.scene){
            case MAIN_MENU:
                render_ui_texture(&game.mainMenuUI.title);

                render_clippy_bubble(&game.screen, &game.textureAssets, &game.mainMenuUI.warning);
                render_ui_texture(&game.mainMenuUI.warning);

                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if(is_mouse_over_ui(&game.mainMenuUI.startButton, mouseX, mouseY)){
                    update_text_ui_element(&game.mainMenuUI.startButton, game.textureAssets.mainFont, "[INDITÁS]", yellow);
                }else{
                    update_text_ui_element(&game.mainMenuUI.startButton, game.textureAssets.mainFont, "[INDITÁS]", white);
                }

                if(is_mouse_over_ui(&game.mainMenuUI.invertYButton, mouseX, mouseY)){
                    const char* invert_text = game.player.camera.isInvertedMouseY ? "Inverz Y-tengely: BEKAPCSOLVA" : "Inverz Y-tengely: KIKAPCSOLVA";
                    update_text_ui_element(&game.mainMenuUI.invertYButton, game.textureAssets.mainFont, invert_text, yellow);
                }else{
                    const char* invert_text = game.player.camera.isInvertedMouseY ? "Inverz Y-tengely: BEKAPCSOLVA" : "Inverz Y-tengely: KIKAPCSOLVA";
                    update_text_ui_element(&game.mainMenuUI.invertYButton, game.textureAssets.mainFont, invert_text, white);
                }

                render_ui_texture(&game.mainMenuUI.startButton);
                render_ui_texture(&game.mainMenuUI.invertYButton);
                render_ui_texture(&game.textureAssets.Clippy);
                break;
            case VN_INTRO:
                if(game.visualNovelState.currentDialogID == DLG_MONSTER_APPEARS){
                    setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
                    scene_switch(&game, DEALER_ROOM);
                }

                render_dialogue_box(SCREEN_WIDTH, SCREEN_HEIGHT, &game.visualNovelState.dialogue);
                update_dialogue(&game.visualNovelState.dialogue, currentTime);
                render_dialogue_name(&game.visualNovelState.dialogue, game.textureAssets.mainFont);
                render_dialogue_text(&game.visualNovelState.dialogue, game.textureAssets.mainFont);
                render_ui_texture(&game.visualNovelState.dialogue.speaker);
                break;
            case DEALER_ROOM:
                setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
                update_camera_view(&game.player.camera);
                update_moveable_model_position(&game.gameObjects.Dealer, deltaTime);
                render_moveable_model(&game.gameObjects.Dealer);
                render_dialogue_box(SCREEN_WIDTH, SCREEN_HEIGHT, &game.visualNovelState.dialogue);
                update_dialogue(&game.visualNovelState.dialogue, currentTime);
                render_dialogue_text(&game.visualNovelState.dialogue, game.textureAssets.mainFont);

                if(game.visualNovelState.currentDialogID == DLG_SCENE_SWITCH_BAT_VISION){
                    scene_switch(&game, PRE_BAT_VISION);
                }else if(game.visualNovelState.currentDialogID == DLG_GYULASZ_SIGN_THE_CONTRACT || game.visualNovelState.currentDialogID == DLG_GYULASZ_SIGN_THE_CONTRACT2){
                    render_dialogue_name(&game.visualNovelState.dialogue, game.textureAssets.mainFont);
                }
                break;
            case PRE_BAT_VISION:
                if(game.visualNovelState.currentDialogID == DLG_GYULASZ_SEE_NOTHING4){
                    scene_switch(&game, BAT_VISION);
                }

                update_camera_view(&game.player.camera);

                render_bat_vision(&game.gameObjects.BatVisionMap, currentTime, 'n');

                render_dialogue_box(SCREEN_WIDTH, SCREEN_HEIGHT, &game.visualNovelState.dialogue);
                update_dialogue(&game.visualNovelState.dialogue, currentTime);
                render_dialogue_name(&game.visualNovelState.dialogue, game.textureAssets.mainFont);
                render_dialogue_text(&game.visualNovelState.dialogue, game.textureAssets.mainFont);
                render_ui_texture(&game.visualNovelState.dialogue.speaker);
                break;
            case BAT_VISION:
                setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
                update_camera_view(&game.player.camera);
                render_model_without_texture(&game.gameObjects.BatVisionMap);
                if(game.caughtBySnailAt == 0) update_vaping(&game.player.camera, deltaTime);
                check_player_collision(&game.player.camera, &game.gameObjects.BatVisionMap, 0.5f);

                if(game.caughtBySnailAt == 0){
                    render_model_without_texture(&game.gameObjects.BatVisionMap);
                    render_bat_vision(&game.gameObjects.BatVisionMap, currentTime, 'n');

                    render_vape_in_hand(&game.gameObjects.Vapelt3, &game.player.camera);

                    update_and_render_smoke(deltaTime);

                    render_light_aura_model(&game.player.camera, &game.gameObjects.PunchPacificMonster);

                    if(game.player.camera.nextShout < currentTime){
                        render_ui_texture(&game.textureAssets.SpaceButton);
                    }


                    if(!game.player.camera.vape.isVaping){
                        render_ui_texture(&game.textureAssets.VButton);
                    }

                    check_trigger_zones(&game);

                    if(game.triggerZones.BatVisionHelsieTakeAHint.isActivated){
                        if(game.visualNovelState.isShowingUI){
                            change_camera_input_handler(&game, false, false);
                            render_dialogue_box(SCREEN_WIDTH, SCREEN_HEIGHT, &game.visualNovelState.dialogue);
                            update_dialogue(&game.visualNovelState.dialogue, currentTime);
                            render_dialogue_name(&game.visualNovelState.dialogue, game.textureAssets.mainFont);
                            render_dialogue_text(&game.visualNovelState.dialogue, game.textureAssets.mainFont);
                            render_ui_texture(&game.visualNovelState.dialogue.speaker);
                        }else{
                            change_camera_input_handler(&game, true, true);
                        }
                    }

                    if(game.gameObjects.ImmortalSnail.isMoving){
                        render_moveable_model(&game.gameObjects.ImmortalSnail);
                        update_snail_ai(&game, deltaTime);
                    }
                }else{
                    if((game.caughtBySnailAt+3000) > currentTime){
                        if(game.caughtBySnailAt+2000 > currentTime){
                            render_bat_vision(&game.gameObjects.BatVisionMap, currentTime, 's');
                            enable_snail_caught_lights(&game.gameObjects.ImmortalSnail, currentTime, game.caughtBySnailAt);
                        }else{
                            render_bat_vision(&game.gameObjects.BatVisionMap, currentTime, 'm');
                        }
                        update_snail_ai(&game, deltaTime);
                    }else{
                        scene_switch(&game, MITA_SAVES_PLAYER);
                    }
                }

                break;
            case MITA_SAVES_PLAYER:
                update_camera_view(&game.player.camera);

                render_moveable_model(&game.gameObjects.Mita);
                check_player_collision_mesh_miside_room(&game.player.camera, &game.gameObjects.MitasRoom, 0.1f);

                if(game.visualNovelState.currentDialogID < DLG_MITA_OPEN_PLAYERS_EYES2){
                    render_bat_vision(&game.gameObjects.MitasRoom, currentTime, 'n');
                }else{
                    glEnable(GL_FOG);
                    render_model(&game.gameObjects.MitasRoom);
                    enable_colored_fog(50.0f, 80.0f, 1.0f, 0.8f, 0.9f, 1.0f, 0.1f);

                    if(game.visualNovelState.currentDialogID == DLG_PLAYER_ACCEPT_MITAS_INVITATION){
                        change_loaded_moveable_obj_positon(&game.gameObjects.Mita, -9.03f, 0.85f, -2.75f, 0.0f, -90.0f, 0.0f);
                    }
                }

                if(game.visualNovelState.isShowingUI){
                    render_dialogue_box(SCREEN_WIDTH, SCREEN_HEIGHT, &game.visualNovelState.dialogue);
                    update_dialogue(&game.visualNovelState.dialogue, currentTime);
                    render_dialogue_name(&game.visualNovelState.dialogue, game.textureAssets.mainFont);
                    render_dialogue_text(&game.visualNovelState.dialogue, game.textureAssets.mainFont);
                    render_ui_texture(&game.visualNovelState.dialogue.speaker);
                }
                break;
            case PRE_LIDAR:
                setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
                update_camera_view(&game.player.camera);
                enable_pre_lidar_lights(&game.gameObjects.PreLidarMap, &game.player.camera);

                break;
            case LIDAR:
                setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
                update_camera_view(&game.player.camera);
                render_model_without_texture(&game.gameObjects.LidarMapWT);

                check_player_collision_mesh(&game.player.camera, &game.gameObjects.LidarMapWT, 0.1f);

                render_light_aura_model(&game.player.camera, &game.gameObjects.WhiteMonster);

                glEnable(GL_FOG);
                enableFog(1, 10, 1.0f);

                check_trigger_zones(&game);

                if(game.triggerZones.LidarChangesToFast.isActivated){
                    render_lidar_fast(game.gameObjects.LidarMap->number_of_vertex);
                    enableFog(1, 4, 1.0f);
                }else{
                    render_lidar(game.gameObjects.LidarMap, &game.player.camera);
                }

                break;
            case LAST_ROOM:

                break;
            case DEAD_ROOM:
                setup_projection(SCREEN_WIDTH, SCREEN_HEIGHT);
                update_camera_view(&game.player.camera);
                render_game_over_scene(&game.gameObjects.Dealer.model, currentTime, game.player.camera.auraLightBrightness);
                render_ui_texture(&game.textureAssets.SpaceButton);

                break;
            default:
                break;
        }

        handle_esc_input(&event, &game, &game.isRunning, &game.scene);
        handle_wasd_input(&game.player.camera, deltaTime, game.sounds, game.scene);
        render_ui_texture(&game.textureAssets.ESCButton);

        SDL_GL_SwapWindow(game.window);
    }

    SDL_GL_DeleteContext(game.glContext);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
    return 0;
}