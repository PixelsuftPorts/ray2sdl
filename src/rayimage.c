#include <raydef.h>
#include <raylib.h>
#include <rayconf.h>
#ifdef IMG_SUPPORT
#include <SDL2/SDL_image.h>
#else
#define IMG_GetError SDL_GetError
#endif

Image GetDummyImage() {
    Image result = { 0 };
    return result;
}

RLCAPI Image LoadImage(const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyImage();
    }
#ifdef IMG_SUPPORT
    SDL_Surface* surf = IMG_Load(fileName);
#else
    SDL_Surface* surf = SDL_LoadBMP(fileName);
#endif
}

RLCAPI Image LoadImageRaw(const char *fileName, int width, int height, int format, int headerSize) {}

RLCAPI Image LoadImageAnim(const char *fileName, int *frames) {}

RLCAPI Image LoadImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize) {}

RLCAPI Image LoadImageFromTexture(Texture2D texture) {}

RLCAPI Image LoadImageFromScreen(void) {}

RLCAPI bool IsImageReady(Image image) {
    return false;
}

RLCAPI void UnloadImage(Image image) {}

RLCAPI bool ExportImage(Image image, const char *fileName) {
    return false;
}

RLCAPI bool ExportImageAsCode(Image image, const char *fileName) {
    return false;
}
