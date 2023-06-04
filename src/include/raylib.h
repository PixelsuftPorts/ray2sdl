#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <raydef.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
What is not done?
cameras and modes, All 3D, fps clock, shaders,
gestures, gamecontrollers (I don't have them lol) and some other things
*/

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
RLAPI FilePathList LoadDirectoryFiles(const char *dirPath);
RLAPI FilePathList LoadDirectoryFilesEx(const char *basePath, const char *filter, bool scanSubdirs);
RLAPI void UnloadDirectoryFiles(FilePathList files);
RLAPI void RegisterFileDrop(char* fp);
RLAPI bool IsFileDropped(void);
RLAPI FilePathList LoadDroppedFiles(void);
RLAPI void UnloadDroppedFiles(FilePathList files);
RLAPI long GetFileModTime(const char *fileName);

RLAPI unsigned char *CompressData(const unsigned char *data, int dataSize, int *compDataSize);
RLAPI unsigned char *DecompressData(const unsigned char *compData, int compDataSize, int *dataSize);
RLAPI char *EncodeDataBase64(const unsigned char *data, int dataSize, int *outputSize);
RLAPI unsigned char *DecodeDataBase64(const unsigned char *data, int *outputSize);

RLAPI bool IsKeyPressed(int key);
RLAPI bool IsKeyDown(int key);
RLAPI bool IsKeyReleased(int key);
RLAPI bool IsKeyUp(int key);
RLAPI void SetExitKey(int key);
RLAPI int GetKeyPressed(void);
RLAPI int GetCharPressed(void);

RLAPI bool IsMouseButtonPressed(int button);
RLAPI bool IsMouseButtonDown(int button);
RLAPI bool IsMouseButtonReleased(int button);
RLAPI bool IsMouseButtonUp(int button);
RLAPI int GetMouseX(void);
RLAPI int GetMouseY(void);
RLAPI Vector2 GetMousePosition(void);
RLAPI Vector2 GetMouseDelta(void);
RLAPI void SetMousePosition(int x, int y);
RLAPI void SetMouseOffset(int offsetX, int offsetY);
RLAPI void SetMouseScale(float scaleX, float scaleY);
RLAPI float GetMouseWheelMove(void);
RLAPI Vector2 GetMouseWheelMoveV(void);
RLAPI void SetMouseCursor(int cursor);

RLAPI int GetTouchX(void);
RLAPI int GetTouchY(void);
RLAPI Vector2 GetTouchPosition(int index);
RLAPI int GetTouchPointId(int index);
RLAPI int GetTouchPointCount(void);

RLAPI void DrawPixel(int posX, int posY, Color color);
RLAPI void DrawPixelV(Vector2 position, Color color);
RLAPI void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);
RLAPI void DrawLineV(Vector2 startPos, Vector2 endPos, Color color);
RLAPI void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);
// TODO: Beizer Curves
RLAPI void DrawLineStrip(Vector2 *points, int pointCount, Color color);
RLAPI void DrawCircle(int centerX, int centerY, float radius, Color color);
RLAPI void DrawCircleGradient(int centerX, int centerY, float radius, Color color1, Color color2);
RLAPI void DrawCircleSector(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color);
RLAPI void DrawCircleSectorLines(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color);
RLAPI void DrawCircleV(Vector2 center, float radius, Color color);
RLAPI void DrawCircleLines(int centerX, int centerY, float radius, Color color);
RLAPI void DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);
RLAPI void DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, Color color);
RLAPI void DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color);
RLAPI void DrawRingLines(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color);
RLAPI void DrawRectangle(int posX, int posY, int width, int height, Color color);
RLAPI void DrawRectangleV(Vector2 position, Vector2 size, Color color);
RLAPI void DrawRectangleRec(Rectangle rec, Color color);

RLAPI void ClearBackground(Color color);
RLAPI void BeginDrawing(void);
RLAPI void EndDrawing(void); 

#if defined(__cplusplus)
}
#endif
