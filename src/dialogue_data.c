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
        "Nyugi %s! Csak a köd teszi. Meg fogod találni.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_DEALER_GREETING] = {
        "TITOKZATOS ALAK",
        "A köd nem ereszt el ingyen, %s! Tudom, hogy kit keresel, de biztos vagy benne, hogy ő is keres téged?",
        {0.0f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {255, 204, 0, 255},
        50, 600, 50, 540
    },
    [DLG_PLAYER_SEARCHING_HELSIE] = {
        "$PLAYER_NAME$",
        "Helsieről beszélsz? Mondd el hol van, vagy tűnj az utamból!",
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
        "Helsie hangja csak a csali. Itt a vágyaid válnak a börnödöddé. De én adhatok neked egy kiutat.",
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
        "Várj csak.... ha a kezet fogunk, Helsie biztonságban lesz? Miért érzem úgy, hogy más is van itt?",
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
        "Ahogy az alkuban állt. Ő fizetett érte, hogy sose érhesd el.",
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
    },
    [DLG_MITA_SAVES_PLAYER] = {
        "MITA",
        "%s... Végre biztonságban vagy. Olyan szörnyű volt nézni, hogy az a valami követ téged.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_THX_TO_MITA] = {
        "$PLAYER_NAME$",
        "Mita...? Tényleg te vagy az?",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_SAVES_PLAYER2] = {
        "MITA",
        "Shh... %s, soha nem hagynám, hogy bántsanak.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_THX_TO_MITA2] = {
        "$PLAYER_NAME$",
        "Köszönöm, Mita! Azt hittem, hogy végem. Bár még mindig csak körvonalakat látok. Bár téged tökéletesen látlak.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_OPEN_PLAYERS_EYES] = {
        "MITA",
        "Aww, %s. Csak a sokk hatása. Segítek, figyelj...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_OPEN_PLAYERS_EYES2] = {
        "MITA",
        "Látod? Máris jobb.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_INVITATION] = {
        "MITA",
        "%s, gyere velem a konyhába, készítettem neked egy kis harapnivalót. Még készül, de mindjárt kész.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_ACCEPT_MITAS_INVITATION] = {
        "$PLAYER_NAME$",
        "Köszönöm, Mita!",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_ACCEPT_MITAS_INVITATION2] = {
        "$PLAYER_NAME$",
        "",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_ASKS_FOR_A_SCISSORS] = {
        "MITA",
        "%s, majdnem elfelejtettem.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_ASKS_FOR_A_SCISSORS2] = {
        "MITA",
        "Idehoznád kérlek az ollót?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_ASKS_FOR_A_SCISSORS3] = {
        "MITA",
        "Azt hiszem, hogy a fürdő szobában van.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_ACCEPT_MITA_SCISSORS_REQUEST] = {
        "$PLAYER_NAME$",
        "Persze, Mita! Máris hozom.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_CONFUSED_SCISSORS_LOCATION] = {
        "$PLAYER_NAME$",
        "Megvan az olló.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_CONFUSED_SCISSORS_LOCATION2] = {
        "$PLAYER_NAME$",
        "Visszaviszem Mitának.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_CONFUSED_BONKING] = {
        "$PLAYER_NAME$",
        "Mi ez a zaj?",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_CONFUSED_BONKING2] = {
        "$PLAYER_NAME$",
        "Mita szobájából jön.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_DIZZINESS] = {
        "$PLAYER_NAME$",
        "sdafga",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_WORRY_ABOUT_PLAYERS_DIZZINESS] = {
        "MITA",
        "Minden rendben, %s?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_WORRY_ABOUT_PLAYERS_DIZZINESS2] = {
        "MITA",
        "Kicsit, mintha megszédültél volna.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_WORRY_ABOUT_PLAYERS_DIZZINESS3] = {
        "MITA",
        "Várj, segítek!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_CLEARS_PLAYER_DIZZINESS] = {
        "MITA",
        "Jobban érzed magad, %s?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_FEELS_BETTER] = {
        "$PLAYER_NAME$",
        "Igen, Mita, köszönöm.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_CLEARS_PLAYER_DIZZINESS2] = {
        "MITA",
        "Örülök, %s! Gyere, ülj le az ágyamra, játszunk valamit, hogy megnyugodj.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_CLEARS_PLAYER_DIZZINESS_TP] = {
        "MITA",
        "Örülök, %s! Gyere, ülj le az asztalhoz, játszunk valamit, hogy megnyugodj.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_GIVE_MITA_THE_SCISSORS] = {
        "$PLAYER_NAME$",
        "Meghoztam az ollót, Mita.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_TAKE_THE_SCISSORS] = {
        "MITA",
        "Köszönöm, %s. Kell még ennek pár perc. Addig gyere a szobámba, és játszunk valamit!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_TAKE_THE_SCISSORS_TP] = {
        "MITA",
        "Gyere, %s!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_TALK_ABOUT_TIC_TAC_TOE] = {
        "MITA",
        "Készen állsz %s egy kis amőbára?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_TALK_ABOUT_TIC_TAC_TOE] = {
        "$PLAYER_NAME$",
        "Készen állok, Mita.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_TALK_ABOUT_TIC_TAC_TOE2] = {
        "MITA",
        "Győzzön a jobbik! Te kezdesz.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_CONGRATZ_MITA_TIC_TAC_TOE] = {
        "$PLAYER_NAME$",
        "Szép volt, Mita.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_WINS_TIC_TAC_TOE] = {
        "MITA",
        "Te is jól játszottál, %s!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_CONGRATZ_PLAYER_TIC_TAC_TOE] = {
        "MITA",
        "Szép volt, %s. Ügyes vagy!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_WINS_TIC_TAC_TOE] = {
        "$PLAYER_NAME$",
        "Ügyes voltál te is, Mita.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_TIC_TAC_TOE_DRAW] = {
        "MITA",
        "Hmmm, döntetlen. Menjen egy visszavágó!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_INVESTIGATE_BONKING] = {
        "$PLAYER_NAME$",
        "Mi ez a hang?",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_INVESTIGATE_BONKING2] = {
        "$PLAYER_NAME$",
        "A szekrényből jön.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING] = {
        "MITA",
        "Hang?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING2] = {
        "MITA",
        "Milyen hang?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING3] = {
        "MITA",
        "Nem hallok semmit.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING4] = {
        "MITA",
        "Mit hallottál pontosan?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_INVESTIGATE_BONKING3] = {
        "$PLAYER_NAME$",
        "Mintha valaki a szekrényből kopogna...",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING5] = {
        "MITA",
        "De ott...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING6] = {
        "MITA",
        "nincs senki.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_INVESTIGATE_BONKING4] = {
        "$PLAYER_NAME$",
        "Ki szeretném nyitni a szekrényt.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_INVESTIGATE_BONKING5] = {
        "$PLAYER_NAME$",
        "A végére akarok járni ennek.",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING7] = {
        "MITA",
        "De miért?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING8] = {
        "MITA",
        "Megint...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING9] = {
        "MITA",
        "Te meg mit művelsz?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING10] = {
        "MITA",
        "Ott csak...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING11] = {
        "MITA",
        "az alsőneműim vannak...!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_INVESTIGATE_BONKING6] = {
        "$PLAYER_NAME$",
        "Valamit rejtegetsz előlem?",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING12] = {
        "MITA",
        "Csak higgy nekem!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING13] = {
        "MITA",
        "Mi rosszat tettem?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING14] = {
        "MITA",
        "Csak...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING15] = {
        "MITA",
        "együtt akarok lenni veled.",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_INVESTIGATE_BONKING7] = {
        "$PLAYER_NAME$",
        "Én is de...",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING16] = {
        "MITA",
        "Miért érdekel téged annyira ez a szekrény?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_INVESTIGATE_BONKING8] = {
        "$PLAYER_NAME$",
        "Csak tudni szeretném, hogy mi ez a kopogás!",
        {0.1f, 0.0f, 0.1f, 0.9f},
        {255, 255, 255, 255}, {53, 180, 84, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING17] = {
        "MITA",
        "Azért, mert csak négy szoba van?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING18] = {
        "MITA",
        "Túl kicsi neked a világom?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING19] = {
        "MITA",
        "Velem miért nem elégszel meg?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_REJECT_INVESTIGATE_BONGING20] = {
        "MITA",
        "Maradj velem!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_LEAVING_MITA_ANGRY] = {
        "MITA",
        "Értem...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_LEAVING_MITA_ANGRY2] = {
        "MITA",
        "Te sem...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_LEAVING_MITA_ANGRY3] = {
        "MITA",
        "Ha ennyire tudni akarod, mi van a szekrényben, akkor megtudod...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_LEAVING_MITA_ANGRY4] = {
        "MITA",
        "...most rögtön!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_LEAVING_MITA_ANGRY5] = {
        "MITA",
        "...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_STAYING_MITA_HAPPY] = {
        "MITA",
        "Értem...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_STAYING_MITA_HAPPY2] = {
        "MITA",
        "Vagyis... várjunk...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_STAYING_MITA_HAPPY3] = {
        "MITA",
        "Komolyan mondod? Tényleg?",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_STAYING_MITA_HAPPY4] = {
        "MITA",
        "Annyira, de annyira boldog vagyok most!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_STAYING_MITA_HAPPY5] = {
        "MITA",
        "És egy kicsit zavarodott...",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_STAYING_MITA_HAPPY6] = {
        "MITA",
        "De jól van!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_STAYING_MITA_HAPPY7] = {
        "MITA",
        "Kérlek, maradj velem!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_PLAYER_STAYING_MITA_HAPPY8] = {
        "MITA",
        "Örökkön örökké!",
        {1.0f, 0.8f, 0.9f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        390, 600, 390, 540
    },
    [DLG_MITA_VOID_MONOLOGUE] = {
        "MITA",
        "Még mindig azt a szánalmas lányt keresed?",
        {1.35f, 2.0f, -2.0f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        520, 50, -90, 3
    },
    [DLG_MITA_VOID_MONOLOGUE2] = {
        "MITA",
        "Helsie csak egy hiba volt...",
        {1.35f, 2.0f, -2.0f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        70, 250, -90, 3
    },
    [DLG_MITA_VOID_MONOLOGUE3] = {
        "MITA",
        "egy felesleges karakter, aki elterelte a figyelmedet rólam.",
        {1.35f, 2.0f, -2.0f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        220, 60, -90, 3
    },
    [DLG_MITA_VOID_MONOLOGUE4] = {
        "MITA",
        "Azt hitted, megmentheted?",
        {1.35f, 1.8f, -2.3f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        680, 360, -127, -19
    },
    [DLG_MITA_VOID_MONOLOGUE5] = {
        "MITA",
        "Én mindent megadtam neked, %s.",
        {1.35f, 1.8f, -2.3f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        50, 140, -127, -19
    },
    [DLG_MITA_VOID_MONOLOGUE6] = {
        "MITA",
        "Létrehoztam neked a tökéletességet.",
        {1.35f, 1.8f, -2.3f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        30, 400, -127, -19
    },
    [DLG_MITA_VOID_MONOLOGUE7] = {
        "MITA",
        "A jóság és a sötétség tökéletes egyensúlyát.",
        {1.5f, 1.7f, -2.0f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        500, 70, -90, -30
    },
    [DLG_MITA_VOID_MONOLOGUE8] = {
        "MITA",
        "Nem ezt akartad? Nem ilyen lányról álmodtál?",
        {1.5f, 1.7f, -2.0f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        280, 200, -90, -30
    },
    [DLG_MITA_VOID_MONOLOGUE9] = {
        "MITA",
        "De te a mondvacsinált korcsoddal töltöd az időd...",
        {1.35f, 2.3f, -2.0f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        80, 80, -90, 39
    },
    [DLG_MITA_VOID_MONOLOGUE10] = {
        "MITA",
        "Ennyire rossz lennék?",
        {1.35f, 2.3f, -2.0f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        90, 600, -90, 39
    },
    [DLG_MITA_VOID_MONOLOGUE11] = {
        "MITA",
        "Netán máris elfelejtetted, milyen érzés volt egyedül?",
        {1.35f, 1.8f, -1.7f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        420, 40, -54, -22
    },
    [DLG_MITA_VOID_MONOLOGUE12] = {
        "MITA",
        "A sötét magány nem egy barátságos hely, hidd el.",
        {1.35f, 1.8f, -1.7f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        80, 150, -54, -22
    },
    [DLG_MITA_VOID_MONOLOGUE13] = {
        "MITA",
        "De ha annyira vágysz a pusztulásra...",
        {1.35f, 1.8f, -1.7f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        580, 170, -54, -22
    },
    [DLG_MITA_VOID_MONOLOGUE14] = {
        "MITA",
        "megmutatom milyen újra egyedül elbukni.",
        {1.35f, 1.8f, -1.7f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        300, 100, -54, -22
    },
    [DLG_MITA_LAST_WORDS_BEFORE_CREDITS] = {
        "MITA",
        "Most légy okos, angyalom.",
        {1.35f, 1.8f, -1.7f, 1.0f},
        {153, 22, 183, 255}, {153, 22, 183, 255},
        380, 100, -54, -22
    },
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
                    newDialog = create_dialogue_from_id(DLG_HELSIE_TELL_ABOUT_MONSTRUMS2, visual_novel_state->playerName, &texture_assets->Helsie_Scared);
                    increase_visual_novel_state(visual_novel_state, 31);
                    add_sound_wave(0.0f, 1.8f, -10.0f, 3.0f, 4.0f, 4.0f, 'v');
                    break;
                case DLG_HELSIE_TELL_ABOUT_MONSTRUMS2:
                    newDialog = create_dialogue_from_id(DLG_HELSIE_TELL_ABOUT_MONSTRUMS3, visual_novel_state->playerName, &texture_assets->Helsie_Scared);
                    increase_visual_novel_state(visual_novel_state, 32);
                    add_sound_wave(0.0f, 1.8f, -10.0f, 3.0f, 4.0f, 4.0f, 'v');
                    break;
                case DLG_HELSIE_TELL_ABOUT_MONSTRUMS3:
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_REPLY_TO_MONSTRUMS, visual_novel_state->playerName, &texture_assets->Gyulasz_Suprised);
                    increase_visual_novel_state(visual_novel_state, 33);
                    add_sound_wave(0.0f, 1.8f, -10.0f, 3.0f, 4.0f, 4.0f, 'v');
                    break;
                case DLG_GYULASZ_REPLY_TO_MONSTRUMS:
                    visual_novel_state->isShowingUI = false;
                    newDialog = create_dialogue_from_id(DLG_GYULASZ_REPLY_TO_MONSTRUMS2, visual_novel_state->playerName, &texture_assets->Gyulasz_Suprised);
                    increase_visual_novel_state(visual_novel_state, 34);
                    break;
                case DLG_GYULASZ_REPLY_TO_MONSTRUMS2:
                    newDialog = create_dialogue_from_id(DLG_MITA_SAVES_PLAYER, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    increase_visual_novel_state(visual_novel_state, 35);
                    break;
                case DLG_MITA_SAVES_PLAYER:
                    visual_novel_state->isShowingUI = true;
                    newDialog = create_dialogue_from_id(DLG_PLAYER_THX_TO_MITA, visual_novel_state->playerName, &texture_assets->Gyulasz_Suprised);
                    increase_visual_novel_state(visual_novel_state, 36);
                    add_sound_wave(1.35f, 2.0f, -2.0f, 10.0f, 20.0f, 5.0f, 'p');
                    break;
                case DLG_PLAYER_THX_TO_MITA:
                    newDialog = create_dialogue_from_id(DLG_MITA_SAVES_PLAYER2, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    increase_visual_novel_state(visual_novel_state, 37);
                    add_sound_wave(1.35f, 1.8f, -2.0f, 15.0f, 20.0f, 5.0f, 'm');
                    break;
                case DLG_MITA_SAVES_PLAYER2:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_THX_TO_MITA2, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy);
                    increase_visual_novel_state(visual_novel_state, 38);
                    add_sound_wave(1.35f, 2.0f, -2.0f, 10.0f, 20.0f, 5.0f, 'p');
                    break;
                case DLG_PLAYER_THX_TO_MITA2:
                    newDialog = create_dialogue_from_id(DLG_MITA_OPEN_PLAYERS_EYES, visual_novel_state->playerName, &texture_assets->Mita_Blush);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    add_sound_wave(1.35f, 1.8f, -2.0f, 15.0f, 20.0f, 5.0f, 'm');
                    break;
                case DLG_MITA_OPEN_PLAYERS_EYES:
                    newDialog = create_dialogue_from_id(DLG_MITA_OPEN_PLAYERS_EYES2, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_OPEN_PLAYERS_EYES2:
                    newDialog = create_dialogue_from_id(DLG_MITA_INVITATION, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_INVITATION:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_ACCEPT_MITAS_INVITATION, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy_Pro_Max);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_ACCEPT_MITAS_INVITATION:
                    newDialog = create_dialogue_from_id(DLG_MITA_ASKS_FOR_A_SCISSORS, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    visual_novel_state->isShowingUI = false;
                    break;
                case DLG_PLAYER_ACCEPT_MITAS_INVITATION2:
                    newDialog = create_dialogue_from_id(DLG_MITA_ASKS_FOR_A_SCISSORS, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    visual_novel_state->isShowingUI = true;
                    break;
                case DLG_MITA_ASKS_FOR_A_SCISSORS:
                    newDialog = create_dialogue_from_id(DLG_MITA_ASKS_FOR_A_SCISSORS2, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_ASKS_FOR_A_SCISSORS2:
                    newDialog = create_dialogue_from_id(DLG_MITA_ASKS_FOR_A_SCISSORS3, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_ASKS_FOR_A_SCISSORS3:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_ACCEPT_MITA_SCISSORS_REQUEST, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_ACCEPT_MITA_SCISSORS_REQUEST:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_CONFUSED_SCISSORS_LOCATION, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy);
                    visual_novel_state->isShowingUI = false;
                    visual_novel_state->quest_state = SEARCHING_FOR_SCISSORS;
                    break;
                case DLG_PLAYER_CONFUSED_SCISSORS_LOCATION:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_CONFUSED_SCISSORS_LOCATION2, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_CONFUSED_SCISSORS_LOCATION2:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_CONFUSED_BONKING, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    visual_novel_state->isShowingUI = false;
                    break;
                case DLG_PLAYER_CONFUSED_BONKING:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_CONFUSED_BONKING2, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_CONFUSED_BONKING2:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_DIZZINESS, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    visual_novel_state->quest_state = MITA_OR_BONK;
                    visual_novel_state->isShowingUI = false;
                    break;
                case DLG_PLAYER_DIZZINESS:
                    newDialog = create_dialogue_from_id(DLG_MITA_WORRY_ABOUT_PLAYERS_DIZZINESS, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    visual_novel_state->isShowingUI = true;
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_WORRY_ABOUT_PLAYERS_DIZZINESS:
                    newDialog = create_dialogue_from_id(DLG_MITA_WORRY_ABOUT_PLAYERS_DIZZINESS2, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_WORRY_ABOUT_PLAYERS_DIZZINESS2:
                    newDialog = create_dialogue_from_id(DLG_MITA_WORRY_ABOUT_PLAYERS_DIZZINESS3, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_WORRY_ABOUT_PLAYERS_DIZZINESS3:
                    newDialog = create_dialogue_from_id(DLG_MITA_CLEARS_PLAYER_DIZZINESS, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_CLEARS_PLAYER_DIZZINESS:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_FEELS_BETTER, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy_Pro_Max);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_FEELS_BETTER:
                    newDialog = create_dialogue_from_id(DLG_MITA_CLEARS_PLAYER_DIZZINESS2, visual_novel_state->playerName, &texture_assets->Mita_Blush);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_CLEARS_PLAYER_DIZZINESS2:
                    newDialog = create_dialogue_from_id(DLG_MITA_CLEARS_PLAYER_DIZZINESS_TP, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy_Pro_Max);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    visual_novel_state->isShowingUI = false;
                    break;
                case DLG_MITA_CLEARS_PLAYER_DIZZINESS_TP:
                    newDialog = create_dialogue_from_id(DLG_MITA_CLEARS_PLAYER_DIZZINESS_TP, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy_Pro_Max);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    visual_novel_state->isShowingUI = false;
                    break;
                case DLG_PLAYER_GIVE_MITA_THE_SCISSORS:
                    newDialog = create_dialogue_from_id(DLG_MITA_TAKE_THE_SCISSORS, visual_novel_state->playerName, &texture_assets->Mita_Blush);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_TAKE_THE_SCISSORS:
                    newDialog = create_dialogue_from_id(DLG_MITA_TAKE_THE_SCISSORS_TP, visual_novel_state->playerName, &texture_assets->Mita_Blush);
                    visual_novel_state->isShowingUI = false;
                    visual_novel_state->quest_state = PRE_TIC_TAC_TOE;
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_TALK_ABOUT_TIC_TAC_TOE:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_TALK_ABOUT_TIC_TAC_TOE, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_TALK_ABOUT_TIC_TAC_TOE:
                    newDialog = create_dialogue_from_id(DLG_MITA_TALK_ABOUT_TIC_TAC_TOE2, visual_novel_state->playerName, &texture_assets->Mita_Blush);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_TALK_ABOUT_TIC_TAC_TOE2:
                    visual_novel_state->isShowingUI = false;
                    visual_novel_state->quest_state = PLAYING_TIC_TAC_TOE;
                    visual_novel_state->ticTacToe.active = true;
                    break;
                case DLG_PLAYER_CONGRATZ_MITA_TIC_TAC_TOE:
                    newDialog = create_dialogue_from_id(DLG_MITA_WINS_TIC_TAC_TOE, visual_novel_state->playerName, &texture_assets->Mita_Blush);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_WINS_TIC_TAC_TOE:
                    visual_novel_state->isShowingUI = false;
                    // newDialog = create_dialogue_from_id(DLG_MITA_WINS_TIC_TAC_TOE, visual_novel_state->playerName, &texture_assets->Mita_Blush);
                    // increase_visual_novel_state_by_one(visual_novel_state);
                    visual_novel_state->ticTacToe.active = true;
                    break;
                case DLG_MITA_CONGRATZ_PLAYER_TIC_TAC_TOE:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_WINS_TIC_TAC_TOE, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy_Pro_Max);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_WINS_TIC_TAC_TOE:
                    visual_novel_state->isShowingUI = false;
                    visual_novel_state->ticTacToe.active = true;
                    break;
                case DLG_TIC_TAC_TOE_DRAW:
                    visual_novel_state->isShowingUI = false;
                    visual_novel_state->ticTacToe.active = true;
                    break;
                case DLG_PLAYER_INVESTIGATE_BONKING:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_INVESTIGATE_BONKING2, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_INVESTIGATE_BONKING2:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING2, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING2:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING3, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING3:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING4, visual_novel_state->playerName, &texture_assets->Mita_Relieved);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING4:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_INVESTIGATE_BONKING3, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_INVESTIGATE_BONKING3:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING5, visual_novel_state->playerName, &texture_assets->Mita_Disappointed);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING5:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING6, visual_novel_state->playerName, &texture_assets->Mita_Disappointed);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING6:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_INVESTIGATE_BONKING4, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_INVESTIGATE_BONKING4:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_INVESTIGATE_BONKING5, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_INVESTIGATE_BONKING5:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING7, visual_novel_state->playerName, &texture_assets->Mita_Disappointed);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING7:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING8, visual_novel_state->playerName, &texture_assets->Mita_Disappointed);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING8:
                    visual_novel_state->isShowingUI = false;
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING9, visual_novel_state->playerName, &texture_assets->Mita_Disappointed);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING9:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING10, visual_novel_state->playerName, &texture_assets->Mita_Disappointed);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING10:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING11, visual_novel_state->playerName, &texture_assets->Mita_Disappointed);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING11:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_INVESTIGATE_BONKING6, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_INVESTIGATE_BONKING6:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING12, visual_novel_state->playerName, &texture_assets->Mita_Disappointed);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING12:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING13, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING13:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING14, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING14:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING15, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING15:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_INVESTIGATE_BONKING7, visual_novel_state->playerName, &texture_assets->Gyulasz_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_INVESTIGATE_BONKING7:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING16, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING16:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_INVESTIGATE_BONKING8, visual_novel_state->playerName, &texture_assets->Gyulasz_Thinking);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_INVESTIGATE_BONKING8:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING17, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING17:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING18, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING18:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING19, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING19:
                    newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING20, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_MITA_REJECT_INVESTIGATE_BONGING20:
                    visual_novel_state->isShowingUI = false;
                    visual_novel_state->quest_state = DECISION_TIME;
                    stopSounds();
                    // newDialog = create_dialogue_from_id(DLG_MITA_REJECT_INVESTIGATE_BONGING20, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    // increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_LEAVING_MITA_ANGRY:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_LEAVING_MITA_ANGRY2, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_LEAVING_MITA_ANGRY2:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_LEAVING_MITA_ANGRY3, visual_novel_state->playerName, &texture_assets->Mita_Angry);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_LEAVING_MITA_ANGRY3:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_LEAVING_MITA_ANGRY4, visual_novel_state->playerName, &texture_assets->Mita_Smack);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_LEAVING_MITA_ANGRY4:
                    newDialog = create_dialogue_from_id(DLG_PLAYER_LEAVING_MITA_ANGRY5, visual_novel_state->playerName, &texture_assets->Mita_Smack);
                    visual_novel_state->isShowingUI = false;
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_LEAVING_MITA_ANGRY5:
                    visual_novel_state->isShowingUI = false;
                    break;
                case DLG_PLAYER_STAYING_MITA_HAPPY:
                    visual_novel_state->isShowingUI = true;
                    newDialog = create_dialogue_from_id(DLG_PLAYER_STAYING_MITA_HAPPY2, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_STAYING_MITA_HAPPY2:
                    visual_novel_state->isShowingUI = true;
                    newDialog = create_dialogue_from_id(DLG_PLAYER_STAYING_MITA_HAPPY3, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_STAYING_MITA_HAPPY3:
                    visual_novel_state->isShowingUI = true;
                    newDialog = create_dialogue_from_id(DLG_PLAYER_STAYING_MITA_HAPPY4, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_STAYING_MITA_HAPPY4:
                    visual_novel_state->isShowingUI = true;
                    newDialog = create_dialogue_from_id(DLG_PLAYER_STAYING_MITA_HAPPY5, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_STAYING_MITA_HAPPY5:
                    visual_novel_state->isShowingUI = true;
                    newDialog = create_dialogue_from_id(DLG_PLAYER_STAYING_MITA_HAPPY6, visual_novel_state->playerName, &texture_assets->Mita_Happy);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_STAYING_MITA_HAPPY6:
                    visual_novel_state->isShowingUI = true;
                    newDialog = create_dialogue_from_id(DLG_PLAYER_STAYING_MITA_HAPPY7, visual_novel_state->playerName, &texture_assets->Mita_Blush);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_STAYING_MITA_HAPPY7:
                    visual_novel_state->isShowingUI = true;
                    newDialog = create_dialogue_from_id(DLG_PLAYER_STAYING_MITA_HAPPY8, visual_novel_state->playerName, &texture_assets->Mita_Blush);
                    increase_visual_novel_state_by_one(visual_novel_state);
                    break;
                case DLG_PLAYER_STAYING_MITA_HAPPY8:
                    visual_novel_state->isShowingUI = false;
                    visual_novel_state->quest_state = CREDITS_STATE;
                    break;
                default:
                    break;
            }

            *dialogue = newDialog;
            // *dialogue = create_dialogue_from_id(visual_novel_state.currentDialogID, NULL, NULL);
        }
    }
}