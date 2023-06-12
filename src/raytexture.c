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
    result.id = 0;
    result.texture.tex = SDL_CreateTexture(
        rl.r, DRAW_TEXTURE_FORMAT_ALPHA, RENDER_TEXTURE_DEFAULT_ACCESS, width, height
    );
    if (result.texture.tex == NULL) {
        TRACELOG(LOG_WARNING, "Failed to create render texture (%s)", SDL_GetError());
        return result;
    }
    result.texture.width = width;
    result.texture.height = height;
    result.texture.mipmaps = 1;
    result.texture.id = GetTextureId(&result.texture);
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
    return (bool)target.texture.tex;
}

RLCAPI void UnloadRenderTexture(RenderTexture2D target) {
    if (target.texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_DestroyTexture(target.texture.tex);
    target.texture.tex = NULL;
}

RLCAPI void UpdateTexture(Texture2D texture, const void *pixels) {
    if (texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
    // Maybe re-create texture?
}

RLCAPI void UpdateTextureRec(Texture2D texture, Rectangle rec, const void *pixels) {
    if (texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void GenTextureMipmaps(Texture2D *texture) {
    if (texture == NULL || texture->tex == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void SetTextureFilter(Texture2D texture, int filter) {
    if (texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
    if (SDL_SetTextureScaleMode(texture.tex, filter) < 0)
        TRACELOG(LOG_WARNING, "Invalid texture");
}

RLCAPI void SetTextureWrap(Texture2D texture, int wrap) {
    if (texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void DrawTexture(Texture2D texture, int posX, int posY, Color tint) {
    DrawTextureV(texture, CLITERAL(Vector2){ (float)posX, (float)posY }, tint);
}

RLCAPI void DrawTextureV(Texture2D texture, Vector2 position, Color tint) {
    if (texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_FRect dst_rect = { position.x, position.y, (float)texture.width, (float)texture.height };
    if (APPLY_TEXTURE_TINT(texture.tex, tint) < 0)
        TINT_WARN();
    if (rl.z_en) {
        if (RENDER_ENABLE_SCALE() < 0)
            SCALE_WARN();
        dst_rect.x += rl.co.x;
        dst_rect.y += rl.co.y;
        if (SDL_RenderCopyF(rl.r, texture.tex, NULL, &dst_rect) < 0)
            RENDER_COPY_WARN();
        if (RENDER_DISABLE_SCALE() < 0)
            SCALE_WARN();
    }
    else if (SDL_RenderCopyF(rl.r, texture.tex, NULL, &dst_rect) < 0)
        RENDER_COPY_WARN();
}

RLCAPI void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint) {
    if (texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_FRect dst_rect = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
    SDL_FPoint center = { 0.0f, 0.0f };
    if (APPLY_TEXTURE_TINT(texture.tex, tint) < 0)
        TINT_WARN();
    if (rl.z_en) {
        if (RENDER_ENABLE_SCALE() < 0)
            SCALE_WARN();
        dst_rect.x += rl.co.x;
        dst_rect.y += rl.co.y;
        if (SDL_RenderCopyExF(rl.r, texture.tex, NULL, &dst_rect, (double)rotation, &center, SDL_FLIP_NONE) < 0)
            RENDER_COPY_WARN();
        if (RENDER_DISABLE_SCALE() < 0)
            SCALE_WARN();
    }
    else if (SDL_RenderCopyExF(rl.r, texture.tex, NULL, &dst_rect, (double)rotation, &center, SDL_FLIP_NONE) < 0)
        RENDER_COPY_WARN();
}

RLCAPI void DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint) {
    if (texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_Rect src_rect = { (int)source.x, (int)source.y, (int)source.width, (int)source.height };
    SDL_FRect dst_rect = { position.x, position.y, source.width, source.height };
    if (APPLY_TEXTURE_TINT(texture.tex, tint) < 0)
        TINT_WARN();
    if (rl.z_en) {
        if (RENDER_ENABLE_SCALE() < 0)
            SCALE_WARN();
        dst_rect.x += rl.co.x;
        dst_rect.y += rl.co.y;
        if (SDL_RenderCopyF(rl.r, texture.tex, &src_rect, &dst_rect) < 0)
            RENDER_COPY_WARN();
        if (RENDER_DISABLE_SCALE() < 0)
            SCALE_WARN();
    }
    else if (SDL_RenderCopyF(rl.r, texture.tex, &src_rect, &dst_rect) < 0)
        RENDER_COPY_WARN();
}

RLCAPI void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint) {
    if (texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
    // TODO: copy texture many times istead of scaling when dst rect > src rect
    SDL_Rect src_rect = { (int)source.x, (int)source.y, (int)source.width, (int)source.height };
    if (APPLY_TEXTURE_TINT(texture.tex, tint) < 0)
        TINT_WARN();
    if (rl.z_en) {
        if (RENDER_ENABLE_SCALE() < 0)
            SCALE_WARN();
        dest.x += rl.co.x;
        dest.y += rl.co.y;
        if (SDL_RenderCopyExF(
            rl.r, texture.tex, &src_rect, (const SDL_FRect*)&dest,
            (double)rotation, (const SDL_FPoint*)&origin, SDL_FLIP_NONE
        ) < 0)
            RENDER_COPY_WARN();
        if (RENDER_DISABLE_SCALE() < 0)
            SCALE_WARN();
    }
    else if (SDL_RenderCopyExF(
            rl.r, texture.tex, &src_rect, (const SDL_FRect*)&dest,
            (double)rotation, (const SDL_FPoint*)&origin, SDL_FLIP_NONE
        ) < 0)
        RENDER_COPY_WARN();
}

RLCAPI void DrawTextureNPatch(Texture2D texture, NPatchInfo nPatchInfo, Rectangle dest, Vector2 origin, float rotation, Color tint) {
    if (texture.tex == NULL) {
        NULLPTR_WARN();
        return;
    }
}
