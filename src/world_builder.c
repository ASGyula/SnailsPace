//
// Created by asgyu on 2026. márc. 19..
//

#include "world_builder.h"

#include <stdio.h>

#include "dialogue_data.h"
#include "graphics.h"

#define IMMORTAL_SNAIL_SPEED 10.0f
#define MITA_SPEED 30.0f

void set_player_defaults(Game* game){
    printf("[INICIALIZALAS] set_player_defaults\n");
    game->player.camera.auraLightBrightness = 1.0f;
    game->player.camera.screenWidth = game->screen.screenWidth;
    game->player.camera.screenHeight = game->screen.screenHeight;
    game->player.camera.lastShout = 0;
    game->player.camera.nextShout = 0;
    game->caughtBySnailAt = 0;
    game->player.camera.isInvertedMouseY = true;
    initialize_camera(&game->player.camera);
}

void set_visual_novel_state(Game* game, const char* player_name){
    printf("[INICIALIZALAS] set_visual_novel_state\n");
    game->visualNovelState.currentDialogID = 0;
    game->visualNovelState.playerName = (char*)player_name;
    Dialogue dialogue = create_dialogue_from_id(DLG_INTRO, player_name, &game->textureAssets.Gyulasz_Scared);
    game->visualNovelState.dialogue = dialogue;
    game->visualNovelState.isShowingUI = false;
}

void set_fonts_standards(Game* game){
    printf("[INICIALIZALAS] set_fonts_standards\n");
    game->textureAssets.mainFont = TTF_OpenFont("../assets/Fonts/Roboto.ttf", 24);
    if(game->textureAssets.mainFont == NULL){
        printf("[HIBA] Nem sikerult a Roboto fontot betolteni\n");
    }
}

void sounds_tuning_up(Game* game){
    printf("[INICIALIZALAS] sounds_tuning_up\n");
    game->sounds.Mita_Bonk = Mix_LoadWAV("../assets/External/AIHASTO/Mita_Bonk.wav");
    game->sounds.Overture = Mix_LoadWAV("../assets/External/Gemini/Overture.wav");
    game->sounds.ShoutSound = Mix_LoadWAV("../assets/External/YouTube/Shout.wav");
    game->sounds.VapeFincsiVape = Mix_LoadWAV("../assets/asgyu/VapeNyamiFincsaVape.wav");
}

void build_up_trigger_zones(Game* game){
    printf("[INICIALIZALAS] building_up_trigger_zones\n");

    //BAT VISION HELSIE TAKE A HINT
    game->triggerZones.BatVisionHelsieTakeAHint.x = 0;
    game->triggerZones.BatVisionHelsieTakeAHint.z = -10;
    game->triggerZones.BatVisionHelsieTakeAHint.radius = 2;
    game->triggerZones.BatVisionHelsieTakeAHint.type = TRIGGER_DIALOGUE;
    game->triggerZones.BatVisionHelsieTakeAHint.isActivated = false;

    //MITA OLLÓT KÉR
    game->triggerZones.MitaAsksForAScissors.x = -7.35f;
    game->triggerZones.MitaAsksForAScissors.z = -2.37f;
    game->triggerZones.MitaAsksForAScissors.radius = 1;
    game->triggerZones.MitaAsksForAScissors.type = TRIGGER_DIALOGUE;
    game->triggerZones.MitaAsksForAScissors.isActivated = false;

    //MITA BONK VÁLASZTÁS
    game->triggerZones.MitaBonkChoice.x = -1.859f;
    game->triggerZones.MitaBonkChoice.z = -2.73f;
    game->triggerZones.MitaBonkChoice.radius = 3;
    game->triggerZones.MitaBonkChoice.type = TRIGGER_DIALOGUE;
    game->triggerZones.MitaBonkChoice.isActivated = false;

    //BELÉPÉS MITA SZOBÁJÁBA BONK ALATT
    game->triggerZones.TryToEnterMitasRoom.x = -2.63f;
    game->triggerZones.TryToEnterMitasRoom.z = -5.45f;
    game->triggerZones.TryToEnterMitasRoom.radius = 0.5f;
    game->triggerZones.TryToEnterMitasRoom.type = TRIGGER_DIALOGUE;
    game->triggerZones.TryToEnterMitasRoom.isActivated = false;

    //MITA MEGKAPJA AZ OLLÓT
    game->triggerZones.MitaTakeScissors.x = -8.49f;
    game->triggerZones.MitaTakeScissors.z = -2.56f;
    game->triggerZones.MitaTakeScissors.radius = 0.5f;
    game->triggerZones.MitaTakeScissors.type = TRIGGER_DIALOGUE;
    game->triggerZones.MitaTakeScissors.isActivated = false;

    //LIDAR > LIDAR FAST
    game->triggerZones.LidarChangesToFast.x = 23;
    game->triggerZones.LidarChangesToFast.z = -28;
    game->triggerZones.LidarChangesToFast.radius = 2;
    game->triggerZones.LidarChangesToFast.type = TRIGGER_DIALOGUE;
    game->triggerZones.LidarChangesToFast.isActivated = false;

}


void build_up_game_objects(Game* game){
    printf("[INICIALIZALAS] building_up_trigger_zones\n");
    
    //HELSIE
    load_textured_obj("External/RedEyes/HelsieMidnightbyRedEyes.obj", &game->gameObjects.Helsie);
    game->gameObjects.Helsie.textureID = load_texture("External/RedEyes/T_MysticFang_Body_D.png");

    //MITA
    load_textured_obj("External/vanessacampbell701/mitasidegirl/model.obj", &game->gameObjects.Mita.model);
    game->gameObjects.Mita.model.textureID = load_texture("External/vanessacampbell701/mitasidegirl/Baked_Atlas.png");
    game->gameObjects.Mita.x = 0.85f;
    game->gameObjects.Mita.y = 0.85f;
    game->gameObjects.Mita.z = -2.0f;
    game->gameObjects.Mita.targetX = 0.85f;
    game->gameObjects.Mita.targetY = 1.0f;
    game->gameObjects.Mita.targetZ = -2.0f;
    game->gameObjects.Mita.pitch = 0.0f;
    game->gameObjects.Mita.yaw = -270.0f;
    game->gameObjects.Mita.animSpeed = MITA_SPEED;
    game->gameObjects.Mita.isMoving = false;

    //MISIDE HÁZ
    load_textured_obj("External/FarawellGames/Miside_room/model.obj", &game->gameObjects.MitasRoom);
    game->gameObjects.MitasRoom.textureID = load_texture("External/FarawellGames/Miside_room/Baked_Atlas.png");

    //LIDAR MAP
    load_obj("External/heyisah78/miside-horror-stage/model.obj", &game->gameObjects.LidarMap);

    load_textured_obj("External/heyisah78/miside-horror-stage/Lidar.obj", &game->gameObjects.LidarMapWT);
    game->gameObjects.LidarMapWT.textureID = load_texture("External/RedEyes/HelsieMidnight/T_BlackMask.png");


    //DEALER
    load_textured_obj("External/VibaPop/TheDealer.obj", &game->gameObjects.Dealer.model);
    game->gameObjects.Dealer.model.textureID = load_texture("External/Vibapop/TheDealerTextureB.png");
    game->gameObjects.Dealer.x = 0;
    game->gameObjects.Dealer.y = 0.0f;
    game->gameObjects.Dealer.z = -2.0f;
    game->gameObjects.Dealer.targetX = 0;
    game->gameObjects.Dealer.targetY = 0.0f;
    game->gameObjects.Dealer.targetZ = 0.0f;
    game->gameObjects.Dealer.animSpeed = IMMORTAL_SNAIL_SPEED;
    game->gameObjects.Dealer.isMoving = true;
    
    //BAT VISION MAP
    load_textured_obj("External/bat_vision_map/map.obj", &game->gameObjects.BatVisionMap);
    game->gameObjects.BatVisionMap.textureID = load_texture("External/RedEyes/HelsieMidnight/T_BlackMask.png");

    //PRE-LIDAR MAP
    load_textured_obj("External/volvor/The_Japanese_School_Classroom/model.obj", &game->gameObjects.PreLidarMap.model);
    game->gameObjects.PreLidarMap.model.textureID = load_texture("External/RedEyes/HelsieMidnight/T_MysticFang_Body_D.png");
    game->gameObjects.PreLidarMap.r = 1.00f;
    game->gameObjects.PreLidarMap.g = 1.00f;
    game->gameObjects.PreLidarMap.b = 1.00f;
    game->gameObjects.PreLidarMap.a = 1.0f;
    game->gameObjects.PreLidarMap.radius = 1.0f;
    game->gameObjects.PreLidarMap.triggerZone.radius = 1.0f;
    game->gameObjects.PreLidarMap.triggerZone.isActivated = false;
    game->gameObjects.PreLidarMap.triggerZone.type = TRIGGER_DRINK;

    //VAPE
    load_textured_obj("External/Figusorasu/lostvape-centaurus-mod-low-poly/model.obj", &game->gameObjects.Vapelt3.model);
    game->gameObjects.Vapelt3.model.textureID = load_texture("External/Figusorasu/lostvape-centaurus-mod-low-poly/texture.png");
    
    //MONSTER ENERGIAITAL SZERETEM SZERETEM HASAMBA BETESZEM
    load_textured_obj("External/Kleyton3D/lata-de-monster/model.obj", &game->gameObjects.PunchPacificMonster.model);
    game->gameObjects.PunchPacificMonster.model.textureID = load_texture("External/Kleyton3D/lata-de-monster/pipeline_punch.png");
    game->gameObjects.PunchPacificMonster.r = 0.89f;
    game->gameObjects.PunchPacificMonster.g = 0.22f;
    game->gameObjects.PunchPacificMonster.b = 0.31f;
    game->gameObjects.PunchPacificMonster.a = 1.0f;
    game->gameObjects.PunchPacificMonster.radius = 5.0f;
    game->gameObjects.PunchPacificMonster.triggerZone.radius = 1.0f;
    game->gameObjects.PunchPacificMonster.triggerZone.isActivated = false;
    game->gameObjects.PunchPacificMonster.triggerZone.type = TRIGGER_DRINK;

    load_textured_obj("External/Kleyton3D/lata-de-monster/model.obj", &game->gameObjects.WhiteMonster.model);
    game->gameObjects.WhiteMonster.model.textureID = load_texture("External/Kleyton3D/lata-de-monster/white.png");
    game->gameObjects.WhiteMonster.r = 1.0f;
    game->gameObjects.WhiteMonster.g = 1.0f;
    game->gameObjects.WhiteMonster.b = 1.0f;
    game->gameObjects.WhiteMonster.a = 1.0f;
    game->gameObjects.WhiteMonster.radius = 5.0f;
    game->gameObjects.WhiteMonster.triggerZone.radius = 1.0f;
    game->gameObjects.WhiteMonster.triggerZone.isActivated = false;
    game->gameObjects.WhiteMonster.triggerZone.type = TRIGGER_DRINK;
    game->gameObjects.WhiteMonster.x = 22.91f;
    game->gameObjects.WhiteMonster.y = 0.0f;
    game->gameObjects.WhiteMonster.z = -50.0f;

    game->gameObjects.WhiteMonster.radius = 1.0f;
    game->gameObjects.WhiteMonster.triggerZone.isActivated = false;
    game->gameObjects.WhiteMonster.triggerZone.x = game->gameObjects.WhiteMonster.x;
    game->gameObjects.WhiteMonster.triggerZone.z = game->gameObjects.WhiteMonster.z;

    //OLLÓ
    load_textured_obj("External/tuturu/day-3-scissors/model.obj", &game->gameObjects.Scissors.model);
    game->gameObjects.Scissors.model.textureID = load_texture("External/tuturu/day-3-scissors/3_scissors_bc.png");
    game->gameObjects.Scissors.r = 0.56f;
    game->gameObjects.Scissors.g = 0.56f;
    game->gameObjects.Scissors.b = 0.61f;
    game->gameObjects.Scissors.a = 0.0f;
    game->gameObjects.Scissors.radius = 1.0f;
    game->gameObjects.Scissors.triggerZone.x = 4.750f;
    game->gameObjects.Scissors.triggerZone.z = -4.1f;
    game->gameObjects.Scissors.triggerZone.radius = 0.5f;
    game->gameObjects.Scissors.triggerZone.isActivated = false;
    game->gameObjects.Scissors.triggerZone.type = TRIGGER_DRINK;
    game->gameObjects.Scissors.x = 4.750f;
    game->gameObjects.Scissors.y = 1.7f;
    game->gameObjects.Scissors.z = -4.1f;

    //IMMORTAL SNAIL
    load_textured_obj("External/rudolfs/snail/model.obj", &game->gameObjects.ImmortalSnail.model);
    game->gameObjects.ImmortalSnail.model.textureID = load_texture("External/rudolfs/snail/snail_dif.png");
    game->gameObjects.ImmortalSnail.triggerZone.x = 0;
    game->gameObjects.ImmortalSnail.triggerZone.z = 0;
    game->gameObjects.ImmortalSnail.triggerZone.isActivated = false;
    game->gameObjects.ImmortalSnail.triggerZone.type = TRIGGER_DIALOGUE;
    game->gameObjects.ImmortalSnail.x = 0;
    game->gameObjects.ImmortalSnail.y = 0.0f;
    game->gameObjects.ImmortalSnail.z = 20.0f;
    game->gameObjects.ImmortalSnail.targetX = 0;
    game->gameObjects.ImmortalSnail.targetY = 0.0f;
    game->gameObjects.ImmortalSnail.targetZ = 0.0f;
    game->gameObjects.ImmortalSnail.animSpeed = 2.0f;
    game->gameObjects.ImmortalSnail.isMoving = false;
}

void paint_texture_assets(Game *game){
    printf("[INICIALIZALAS] paint_texture_assets\n");

    game->textureAssets.Helsie_Happy.textureID = load_texture("External/Gemini/Helsie_Happy.png");
    game->textureAssets.Helsie_Happy.w = 400;
    game->textureAssets.Helsie_Happy.h = 400;
    game->textureAssets.Helsie_Happy.x = 0;
    game->textureAssets.Helsie_Happy.y = game->screen.screenHeight - game->textureAssets.Helsie_Scared.h;
    game->textureAssets.Helsie_Happy.isShowing = true;

    game->textureAssets.Helsie_Scared.textureID = load_texture("External/Gemini/Helsie_Scared.png");
    game->textureAssets.Helsie_Scared.w = 400;
    game->textureAssets.Helsie_Scared.h = 400;
    game->textureAssets.Helsie_Scared.x = 0;
    game->textureAssets.Helsie_Scared.y = game->screen.screenHeight - game->textureAssets.Helsie_Scared.h;
    game->textureAssets.Helsie_Scared.isShowing = true;

    game->textureAssets.Gyulasz_Brave.textureID = load_texture("External/Gemini/Gyulasz_Brave.png");
    game->textureAssets.Gyulasz_Brave.w = 400;
    game->textureAssets.Gyulasz_Brave.h = 450;
    game->textureAssets.Gyulasz_Brave.x = -10;
    game->textureAssets.Gyulasz_Brave.y = game->screen.screenHeight - game->textureAssets.Gyulasz_Brave.h;
    game->textureAssets.Gyulasz_Brave.isShowing = true;

    game->textureAssets.Gyulasz_Determined.textureID = load_texture("External/Gemini/Gyulasz_Determined.png");
    game->textureAssets.Gyulasz_Determined.w = 400;
    game->textureAssets.Gyulasz_Determined.h = 450;
    game->textureAssets.Gyulasz_Determined.x = -10;
    game->textureAssets.Gyulasz_Determined.y = game->screen.screenHeight - game->textureAssets.Gyulasz_Determined.h;
    game->textureAssets.Gyulasz_Determined.isShowing = true;

    game->textureAssets.Helsie_lt3_Gyula.textureID = load_texture("External/Gemini/Helsie_lt3_Gyula.png");
    game->textureAssets.Helsie_lt3_Gyula.w = 400;
    game->textureAssets.Helsie_lt3_Gyula.h = 450;
    game->textureAssets.Helsie_lt3_Gyula.x = -10;
    game->textureAssets.Helsie_lt3_Gyula.y = game->screen.screenHeight - game->textureAssets.Helsie_lt3_Gyula.h;
    game->textureAssets.Helsie_lt3_Gyula.isShowing = true;

    game->textureAssets.Mita_Relieved.textureID = load_texture("External/Gemini/Mita_Relieved.png");
    game->textureAssets.Mita_Relieved.w = 400;
    game->textureAssets.Mita_Relieved.h = 400;
    game->textureAssets.Mita_Relieved.x = 0;
    game->textureAssets.Mita_Relieved.y = game->screen.screenHeight - game->textureAssets.Mita_Relieved.h;
    game->textureAssets.Mita_Relieved.isShowing = true;

    game->textureAssets.Mita_Happy.textureID = load_texture("External/Gemini/Mita_Happy.png");
    game->textureAssets.Mita_Happy.w = 400;
    game->textureAssets.Mita_Happy.h = 400;
    game->textureAssets.Mita_Happy.x = 0;
    game->textureAssets.Mita_Happy.y = game->screen.screenHeight - game->textureAssets.Mita_Happy.h;
    game->textureAssets.Mita_Happy.isShowing = true;

    game->textureAssets.Mita_Blush.textureID = load_texture("External/Gemini/Mita_Blush.png");
    game->textureAssets.Mita_Blush.w = 400;
    game->textureAssets.Mita_Blush.h = 400;
    game->textureAssets.Mita_Blush.x = 0;
    game->textureAssets.Mita_Blush.y = game->screen.screenHeight - game->textureAssets.Mita_Blush.h;
    game->textureAssets.Mita_Blush.isShowing = true;

    game->textureAssets.Mita_Smack.textureID = load_texture("External/Gemini/Mita_Smack.png");
    game->textureAssets.Mita_Smack.w = 400;
    game->textureAssets.Mita_Smack.h = 400;
    game->textureAssets.Mita_Smack.x = 0;
    game->textureAssets.Mita_Smack.y = game->screen.screenHeight - game->textureAssets.Mita_Smack.h;
    game->textureAssets.Mita_Smack.isShowing = true;

        game->textureAssets.Gyulasz_Thinking.textureID = load_texture("External/Gemini/Gyulasz_Thinking.png");
    game->textureAssets.Gyulasz_Thinking.w = 400;
    game->textureAssets.Gyulasz_Thinking.h = 450;
    game->textureAssets.Gyulasz_Thinking.x = 0;
    game->textureAssets.Gyulasz_Thinking.y = game->screen.screenHeight - game->textureAssets.Gyulasz_Thinking.h;
    game->textureAssets.Gyulasz_Thinking.isShowing = true;

    game->textureAssets.Gyulasz_Have_an_evil_idea.textureID = load_texture("External/Gemini/Gyulasz_Have_an_evil_idea.png");
    game->textureAssets.Gyulasz_Have_an_evil_idea.w = 400;
    game->textureAssets.Gyulasz_Have_an_evil_idea.h = 450;
    game->textureAssets.Gyulasz_Have_an_evil_idea.x = 0;
    game->textureAssets.Gyulasz_Have_an_evil_idea.y = game->screen.screenHeight - game->textureAssets.Gyulasz_Have_an_evil_idea.h;
    game->textureAssets.Gyulasz_Have_an_evil_idea.isShowing = true;

    game->textureAssets.Gyulasz_Suprised.textureID = load_texture("External/Gemini/Gyulasz_Suprised.png");
    game->textureAssets.Gyulasz_Suprised.w = 400;
    game->textureAssets.Gyulasz_Suprised.h = 450;
    game->textureAssets.Gyulasz_Suprised.x = 0;
    game->textureAssets.Gyulasz_Suprised.y = game->screen.screenHeight - game->textureAssets.Gyulasz_Suprised.h;
    game->textureAssets.Gyulasz_Suprised.isShowing = true;

    game->textureAssets.Gyulasz_Scared.textureID = load_texture("External/Gemini/Gyulasz_Scared.png");
    game->textureAssets.Gyulasz_Scared.w = 400;
    game->textureAssets.Gyulasz_Scared.h = 450;
    game->textureAssets.Gyulasz_Scared.x = -10;
    game->textureAssets.Gyulasz_Scared.y = game->screen.screenHeight - game->textureAssets.Gyulasz_Scared.h;
    game->textureAssets.Gyulasz_Scared.isShowing = true;

    game->textureAssets.Gyulasz_Hand_Shake.textureID = load_texture("External/Gemini/Gyulasz_Hand_Shake.png");
    game->textureAssets.Gyulasz_Hand_Shake.w = 400;
    game->textureAssets.Gyulasz_Hand_Shake.h = 450;
    game->textureAssets.Gyulasz_Hand_Shake.x = 0;
    game->textureAssets.Gyulasz_Hand_Shake.y = game->screen.screenHeight - game->textureAssets.Gyulasz_Hand_Shake.h;
    game->textureAssets.Gyulasz_Hand_Shake.isShowing = true;

    game->textureAssets.Gyulasz_Happy.textureID = load_texture("External/Gemini/Gyulasz_Happy.png");
    game->textureAssets.Gyulasz_Happy.w = 400;
    game->textureAssets.Gyulasz_Happy.h = 400;
    game->textureAssets.Gyulasz_Happy.x = 0;
    game->textureAssets.Gyulasz_Happy.y = game->screen.screenHeight - game->textureAssets.Gyulasz_Happy.h;
    game->textureAssets.Gyulasz_Happy.isShowing = true;

    game->textureAssets.Gyulasz_Happy_Pro_Max.textureID = load_texture("External/Gemini/Gyulasz_Happy_Pro_Max.png");
    game->textureAssets.Gyulasz_Happy_Pro_Max.w = 400;
    game->textureAssets.Gyulasz_Happy_Pro_Max.h = 400;
    game->textureAssets.Gyulasz_Happy_Pro_Max.x = 0;
    game->textureAssets.Gyulasz_Happy_Pro_Max.y = game->screen.screenHeight - game->textureAssets.Gyulasz_Happy.h;
    game->textureAssets.Gyulasz_Happy_Pro_Max.isShowing = true;

    game->textureAssets.Clippy.textureID = load_texture("External/JokeBattlesWikia/Clippy.png");
    game->textureAssets.Clippy.w = 200;
    game->textureAssets.Clippy.h = 200;
    game->textureAssets.Clippy.x = 5;
    game->textureAssets.Clippy.y = (game->screen.screenHeight) - (game->textureAssets.Clippy.h);
    game->textureAssets.Clippy.isShowing = true;

    game->textureAssets.Dealer.textureID = load_texture("External/Gemini/Dealer.png");
    game->textureAssets.Dealer.w = 400;
    game->textureAssets.Dealer.h = 450;
    game->textureAssets.Dealer.x = game->screen.screenWidth - game->textureAssets.Dealer.w;
    game->textureAssets.Dealer.y = game->screen.screenHeight - game->textureAssets.Dealer.h;
    game->textureAssets.Dealer.isShowing = true;

    game->textureAssets.Snail.textureID = load_texture("External/Gemini/Snail.png");
    game->textureAssets.Snail.w = 400;
    game->textureAssets.Snail.h = 450;
    game->textureAssets.Snail.x = game->screen.screenWidth - game->textureAssets.Snail.w;
    game->textureAssets.Snail.y = game->screen.screenHeight - game->textureAssets.Snail.h;
    game->textureAssets.Snail.isShowing = true;

    game->textureAssets.SpaceButton.textureID = load_texture("External/littleicon/space.png");
    game->textureAssets.SpaceButton.w = 80;
    game->textureAssets.SpaceButton.h = 80;
    game->textureAssets.SpaceButton.x = (game->screen.screenWidth - game->textureAssets.SpaceButton.w) / 2;
    game->textureAssets.SpaceButton.y = game->screen.screenHeight - game->textureAssets.SpaceButton.h - 21;
    game->textureAssets.SpaceButton.isShowing = true;

    game->textureAssets.ESCButton.textureID = load_texture("External/littleicon/escape-sign.png");
    game->textureAssets.ESCButton.w = 64;
    game->textureAssets.ESCButton.h = 64;
    game->textureAssets.ESCButton.x = 0;
    game->textureAssets.ESCButton.y = 0;
    game->textureAssets.ESCButton.isShowing = true;

    game->textureAssets.VButton.textureID = load_texture("External/littleicon/letter-v.png");
    game->textureAssets.VButton.w = 64;
    game->textureAssets.VButton.h = 64;
    game->textureAssets.VButton.x = (game->screen.screenWidth - game->textureAssets.VButton.w) - 30;
    game->textureAssets.VButton.y = game->screen.screenHeight - game->textureAssets.VButton.h - 30;
    game->textureAssets.VButton.isShowing = true;

    game->textureAssets.EButton.textureID = load_texture("External/littleicon/letter-e.png");
    game->textureAssets.EButton.w = 64;
    game->textureAssets.EButton.h = 64;
    game->textureAssets.EButton.x = (game->screen.screenWidth/2 - 10);
    game->textureAssets.EButton.y = game->screen.screenHeight/2 - game->textureAssets.EButton.h/2;
    game->textureAssets.EButton.isShowing = true;
}

void build_main_menu_ui(Game* game) {
    if(!game->textureAssets.mainFont) return;

    SDL_Color red = {255, 50, 50, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gray = {150, 150, 150, 255};

    int cw = game->screen.screenWidth / 2;

    game->mainMenuUI.title = create_text_ui_element(game->textureAssets.mainFont, "Snail's Pace", red, 0, 150);
    game->mainMenuUI.title.x = cw - (game->mainMenuUI.title.w / 2);

    const char* warning_text = "Ez a játék hirtelen ijesztéseket és\nerős hanghatásokat tartalmazhat!";
    game->mainMenuUI.warning = create_text_ui_element(game->textureAssets.mainFont, warning_text, gray, game->textureAssets.Clippy.w + 10, (game->screen.screenHeight-game->textureAssets.Clippy.h));

    game->mainMenuUI.startButton = create_text_ui_element(game->textureAssets.mainFont, "[INDÍTÁS]", white, 0, game->screen.screenHeight/2);
    game->mainMenuUI.startButton.x = cw - (game->mainMenuUI.startButton.w / 2);

    const char* invert_text = game->player.camera.isInvertedMouseY ? "Inverz Y-tengely: BEKAPCSOLVA" : "Inverz Y-tengely: KIKAPCSOLVA";
    game->mainMenuUI.invertYButton = create_text_ui_element(game->textureAssets.mainFont, invert_text, white, 0, (game->screen.screenHeight/2)+50);
    game->mainMenuUI.invertYButton.x = cw - (game->mainMenuUI.invertYButton.w / 2);
}

void world_initialization(Game* game, const char* player_name){
    set_player_defaults(game);
    set_fonts_standards(game);
    build_up_trigger_zones(game);
    build_up_game_objects(game);
    sounds_tuning_up(game);
    paint_texture_assets(game);
    set_visual_novel_state(game, player_name);
    build_main_menu_ui(game);
}

void randomize_monster_energy(Game* game, LightAuraModel* monster) {
    printf("[INFO] randomizing_monster_energy: ");
    Coordinates coordinates[] = {
        {-2.947048, 0, -28.924328},
        {-4.55217, 0, -34.335026},
        {-7.032827, 0, -34.335030},
        {-8.843879, 0, -29.709135},
        {-5.227485, 0, -50.855663},
        {-5.373802, 0, -55.148785},
        {-7.416370, 0, -57.397461},
        {-4.407894, 0, -75.032768},
        {-8.049614, 0, -74.696838},
        {-5.383556, 0, -80.012871},
        {-5.574059, 0, -80.016464},
        {0.075910, 0, -87.432220},
    };
    int randomIndex = rand() % (sizeof(coordinates) / sizeof(coordinates[0]));
    const Coordinates randomCoordinate = coordinates[randomIndex];
    monster->x = randomCoordinate.x;
    monster->y = randomCoordinate.y;
    monster->z = randomCoordinate.z;

    monster->radius = 1.0f;
    monster->triggerZone.isActivated = false;
    monster->triggerZone.x = randomCoordinate.x;
    monster->triggerZone.z = randomCoordinate.z;
    printf("(%.2f, %.2f, %.2f)\n", monster->x, monster->y, monster->z);
}

void build_scene_main_menu(Game* game){
    printf("MAIN_MENU\n");
    change_camera_input_handler(game, false, false);
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

void build_scene_vn_intro(Game* game){
    printf("VN_INTRO\n");
    game->visualNovelState.currentDialogID = 0;
    game->visualNovelState.dialogue = create_dialogue_from_id(DLG_INTRO, game->visualNovelState.playerName, &game->textureAssets.Gyulasz_Scared);
    game->visualNovelState.quest_state = NOTHING;
    change_camera_input_handler(game, false, false);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void build_scene_dealer_room(Game* game){
    printf("DEALER_ROOM\n");
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    playSound(game->sounds.Overture);
    enableFog(2.0f, 10.0f, 0.4f);
    SDL_SetWindowBrightness(game->window, 0.4f);
    game->gameObjects.Dealer.isMoving = true;
    game->visualNovelState.currentDialogID = DLG_MONSTER_APPEARS;
    game->visualNovelState.quest_state = NOTHING;
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void build_scene_pre_bat_vision(Game* game){
    printf("PRE_BAT_VISION\n");
    enableFog(2,10,0.4f);
    SDL_SetWindowBrightness(game->window, 1.0f);
    game->visualNovelState.currentDialogID = DLG_SCENE_SWITCH_BAT_VISION;
    game->visualNovelState.quest_state = NOTHING;
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void build_scene_bat_vision(Game* game){
    printf("BAT_VISION\n");
    glDisable(GL_LIGHT3);
    glEnable(GL_LIGHT2);
    set_camera_position_default(&game->player.camera);
    setup_projection(game->screen.screenWidth, game->screen.screenHeight);
    randomize_monster_energy(game, &game->gameObjects.PunchPacificMonster);
    game->gameObjects.ImmortalSnail.isMoving = false;
    game->triggerZones.BatVisionHelsieTakeAHint.isActivated = false;
    game->gameObjects.ImmortalSnail.x = 0;
    game->gameObjects.ImmortalSnail.y = 0;
    game->gameObjects.ImmortalSnail.z = 20.0f;
    game->visualNovelState.isShowingUI = false;
    game->caughtBySnailAt = 0;

    glEnable(GL_TEXTURE_2D);
    game->visualNovelState.dialogue = create_dialogue_from_id(DLG_GYULASZ_SEE_NOTHING4, game->visualNovelState.playerName, &game->textureAssets.Gyulasz_Scared);
    game->visualNovelState.dialogue.isFinished = false;
    game->visualNovelState.dialogue.isShowing = true;
    change_camera_input_handler(game, true, true);
    game->visualNovelState.quest_state = NOTHING;
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void build_scene_mita_saves_player(Game* game){
    printf("MITA_SAVES_PLAYER\n");
    glDisable(GL_LIGHT3);
    Coordinates coordinates = {1.35f, 2.0f, -2.0f};
    set_camera_position(&game->player.camera, coordinates,-90, 3);
    setup_projection(game->screen.screenWidth, game->screen.screenHeight);

    game->gameObjects.Mita.x = 0.85f;
    game->gameObjects.Mita.y = 0.85f;
    game->gameObjects.Mita.z = -2.0f;
    game->gameObjects.Mita.targetX = 0.85f;
    game->gameObjects.Mita.targetY = 1.0f;
    game->gameObjects.Mita.targetZ = -2.0f;
    game->gameObjects.Mita.pitch = 0.0f;
    game->gameObjects.Mita.yaw = -270.0f;
    game->gameObjects.Mita.animSpeed = MITA_SPEED;
    game->gameObjects.Mita.isMoving = false;

    glEnable(GL_TEXTURE_2D);
    game->visualNovelState.currentDialogID = 35;
    game->visualNovelState.dialogue = create_dialogue_from_id(DLG_MITA_SAVES_PLAYER, game->visualNovelState.playerName, &game->textureAssets.Mita_Relieved);
    game->visualNovelState.dialogue.isFinished = false;
    game->visualNovelState.dialogue.isShowing = true;
    game->visualNovelState.isShowingUI = true;
    game->triggerZones.MitaAsksForAScissors.isActivated = false;
    game->triggerZones.MitaBonkChoice.isActivated = false;
    game->triggerZones.TryToEnterMitasRoom.isActivated = false;
    game->gameObjects.Scissors.triggerZone.isActivated = false;
    game->triggerZones.MitaTakeScissors.isActivated = false;
    game->tryToEnterMitasRoom=0;
    game->visualNovelState.quest_state = KITCHEN_START;
    change_camera_input_handler(game, true, true);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    add_sound_wave(1.35f, 1.9f, -2.0f, 15.0f, 20.0f, 5.0f, 'm');
}

void build_scene_pre_lidar(Game* game){
    printf("PRE_LIDAR\n");
    Coordinates coordinates = {0.6f, 1.6f, 0.0f};
    set_camera_position(&game->player.camera, coordinates, 90.0f, 0.0f);
    prepare_lidar_data(game->gameObjects.LidarMap);
    SDL_SetWindowBrightness(game->window, 1.0f);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void build_scene_lidar(Game* game){
    printf("LIDAR\n");
    Coordinates coordinates = {0.5f, 0.75f, 0.0f};
    set_camera_position(&game->player.camera, coordinates, 0.0f, 0.0f);
    prepare_lidar_data(game->gameObjects.LidarMap);
    SDL_SetWindowBrightness(game->window, 1.0f);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    change_camera_input_handler(game, true, true);
}

void build_scene_last_room(Game* game){
    printf("LAST_ROOM\n");
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void build_scene_dead_room(Game* game){
    printf("DEAD_ROOM\n");
    glDisable(GL_LIGHT2);
    set_camera_position_default(&game->player.camera);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void build_scene(Game* game, GameScene game_scene){
    switch(game_scene){
        case MAIN_MENU:
            build_scene_main_menu(game);
            break;
        case VN_INTRO:
            build_scene_vn_intro(game);
            break;
        case DEALER_ROOM:
            build_scene_dealer_room(game);
            break;
        case PRE_BAT_VISION:
            build_scene_pre_bat_vision(game);
            break;
        case BAT_VISION:
            build_scene_bat_vision(game);
            break;
        case MITA_SAVES_PLAYER:
            build_scene_mita_saves_player(game);
            break;
        case PRE_LIDAR:
            build_scene_pre_lidar(game);
            break;
        case LIDAR:
            build_scene_lidar(game);
            break;
        case LAST_ROOM:
            build_scene_last_room(game);
            break;
        case DEAD_ROOM:
            build_scene_dead_room(game);
            break;
        default:
            printf("default\n");
            break;
    }
}
