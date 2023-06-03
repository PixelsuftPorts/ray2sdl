#include <stdio.h>
#include <raylib.h>
#include <raydef.h>
#include <raydark.h>
#include <rayconf.h>

RLCAPI void SetTraceLogCallback(TraceLogCallback callback) {
    rl.traceLog = callback;
}

RLCAPI void SetLoadFileDataCallback(LoadFileDataCallback callback) {
    rl.loadFileData = callback;
}

RLCAPI void SetSaveFileDataCallback(SaveFileDataCallback callback) {
    rl.saveFileData = callback;
}

RLCAPI void SetLoadFileTextCallback(LoadFileTextCallback callback) {
    rl.loadFileText = callback;
}

RLCAPI void SetSaveFileTextCallback(SaveFileTextCallback callback) {
    rl.saveFileText = callback;
}

RLCAPI unsigned char *LoadFileData(const char *fileName, unsigned int *bytesRead) {
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
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to open file for reading (%s)", fileName, SDL_GetError());
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
        TRACELOG(LOG_INFO, "FILEIO: [%s] File loaded successfully", fileName);
    else
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to properly read file (%s)", fileName, SDL_GetError());
    if (SDL_RWclose(file) < 0)
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to close file (%s)", fileName, SDL_GetError());
    return data;
}

RLCAPI void UnloadFileData(unsigned char *data) {
    if (data == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: NULL pointer passed");
        return;
    }
    SDL_free(data);
}

RLCAPI bool SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite) {
    if (fileName == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: File name provided is not valid");
        return false;
    }
    if (rl.saveFileData) {
        return rl.saveFileData(fileName, data, bytesToWrite);
    }
    SDL_RWops* file = SDL_RWFromFile(fileName, "wb");
    if (file == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to open file for writing (%s)", fileName, SDL_GetError());
        return false;
    }
    size_t count = SDL_RWwrite(file, (const void*)data, 1, bytesToWrite);
    if (count >= (size_t)bytesToWrite) {
        TRACELOG(LOG_INFO, "FILEIO: [%s] File saved successfully", fileName);
    }
    else {
        if (count > 0)
            TRACELOG(LOG_WARNING, "FILEIO: [%s] File partially written (%s)", fileName, SDL_GetError());
        else 
            TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to write file (%s)", fileName, SDL_GetError());
    }
    if (SDL_RWclose(file) < 0) {
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to close file (%s)", fileName, SDL_GetError());
        return false;
    }
    return true;
}

RLCAPI bool ExportDataAsCode(const unsigned char *data, unsigned int size, const char *fileName)
{
    bool success = false;

#ifndef TEXT_BYTES_PER_LINE
    #define TEXT_BYTES_PER_LINE     20
#endif

    // NOTE: Text data buffer size is estimated considering raw data size in bytes
    // and requiring 6 char bytes for every byte: "0x00, "
    /*char *txtData = (char *)SDL_calloc(size*6 + 2000, sizeof(char));

    int byteCount = 0;
    byteCount += sprintf(txtData + byteCount, "////////////////////////////////////////////////////////////////////////////////////////\n");
    byteCount += sprintf(txtData + byteCount, "//                                                                                    //\n");
    byteCount += sprintf(txtData + byteCount, "// DataAsCode exporter v1.0 - Raw data exported as an array of bytes                  //\n");
    byteCount += sprintf(txtData + byteCount, "//                                                                                    //\n");
    byteCount += sprintf(txtData + byteCount, "// more info and bugs-report:  github.com/raysan5/raylib                              //\n");
    byteCount += sprintf(txtData + byteCount, "// feedback and support:       ray[at]raylib.com                                      //\n");
    byteCount += sprintf(txtData + byteCount, "//                                                                                    //\n");
    byteCount += sprintf(txtData + byteCount, "// Copyright (c) 2022-$YEAR Ramon Santamaria (@raysan5)                               //\n");
    byteCount += sprintf(txtData + byteCount, "//                                                                                    //\n");
    byteCount += sprintf(txtData + byteCount, "////////////////////////////////////////////////////////////////////////////////////////\n\n");

    // Get file name from path and convert variable name to uppercase
    char varFileName[256] = { 0 };
    strcpy(varFileName, GetFileNameWithoutExt(fileName));
    for (int i = 0; varFileName[i] != '\0'; i++) if ((varFileName[i] >= 'a') && (varFileName[i] <= 'z')) { varFileName[i] = varFileName[i] - 32; }

    byteCount += sprintf(txtData + byteCount, "#define %s_DATA_SIZE     %i\n\n", varFileName, size);

    byteCount += sprintf(txtData + byteCount, "static unsigned char %s_DATA[%s_DATA_SIZE] = { ", varFileName, varFileName);
    for (unsigned int i = 0; i < size - 1; i++) byteCount += sprintf(txtData + byteCount, ((i%TEXT_BYTES_PER_LINE == 0)? "0x%x,\n" : "0x%x, "), data[i]);
    byteCount += sprintf(txtData + byteCount, "0x%x };\n", data[size - 1]);

    // NOTE: Text data size exported is determined by '\0' (NULL) character
    success = SaveFileText(fileName, txtData);

    SDL_free(txtData);

    if (success != 0) TRACELOG(LOG_INFO, "FILEIO: [%s] Data as code exported successfully", fileName);
    else TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to export data as code", fileName);
*/
    return success;
}
