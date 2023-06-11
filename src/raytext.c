#include <raydef.h>
#include <raylib.h>
#include <rayconf.h>

// WARNING: too much code was copy pasted

RLCAPI unsigned int TextLength(const char *text)
{
    unsigned int length = 0;
    if (text == NULL) {
        NULLPTR_WARN();
    }
    else {
        length = SDL_strlen(text);
    }
    return length;
}

RLCAPI int TextCopy(char *dst, const char *src) {
    if (dst == NULL || src == NULL) {
        NULLPTR_WARN();
        return 0;
    }
    return (int)SDL_strlcpy(dst, src, (size_t)MAX_STR_LENGTH);
}

RLCAPI bool TextIsEqual(const char *text1, const char *text2) {
    if (text1 == NULL || text2 == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return !SDL_strcmp(text1, text2);
}

RLCAPI const char *TextFormat(const char *text, ...) {
    if (text == NULL) {
        NULLPTR_WARN();
        return false;
    }
    // TODO
    return text;
}

RLCAPI const char *TextSubtext(const char *text, int position, int length) {
    if (text == NULL) {
        NULLPTR_WARN();
        return false;
    }
    static char buffer[MAX_STR_LENGTH] = { 0 };
    SDL_memset(buffer, 0, MAX_STR_LENGTH);
    int textLength = TextLength(text);
    if (position >= textLength)
    {
        position = textLength - 1;
        length = 0;
    }
    if (length >= textLength) length = textLength;
    for (int c = 0 ; c < length ; c++)
    {
        *(buffer + c) = *(text + position);
        text++;
    }
    *(buffer + length) = '\0';
    return buffer;
}

RLCAPI char *TextReplace(char *text, const char *replace, const char *by) {
    if (text == NULL || replace == NULL || by == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    return NULL;
}

RLCAPI char *TextInsert(const char *text, const char *insert, int position) {
    if (text == NULL || insert == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    return NULL;
}

RLCAPI const char *TextJoin(const char **textList, int count, const char *delimiter) {
    if (textList == NULL || delimiter == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    return NULL;
}

RLCAPI const char **TextSplit(const char *text, char delimiter, int *count) {
    if (text == NULL) {
        NULLPTR_WARN();
        return (const char**)NULL;
    }
    return NULL;
}

RLCAPI void TextAppend(char *text, const char *append, int *position) {
    if (text == NULL || append == NULL || position == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_strlcpy(text + *position, append, MAX_STR_LENGTH);
    *position += TextLength(append);
}

RLCAPI int TextFindIndex(const char *text, const char *find) {
    if (text == NULL || find == NULL) {
        NULLPTR_WARN();
        return false;
    }
    char *ptr = SDL_strstr(text, find);
    if (ptr == NULL)
        return -1;
    return (int)(ptr - text);
}

RLCAPI const char *TextToUpper(const char *text) {
    if (text == NULL) {
        NULLPTR_WARN();
        return false;
    }
    static char buffer[MAX_STR_LENGTH] = { 0 };
    SDL_memset(buffer, 0, MAX_STR_LENGTH);
    for (size_t i = 0; i < MAX_STR_LENGTH; i++) {
        if (!text[i]) {
            return buffer;
        }
        buffer[i] = SDL_toupper(text[i]);
    }
    return NULL;
}

RLCAPI const char *TextToLower(const char *text) {
    if (text == NULL) {
        NULLPTR_WARN();
        return false;
    }
    static char buffer[MAX_STR_LENGTH] = { 0 };
    SDL_memset(buffer, 0, MAX_STR_LENGTH);
    for (size_t i = 0; i < MAX_STR_LENGTH; i++) {
        if (!text[i]) {
            return buffer;
        }
        buffer[i] = SDL_tolower(text[i]);
    }
    return NULL;
}

RLCAPI const char *TextToPascal(const char *text) {
    if (text == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    return NULL;
}

RLCAPI int TextToInteger(const char *text) {
    if (text == NULL) {
        NULLPTR_WARN();
        return 0;
    }
    return SDL_atoi(text);
}

RLCAPI char *LoadUTF8(const int *codepoints, int length) {
    char *text = (char *)SDL_malloc(length * 5);
    if (text == NULL) {
        TRACELOG(LOG_WARNING, "Failed to allocate text");
        return NULL;
    }
    const char *utf8 = NULL;
    int size = 0;

    for (int i = 0, bytes = 0; i < length; i++)
    {
        utf8 = CodepointToUTF8(codepoints[i], &bytes);
        SDL_memcpy(text + size, utf8, bytes);
        size += bytes;
    }
    // Resize memory to text length + string NULL terminator
    void *ptr = SDL_realloc(text, size + 1);
    if (ptr != NULL) text = (char *)ptr;
    return text;
}

RLCAPI void UnloadUTF8(char *text) {
    if (text == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_free(text);
}

RLCAPI int *LoadCodepoints(const char *text, int *count) {
    int textLength = TextLength(text);

    int codepointSize = 0;
    int codepointCount = 0;

    // Allocate a big enough buffer to store as many codepoints as text bytes
    int *codepoints = (int *)SDL_calloc(textLength, sizeof(int));
    if (codepoints == NULL) {
        TRACELOG(LOG_WARNING, "Failed to allocate memory for codepoints");
        return NULL;
    }

    for (int i = 0; i < textLength; codepointCount++)
    {
        codepoints[codepointCount] = GetCodepointNext(text + i, &codepointSize);
        i += codepointSize;
    }

    // Re-allocate buffer to the actual number of codepoints loaded
    int *temp = (int *)SDL_realloc(codepoints, codepointCount*sizeof(int));
    if (temp != NULL) codepoints = temp;

    *count = codepointCount;

    return codepoints;
}

RLCAPI void UnloadCodepoints(int *codepoints) {
    if (codepoints == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_free(codepoints);
}

RLCAPI int GetCodepointCount(const char *text) {
    if (text == NULL) {
        NULLPTR_WARN();
        return 0;
    }
    unsigned int length = 0;
    char *ptr = (char *)&text[0];
    while (*ptr != '\0')
    {
        int next = 0;
        // int letter = GetCodepointNext(ptr, &next);

        ptr += next;

        length++;
    }
    return length;
}

RLCAPI int GetCodepoint(const char *text, int *codepointSize) {
    if (text == NULL || codepointSize == NULL) {
        NULLPTR_WARN();
        return 0;
    }
    int codepoint = 0x3f;
    int octet = (unsigned char)(text[0]);
    *codepointSize = 1;
    if (octet <= 0x7f)
    {
        codepoint = text[0];
    }
    else if ((octet & 0xe0) == 0xc0)
    {
        unsigned char octet1 = text[1];
        if ((octet1 == '\0') || ((octet1 >> 6) != 2)) { *codepointSize = 2; return codepoint; } // Unexpected sequence
        if ((octet >= 0xc2) && (octet <= 0xdf))
        {
            codepoint = ((octet & 0x1f) << 6) | (octet1 & 0x3f);
            *codepointSize = 2;
        }
    }
    else if ((octet & 0xf0) == 0xe0)
    {
        unsigned char octet1 = text[1];
        unsigned char octet2 = '\0';
        if ((octet1 == '\0') || ((octet1 >> 6) != 2)) { *codepointSize = 2; return codepoint; } // Unexpected sequence
        octet2 = text[2];
        if ((octet2 == '\0') || ((octet2 >> 6) != 2)) { *codepointSize = 3; return codepoint; } // Unexpected sequence
        if (((octet == 0xe0) && !((octet1 >= 0xa0) && (octet1 <= 0xbf))) ||
            ((octet == 0xed) && !((octet1 >= 0x80) && (octet1 <= 0x9f)))) { *codepointSize = 2; return codepoint; }
        if ((octet >= 0xe0) && (octet <= 0xef))
        {
            codepoint = ((octet & 0xf) << 12) | ((octet1 & 0x3f) << 6) | (octet2 & 0x3f);
            *codepointSize = 3;
        }
    }
    else if ((octet & 0xf8) == 0xf0)
    {
        if (octet > 0xf4) return codepoint;
        unsigned char octet1 = text[1];
        unsigned char octet2 = '\0';
        unsigned char octet3 = '\0';
        if ((octet1 == '\0') || ((octet1 >> 6) != 2)) { *codepointSize = 2; return codepoint; }  // Unexpected sequence
        octet2 = text[2];
        if ((octet2 == '\0') || ((octet2 >> 6) != 2)) { *codepointSize = 3; return codepoint; }  // Unexpected sequence
        octet3 = text[3];
        if ((octet3 == '\0') || ((octet3 >> 6) != 2)) { *codepointSize = 4; return codepoint; }  // Unexpected sequence
        if (((octet == 0xf0) && !((octet1 >= 0x90) && (octet1 <= 0xbf))) ||
            ((octet == 0xf4) && !((octet1 >= 0x80) && (octet1 <= 0x8f)))) { *codepointSize = 2; return codepoint; } // Unexpected sequence
        if (octet >= 0xf0)
        {
            codepoint = ((octet & 0x7) << 18) | ((octet1 & 0x3f) << 12) | ((octet2 & 0x3f) << 6) | (octet3 & 0x3f);
            *codepointSize = 4;
        }
    }

    if (codepoint > 0x10ffff) codepoint = 0x3f;

    return codepoint;
}

RLCAPI int GetCodepointNext(const char *text, int *codepointSize) {
    if (text == NULL || codepointSize == NULL) {
        NULLPTR_WARN();
        return 0;
    }
    const char *ptr = text;
    int codepoint = 0x3f;
    *codepointSize = 1;
    if (0xf0 == (0xf8 & ptr[0]))
    {
        if(((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80) || ((ptr[3] & 0xC0) ^ 0x80)) { return codepoint; } //10xxxxxx checks
        codepoint = ((0x07 & ptr[0]) << 18) | ((0x3f & ptr[1]) << 12) | ((0x3f & ptr[2]) << 6) | (0x3f & ptr[3]);
        *codepointSize = 4;
    }
    else if (0xe0 == (0xf0 & ptr[0]))
    {
        if(((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80)) { return codepoint; }
        codepoint = ((0x0f & ptr[0]) << 12) | ((0x3f & ptr[1]) << 6) | (0x3f & ptr[2]);
        *codepointSize = 3;
    }
    else if (0xc0 == (0xe0 & ptr[0]))
    {
        if((ptr[1] & 0xC0) ^ 0x80) { return codepoint; }
        codepoint = ((0x1f & ptr[0]) << 6) | (0x3f & ptr[1]);
        *codepointSize = 2;
    }
    else if (0x00 == (0x80 & ptr[0]))
    {
        codepoint = ptr[0];
        *codepointSize = 1;
    }
    return codepoint;
}

RLCAPI int GetCodepointPrevious(const char *text, int *codepointSize) {
    if (text == NULL || codepointSize == NULL) {
        NULLPTR_WARN();
        return 0;
    }
    const char *ptr = text;
    int codepoint = 0x3f;
    int cpSize = 0;
    *codepointSize = 0;
    do ptr--;
    while (((0x80 & ptr[0]) != 0) && ((0xc0 & ptr[0]) ==  0x80));
    codepoint = GetCodepointNext(ptr, &cpSize);
    if (codepoint != 0) *codepointSize = cpSize;
    return codepoint;
}

RLCAPI const char *CodepointToUTF8(int codepoint, int *utf8Size) {
    if (utf8Size == NULL) {
        NULLPTR_WARN();
        return 0;
    }
    static char utf8[6] = { 0 };
    int size = 0;
    if (codepoint <= 0x7f)
    {
        utf8[0] = (char)codepoint;
        size = 1;
    }
    else if (codepoint <= 0x7ff)
    {
        utf8[0] = (char)(((codepoint >> 6) & 0x1f) | 0xc0);
        utf8[1] = (char)((codepoint & 0x3f) | 0x80);
        size = 2;
    }
    else if (codepoint <= 0xffff)
    {
        utf8[0] = (char)(((codepoint >> 12) & 0x0f) | 0xe0);
        utf8[1] = (char)(((codepoint >>  6) & 0x3f) | 0x80);
        utf8[2] = (char)((codepoint & 0x3f) | 0x80);
        size = 3;
    }
    else if (codepoint <= 0x10ffff)
    {
        utf8[0] = (char)(((codepoint >> 18) & 0x07) | 0xf0);
        utf8[1] = (char)(((codepoint >> 12) & 0x3f) | 0x80);
        utf8[2] = (char)(((codepoint >>  6) & 0x3f) | 0x80);
        utf8[3] = (char)((codepoint & 0x3f) | 0x80);
        size = 4;
    }
    *utf8Size = size;
    return utf8;
}
