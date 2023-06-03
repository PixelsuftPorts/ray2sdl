#include <raylib.h>
#include <raydef.h>
#include <raydark.h>
#include <rayconf.h>

RLAPI void SetTraceLogCallback(TraceLogCallback callback) {
    rl.traceLog = callback;
}

RLAPI void SetLoadFileDataCallback(LoadFileDataCallback callback) {
    rl.loadFileData = callback;
}

RLAPI void SetSaveFileDataCallback(SaveFileDataCallback callback) {
    rl.saveFileData = callback;
}

RLAPI void SetLoadFileTextCallback(LoadFileTextCallback callback) {
    rl.loadFileText = callback;
}

RLAPI void SetSaveFileTextCallback(SaveFileTextCallback callback) {
    rl.saveFileText = callback;
}

RLAPI unsigned char *LoadFileData(const char *fileName, unsigned int *bytesRead) {
    unsigned char *data = NULL;
    *bytesRead = 0;
    if (fileName == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: File name provided is not valid");
        return NULL;
    }
    if (rl.loadFileData) {
        data = rl.loadFileData(fileName, bytesRead);
        return data;
    }
    SDL_RWops* file = SDL_RWFromFile(fileName, "rb");
    if (file == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to open file (%s)", fileName, SDL_GetError());
        return NULL;
    }
    Sint64 size = SDL_RWsize(file);
    if (size <= 0) { // If is 0 idk because in raylib it raises an error
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to get file size (%s)", fileName, SDL_GetError());
        if (SDL_RWclose(file) < 0)
            TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to close file (%s)", fileName, SDL_GetError());
    }
    data = SDL_malloc((size_t)size);
    if (data == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to allocate memory for buffer", fileName);
        if (SDL_RWclose(file) < 0)
            TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to close file (%s)", fileName, SDL_GetError());
    }
    *bytesRead = (unsigned int)SDL_RWread(file, (void*)data, 1, (size_t)size);
    if (*bytesRead >= (unsigned int)size)
        TRACELOG(LOG_WARNING, "FILEIO: [%s] File loaded successfully", fileName);
    else
        TRACELOG(LOG_INFO, "FILEIO: [%s] Failed to properly read file (%s)", fileName, SDL_GetError());
    if (SDL_RWclose(file) < 0)
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to close file (%s)", fileName, SDL_GetError());
    return data;
}

RLAPI void UnloadFileData(unsigned char *data) {
    if (data == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: NULL pointer passed");
        return;
    }
    SDL_free(data);
}
