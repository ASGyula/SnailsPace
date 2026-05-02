//
// Created by asgyu on 2026. ápr. 17..
//

#include "scene.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>

#include "ai.h"
#include "credits.h"
#include "dialogue_data.h"
#include "graphics_effects.h"
#include "graphics_lighting.h"
#include "graphics_particles.h"
#include "input_manager.h"
#include "triggers.h"
#include "ui_manager.h"

static void render_dialogue_ui(Game* game, Uint32 currentTime){
    render_dialogue_box(game->screen.screenWidth, game->screen.screenHeight, &game->visualNovelState.dialogue);
    update_dialogue(&game->visualNovelState.dialogue, currentTime);
    render_dialogue_name(&game->visualNovelState.dialogue, game->textureAssets.mainFont);
    render_dialogue_text(&game->visualNovelState.dialogue, game->textureAssets.mainFont);
    render_ui_texture(&game->visualNovelState.dialogue.speaker);
}

static void render_main_menu_scene(Game* game){
    render_ui_texture(&game->mainMenuUI.title);
    render_clippy_bubble(&game->screen, &game->textureAssets, &game->mainMenuUI.warning);
    render_ui_texture(&game->mainMenuUI.warning);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    const char* invert_text = game->player.camera.isInvertedMouseY
        ? "Inverz Y-tengely: BEKAPCSOLVA"
        : "Inverz Y-tengely: KIKAPCSOLVA";

    update_text_ui_element(
        &game->mainMenuUI.startButton,
        game->textureAssets.mainFont,
        "[INDITÁS]",
        is_mouse_over_ui(&game->mainMenuUI.startButton, mouseX, mouseY) ? (SDL_Color){255, 255, 0, 255} : (SDL_Color){255, 255, 255, 255}
    );

    update_text_ui_element(
        &game->mainMenuUI.invertYButton,
        game->textureAssets.mainFont,
        invert_text,
        is_mouse_over_ui(&game->mainMenuUI.invertYButton, mouseX, mouseY) ? (SDL_Color){255, 255, 0, 255} : (SDL_Color){255, 255, 255, 255}
    );

    render_ui_texture(&game->mainMenuUI.startButton);
    render_ui_texture(&game->mainMenuUI.invertYButton);
    render_ui_texture(&game->textureAssets.Clippy);
}

static void render_vn_intro_scene(Game* game, Uint32 currentTime){
    if(game->visualNovelState.currentDialogID == DLG_MONSTER_APPEARS){
        setup_projection(game->screen.screenWidth, game->screen.screenHeight);
        scene_switch(game, DEALER_ROOM);
    }
    render_dialogue_ui(game, currentTime);
}

static void render_dealer_room_scene(Game* game, Uint32 currentTime, float deltaTime){
    setup_projection(game->screen.screenWidth, game->screen.screenHeight);
    update_camera_view(&game->player.camera);
    update_moveable_model_position(&game->gameObjects.Dealer, deltaTime);
    render_moveable_model(&game->gameObjects.Dealer);
    render_dialogue_box(game->screen.screenWidth, game->screen.screenHeight, &game->visualNovelState.dialogue);
    update_dialogue(&game->visualNovelState.dialogue, currentTime);
    render_dialogue_text(&game->visualNovelState.dialogue, game->textureAssets.mainFont);

    if(game->visualNovelState.currentDialogID == DLG_SCENE_SWITCH_BAT_VISION){
        scene_switch(game, PRE_BAT_VISION);
    }else if(game->visualNovelState.currentDialogID == DLG_GYULASZ_SIGN_THE_CONTRACT || game->visualNovelState.currentDialogID == DLG_GYULASZ_SIGN_THE_CONTRACT2){
        render_dialogue_name(&game->visualNovelState.dialogue, game->textureAssets.mainFont);
    }
}

static void render_pre_bat_vision_scene(Game* game, Uint32 currentTime){
    if(game->visualNovelState.currentDialogID == DLG_GYULASZ_SEE_NOTHING4){
        scene_switch(game, BAT_VISION);
    }
    update_camera_view(&game->player.camera);
    render_bat_vision(&game->gameObjects.BatVisionMap, currentTime, 'n');
    render_dialogue_ui(game, currentTime);
}

static void render_bat_vision_scene(Game* game, Uint32 currentTime, float deltaTime){
    setup_projection(game->screen.screenWidth, game->screen.screenHeight);
    update_camera_view(&game->player.camera);
    render_model_without_texture(&game->gameObjects.BatVisionMap);

    if(game->caughtBySnailAt == 0){
        update_vaping(&game->player.camera, deltaTime);
    }
    check_player_collision(&game->player.camera, &game->gameObjects.BatVisionMap, 0.5f);

    if(game->caughtBySnailAt == 0){
        render_bat_vision(&game->gameObjects.BatVisionMap, currentTime, 'n');
        render_vape_in_hand(&game->gameObjects.Vapelt3, &game->player.camera);
        update_and_render_smoke(deltaTime);
        render_light_aura_model(&game->player.camera, &game->gameObjects.PunchPacificMonster);

        if(game->player.camera.nextShout < currentTime){
            render_ui_texture(&game->textureAssets.SpaceButton);
        }

        if(!game->player.camera.vape.isVaping){
            render_ui_texture(&game->textureAssets.VButton);
        }

        check_trigger_zones(game);

        if(game->triggerZones.BatVisionHelsieTakeAHint.isActivated){
            if(game->visualNovelState.isShowingUI){
                change_camera_input_handler(game, false, false);
                render_dialogue_ui(game, currentTime);
            }else{
                change_camera_input_handler(game, true, true);
            }
        }

        if(game->gameObjects.ImmortalSnail.isMoving){
            render_moveable_model(&game->gameObjects.ImmortalSnail);
            update_snail_ai(game, deltaTime);
        }
    }else{
        if((game->caughtBySnailAt + 3000) > currentTime){
            if(game->caughtBySnailAt + 2000 > currentTime){
                render_bat_vision(&game->gameObjects.BatVisionMap, currentTime, 's');
                enable_snail_caught_lights(&game->gameObjects.ImmortalSnail, currentTime, game->caughtBySnailAt);
            }else{
                render_bat_vision(&game->gameObjects.BatVisionMap, currentTime, 'm');
            }
            update_snail_ai(game, deltaTime);
        }else{
            scene_switch(game, MITA_SAVES_PLAYER);
        }
    }
}

static void render_mita_scene(Game* game, Uint32 currentTime, float deltaTime){
    update_camera_view(&game->player.camera);
    render_moveable_model(&game->gameObjects.Mita);
    check_player_collision_miside_room(&game->player.camera, &game->gameObjects.MitasRoom, 0.15f, 0.3f);

    check_miside_trigger_zones(game, &game->textureAssets, &game->gameObjects.Mita);

    if(game->visualNovelState.currentDialogID < DLG_MITA_OPEN_PLAYERS_EYES2){
        render_bat_vision(&game->gameObjects.MitasRoom, currentTime, 'm');
    }else{
        if(game->triggerZones.TryToEnterMitasRoom.isActivated){
            if(game->visualNovelState.currentDialogID < DLG_MITA_CLEARS_PLAYER_DIZZINESS){
                if(game->visualNovelState.currentDialogID == DLG_PLAYER_DIZZINESS){
                    change_loaded_moveable_obj_positon(&game->gameObjects.Mita, -3.69f, 0.9f, -5.47f, 0.0f, 90.0f, 0.0f);
                    rotate_camera_towards_mita(game, &game->gameObjects.Mita);
                }
                render_chromatic(&game->gameObjects.MitasRoom, 1.0f);
            }else{
                if(game->visualNovelState.currentDialogID == DLG_MITA_CLEARS_PLAYER_DIZZINESS2){
                    change_loaded_moveable_obj_positon(&game->gameObjects.Mita, -7.048f, 0.9f, -7.91f, 0.0f, -290.0f, 0.0f);
                }
                render_model(&game->gameObjects.MitasRoom);
                enable_colored_fog(50.0f, 80.0f, 1.0f, 0.8f, 0.9f, 1.0f, 0.1f);
            }
        }else{
            glEnable(GL_FOG);
            render_model(&game->gameObjects.MitasRoom);
            enable_colored_fog(50.0f, 80.0f, 1.0f, 0.8f, 0.9f, 1.0f, 0.1f);

            if(game->visualNovelState.currentDialogID == DLG_PLAYER_ACCEPT_MITAS_INVITATION){
                change_loaded_moveable_obj_positon(&game->gameObjects.Mita, -9.03f, 0.9f, -2.75f, 0.0f, -90.0f, 0.0f);
            }
            if(game->visualNovelState.currentDialogID == DLG_MITA_TAKE_THE_SCISSORS){
                change_loaded_moveable_obj_positon(&game->gameObjects.Mita, -7.048f, 0.9f, -7.91f, 0.0f, -290.0f, 0.0f);
            }
        }
    }

    if(game->visualNovelState.quest_state == PLAYING_TIC_TAC_TOE){
        render_tic_tac_toe(&game->visualNovelState.ticTacToe, game->textureAssets.Gyulasz_Happy.textureID, game->textureAssets.Mita_Blush.textureID, game->screen.screenWidth, game->screen.screenHeight);
        render_ui_texture(&game->textureAssets.TicTacToeControll);

        if(game->visualNovelState.ticTacToe.isGameOver){
            if(game->visualNovelState.ticTacToe.winner == 1){
                game->visualNovelState.dialogue = create_dialogue_from_id(DLG_MITA_CONGRATZ_PLAYER_TIC_TAC_TOE, game->visualNovelState.playerName, &game->textureAssets.Mita_Blush);
                increase_visual_novel_state(&game->visualNovelState, DLG_MITA_CONGRATZ_PLAYER_TIC_TAC_TOE);
            }else if(game->visualNovelState.ticTacToe.winner == 2){
                game->visualNovelState.dialogue = create_dialogue_from_id(DLG_PLAYER_CONGRATZ_MITA_TIC_TAC_TOE, game->visualNovelState.playerName, &game->textureAssets.Gyulasz_Happy_Pro_Max);
                increase_visual_novel_state(&game->visualNovelState, DLG_PLAYER_CONGRATZ_MITA_TIC_TAC_TOE);
            }else if(game->visualNovelState.ticTacToe.winner == 3){
                game->visualNovelState.dialogue = create_dialogue_from_id(DLG_TIC_TAC_TOE_DRAW, game->visualNovelState.playerName, &game->textureAssets.Mita_Happy);
                increase_visual_novel_state(&game->visualNovelState, DLG_TIC_TAC_TOE_DRAW);
            }

            static Uint32 closeTimer = 0;
            if(closeTimer == 0) closeTimer = currentTime + 1000;
            if(currentTime > closeTimer){
                if(game->visualNovelState.ticTacToe.round >= 2 && !game->isMitaEnding){
                    game->visualNovelState.quest_state = AFTER_TIC_TAC_TOE;
                    change_camera_input_handler(game, true, true);
                    game->visualNovelState.dialogue = create_dialogue_from_id(DLG_PLAYER_INVESTIGATE_BONKING, game->visualNovelState.playerName, &game->textureAssets.Gyulasz_Thinking);
                    increase_visual_novel_state(&game->visualNovelState, DLG_PLAYER_INVESTIGATE_BONKING);
                }
                game->visualNovelState.ticTacToe.active = false;
                clear_tic_tac_toe(&game->visualNovelState.ticTacToe);
                closeTimer = 0;
                game->visualNovelState.isShowingUI = true;
            }
        }else{
            update_mita_ai(&game->visualNovelState.ticTacToe, SDL_GetTicks());
        }
    }

    if(game->visualNovelState.quest_state == DECISION_TIME){
        render_decision_ui(game->screen, &game->visualNovelState.decisionUI, &game->textureAssets);
    }

    if(game->visualNovelState.quest_state == CREDITS_STATE){
        static Uint32 startTime = 0;
        if(startTime == 0){
            startTime = SDL_GetTicks();
        }else{
            Uint32 elapsed = SDL_GetTicks() - startTime;
            float alpha = 0.0f;
            if(elapsed > 1000){
                alpha = (float)(elapsed - 1000) / 4000.0f;
            }
            if(alpha > 1) alpha=1.0f;
            render_black_overlay(game->screen.screenWidth, game->screen.screenHeight, alpha);

            if(elapsed > 5000){
                scene_switch(game, CREDITS);
            }
        }
    }



    if(game->visualNovelState.isShowingUI){
        render_dialogue_box(game->screen.screenWidth, game->screen.screenHeight, &game->visualNovelState.dialogue);
        update_dialogue(&game->visualNovelState.dialogue, currentTime);
        render_dialogue_name(&game->visualNovelState.dialogue, game->textureAssets.mainFont);
        render_dialogue_text(&game->visualNovelState.dialogue, game->textureAssets.mainFont);
        render_ui_texture(&game->visualNovelState.dialogue.speaker);
    }

    if(game->visualNovelState.currentDialogID == DLG_PLAYER_LEAVING_MITA_ANGRY5){
        scene_switch(game, PRE_LIDAR);
    }

    if(game->visualNovelState.quest_state == SEARCHING_FOR_SCISSORS){
        render_light_aura_model(&game->player.camera, &game->gameObjects.Scissors);

        if(game->gameObjects.Scissors.triggerZone.isActivated){
            render_ui_texture(&game->textureAssets.EButton);
        }
    }
}

static void render_pre_lidar_scene(Game* game, Uint32 currentTime){
    setup_projection(game->screen.screenWidth, game->screen.screenHeight);
    update_camera_view(&game->player.camera);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_white_mita(&game->gameObjects.Mita);
    update_dialogue(&game->visualNovelState.dialogue, currentTime);
    render_meta_dialogue(&game->visualNovelState.dialogue, game->textureAssets.mainFont);

    static Uint32 nextDialogueTimer = 0;

    if(game->visualNovelState.dialogue.isFinished){
        if(nextDialogueTimer == 0){
            nextDialogueTimer = currentTime + 1300;
        }

        if(currentTime > nextDialogueTimer){
            nextDialogueTimer = 0;

            if(game->visualNovelState.currentDialogID < DLG_MITA_VOID_MONOLOGUE14){
                game->visualNovelState.currentDialogID++;
                game->visualNovelState.dialogue = create_dialogue_from_id(game->visualNovelState.currentDialogID,game->visualNovelState.playerName,&game->textureAssets.Mita_Angry);

                Coordinates coordinates = {
                    game->visualNovelState.dialogue.bgColor.red,
                    game->visualNovelState.dialogue.bgColor.green,
                    game->visualNovelState.dialogue.bgColor.blue
                };

                set_camera_position(&game->player.camera,coordinates,game->visualNovelState.dialogue.nameStartX, game->visualNovelState.dialogue.nameStartY);

                change_camera_input_handler(game, false, false);
            }else{
                if(game->isLeaveEnding)game->isRunning = false;
                scene_switch(game, LIDAR);
            }
        }
    }else{
        nextDialogueTimer = 0;
    }
}

static void render_lidar_scene(Game* game){
    setup_projection(game->screen.screenWidth, game->screen.screenHeight);
    update_camera_view(&game->player.camera);
    render_model_without_texture(&game->gameObjects.LidarMapWT);
    check_player_collision_mesh(&game->player.camera, &game->gameObjects.LidarMapWT, 0.1f);
    render_light_aura_model(&game->player.camera, &game->gameObjects.WhiteMonster);
    glEnable(GL_FOG);
    enableFog(1, 10, 1.0f);
    check_trigger_zones(game);
    game->visualNovelState.isShowingUI = false;

    if(!game->triggerZones.LidarChangesToFast.isActivated){
        render_lidar_fast(game->gameObjects.LidarMap->number_of_vertex);
        enableFog(1, 4, 1.0f);
    }else{
        render_lidar(game->gameObjects.LidarMap, &game->player.camera);
    }
}

static void render_last_room_scene(Game* game, Uint32 currentTime){
    static Uint32 startTime = 0;
    if(startTime == 0){
        startTime = SDL_GetTicks();
    }
    Uint32 elapsed = SDL_GetTicks() - startTime;

    setup_projection(game->screen.screenWidth, game->screen.screenHeight);
    update_camera_view(&game->player.camera);
    enable_pre_lidar_lights(&game->gameObjects.PreLidarMap, &game->player.camera);

    float alpha = 0.0f;
    if(elapsed > 3000){
        alpha = (float)(elapsed - 3000) / 11000.0f;
        if(alpha > 1.0f) alpha = 1.0f;
    }

    render_black_overlay(game->screen.screenWidth, game->screen.screenHeight, alpha);
    game->visualNovelState.dialogue.startTime = startTime+4000;

    if(elapsed > 4000 && elapsed < 11500){
        update_dialogue(&game->visualNovelState.dialogue, currentTime);
        render_meta_dialogue(&game->visualNovelState.dialogue, game->textureAssets.mainFont);
    }

    if(elapsed > 12000){
        startTime = 0;
        scene_switch(game, CREDITS);
    }
}

static void render_dead_room_scene(Game* game, Uint32 currentTime){
    setup_projection(game->screen.screenWidth, game->screen.screenHeight);
    update_camera_view(&game->player.camera);
    render_game_over_scene(&game->gameObjects.Dealer.model, currentTime, game->player.camera.auraLightBrightness);
    render_ui_texture(&game->textureAssets.SpaceButton);
}

static void render_credits_scene(Game* game, Uint32 currentTime) {
    static Uint32 startTime = 0;
    if (startTime == 0) startTime = currentTime;
    Uint32 elapsed = currentTime - startTime;

    int page = (elapsed / 3000) % CREDIT_PAGE_COUNT;

    if((elapsed / 3000) >= CREDIT_PAGE_COUNT){
        if(game->isMitaEnding) scene_switch(game, MITA_SAVES_PLAYER);
        else game->isRunning = false;
        return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_text_centered(game->textureAssets.mainFont, credits[page].title, 300);
    render_text_centered(game->textureAssets.mainFont, credits[page].content, 400);
}

void handle_scene_events(Game* game, SDL_Event* event){
    while(SDL_PollEvent(event)){
        if(event->type == SDL_QUIT) game->isRunning = false;

        switch(game->scene){
            case MAIN_MENU:
                handle_mouse_input_main_menu(event, game);
                break;
            case VN_INTRO:
            case DEALER_ROOM:
            case PRE_BAT_VISION:
                handle_mouse_input_visual_novel(event, &game->visualNovelState, &game->visualNovelState.dialogue, &game->textureAssets);
                break;
            case BAT_VISION:
                if(game->visualNovelState.isShowingUI){
                    handle_mouse_input_visual_novel(event, &game->visualNovelState, &game->visualNovelState.dialogue, &game->textureAssets);
                }else{
                    handle_mouse_input(event, &game->player.camera);
                }
                break;
            case MITA_SAVES_PLAYER:
                if(game->visualNovelState.isShowingUI){
                    handle_mouse_input_visual_novel(event, &game->visualNovelState, &game->visualNovelState.dialogue, &game->textureAssets);
                }else{
                    handle_miside_interact_input(event, &game->visualNovelState, &game->gameObjects.Scissors.triggerZone, &game->textureAssets);
                    handle_mouse_input(event, &game->player.camera);
                }
                break;
            case PRE_LIDAR:
            case LIDAR:
                handle_mouse_input(event, &game->player.camera);
                break;
            case LAST_ROOM:
                handle_mouse_invert_input(event, &game->player.camera);
                break;
            case DEAD_ROOM:
                if(event->type == SDL_KEYDOWN){
                    if(event->key.keysym.sym == SDLK_r || event->key.keysym.sym == SDLK_SPACE || event->key.keysym.sym == SDLK_RETURN){
                        load_last_checkpoint(game);
                    }
                }
                break;
        }
    }
}

void render_scene(Game* game, Uint32 currentTime, float deltaTime){
    switch(game->scene){
        case MAIN_MENU:
            render_main_menu_scene(game);
            break;
        case VN_INTRO:
            render_vn_intro_scene(game, currentTime);
            break;
        case DEALER_ROOM:
            render_dealer_room_scene(game, currentTime, deltaTime);
            break;
        case PRE_BAT_VISION:
            render_pre_bat_vision_scene(game, currentTime);
            break;
        case BAT_VISION:
            render_bat_vision_scene(game, currentTime, deltaTime);
            break;
        case MITA_SAVES_PLAYER:
            render_mita_scene(game, currentTime, deltaTime);
            break;
        case PRE_LIDAR:
            render_pre_lidar_scene(game, currentTime);
            break;
        case LIDAR:
            render_lidar_scene(game);
            break;
        case LAST_ROOM:
            render_last_room_scene(game, currentTime);
            break;
        case DEAD_ROOM:
            render_dead_room_scene(game, currentTime);
            break;
        case CREDITS:
            render_credits_scene(game, currentTime);
            break;
        default:
            break;
    }
}