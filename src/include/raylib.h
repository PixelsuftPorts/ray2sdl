#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <raydef.h>

#if defined(__cplusplus)
extern "C" {
#endif

RLAPI void InitWindow(int width, int height, const char *title);
RLAPI bool WindowShouldClose(void);
RLAPI void CloseWindow(void);

RLAPI void ClearBackground(Color color);
RLAPI void BeginDrawing(void);
RLAPI void EndDrawing(void); 

#if defined(__cplusplus)
}
#endif
