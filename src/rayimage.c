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

RLCAPI void SetWindowIcon(Image image) {
    if (image.surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_SetWindowIcon(rl.w, image.surf);
}

RLCAPI void SetWindowIcons(Image *images, int count) {
    SDL_Surface* nice_img = NULL;
    for (int i = 0; i < count; i++) {
        SDL_Surface* maybe_nice_img = images[i].surf;
        if (maybe_nice_img && (
            nice_img == NULL || (maybe_nice_img->w > nice_img->w && maybe_nice_img->h > nice_img->h)
        ))
            nice_img = maybe_nice_img;
    }
    if (nice_img == NULL) {
        TRACELOG(LOG_WARNING, "No images found");
        return;
    }
    SDL_SetWindowIcon(rl.w, nice_img);
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
    if (surf == NULL) {
        TRACELOG(LOG_WARNING, "Failed to load image (%s)", IMG_GetError());
        return GetDummyImage();
    }
    // TODO: format
    Image result = { .surf = surf, .format = 0, .mipmaps = 1, .width = surf->w, .height = surf->h };
    return result;
}

RLCAPI Image LoadImageRaw(const char *fileName, int width, int height, int format, int headerSize) {
    return GetDummyImage();
}

RLCAPI Image LoadImageAnim(const char *fileName, int *frames) {
    return GetDummyImage();
}

RLCAPI Image LoadImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize) {
    return GetDummyImage();
}

RLCAPI Image LoadImageFromTexture(Texture2D texture) {
    return GetDummyImage();
}

RLCAPI Image LoadImageFromScreen(void) {
    return GetDummyImage();
}

RLCAPI bool IsImageReady(Image image) {
    return (bool)image.surf;
}

RLCAPI void UnloadImage(Image image) {
    if (image.surf == NULL) {
        TRACELOG(LOG_WARNING, "Attempt to unload not loaded image");
        return;
    }
    SDL_FreeSurface(image.surf);
    image.surf = NULL;
}

RLCAPI bool ExportImage(Image image, const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
    }
#ifdef IMG_SUPPORT
    if (IsFileExtension(fileName, ".png")) {
        if (IMG_SavePNG(image.surf, fileName) < 0) {
            TRACELOG(LOG_WARNING, "Failed to save PNG image (%s)", IMG_GetError());
            return false;
        }
        return true;
    }
    if (IsFileExtension(fileName, ".jpg") || IsFileExtension(fileName, ".jpeg")) {
        if (IMG_SaveJPG(image.surf, fileName, 90) < 0) { // Quality from original
            TRACELOG(LOG_WARNING, "Failed to save JPG image (%s)", IMG_GetError());
            return false;
        }
        return true;
    }
#endif
    if (IsFileExtension(fileName, ".bmp")) {
        if (SDL_SaveBMP(image.surf, fileName) < 0) {
            TRACELOG(LOG_WARNING, "Failed to save BMP image (%s)", SDL_GetError());
            return false;
        }
        return true;
    }
    TRACELOG(LOG_WARNING, "Unsupported format %s for file %s", GetFileExtension(fileName), fileName);
    return false;
}

RLCAPI bool ExportImageAsCode(Image image, const char *fileName) {
    return false;
}
