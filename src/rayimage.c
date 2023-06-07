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
    Image result = { .surf = surf, .format = surf->format->format,
     .mipmaps = 1, .width = surf->w, .height = surf->h };
    return result;
}

RLCAPI Image LoadImageRaw(const char *fileName, int width, int height, int format, int headerSize) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyImage();
    }
    // TODO
    return GetDummyImage();
}

RLCAPI Image LoadImageAnim(const char *fileName, int *frames) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyImage();
    }
    return GetDummyImage();
}

RLCAPI Image LoadImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize) {
    // TODO
    return GetDummyImage();
}

RLCAPI Image LoadImageFromTexture(Texture2D texture) {
    return GetDummyImage();
}

RLCAPI Image LoadImageFromScreen(void) {
    int w = GetRenderWidth();
    int h = GetRenderHeight();
    SDL_Surface* surf = SDL_CreateRGBSurface(0, w, h, DEFAULT_BPP, 0, 0, 0, 0);
    if (surf == NULL) {
        CREATE_SURF_WARN();
        return GetDummyImage();
    }
    if (SDL_LockSurface(surf) < 0)
        TRACELOG(LOG_WARNING, "Failed to lock surface (%s)", SDL_GetError());
    if (SDL_RenderReadPixels(rl.r, NULL, DRAW_RGB_FORMAT, surf->pixels, surf->pitch) < 0) {
        TRACELOG(LOG_WARNING, "Failed to read screen pixels (%s)", SDL_GetError());
        SDL_UnlockSurface(surf);
        SDL_FreeSurface(surf);
        return GetDummyImage();
    }
    SDL_UnlockSurface(surf);
    Image result = { .surf = surf, .format = surf->format->format,
     .mipmaps = 1, .width = w, .height = h };
    return result;
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
        return false;
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
    if (fileName == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return false;
}

RLCAPI Image GenImageColor(int width, int height, Color color) {
    int pixel_format = color.a >= 255 ? DRAW_RGB_FORMAT : DRAW_RGBA_FORMAT;
    int bpp = DEFAULT_BPP;
    Uint32 r_m, g_m, b_m, a_m;
    if (SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r_m, &g_m, &b_m, &a_m) == SDL_FALSE) {
        GET_COLOR_MASK_WARN();
        return GetDummyImage();
    }
    SDL_Surface* surf = SDL_CreateRGBSurface(0, width, height, bpp, r_m, g_m, b_m, a_m);
    if (surf == NULL) {
        CREATE_SURF_WARN();
        return GetDummyImage();
    }
    SDL_PixelFormat* format = SDL_AllocFormat(pixel_format);
    if (format == NULL)
        TRACELOG(LOG_WARNING, "Failed to allocate pixel format (%s)", SDL_GetError());
    if (SDL_FillRect(surf, NULL, SDL_MapRGBA(format, color.r, color.g, color.b, color.a)) < 0)
        TRACELOG(LOG_WARNING, "Failed to fill surface (%s)", SDL_GetError());
    Image result = { .surf = surf, .format = pixel_format,
     .mipmaps = 1, .width = width, .height = height };
    return result;
}

RLCAPI Image GenImageGradientV(int width, int height, Color top, Color bottom) {
    return GetDummyImage();
}

RLCAPI Image GenImageGradientH(int width, int height, Color left, Color right) {
    return GetDummyImage();
}

RLCAPI Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer) {
    return GetDummyImage();
}

RLCAPI Image GenImageChecked(int width, int height, int checksX, int checksY, Color col1, Color col2) {
    return GetDummyImage();
}

RLCAPI Image GenImageWhiteNoise(int width, int height, float factor) {
    return GetDummyImage();
}

RLCAPI Image GenImagePerlinNoise(int width, int height, int offsetX, int offsetY, float scale) {
    return GetDummyImage();
}

RLCAPI Image GenImageCellular(int width, int height, int tileSize) {
    return GetDummyImage();
}

RLCAPI Image GenImageText(int width, int height, const char *text) {
    return GetDummyImage();
}
