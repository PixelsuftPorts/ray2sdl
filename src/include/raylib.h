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
RLAPI void SetWindowIcon(Image image);
RLAPI void SetWindowIcons(Image *images, int count);
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

RLAPI void ClearBackground(Color color);
RLAPI void BeginDrawing(void);
RLAPI void EndDrawing(void);
RLAPI void BeginMode2D(Camera2D camera);
RLAPI void UpdateCameraTexture();
RLAPI void EndMode2D(void);

RLAPI Matrix GetCameraMatrix2D(Camera2D camera);
RLAPI Vector2 GetScreenToWorld2D(Vector2 position, Camera2D camera);
RLAPI Vector2 GetWorldToScreen2D(Vector2 position, Camera2D camera);

RLAPI void ClockReset(void);
RLAPI void ClockStep(void);
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
RLAPI void DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color);
RLAPI void DrawRectangleGradientV(int posX, int posY, int width, int height, Color color1, Color color2);
RLAPI void DrawRectangleGradientH(int posX, int posY, int width, int height, Color color1, Color color2);
RLAPI void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4);
RLAPI void DrawRectangleLines(int posX, int posY, int width, int height, Color color);
RLAPI void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color);
RLAPI void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);
RLAPI void DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, float lineThick, Color color);
RLAPI void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
RLAPI void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
RLAPI void DrawTriangleFan(Vector2 *points, int pointCount, Color color);
RLAPI void DrawTriangleStrip(Vector2 *points, int pointCount, Color color);
RLAPI void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);
RLAPI void DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color);
RLAPI void DrawPolyLinesEx(Vector2 center, int sides, float radius, float rotation, float lineThick, Color color);

RLAPI bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2);
RLAPI bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2);
RLAPI bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec);
RLAPI bool CheckCollisionPointRec(Vector2 point, Rectangle rec);
RLAPI bool CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius);
RLAPI bool CheckCollisionPointTriangle(Vector2 point, Vector2 p1, Vector2 p2, Vector2 p3);
RLAPI bool CheckCollisionPointPoly(Vector2 point, Vector2 *points, int pointCount);
RLAPI bool CheckCollisionLines(Vector2 startPos1, Vector2 endPos1, Vector2 startPos2, Vector2 endPos2, Vector2 *collisionPoint);
RLAPI bool CheckCollisionPointLine(Vector2 point, Vector2 p1, Vector2 p2, int threshold);
RLAPI Rectangle GetCollisionRec(Rectangle rec1, Rectangle rec2);

RLAPI Image LoadImage(const char *fileName);
RLAPI Image LoadImageRaw(const char *fileName, int width, int height, int format, int headerSize);
RLAPI Image LoadImageAnim(const char *fileName, int *frames);
RLAPI Image LoadImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize);
RLAPI Image LoadImageFromTexture(Texture2D texture);
RLAPI Image LoadImageFromScreen(void);
RLAPI bool IsImageReady(Image image);
RLAPI void UnloadImage(Image image);
RLAPI bool ExportImage(Image image, const char *fileName);
RLAPI bool ExportImageAsCode(Image image, const char *fileName);

RLAPI Image GenImageColor(int width, int height, Color color);
RLAPI Image GenImageGradientV(int width, int height, Color top, Color bottom);
RLAPI Image GenImageGradientH(int width, int height, Color left, Color right);
RLAPI Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer);
RLAPI Image GenImageChecked(int width, int height, int checksX, int checksY, Color col1, Color col2);
RLAPI Image GenImageWhiteNoise(int width, int height, float factor);
RLAPI Image GenImagePerlinNoise(int width, int height, int offsetX, int offsetY, float scale);
RLAPI Image GenImageCellular(int width, int height, int tileSize);
RLAPI Image GenImageText(int width, int height, const char *text);

RLAPI Image ImageCopy(Image image);
RLAPI Image ImageFromImage(Image image, Rectangle rec);
RLAPI Image ImageText(const char *text, int fontSize, Color color);
RLAPI Image ImageTextEx(Font font, const char *text, float fontSize, float spacing, Color tint);
RLAPI void ImageFormat(Image *image, int newFormat);
RLAPI void ImageToPOT(Image *image, Color fill);
RLAPI void ImageCrop(Image *image, Rectangle crop);
RLAPI void ImageAlphaCrop(Image *image, float threshold);
RLAPI void ImageAlphaClear(Image *image, Color color, float threshold);
RLAPI void ImageAlphaMask(Image *image, Image alphaMask);
RLAPI void ImageAlphaPremultiply(Image *image);
RLAPI void ImageBlurGaussian(Image *image, int blurSize);
RLAPI void ImageResize(Image *image, int newWidth, int newHeight);
RLAPI void ImageResizeNN(Image *image, int newWidth,int newHeight);
RLAPI void ImageResizeCanvas(Image *image, int newWidth, int newHeight, int offsetX, int offsetY, Color fill);
RLAPI void ImageMipmaps(Image *image);
RLAPI void ImageDither(Image *image, int rBpp, int gBpp, int bBpp, int aBpp);
RLAPI void ImageFlipVertical(Image *image);
RLAPI void ImageFlipHorizontal(Image *image);
RLAPI void ImageRotateCW(Image *image);
RLAPI void ImageRotateCCW(Image *image);
RLAPI void ImageColorTint(Image *image, Color color);
RLAPI void ImageColorInvert(Image *image);
RLAPI void ImageColorGrayscale(Image *image);
RLAPI void ImageColorContrast(Image *image, float contrast);
RLAPI void ImageColorBrightness(Image *image, int brightness);
RLAPI void ImageColorReplace(Image *image, Color color, Color replace);
RLAPI Color *LoadImageColors(Image image);
RLAPI Color *LoadImagePalette(Image image, int maxPaletteSize, int *colorCount);
RLAPI void UnloadImageColors(Color *colors);
RLAPI void UnloadImagePalette(Color *colors);
RLAPI Rectangle GetImageAlphaBorder(Image image, float threshold);
RLAPI Color GetImageColor(Image image, int x, int y);

RLAPI void ImageClearBackground(Image *dst, Color color);
RLAPI void ImageDrawPixel(Image *dst, int posX, int posY, Color color);
RLAPI void ImageDrawPixelV(Image *dst, Vector2 position, Color color);
RLAPI void ImageDrawLine(Image *dst, int startPosX, int startPosY, int endPosX, int endPosY, Color color);
RLAPI void ImageDrawLineV(Image *dst, Vector2 start, Vector2 end, Color color);
RLAPI void ImageDrawCircle(Image *dst, int centerX, int centerY, int radius, Color color);
RLAPI void ImageDrawCircleV(Image *dst, Vector2 center, int radius, Color color);
RLAPI void ImageDrawCircleLines(Image *dst, int centerX, int centerY, int radius, Color color);
RLAPI void ImageDrawCircleLinesV(Image *dst, Vector2 center, int radius, Color color);
RLAPI void ImageDrawRectangle(Image *dst, int posX, int posY, int width, int height, Color color);
RLAPI void ImageDrawRectangleV(Image *dst, Vector2 position, Vector2 size, Color color);
RLAPI void ImageDrawRectangleRec(Image *dst, Rectangle rec, Color color);
RLAPI void ImageDrawRectangleLines(Image *dst, Rectangle rec, int thick, Color color);
RLAPI void ImageDraw(Image *dst, Image src, Rectangle srcRec, Rectangle dstRec, Color tint);
RLAPI void ImageDrawText(Image *dst, const char *text, int posX, int posY, int fontSize, Color color);
RLAPI void ImageDrawTextEx(Image *dst, Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);

RLAPI Texture2D LoadTexture(const char *fileName);
RLAPI Texture2D LoadTextureFromImage(Image image);
RLAPI RenderTexture2D LoadRenderTexture(int width, int height);
RLAPI bool IsTextureReady(Texture2D texture);
RLAPI void UnloadTexture(Texture2D texture);
RLAPI bool IsRenderTextureReady(RenderTexture2D target);
RLAPI void UnloadRenderTexture(RenderTexture2D target);
RLAPI void UpdateTexture(Texture2D texture, const void *pixels);
RLAPI void UpdateTextureRec(Texture2D texture, Rectangle rec, const void *pixels);

RLAPI void GenTextureMipmaps(Texture2D *texture);
RLAPI void SetTextureFilter(Texture2D texture, int filter);
RLAPI void SetTextureWrap(Texture2D texture, int wrap);

RLAPI void DrawTexture(Texture2D texture, int posX, int posY, Color tint);
RLAPI void DrawTextureV(Texture2D texture, Vector2 position, Color tint);
RLAPI void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
RLAPI void DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);
RLAPI void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);
RLAPI void DrawTextureNPatch(Texture2D texture, NPatchInfo nPatchInfo, Rectangle dest, Vector2 origin, float rotation, Color tint);

RLAPI Color Fade(Color color, float alpha);
RLAPI int ColorToInt(Color color);
RLAPI Vector4 ColorNormalize(Color color);
RLAPI Color ColorFromNormalized(Vector4 normalized);
RLAPI Vector3 ColorToHSV(Color color);
RLAPI Color ColorFromHSV(float hue, float saturation, float value);
RLAPI Color ColorTint(Color color, Color tint);
RLAPI Color ColorBrightness(Color color, float factor);
RLAPI Color ColorContrast(Color color, float contrast);
RLAPI Color ColorAlpha(Color color, float alpha);
RLAPI Color ColorAlphaBlend(Color dst, Color src, Color tint);
RLAPI Color GetColor(unsigned int hexValue);
RLAPI Color GetPixelColor(void *srcPtr, int format);
RLAPI void SetPixelColor(void *dstPtr, Color color, int format);
RLAPI int GetPixelDataSize(int width, int height, int format);

RLAPI char *LoadUTF8(const int *codepoints, int length);
RLAPI void UnloadUTF8(char *text);
RLAPI int *LoadCodepoints(const char *text, int *count);
RLAPI void UnloadCodepoints(int *codepoints);
RLAPI int GetCodepointCount(const char *text);
RLAPI int GetCodepoint(const char *text, int *codepointSize);
RLAPI int GetCodepointNext(const char *text, int *codepointSize);
RLAPI int GetCodepointPrevious(const char *text, int *codepointSize);
RLAPI const char *CodepointToUTF8(int codepoint, int *utf8Size);

#if defined(__cplusplus)
}
#endif
