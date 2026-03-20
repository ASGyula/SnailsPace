//
// Created by asgyu on 2026. márc. 7..
//

#ifndef SNAILSPACE_UI_MANAGER_H
#define SNAILSPACE_UI_MANAGER_H
#include <SDL_opengl.h>
#include <SDL_stdinc.h>
#include <SDL_ttf.h>
#include <stdbool.h>

typedef struct{
    GLuint textureID;
    int x, y, w, h;
    bool isShowing;
} UIElement;

typedef struct{
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;
} Color;

typedef struct{
    char fullText[512];
    char currentVisibleText[512];
    char speakerName[64];
    Uint32 startTime;
    int charIndex;
    int lastCharIndex;
    bool isFinished;

    GLuint textTexture;
    int textWidth;
    int textHeight;

    GLuint nameTexture;
    int nameWidth;
    int nameHeight;

    Color bgColor;
    SDL_Color textColor;
    SDL_Color nameColor;
    int textStartX;
    int textStartY;
    int nameStartX;
    int nameStartY;
    UIElement speaker;
    bool isShowing;
} Dialogue;

typedef struct{
    int screenWidth;
    int screenHeight;
} Screen;

UIElement create_text_ui_element(TTF_Font* font, const char* text, SDL_Color color, int x, int y);

void update_text_ui_element(UIElement* element, TTF_Font* font, const char* text, SDL_Color color);

bool is_mouse_over_ui(UIElement* element, int mouseX, int mouseY);

void render_ui_texture(UIElement* element);

void render_dialogue_box(int screenWidth, int screenHeight, Dialogue* dialogue);

void render_dialogue_text(Dialogue* dialogue, TTF_Font* font);

void render_dialogue_name(Dialogue* dialogue, TTF_Font* font);

void update_dialogue(Dialogue* d, Uint32 currentTime);


#endif //SNAILSPACE_UI_MANAGER_H