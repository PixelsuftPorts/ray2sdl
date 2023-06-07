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
#ifdef ENABLE_IMAGE_RLE
    if (SDL_SetSurfaceRLE(surf, 1) < 0)
        SET_RLE_WARN();
#endif
    Image result = { .surf = surf, .format = surf->format->format,
     .mipmaps = 1, .width = surf->w, .height = surf->h, .data = surf->pixels };
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
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, DEFAULT_BPP, DRAW_RGB_FORMAT);
    if (surf == NULL) {
        CREATE_SURF_WARN();
        return GetDummyImage();
    }
#ifdef ENABLE_IMAGE_RLE
    if (SDL_SetSurfaceRLE(surf, 1) < 0)
        SET_RLE_WARN();
#endif
    if (SDL_MUSTLOCK(surf) && SDL_LockSurface(surf) < 0)
        TRACELOG(LOG_WARNING, "Failed to lock surface (%s)", SDL_GetError());
    if (SDL_RenderReadPixels(rl.r, NULL, DRAW_RGB_FORMAT, surf->pixels, surf->pitch) < 0) {
        TRACELOG(LOG_WARNING, "Failed to read screen pixels (%s)", SDL_GetError());
        SDL_UnlockSurface(surf);
        SDL_FreeSurface(surf);
        return GetDummyImage();
    }
    SDL_UnlockSurface(surf);
    Image result = { .surf = surf, .format = surf->format->format,
     .mipmaps = 1, .width = w, .height = h, .data = surf->pixels };
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
    if (image.surf == NULL || fileName == NULL) {
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
    if (image.surf == NULL || fileName == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return false;
}

RLCAPI Image GenImageColor(int width, int height, Color color) {
    Uint32 pixel_format = color.a >= 255 ? DRAW_RGB_FORMAT : DRAW_RGBA_FORMAT;
    int bpp = DEFAULT_BPP;
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, width, height, bpp, pixel_format);
    if (surf == NULL) {
        CREATE_SURF_WARN();
        return GetDummyImage();
    }
#ifdef ENABLE_IMAGE_RLE
    if (SDL_SetSurfaceRLE(surf, 1) < 0)
        SET_RLE_WARN();
#endif
    SDL_PixelFormat* format = SDL_AllocFormat(pixel_format);
    if (format == NULL)
        TRACELOG(LOG_WARNING, "Failed to allocate pixel format (%s)", SDL_GetError());
    if (SDL_FillRect(surf, NULL, SDL_MapRGBA(format, color.r, color.g, color.b, color.a)) < 0)
        TRACELOG(LOG_WARNING, "Failed to fill surface (%s)", SDL_GetError());
    Image result = { .surf = surf, .format = pixel_format,
     .mipmaps = 1, .width = width, .height = height, .data = surf->pixels };
    return result;
}

RLCAPI Image GenImageGradientV(int width, int height, Color top, Color bottom) {
    // TODO
    return GenImageColor(width, height, top);
}

RLCAPI Image GenImageGradientH(int width, int height, Color left, Color right) {
    return GenImageColor(width, height, left);
}

RLCAPI Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer) {
    return GenImageColor(width, height, inner);
}

RLCAPI Image GenImageChecked(int width, int height, int checksX, int checksY, Color col1, Color col2) {
    return GenImageColor(width, height, col1);
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

RLCAPI Image ImageCopy(Image image) {
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(
        0,
        image.width,
        image.height,
        (image.surf->pitch << 3) / image.width,
        image.surf->format->format
    );
    if (surf == NULL) {
        CREATE_SURF_WARN();
        return GetDummyImage();
    }
#ifdef ENABLE_IMAGE_RLE
    if (SDL_SetSurfaceRLE(surf, 1) < 0)
        SET_RLE_WARN();
#endif
    if (SDL_MUSTLOCK(surf) && SDL_LockSurface(surf) < 0)
        TRACELOG(LOG_WARNING, "Failed to lock surface (%s)", SDL_GetError());
    SDL_memcpy(surf->pixels, image.surf->pixels, image.surf->pitch * image.height);
    SDL_UnlockSurface(surf);
    image.surf = surf;
    image.data = surf->pixels;
    return image;
}

RLCAPI Image ImageFromImage(Image image, Rectangle rec) {
    SDL_Rect src_rect = { (int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height };
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(
        0,
        src_rect.w,
        src_rect.h,
        (image.surf->pitch << 3) / image.width,
        image.surf->format->format
    );
    if (surf == NULL) {
        CREATE_SURF_WARN();
        return GetDummyImage();
    }
#ifdef ENABLE_IMAGE_RLE
    if (SDL_SetSurfaceRLE(surf, 1) < 0)
        SET_RLE_WARN();
#endif
    if (SDL_BlitSurface(image.surf, &src_rect, surf, NULL) < 0)
        TRACELOG(LOG_WARNING, "Failed to blit surface (%s)", SDL_GetError());
    image.width = src_rect.w;
    image.height = src_rect.h;
    image.surf = surf;
    image.data = surf->pixels;
    return image;
}

RLCAPI Image ImageText(const char *text, int fontSize, Color color) {
    return GetDummyImage();
}

RLCAPI Image ImageTextEx(Font font, const char *text, float fontSize, float spacing, Color tint) {
    return GetDummyImage();
}

RLCAPI void ImageFormat(Image *image, int newFormat) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_Surface* surf = SDL_ConvertSurfaceFormat(image->surf, (Uint32)newFormat, 0);
    if (surf == NULL) {
        TRACELOG(LOG_WARNING, "Failed to convert surface (%s)", SDL_GetError());
        return;
    }
    SDL_FreeSurface(image->surf);
    image->surf = surf;
    image->data = surf->pixels;
}

RLCAPI void ImageToPOT(Image *image, Color fill) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    // TODO
}

RLCAPI void ImageCrop(Image *image, Rectangle crop) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_Rect src_rect = { (int)crop.x, (int)crop.y, (int)crop.width, (int)crop.height };
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(
        0,
        src_rect.w,
        src_rect.h,
        (image->surf->pitch << 3) / image->width,
        image->surf->format->format
    );
    if (surf == NULL) {
        CREATE_SURF_WARN();
        return;
    }
#ifdef ENABLE_IMAGE_RLE
    if (SDL_SetSurfaceRLE(surf, 1) < 0)
        SET_RLE_WARN();
#endif
    if (SDL_BlitSurface(image->surf, &src_rect, surf, NULL) < 0)
        TRACELOG(LOG_WARNING, "Failed to blit surface (%s)", SDL_GetError());
    image->width = src_rect.w;
    image->height = src_rect.h;
    image->data = surf->pixels;
    SDL_FreeSurface(image->surf);
    image->surf = surf;
}

RLCAPI void ImageAlphaCrop(Image *image, float threshold) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    Rectangle crop = GetImageAlphaBorder(*image, threshold);
    if ((bool)crop.width && (bool)crop.height)
        ImageCrop(image, crop);
}

RLCAPI void ImageAlphaClear(Image *image, Color color, float threshold) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    // TODO
}

RLCAPI void ImageAlphaMask(Image *image, Image alphaMask) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageAlphaPremultiply(Image *image) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageBlurGaussian(Image *image, int blurSize) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageResize(Image *image, int newWidth, int newHeight) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(
        0,
        newWidth,
        newHeight,
        (image->surf->pitch << 3) / image->width,
        image->surf->format->format
    );
    if (surf == NULL) {
        CREATE_SURF_WARN();
        return;
    }
#ifdef ENABLE_IMAGE_RLE
    if (SDL_SetSurfaceRLE(surf, 1) < 0)
        SET_RLE_WARN();
#endif
    if (SDL_BlitScaled(image->surf, NULL, surf, NULL) < 0)
        TRACELOG(LOG_WARNING, "Failed to blit surface (%s)", SDL_GetError());
    image->width = newWidth;
    image->height = newHeight;
    image->data = surf->pixels;
    SDL_FreeSurface(image->surf);
    image->surf = surf;
}

RLCAPI void ImageResizeNN(Image *image, int newWidth, int newHeight) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    ImageResize(image, newWidth, newHeight);
}

RLCAPI void ImageResizeCanvas(Image *image, int newWidth, int newHeight, int offsetX, int offsetY, Color fill) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(
        0,
        newWidth,
        newHeight,
        (image->surf->pitch << 3) / image->width,
        image->surf->format->format
    );
    if (surf == NULL) {
        CREATE_SURF_WARN();
        return;
    }
#ifdef ENABLE_IMAGE_RLE
    if (SDL_SetSurfaceRLE(surf, 1) < 0)
        SET_RLE_WARN();
#endif
    // TODO: fill image with color
    if (SDL_BlitSurface(image->surf, NULL, surf, NULL) < 0)
        TRACELOG(LOG_WARNING, "Failed to blit surface (%s)", SDL_GetError());
    image->width = newWidth;
    image->height = newHeight;
    image->data = surf->pixels;
    SDL_FreeSurface(image->surf);
    image->surf = surf;
}

RLCAPI void ImageMipmaps(Image *image) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageDither(Image *image, int rBpp, int gBpp, int bBpp, int aBpp) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    ImageFormat(image, PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA);
}

RLCAPI void ImageFlipVertical(Image *image) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    if (SDL_MUSTLOCK(image->surf) && SDL_LockSurface(image->surf) < 0)
        TRACELOG(LOG_WARNING, "Failed to lock surface (%s)", SDL_GetError());
    unsigned char temp[image->surf->pitch];
    for (int i = 0; i < image->surf->h / 2; i++) {
        void* row1 = image->surf->pixels + i * image->surf->pitch;
        void* row2 = image->surf->pixels + (image->surf->h - i - 1) * image->surf->pitch;
        SDL_memcpy(temp, row1, image->surf->pitch);
        SDL_memcpy(row1, row2, image->surf->pitch);
        SDL_memcpy(row2, temp, image->surf->pitch);
    }
    SDL_UnlockSurface(image->surf);
}

RLCAPI void ImageFlipHorizontal(Image *image) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageRotateCW(Image *image) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageRotateCCW(Image *image) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageColorTint(Image *image, Color color) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageColorInvert(Image *image) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    if (SDL_MUSTLOCK(image->surf) && SDL_LockSurface(image->surf) < 0)
        TRACELOG(LOG_WARNING, "Failed to lock surface (%s)", SDL_GetError());
    for (int i = 0; i < image->surf->pitch * image->height; i++) {
        if (image->surf->format->BytesPerPixel == 4 && (i % 4 != 3)) // FIXME: Will work only with RGB(A)
            ((unsigned char*)image->surf->pixels)[i] = 255 - ((unsigned char*)image->surf->pixels)[i];
    }
    SDL_UnlockSurface(image->surf);
}

RLCAPI void ImageColorGrayscale(Image *image) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    ImageFormat(image, PIXELFORMAT_UNCOMPRESSED_GRAYSCALE);
}

RLCAPI void ImageColorContrast(Image *image, float contrast) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageColorBrightness(Image *image, int brightness) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void ImageColorReplace(Image *image, Color color, Color replace) {
    if (image == NULL || image->surf == NULL) {
        NULLPTR_WARN();
        return;
    }
    for (int i = 0; i < image->surf->pitch * image->height; i += image->surf->format->BytesPerPixel) {
        if (!SDL_memcmp(&image->surf->pixels[i], &color, image->surf->format->BytesPerPixel))
            SDL_memcpy(&image->surf->pixels[i], &replace, image->surf->format->BytesPerPixel);
    }
}

RLCAPI Color *LoadImageColors(Image image) {}

RLCAPI Color *LoadImagePalette(Image image, int maxPaletteSize, int *colorCount) {}

RLCAPI void UnloadImageColors(Color *colors) {}

RLCAPI void UnloadImagePalette(Color *colors) {}

RLCAPI Rectangle GetImageAlphaBorder(Image image, float threshold) {}

RLCAPI Color GetImageColor(Image image, int x, int y) {}
