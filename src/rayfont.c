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
    return GetDummyFont();
}

RLCAPI Font LoadFontEx(const char *fileName, int fontSize, int *fontChars, int glyphCount) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyFont();
    }
    return GetDummyFont();
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

RLCAPI void UnloadFont(Font font) {}

RLCAPI bool ExportFontAsCode(Font font, const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return false;
}
