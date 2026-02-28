//
// Created by asgyu on 2026. febr. 26..
//

#ifndef SNAILSPACE_CAMERA_H
#define SNAILSPACE_CAMERA_H
#include <SDL_events.h>
#include <SDL_opengl.h>

typedef struct{
    Uint32 startTime;
    Uint16 duration;
    Uint16 delay;
    float sens;
    bool isSouthSource;
} Shout;

typedef struct{
    float x, y, z;
    float pitch, yaw;
    float mouseSpeed;
    float moveSpeed;
    bool isInvertedMouseY;
    Shout shout;
    int screenWidth;
    int screenHeight;
} Camera;


typedef struct{
    GLuint textureID;
    int x, y, w, h;
    bool isShowing;
} UIElement;


void initialize_camera(Camera* camera);
void update_camera(Camera* camera);
void handle_camera_input(SDL_Event* event, Camera* camera);
void handle_wasd_input(SDL_Event* event, Camera* camera, bool* isRunning, float deltaTime);

#endif //SNAILSPACE_CAMERA_H