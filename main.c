#include <math.h>

#include "include/obj_loader.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <stdio.h>


#include "dialogue_data.h"
#include "game_manager.h"
#include "player.h"
#include "scene.h"
#include "ui_manager.h"
#include "input_manager.h"
#ifdef _WIN32
    #include <windows.h>
    #include <lmcons.h>
    #define PLATFORM_GET_USER(buf, size) GetUserNameA(buf, size)
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

static void init_audio_systems(void){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
        printf("[HIBA] SDL Init hiba: %s\n", SDL_GetError());
        return;
    }

    if(TTF_Init() == -1){
        printf("[HIBA] SDL_ttf nem indult el: %s\n", TTF_GetError());
        return;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("[HIBA] Nem sikerult inincializalni a Mixert: %s\n", Mix_GetError());
    }
}

int main(int argc, char *argv[]){
    init_audio_systems();

    char winName[UNLEN + 1];
    DWORD winSize = sizeof(winName);
    GetUserName(winName, &winSize);

    Game game = init_game(SCREEN_WIDTH, SCREEN_HEIGHT, winName);

    SDL_Event event;
    while(game.isRunning){
        Uint32 currentTime = SDL_GetTicks();
        const float deltaTime = (currentTime - game.lastTime) / 1000.0f;
        game.lastTime = currentTime;

        handle_scene_events(&game, &event);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_scene(&game, currentTime, deltaTime);

        handle_esc_input(&event, &game, &game.isRunning, &game.scene);
        if(!game.visualNovelState.isShowingUI) handle_wasd_input(&game.player.camera, deltaTime, game.sounds, game.scene);
        render_ui_texture(&game.textureAssets.ESCButton);

        SDL_GL_SwapWindow(game.window);
    }

    SDL_GL_DeleteContext(game.glContext);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
    return 0;
}