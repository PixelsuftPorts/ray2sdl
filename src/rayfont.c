#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>

// TODO: support BMFont
#ifdef TTF_SUPPORT
#ifdef TTF_AA
#define TTF_RenderFunc TTF_RenderUTF8_Blended_Wrapped
#else
#define TTF_RenderFunc TTF_RenderUTF8_Solid_Wrapped
#endif
#endif

Font GetDummyFont() {
    Font result = { 0 };
    return result;
}

RLCAPI Font GetFontDefault(void) {
    if (!IsFontReady(rl.default_font))
        rl.default_font = LoadFont(DEFAULT_FONT_PATH);
    return rl.default_font;
}

RLCAPI Font LoadFont(const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyFont();
    }
#ifdef TTF_SUPPORT
    return LoadFontEx(fileName, DEFAULT_FONT_SIZE, NULL, 95);
#endif
    return GetDummyFont();
}

RLCAPI Font LoadFontEx(const char *fileName, int fontSize, int *fontChars, int glyphCount) {
#ifdef TTF_SUPPORT
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyFont();
    }
    TTF_Font* ttf = TTF_OpenFont(fileName, fontSize);
    if (ttf == NULL) {
        TRACELOG(LOG_WARNING, "Failed to open ttf font %s (%s)", fileName, TTF_GetError());
        return GetDummyFont();
    }
    Font result = { 0 };
    result.ttf = ttf;
    result.baseSize = fontSize;
    result.glyphPadding = 4;
    result.glyphCount = 0;
    for (Uint16 i = 0; i < 256; i++) {
        if (TTF_GlyphIsProvided(ttf, i) > 0)
            result.glyphCount++;
    }
    if (result.glyphCount <= 0)
        result.glyphCount = glyphCount;
    result.is_ttf = true;
    return result;
#else
    return GetDummyFont();
#endif
}

RLCAPI Font LoadFontFromImage(Image image, Color key, int firstChar) {
    return GetDummyFont();
}

RLCAPI Font LoadFontFromMemory(const char *fileType, const unsigned char *fileData, int dataSize, int fontSize, int *fontChars, int glyphCount) {
    if (fileType == NULL || fileData == NULL) {
        NULLPTR_WARN();
        return GetDummyFont();
    }
    return GetDummyFont();
}

RLCAPI bool IsFontReady(Font font) {
#ifdef TTF_SUPPORT
    if (font.is_ttf) {
        return (bool)font.ttf;
    }
#endif
    return false;
}

RLCAPI GlyphInfo *LoadFontData(const unsigned char *fileData, int dataSize, int fontSize, int *fontChars, int glyphCount, int type) {
    if (fileData == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    return NULL;
}

RLCAPI Image GenImageFontAtlas(const GlyphInfo *chars, Rectangle **recs, int glyphCount, int fontSize, int padding, int packMethod) {
    Image result = { 0 };
    if (chars == NULL) {
        NULLPTR_WARN();
        return result;
    }
    return result;
}

RLCAPI void UnloadFontData(GlyphInfo *chars, int glyphCount) {}

RLCAPI void UnloadFont(Font font) {
#ifdef TTF_SUPPORT
    if (font.is_ttf) {
        if (font.ttf == NULL) {
            NULLPTR_WARN();
            return;
        }
        TTF_CloseFont(font.ttf);
        font.ttf = NULL;
    }
#endif
}

RLCAPI bool ExportFontAsCode(Font font, const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return false;
}

RLCAPI void DrawFPS(int posX, int posY) {
    char fps_buf[10] = "FPS: ";
    int fps = GetFPS();
    Color fps_color = LIME;
    if (fps < 15)
        fps_color = RED;
    else if (fps < 30)
        fps_color = ORANGE;
    if (SDL_itoa(fps, fps_buf + 5, 10)) {
        DrawText(fps_buf, 0, 0, DEFAULT_FONT_SIZE, fps_color);
    }
}

RLCAPI void DrawText(const char *text, int posX, int posY, int fontSize, Color color) {
    DrawTextEx(GetFontDefault(), text, VECLITERAL(Vector2){ (float)posX, (float)posY }, fontSize, 0, color);
}

RLCAPI void DrawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint) {
#ifdef TTF_SUPPORT
    if (font.is_ttf) {
        if (font.ttf == NULL) {
            NULLPTR_WARN();
            return;
        }
        Uint8 a = tint.a;
        tint.a = 255;
        if (rl.z_en) {
            if (TTF_SetFontSizeDPI(font.ttf, (int)fontSize, (int)(72.0f * rl.z), (int)(72.0f * rl.z)) < 0)
                TTF_SIZE_WARN();
        }
        else if (TTF_SetFontSize(font.ttf, (int)fontSize) < 0)
            TTF_SIZE_WARN();
        SDL_Surface* surf = TTF_RenderFunc(font.ttf, text, *((SDL_Color*)&tint), 0);
        if (surf == NULL) {
            TRACELOG(LOG_WARNING, "Failed to render ttf font (%s)", TTF_GetError());
            return;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(rl.r, surf);
        if (tex == NULL) {
            SDL_FreeSurface(surf);
            TRACELOG(LOG_WARNING, "Failed to create texture from surface (%s)", SDL_GetError());
            return;
        }
        if (a < 255 && (SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND) | SDL_SetTextureAlphaMod(tex, a)) < 0)
            BLEND_WARN();
        SDL_FRect dst_rect = { position.x, position.y, (float)surf->w, (float)surf->h };
        if (SDL_RenderCopyF(rl.r, tex, NULL, &dst_rect) < 0)
            RENDER_COPY_WARN();
        SDL_FreeSurface(surf);
    }
#endif
}

RLCAPI void DrawTextPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint) {
#ifdef TTF_SUPPORT
    if (font.is_ttf) {
        if (font.ttf == NULL) {
            NULLPTR_WARN();
            return;
        }
        Uint8 a = tint.a;
        tint.a = 255;
        if (rl.z_en) {
            if (TTF_SetFontSizeDPI(font.ttf, (int)fontSize, (int)(72.0f * rl.z), (int)(72.0f * rl.z)) < 0)
                TTF_SIZE_WARN();
        }
        else if (TTF_SetFontSize(font.ttf, (int)fontSize) < 0)
            TTF_SIZE_WARN();
        SDL_Surface* surf = TTF_RenderFunc(font.ttf, text, *((SDL_Color*)&tint), 0);
        if (surf == NULL) {
            TRACELOG(LOG_WARNING, "Failed to render ttf font (%s)", TTF_GetError());
            return;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(rl.r, surf);
        if (tex == NULL) {
            SDL_FreeSurface(surf);
            TRACELOG(LOG_WARNING, "Failed to create texture from surface (%s)", SDL_GetError());
            return;
        }
        if (a < 255 && (SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND) | SDL_SetTextureAlphaMod(tex, a)) < 0)
            BLEND_WARN();
        SDL_FRect dst_rect = { position.x, position.y, (float)surf->w, (float)surf->h };
        if (SDL_RenderCopyExF(rl.r, tex, NULL, &dst_rect,
            (const double)rotation, (const SDL_FPoint*)&origin, SDL_FLIP_NONE) < 0)
            RENDER_COPY_WARN();
        SDL_FreeSurface(surf);
    }
#endif
}

RLCAPI void DrawTextCodepoint(Font font, int codepoint, Vector2 position, float fontSize, Color tint) {}

RLCAPI void DrawTextCodepoints(Font font, const int *codepoints, int count, Vector2 position, float fontSize, float spacing, Color tint) {}
