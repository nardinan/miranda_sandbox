cmake_minimum_required(VERSION 3.9)
project("sandbox")
execute_process(COMMAND sdl2-config --libs 2>/dev/null OUTPUT_VARIABLE STATUS_SDL_LIBRARY)
execute_process(COMMAND sdl2-config --cflags 2>/dev/null OUTPUT_VARIABLE STATUS_SDL_INCLUDE)
string(REGEX REPLACE "\n$" "" STATUS_SDL_LIBRARY "${STATUS_SDL_LIBRARY}")
string(REGEX REPLACE "\n$" "" STATUS_SDL_INCLUDE "${STATUS_SDL_INCLUDE}")

add_executable(sandbox
        game_object.obj.c
        game_object.obj.h
        camera_crafter.obj.c
        camera_crafter.obj.h
        loader.obj.c
        loader.obj.h
        miranda.c
        miranda.h
        sandbox.c)

target_compile_options(sandbox PRIVATE "-Wunused-variable")

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer)
PKG_SEARCH_MODULE(SDL2GFX REQUIRED SDL2_gfx)

INCLUDE_DIRECTORIES(${STATUS_SDL_INCLUDE} ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS} ${SDL2GFX_INCLUDE_DIRS} /usr/local/include)

target_link_libraries(sandbox ${STATUS_SDL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${SDL2GFX_LIBRARIES}
        pthread
        miranda_ground miranda_objects
        miranda_geometry miranda_io
        miranda_network miranda_media
        miranda_cameras miranda_ui
        m)


# We need to transfer the 'data' directory into the executable file (symbolic link)
set (source "${CMAKE_SOURCE_DIR}/data")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/data")
add_custom_command(
        TARGET sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "symbolic link resources folder from ${source} to ${destination}"
)
