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
RLAPI bool IsWindowReady(void);
RLAPI bool IsWindowFullscreen(void);
RLAPI bool IsWindowHidden(void);
RLAPI bool IsWindowMinimized(void);
RLAPI bool IsWindowMaximized(void);
RLAPI bool IsWindowFocused(void);
RLAPI bool IsWindowResized(void);
RLAPI bool IsWindowState(unsigned int flag);
RLAPI void SetWindowState(unsigned int flags);
RLAPI void ClearWindowState(unsigned int flags);
RLAPI void ToggleFullscreen(void);
RLAPI void MaximizeWindow(void);
RLAPI void MinimizeWindow(void);
RLAPI void RestoreWindow(void);
RLAPI void SetWindowTitle(const char *title);
RLAPI void SetWindowPosition(int x, int y);
RLAPI void SetWindowMonitor(int monitor);
RLAPI void SetWindowMinSize(int width, int height);
RLAPI void SetWindowSize(int width, int height);
RLAPI void SetWindowOpacity(float opacity);
RLAPI void *GetWindowHandle(void);     

RLAPI void ClearBackground(Color color);
RLAPI void BeginDrawing(void);
RLAPI void EndDrawing(void); 

#if defined(__cplusplus)
}
#endif
