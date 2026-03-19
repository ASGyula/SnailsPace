cmake_minimum_required(VERSION 4.1)
project(SnailsPace C)

set(CMAKE_C_STANDARD 23)

#útvonalak
set(SDL2_PATH "C:/libs/SDL2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "C:/libs/SDL2_image/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "C:/libs/SDL2_ttf/x86_64-w64-mingw32")
set(SDL2_MIXER_PATH "C:/libs/SDL2_mixer/x86_64-w64-mingw32")

#headerszök
include_directories(${SDL2_PATH}/include ${SDL2_PATH}/include/SDL2)
include_directories(${SDL2_IMAGE_PATH}/include ${SDL2_IMAGE_PATH}/include/SDL2)
include_directories(${SDL2_TTF_PATH}/include ${SDL2_TTF_PATH}/include/SDL2)
include_directories(${SDL2_MIXER_PATH}/include ${SDL2_MIXER_PATH}/include/SDL2)

#a linkernek
link_directories(${SDL2_PATH}/lib)
link_directories(${SDL2_IMAGE_PATH}/lib)
link_directories(${SDL2_TTF_PATH}/lib)
link_directories(${SDL2_MIXER_PATH}/lib)

include_directories(include)

add_executable(SnailsPace main.c include/obj_loader.h src/obj_loader.c
        include/graphics.h
        src/graphics.c
        include/camera.h
        src/camera.c
        include/player.h
        include/game_manager.h
        src/game_manager.c
        include/ui_manager.h
        src/ui_manager.c
        src/dialogue_data.c
        include/dialogue_data.h
        include/visual_novel.h
        src/visual_novel.c
        src/game_types.c
        include/sounds.h
        src/sounds.c
        src/player.c
        include/triggers.h
        src/triggers.c
        include/ai.h
        src/ai.c
        include/world_builder.h
        src/world_builder.c
)

#összekapcsolás
target_link_libraries(SnailsPace mingw32 SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer opengl32 advapi32)