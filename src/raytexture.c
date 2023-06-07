#include <raydef.h>
#include <raylib.h>
#include <rayconf.h>
#ifdef IMG_SUPPORT
#include <SDL2/SDL_image.h>
#else
#define IMG_GetError SDL_GetError
#endif

// TODO:
//  - maybe check if renderer exists when trying something like destroying a texture?

int GetTextureId(Texture* texture) {
    return 0;
}

Texture GetDummyTexture() {
    Texture result = { 0 };
    return result;
}

RLCAPI Texture2D LoadTexture(const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyTexture();
    }
#ifdef IMG_SUPPORT
    SDL_Texture* tex = IMG_LoadTexture(rl.r, fileName);
#else
    SDL_Surface* surf = SDL_LoadBMP(fileName);
    if (surf == NULL) {
        TRACELOG(LOG_WARNING, "Failed to load image %s (%s)", fileName, SDL_GetError());
        return GetDummyTexture();
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rl.r, surf);
    SDL_FreeSurface(surf);
#endif
    if (tex == NULL) {
        TRACELOG(LOG_WARNING, "Failed to load texture %s (%s)", fileName, IMG_GetError());
        return GetDummyTexture();
    }
    Texture result = { .tex = tex, .mipmaps = 1 };
    Uint32 temp_format;
    if (SDL_QueryTexture(tex, &temp_format, NULL, &result.width, &result.height) < 0)
        QUERY_TEXTURE_WARN();
    result.format = (int)temp_format;
    result.id = GetTextureId(&result);
    return result;
}

RLCAPI Texture2D LoadTextureFromImage(Image image) {
    if (image.surf == NULL) {
        NULLPTR_WARN();
        return GetDummyTexture();
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rl.r, image.surf);
    if (tex == NULL) {
        TRACELOG(LOG_WARNING, "Failed to convert image into texture (%s)", IMG_GetError());
        return GetDummyTexture();
    }
    Texture result = { .tex = tex, .mipmaps = 1 };
    Uint32 temp_format;
    if (SDL_QueryTexture(tex, &temp_format, NULL, &result.width, &result.height) < 0)
        QUERY_TEXTURE_WARN();
    result.format = (int)temp_format;
    result.id = GetTextureId(&result);
    return result;
}

RLCAPI RenderTexture2D LoadRenderTexture(int width, int height) {
    RenderTexture2D result = { 0 };
    return result;
}

RLCAPI bool IsTextureReady(Texture2D texture) {
    return (bool)texture.tex;
}

RLCAPI void UnloadTexture(Texture2D texture) {
    if (texture.tex == NULL) {
        TRACELOG(LOG_WARNING, "Attempt to unload unloaded texture");
        return;
    }
    SDL_DestroyTexture(texture.tex);
    texture.tex = NULL;
}

RLCAPI bool IsRenderTextureReady(RenderTexture2D target) {
    return false;
}

RLCAPI void UnloadRenderTexture(RenderTexture2D target) {}

RLCAPI void UpdateTexture(Texture2D texture, const void *pixels) {}

RLCAPI void UpdateTextureRec(Texture2D texture, Rectangle rec, const void *pixels) {}
