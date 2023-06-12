#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>

Font GetDummyFont() {
    Font result = { 0 };
    return result;
}

RLCAPI Font GetFontDefault(void) {
    return GetDummyFont();
}

RLCAPI Font LoadFont(const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyFont();
    }
#ifdef TTF_SUPPORT
    return LoadFontEx(fileName, DEFAULT_TTF_SIZE, NULL, 95);
#else
    return GetDummyFont();
#endif
}

RLCAPI Font LoadFontEx(const char *fileName, int fontSize, int *fontChars, int glyphCount) {
#ifdef TTF_SUPPORT
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyFont();
    }
    TTF_Font* ttf = TTF_OpenFont(fileName, DEFAULT_TTF_SIZE);
    if (ttf == NULL) {
        TRACELOG(LOG_WARNING, "Failed to open ttf font %s (%s)", fileName, TTF_GetError());
        return GetDummyFont();
    }
    Font result = { 0 };
    result.ttf = ttf;
    result.baseSize = TTF_FontHeight(ttf);
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
