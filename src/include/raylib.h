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

RLAPI void ShowCursor(void);
RLAPI void HideCursor(void);
RLAPI bool IsCursorHidden(void);
RLAPI void EnableCursor(void);
RLAPI void DisableCursor(void);
RLAPI bool IsCursorOnScreen(void);

RLAPI void SetTargetFPS(int fps);
RLAPI int GetFPS(void);
RLAPI float GetFrameTime(void);
RLAPI double GetTime(void);

RLAPI int GetRandomValue(int min, int max);
RLAPI void SetRandomSeed(unsigned int seed);
RLAPI void TakeScreenshot(const char *fileName);
RLAPI void SetConfigFlags(unsigned int flags);

RLAPI void TraceLog(int logLevel, const char *text, ...);
RLAPI void SetTraceLogLevel(int logLevel);
RLAPI void *MemAlloc(unsigned int size);
RLAPI void *MemRealloc(void *ptr, unsigned int size);
RLAPI void MemFree(void *ptr);
RLAPI void OpenURL(const char *url);

RLAPI void SetTraceLogCallback(TraceLogCallback callback);
RLAPI void SetLoadFileDataCallback(LoadFileDataCallback callback);
RLAPI void SetSaveFileDataCallback(SaveFileDataCallback callback);
RLAPI void SetLoadFileTextCallback(LoadFileTextCallback callback);
RLAPI void SetSaveFileTextCallback(SaveFileTextCallback callback); 

RLAPI unsigned char *LoadFileData(const char *fileName, unsigned int *bytesRead);
RLAPI void UnloadFileData(unsigned char *data);
RLAPI bool SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite);
RLAPI bool ExportDataAsCode(const unsigned char *data, unsigned int size, const char *fileName);
RLAPI char *LoadFileText(const char *fileName);
RLAPI void UnloadFileText(char *text);
RLAPI bool SaveFileText(const char *fileName, char *text);
RLAPI bool FileExists(const char *fileName);
RLAPI bool DirectoryExists(const char *dirPath);
RLAPI bool IsFileExtension(const char *fileName, const char *ext);
RLAPI const char *GetFileExtension(const char *fileName);
RLAPI const char *GetFileName(const char *filePath);
RLAPI const char *GetFileNameWithoutExt(const char *filePath);
RLAPI const char *GetDirectoryPath(const char *filePath);
RLAPI const char *GetPrevDirectoryPath(const char *dirPath);
RLAPI const char *GetWorkingDirectory(void);
RLAPI const char *GetApplicationDirectory(void);
RLAPI bool ChangeDirectory(const char *dir);
RLAPI bool IsPathFile(const char *path);

RLAPI void ClearBackground(Color color);
RLAPI void BeginDrawing(void);
RLAPI void EndDrawing(void); 

#if defined(__cplusplus)
}
#endif
