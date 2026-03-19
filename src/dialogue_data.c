#include <lmcons.h>
#include <string.h>
#include <stdio.h>

#include "dialogue_data.h"

#include "game_manager.h"
#include "graphics.h"
#include "visual_novel.h"

typedef struct{
    const char* speaker_name;
    const char* text_template;
    Color bg_color;
    SDL_Color text_color;
    SDL_Color name_color;
    int text_start_x;
    int text_start_y;
    int name_start_x;
    int name_start_y;
} DialogueTemplate;

static const DialogueTemplate ALL_DIALOGUES[] = {
    [DLG_INTRO] = {
        "$PLAYER_NAME$",
        "Helsie? Ez nem vicces. Gyere elő!",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_GYULASZ_CALM_HIMSELF_DOWN] = {
        "$PLAYER_NAME$",
        "Nyugi %s! Csak tiszta fejjel találod meg őt.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_DEALER_GREETING] = {
        "TITOKZATOS ALAK",
        "Azt hitted, egyedül vagy a ködben, %s? Én tudom, miért jöttél... vagy inkább... Kiért. De a válasz nincs ingyen.",
        {0.0f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {255, 204, 0, 255},
        50, 600, 50, 540
    },
    [DLG_PLAYER_SEARCHING_HELSIE] = {
        "$PLAYER_NAME$",
        "Nem érdekelnek a játékaid. Hol van Helsie? Mit tettél vele?",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_HELSIE_GREETING] = {
        "[Helsie suttogása]",
        "%s... se....gíí...ts!",
        {1.0f, 1.0f, 1.0f, 0.9f},
        {90, 37, 255, 101}, {90, 37, 101, 255},
        380, 600, 380, 540
    },
    [DLG_GYULASZ_GREETING_HELSIE] = {
        "$PLAYER_NAME$",
        "Hallom őt! Helsie! Itt vagy?",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_DEALER_KNOWS_HELSIE] = {
        "TITOKZATOS ALAK",
        "Helsie már messze jár. De adhatok valamit, amivel utoléred. Egy szempárt a sötétségben.",
        {0.0f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {255, 204, 0, 255},
        50, 600, 50, 540
    },
    [DLG_GYULASZ_EYES_SPARKING] = {
        "$PLAYER_NAME$",
        "Bármit megteszek. Csak mondd el, hogy hol találom!",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_DEALER_MAKE_AN_OFFER] = {
        "TITOKZATOS ALAK",
        "%s... Óvatosan a szavakkal. Megkapod a látást....",
        {0.0f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {255, 204, 0, 255},
        50, 600, 50, 540
    },
    [DLG_DEALER_MAKE_AN_OFFER2] = {
        "[TITOKZATOS ALAK SUTTOGÁSA]",
        "...cserébe az örökös üldözésért.",
        {0.0f, 0.0f, 0.1f, 1.0f},
        {255, 255, 255, 100}, {255, 204, 0, 100},
        50, 600, 50, 540
    },
    [DLG_GYULASZ_ASKING_ABOUT_OFFER] = {
        "$PLAYER_NAME$",
        "Bármit!",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_DEALERS_MAKE_A_DEAL] = {
        "TITOKZATOS ALAK",
        "Rendben, %s. Kezet rá!",
        {0.0f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {255, 204, 0, 255},
        50, 600, 50, 540
    },
    [DLG_GYULASZ_READING_THE_OFFER] = {
        "$PLAYER_NAME$",
        "*a kezét nyújtja*",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_GYULASZ_READING_THE_OFFER2] = {
        "$PLAYER_NAME$",
        "Mit tettél Helsievel?",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_DEALER_MAKE_A_DEAL2] = {
        "TITOKZATOS ALAK",
        "Mit szeretni jobban, hogy Helsie biztonságban legyen, vagy hogy örök homályba vesszen?",
        {0.0f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {255, 204, 0, 255},
        50, 600, 50, 540
    },
    [DLG_MONSTER_APPEARS] = {
        "TITOKZATOS ALAK",
        "ÍRD ALÁ!",
        {0.5f, 0.0f, 0.0f, 0.95f},
        {200, 0, 0, 255}, {100, 0, 0, 255},
        50, 600, 50, 540
    },
    [DLG_GYULASZ_SIGN_THE_CONTRACT] = {
        "SZERZŐDÉS",
        "%s",
        {0.87f, 0.78f, 0.65f, 0.5f},
        {255, 255, 255, 255}, {224, 199, 168, 255},
        50, 600, 50, 540
    },
    [DLG_GYULASZ_SIGN_THE_CONTRACT2] = {
        "$PLAYER_NAME$",
        "Megtalállak Helsie!",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        50, 600, 50, 540
    },
    [DLG_SCENE_SWITCH_BAT_VISION]={
        "[Helsie suttogása]",
        "%s NE!",
        {1.0f, 1.0f, 1.0f, 0.9f},
        {90, 37, 255, 101}, {90, 37, 101, 255},
        380, 600, 380, 540
    },
    [DLG_IN_THE_BAT_VISION]={
        "[Helsie suttogása]",
        "Hová kerültél? Hol vagy most?",
        {1.0f, 1.0f, 1.0f, 0.9f},
        {90, 37, 255, 101}, {90, 37, 101, 255},
        380, 600, 380, 540
    },
    [DLG_IN_THE_BAT_VISION2] = {
        "$PLAYER_NAME$",
        "Helsie?! Merre vagy?",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_IN_THE_BAT_VISION_SNAIL] = {
        "Immortal Snail",
        "Nem hall téged.",
        {0.2f, 0.0f, 0.0f, 0.95f},
        {200, 0, 0, 255}, {100, 0, 0, 255},
        50, 600, 50, 540
    },
    [DLG_IN_THE_BAT_VISION_SNAIL2] = {
        "Immortal Snail",
        "De én igen.",
        {0.2f, 0.0f, 0.0f, 0.95f},
        {200, 0, 0, 255}, {100, 0, 0, 255},
        50, 600, 50, 540
    },
    [DLG_IN_THE_BAT_VISION_SNAIL3] = {
        "Immortal Snail",
        "És látlak is.",
        {0.2f, 0.0f, 0.0f, 0.95f},
        {200, 0, 0, 255}, {100, 0, 0, 255},
        50, 600, 50, 540
    },
    [DLG_IN_THE_BAT_VISION_SNAIL4] = {
        "Immortal Snail",
        "Tudom hol vagy. És jövök érted.",
        {0.2f, 0.0f, 0.0f, 0.95f},
        {200, 0, 0, 255}, {100, 0, 0, 255},
        50, 600, 50, 540
    },
    [DLG_IN_THE_BAT_VISION_SNAIL5] = {
        "Immortal Snail",
        "Ahogy az alkuban állt.",
        {0.2f, 0.0f, 0.0f, 0.95f},
        {200, 0, 0, 255}, {100, 0, 0, 255},
        50, 600, 50, 540
    },
    [DLG_GYULASZ_SEE_NOTHING] = {
        "$PLAYER_NAME$",
        "Nem látok semmit.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_GYULASZ_SEE_NOTHING2] = {
        "$PLAYER_NAME$",
        "Vagyis csak...",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_GYULASZ_SEE_NOTHING3] = {
        "$PLAYER_NAME$",
        "Talán a hanggokkal látok?",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_GYULASZ_SEE_NOTHING4] = {
        "$PLAYER_NAME$",
        "...",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_HELSIE_TELL_ABOUT_MONSTRUMS]={
        "[Helsie suttogása]",
        "Jól vagy %s? Aggódok érted. Nincs sok időnk.",
        {1.0f, 1.0f, 1.0f, 0.9f},
        {90, 37, 255, 101}, {90, 37, 101, 255},
        380, 600, 380, 540
    },
    [DLG_HELSIE_TELL_ABOUT_MONSTRUMS2]={
        "[Helsie suttogása]",
        "Azt hiszem, hogy tudom hol vagy.",
        {1.0f, 1.0f, 1.0f, 0.9f},
        {90, 37, 255, 101}, {90, 37, 101, 255},
        380, 600, 380, 540
    },
    [DLG_HELSIE_TELL_ABOUT_MONSTRUMS3]={
        "[Helsie suttogása]",
        "Keress Monstrum energiaitalt és i...",
        {1.0f, 1.0f, 1.0f, 0.9f},
        {90, 37, 255, 101}, {90, 37, 101, 255},
        380, 600, 380, 540
    },
    [DLG_GYULASZ_REPLY_TO_MONSTRUMS] = {
        "$PLAYER_NAME$",
        "Helsie...",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_GYULASZ_REPLY_TO_MONSTRUMS2] = {
        "$PLAYER_NAME$",
        "...",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    }
};

Dialogue create_dialogue_from_id(DialogueID id, const char* name_placeholder, const UIElement* speaker_img){
    Dialogue d = {0};
    if(id >= DLG_COUNT) return d;

    const DialogueTemplate* t = &ALL_DIALOGUES[id];

    if(strcmp(t->speaker_name, "$PLAYER_NAME$") == 0){
        if(name_placeholder) strcpy(d.speakerName, name_placeholder);
        else{
            char winName[UNLEN + 1];
            DWORD winSize = sizeof(winName);
            GetUserName(winName, &winSize);
            strcpy(d.speakerName, winName);
        }

        if(speaker_img) d.speaker = *speaker_img;
    }else{
        strcpy(d.speakerName, t->speaker_name);
        d.speaker = *speaker_img;
    }

    if(strstr(t->text_template, "%s") && name_placeholder){
        snprintf(d.fullText, sizeof(d.fullText), t->text_template, name_placeholder);
    }else{
        strncpy(d.fullText, t->text_template, sizeof(d.fullText));
    }

    d.bgColor = t->bg_color;
    d.textColor = t->text_color;
    d.nameColor = t->name_color;
    d.textStartX = t->text_start_x;
    d.textStartY = t->text_start_y;
    d.nameStartX = t->name_start_x;
    d.nameStartY = t->name_start_y;
    d.startTime = SDL_GetTicks();

    return d;
}

void handle_mouse_input_visual_novel(SDL_Event* event, VisualNovelState* visual_novel_state, Dialogue* dialogue, TextureAssets* texture_assets){
    if(event->type == SDL_MOUSEBUTTONDOWN){
        if(event->button.button == SDL_BUTTON_LEFT){
            if(!dialogue->isFinished){
                dialogue->charIndex = strlen(dialogue->fullText);
                strcpy(dialogue->currentVisibleText, dialogue->fullText);
                dialogue->isFinished = true;
                return;
            }

            Dialogue newDialog;
            switch(visual_novel_state->currentDialogID){
                case DLG_INTRO:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_CALM_HIMSELF_DOWN, visual_novel_state->playerName, &texture_assets->Gyulasz_Scared);
                    increase_visual_novel_state(visual_novel_state, 1);
                    break;
                case DLG_GYULASZ_CALM_HIMSELF_DOWN:
                    newDialog = create_dialogue_from_id(DLG_DEALER_GREETING, visual_novel_state->playerName, &texture_assets->Dealer);
                    increase_visual_novel_state(visual_novel_state, 2);
                    break;
                case DLG_DEALER_GREETING:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_SEARCHING_HELSIE, visual_novel_state->playerName, &texture_assets->Gyulasz_Determined);
                    increase_visual_novel_state(visual_novel_state, 3);
                    break;
                case DLG_PLAYER_SEARCHING_HELSIE:
                    newDialog = create_dialogue_from_id(DLG_HELSIE_GREETING, visual_novel_state->playerName, &texture_assets->Helsie_Scared);
                    increase_visual_novel_state(visual_novel_state, 4);
                    break;
                case DLG_HELSIE_GREETING:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_GREETING_HELSIE, visual_novel_state->playerName, &texture_assets->Gyulasz_Suprised);
                    increase_visual_novel_state(visual_novel_state, 5);
                    break;
                case DLG_GYULASZ_GREETING_HELSIE:
                    newDialog = create_dialogue_from_id(DLG_DEALER_KNOWS_HELSIE, visual_novel_state->playerName, &texture_assets->Dealer);
                    increase_visual_novel_state(visual_novel_state, 6);
                    break;
                case DLG_DEALER_KNOWS_HELSIE:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_EYES_SPARKING, visual_novel_state->playerName, &texture_assets->Gyulasz_Determined);
                    increase_visual_novel_state(visual_novel_state, 7);
                    break;
                case DLG_GYULASZ_EYES_SPARKING:
                    newDialog = create_dialogue_from_id(DLG_DEALER_MAKE_AN_OFFER, visual_novel_state->playerName, &texture_assets->Dealer);
                    increase_visual_novel_state(visual_novel_state, 8);
                    break;
                case DLG_DEALER_MAKE_AN_OFFER:
                    newDialog = create_dialogue_from_id(DLG_DEALER_MAKE_AN_OFFER2, visual_novel_state->playerName, &texture_assets->Dealer);
                    increase_visual_novel_state(visual_novel_state, 9);
                    break;
                case DLG_DEALER_MAKE_AN_OFFER2:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_ASKING_ABOUT_OFFER, visual_novel_state->playerName, &texture_assets->Gyulasz_Determined);
                    increase_visual_novel_state(visual_novel_state, 10);
                    break;
                case DLG_GYULASZ_ASKING_ABOUT_OFFER:
                    newDialog = create_dialogue_from_id(DLG_DEALERS_MAKE_A_DEAL, visual_novel_state->playerName, &texture_assets->Dealer);
                    increase_visual_novel_state(visual_novel_state, 11);
                    break;
                case DLG_DEALERS_MAKE_A_DEAL:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_READING_THE_OFFER, visual_novel_state->playerName, &texture_assets->Gyulasz_Hand_Shake);
                    increase_visual_novel_state(visual_novel_state, 12);
                    break;
                case DLG_GYULASZ_READING_THE_OFFER:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_READING_THE_OFFER2, visual_novel_state->playerName, &texture_assets->Gyulasz_Determined);
                    increase_visual_novel_state(visual_novel_state, 13);
                    break;
                case DLG_GYULASZ_READING_THE_OFFER2:
                    newDialog = create_dialogue_from_id(DLG_DEALER_MAKE_A_DEAL2, visual_novel_state->playerName, &texture_assets->Dealer);
                    increase_visual_novel_state(visual_novel_state, 14);
                    break;
                case DLG_DEALER_MAKE_A_DEAL2:
                    newDialog = create_dialogue_from_id(DLG_MONSTER_APPEARS, visual_novel_state->playerName, &texture_assets->Dealer);
                    increase_visual_novel_state(visual_novel_state, 15);
                    break;
                case DLG_MONSTER_APPEARS:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_SIGN_THE_CONTRACT, visual_novel_state->playerName, &texture_assets->Gyulasz_Brave);
                    increase_visual_novel_state(visual_novel_state, 16);
                    break;
                case DLG_GYULASZ_SIGN_THE_CONTRACT:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_SIGN_THE_CONTRACT2, visual_novel_state->playerName, &texture_assets->Gyulasz_Determined);
                    increase_visual_novel_state(visual_novel_state, 17);
                    break;
                case DLG_GYULASZ_SIGN_THE_CONTRACT2:
                    newDialog = create_dialogue_from_id(DLG_SCENE_SWITCH_BAT_VISION, visual_novel_state->playerName, &texture_assets->Helsie_Scared);
                    increase_visual_novel_state(visual_novel_state, 18);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 15.0f, 5.0f, 'v');
                    break;
                case DLG_SCENE_SWITCH_BAT_VISION:
                    newDialog = create_dialogue_from_id(DLG_IN_THE_BAT_VISION, visual_novel_state->playerName, &texture_assets->Helsie_Scared);
                    increase_visual_novel_state(visual_novel_state, 19);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 15.0f, 5.0f, 'v');
                    break;
                case DLG_IN_THE_BAT_VISION:
                    newDialog = create_dialogue_from_id(DLG_IN_THE_BAT_VISION2, visual_novel_state->playerName, &texture_assets->Gyulasz_Scared);
                    increase_visual_novel_state(visual_novel_state, 20);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 20.0f, 5.0f, 'v');
                    break;
                case DLG_IN_THE_BAT_VISION2:
                    newDialog = create_dialogue_from_id(DLG_IN_THE_BAT_VISION_SNAIL, visual_novel_state->playerName, &texture_assets->Snail);
                    increase_visual_novel_state(visual_novel_state, 21);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 10.0f, 4.0f, 'v');
                    break;
                case DLG_IN_THE_BAT_VISION_SNAIL:
                    newDialog = create_dialogue_from_id(DLG_IN_THE_BAT_VISION_SNAIL2, visual_novel_state->playerName, &texture_assets->Snail);
                    increase_visual_novel_state(visual_novel_state, 22);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 10.0f, 4.0f, 'v');
                    break;
                case DLG_IN_THE_BAT_VISION_SNAIL2:
                    newDialog = create_dialogue_from_id(DLG_IN_THE_BAT_VISION_SNAIL3, visual_novel_state->playerName, &texture_assets->Snail);
                    increase_visual_novel_state(visual_novel_state, 23);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 10.0f, 4.0f, 'v');
                    break;
                case DLG_IN_THE_BAT_VISION_SNAIL3:
                    newDialog = create_dialogue_from_id(DLG_IN_THE_BAT_VISION_SNAIL4, visual_novel_state->playerName, &texture_assets->Snail);
                    increase_visual_novel_state(visual_novel_state, 24);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 10.0f, 4.0f, 'v');
                    break;
                case DLG_IN_THE_BAT_VISION_SNAIL4:
                    newDialog = create_dialogue_from_id(DLG_IN_THE_BAT_VISION_SNAIL5, visual_novel_state->playerName, &texture_assets->Snail);
                    increase_visual_novel_state(visual_novel_state, 25);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 10.0f, 4.0f, 'v');
                    break;
                case DLG_IN_THE_BAT_VISION_SNAIL5:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_SEE_NOTHING, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state(visual_novel_state, 26);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 20.0f, 5.0f, 'v');
                    break;
                case DLG_GYULASZ_SEE_NOTHING:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_SEE_NOTHING2, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state(visual_novel_state, 27);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 20.0f, 5.0f, 'v');
                    break;
                case DLG_GYULASZ_SEE_NOTHING2:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_SEE_NOTHING3, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state(visual_novel_state, 28);
                    add_sound_wave(0.0f, 1.8f, 5.0f, 10.0f, 20.0f, 5.0f, 'v');
                    break;
                case DLG_GYULASZ_SEE_NOTHING3:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_SEE_NOTHING4, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state(visual_novel_state, 29);
                    visual_novel_state->isShowingUI = false;
                    break;
                case DLG_GYULASZ_SEE_NOTHING4:
                    newDialog = create_dialogue_from_id(DLG_HELSIE_TELL_ABOUT_MONSTRUMS, visual_novel_state->playerName, &texture_assets->Helsie_Scared);
                    increase_visual_novel_state(visual_novel_state, 30);
                    break;
                case DLG_HELSIE_TELL_ABOUT_MONSTRUMS:
                    printf("Helsie");
                    newDialog = create_dialogue_from_id(DLG_HELSIE_TELL_ABOUT_MONSTRUMS2, visual_novel_state->playerName, &texture_assets->Helsie_Scared);
                    increase_visual_novel_state(visual_novel_state, 31);
                    add_sound_wave(0.0f, 1.8f, -20.0f, 3.0f, 4.0f, 4.0f, 'v');
                    break;
                case DLG_HELSIE_TELL_ABOUT_MONSTRUMS2:
                    newDialog = create_dialogue_from_id(DLG_HELSIE_TELL_ABOUT_MONSTRUMS3, visual_novel_state->playerName, &texture_assets->Helsie_Scared);
                    increase_visual_novel_state(visual_novel_state, 32);
                    add_sound_wave(0.0f, 1.8f, -20.0f, 3.0f, 4.0f, 4.0f, 'v');
                    break;
                case DLG_HELSIE_TELL_ABOUT_MONSTRUMS3:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_REPLY_TO_MONSTRUMS, visual_novel_state->playerName, &texture_assets->Gyulasz_Suprised);
                    increase_visual_novel_state(visual_novel_state, 33);
                    add_sound_wave(0.0f, 1.8f, -20.0f, 3.0f, 4.0f, 4.0f, 'v');
                    break;
                case DLG_GYULASZ_REPLY_TO_MONSTRUMS:
                    visual_novel_state->isShowingUI = false;
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_REPLY_TO_MONSTRUMS2, visual_novel_state->playerName, &texture_assets->Gyulasz_Suprised);
                    increase_visual_novel_state(visual_novel_state, 34);
                    break;
                default:
                    break;
            }

            *dialogue = newDialog;
            // *dialogue = create_dialogue_from_id(visual_novel_state.currentDialogID, NULL, NULL);
        }
    }
}