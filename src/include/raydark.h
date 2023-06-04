#pragma once
#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

void* GetHandleBySDLWindow(SDL_Window* window);
void CheckDarkMode(SDL_Window* window);

#ifdef __cplusplus
}
#endif
