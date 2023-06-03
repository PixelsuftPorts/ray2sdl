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
RLAPI int GetScreenWidth(void);
RLAPI int GetScreenHeight(void);
RLAPI int GetRenderWidth(void);
RLAPI int GetRenderHeight(void);
RLAPI int GetMonitorCount(void);
RLAPI int GetCurrentMonitor(void);
RLAPI Vector2 GetMonitorPosition(int monitor);
RLAPI int GetMonitorWidth(int monitor);
RLAPI int GetMonitorHeight(int monitor);
RLAPI int GetMonitorPhysicalWidth(int monitor);
RLAPI int GetMonitorPhysicalHeight(int monitor);
RLAPI int GetMonitorRefreshRate(int monitor);
RLAPI Vector2 GetWindowPosition(void);
RLAPI Vector2 GetWindowScaleDPI(void);
RLAPI const char *GetMonitorName(int monitor);
RLAPI void SetClipboardText(const char *text);
RLAPI const char *GetClipboardText(void);
RLAPI void EnableEventWaiting(void);
RLAPI void DisableEventWaiting(void);

RLAPI void SwapScreenBuffer(void);
RLAPI void PollInputEvents(void);
RLAPI void WaitTime(double seconds); 

RLAPI void ClearBackground(Color color);
RLAPI void BeginDrawing(void);
RLAPI void EndDrawing(void); 

#if defined(__cplusplus)
}
#endif
