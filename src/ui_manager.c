//
// Created by asgyu on 2026. márc. 7..
//

#include "ui_manager.h"
#include <SDL_opengl.h>
#include <SDL_surface.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <math.h>

#include "dialogue_data.h"

static GLuint create_text_texture(TTF_Font* font, const char* text, SDL_Color color, int* width, int* height) {
    SDL_Surface* s = TTF_RenderUTF8_Blended_Wrapped(font, text, color, 500);
    if (!s) return 0;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int mode = GL_RGBA;
    if(s->format->BytesPerPixel == 4){
        if (s->format->Rmask == 0x000000ff) mode = GL_RGBA;
        else mode = GL_BGRA;
    }else if(s->format->BytesPerPixel == 3){
        if (s->format->Rmask == 0x000000ff) mode = GL_RGB;
        else mode = GL_BGR;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, s->pitch / s->format->BytesPerPixel);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, mode, GL_UNSIGNED_BYTE, s->pixels);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    if (width) *width = s->w;
    if (height) *height = s->h;

    SDL_FreeSurface(s);
    return textureID;
}

static void draw_texture_2d(GLuint textureID, int x, int y, int w, int h) {
    if (textureID == 0) return;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glOrtho(0, viewport[2], viewport[3], 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2i(x, y);
    glTexCoord2f(1, 0); glVertex2i(x + w, y);
    glTexCoord2f(1, 1); glVertex2i(x + w, y + h);
    glTexCoord2f(0, 1); glVertex2i(x, y + h);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

void render_ui_texture(UIElement* element){
    if(!element->isShowing) return;
    draw_texture_2d(element->textureID, element->x, element->y, element->w, element->h);
}

void render_dialogue_box(int screenWidth, int screenHeight, Dialogue* dialogue){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(dialogue->bgColor.red, dialogue->bgColor.green, dialogue->bgColor.blue, dialogue->bgColor.alpha);
    glBegin(GL_QUADS);
    glVertex2i(0, screenHeight - 220);
    glVertex2i(screenWidth, screenHeight - 220);
    glVertex2i(screenWidth, screenHeight);
    glVertex2i(0, screenHeight);
    glEnd();

    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2i(0, screenHeight - 220);
    glVertex2i(screenWidth, screenHeight - 220);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    if(dialogue->isFinished && (SDL_GetTicks() / 500) % 2){
        glBegin(GL_TRIANGLES);
        glVertex2i(screenWidth/2-10, screenHeight - 70);
        glVertex2i(screenWidth/2 + 10, screenHeight - 70);
        glVertex2i(screenWidth/2, screenHeight - 60);
        glEnd();
    }

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void render_dialogue_text(Dialogue* d, TTF_Font* font){
    if (!font || !d->currentVisibleText[0]) return;

    if (d->charIndex != d->lastCharIndex && d->charIndex != 0) {
        if (d->textTexture != 0) glDeleteTextures(1, &d->textTexture);

        d->textTexture = create_text_texture(font, d->currentVisibleText, d->textColor, &d->textWidth, &d->textHeight);
        d->lastCharIndex = d->charIndex;
    }

    draw_texture_2d(d->textTexture, d->textStartX, d->textStartY, d->textWidth, d->textHeight);
}

void render_dialogue_name(Dialogue* d, TTF_Font* font){
    if (!font || !d->speakerName[0]) return;

    if (d->nameTexture == 0) {
        d->nameTexture = create_text_texture(font, d->speakerName, d->nameColor, &d->nameWidth, &d->nameHeight);
    }

    draw_texture_2d(d->nameTexture, d->nameStartX, d->nameStartY, d->nameWidth, d->nameHeight);
}

void update_dialogue(Dialogue* d, Uint32 currentTime){
    if (d->isFinished) return;

    int elapsed = currentTime - d->startTime;
    int speed = 30;
    d->charIndex = elapsed / speed;

    size_t fullLen = strlen(d->fullText);
    if(d->charIndex < fullLen){
        strncpy(d->currentVisibleText, d->fullText, d->charIndex);
        d->currentVisibleText[d->charIndex] = '\0';
    }else{
        strcpy(d->currentVisibleText, d->fullText);
        d->isFinished = true;
        d->charIndex = fullLen;
    }
}