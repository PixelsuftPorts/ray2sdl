#include <stdio.h>
#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>
#ifndef _MSC_VER
#include <dirent.h>
#endif
#if defined(_WIN32)
    #include <direct.h>
    #define GETCWD _getcwd
    #define CHDIR _chdir
#else
    #include <unistd.h>
    #define GETCWD getcwd
    #define CHDIR chdir
#endif

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
        FILENAME_WARN();
        return NULL;
    }
    if (rl.loadFileData) {
        data = rl.loadFileData(fileName, bytesRead);
        return data;
    }
    SDL_RWops* file = SDL_RWFromFile(fileName, "rb");
    if (file == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to open file for reading bytes (%s)", fileName, SDL_GetError());
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
        NULLPTR_WARN();
        return;
    }
    SDL_free(data);
}

RLCAPI bool SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite) {
    if (fileName == NULL) {
        FILENAME_WARN();
        return false;
    }
    if (rl.saveFileData) {
        return rl.saveFileData(fileName, data, bytesToWrite);
    }
    SDL_RWops* file = SDL_RWFromFile(fileName, "wb");
    if (file == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to open file for writing bytes (%s)", fileName, SDL_GetError());
        return false;
    }
    size_t count = SDL_RWwrite(file, (const void*)data, 1, (size_t)bytesToWrite);
    if (count >= (size_t)bytesToWrite) {
        TRACELOG(LOG_INFO, "FILEIO: [%s] File saved successfully", fileName);
    }
    else if (count > 0)
        TRACELOG(LOG_WARNING, "FILEIO: [%s] File partially written (%s)", fileName, SDL_GetError());
    else 
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to write file (%s)", fileName, SDL_GetError());
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

RLCAPI char *LoadFileText(const char *fileName) {
    if (fileName == NULL) {
        FILENAME_WARN();
        return NULL;
    }
    if (rl.loadFileText) {
        return rl.loadFileText(fileName);
    }
    char* result = SDL_LoadFile(fileName, NULL);
    if (result == NULL)
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed read file as text (%s)", fileName, SDL_GetError());
    return result;
}

RLCAPI void UnloadFileText(char *text) {
    if (text == NULL) {
        NULLPTR_WARN();
        return;
    }
    SDL_free(text);
}

RLCAPI bool SaveFileText(const char *fileName, char *text) {
    if (fileName == NULL) {
        FILENAME_WARN();
        return false;
    }
    if (rl.saveFileText) {
        return rl.saveFileText(fileName, text);
    }
    SDL_RWops* file = SDL_RWFromFile(fileName, "w");
    if (file == NULL) {
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to open text file for writing (%s)", fileName, SDL_GetError());
        return false;
    }
    size_t bytesToWrite = SDL_strlen(text);
    size_t count = SDL_RWwrite(file, (const void*)text, 1, bytesToWrite);
    if (count >= bytesToWrite) {
        TRACELOG(LOG_INFO, "FILEIO: [%s] Text file saved successfully", fileName);
    }
    else if (count > 0)
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Text file partially written (%s)", fileName, SDL_GetError());
    else 
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to write text file (%s)", fileName, SDL_GetError());
    if (SDL_RWclose(file) < 0) {
        TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to close text file (%s)", fileName, SDL_GetError());
        return false;
    }
    return true;
}

RLCAPI bool FileExists(const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return false;
    }
    SDL_RWops* file = SDL_RWFromFile(fileName, "rb");
    if (file == NULL) {
        return false;
    }
    if (SDL_RWclose(file) < 0) {
        TRACELOG(LOG_WARNING, "FILEIO: [%s] WTF Failed to close text file (%s)", fileName, SDL_GetError());
    }
    return true;
}

RLCAPI bool DirectoryExists(const char *dirPath) {
    if (dirPath == NULL) {
        NULLPTR_WARN();
        return false;
    }
#ifdef _MSC_VER
    return false; // TODO
#else
    DIR *dir = opendir(dirPath);
    if (dir == NULL)
        return false;
    if (closedir(dir) < 0)
        TRACELOG(LOG_WARNING, "FILEIO: [%s] WTF Failed to close dir", dirPath);
    return true;
#endif
}

RLCAPI bool IsFileExtension(const char *fileName, const char *ext) {
    if (fileName == NULL || ext == NULL) {
        NULLPTR_WARN();
        return false;
    }
    const char *fileExt = GetFileExtension(fileName);
    if (fileExt == NULL)
        return false;
    return !SDL_strcmp(fileExt, ext);
}

RLCAPI const char *GetFileExtension(const char *fileName) {
    // This one doesn't check UPPER or lower cases so it's not mine problem
    if (fileName == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    const char *dot = SDL_strrchr(fileName, '.');
    if (dot == NULL || dot == fileName)
        return NULL;
    return dot;
}

RLCAPI const char *GetFileName(const char *filePath) {
    if (filePath == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    const char *fileName = SDL_max(SDL_strrchr(filePath, '\\'), SDL_strrchr(filePath, '/'));
    if (fileName == NULL)
        return filePath;
    return fileName + 1;
}

RLCAPI const char *GetFileNameWithoutExt(const char *filePath) {
    if (filePath == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    static char result[MAX_FILENAME_LENGTH];
    const char* fileName = GetFileName(filePath);
    size_t size = SDL_strlen(fileName) - SDL_strlen(GetFileExtension(fileName));
    SDL_memcpy(result, fileName, size);
    result[size] = '\0';
    return result;
}

RLCAPI const char *GetDirectoryPath(const char *filePath) {
    if (filePath == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    // I don't think it's safe (or it is?)
    static char result[MAX_FILEPATH_LENGTH];
    const char* fileName = GetFileName(filePath);
    size_t size = SDL_strlen(filePath) - SDL_strlen(fileName) - 1;
    SDL_memcpy(result, filePath, size);
    result[size] = '\0';
    return result;
}

RLCAPI const char *GetPrevDirectoryPath(const char *dirPath) {
    if (dirPath == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    return GetDirectoryPath(dirPath); // LOL
}

RLCAPI const char *GetWorkingDirectory(void) {
    static char currentDir[MAX_FILEPATH_LENGTH] = { 0 };
    char *path = GETCWD(currentDir, MAX_FILEPATH_LENGTH - 1);
    if (path == NULL)
        TRACELOG(LOG_WARNING, "FILEIO: Failed to get CWD");
    return path;
}

RLCAPI const char *GetApplicationDirectory(void) {
    return (const char*)SDL_GetBasePath();
}

RLCAPI bool ChangeDirectory(const char *dir) {
    if (dir == NULL) {
        NULLPTR_WARN();
        return NULL;
    }
    if (!CHDIR(dir))
        return true;
    TRACELOG(LOG_WARNING, "FILEIO: Failed to change dir to %s", dir);
    return false;
}

RLCAPI bool IsPathFile(const char *path) {
    return FileExists(path); // I don't think it's correct
}

RLCAPI FilePathList LoadDirectoryFiles(const char *dirPath) {
    // TODO
    FilePathList result = { 0 };
    return result;
}

RLCAPI FilePathList LoadDirectoryFilesEx(const char *basePath, const char *filter, bool scanSubdirs) {
    FilePathList result = { 0 };
    return result;
}

RLCAPI void UnloadDirectoryFiles(FilePathList files) {

}

RLCAPI void RegisterFileDrop(char* fp) {
#ifdef SUPPORT_FILES_DROPPING
    if (rl.drops.capacity <= 0) {
        rl.drops.count = 0;
        rl.drops.paths = (char**)SDL_malloc(MAX_FILEPATH_CAPACITY);
        if (rl.drops.paths == NULL) {
            TRACELOG(LOG_WARNING, "Failed to allocate drops paths");
            return;
        }
        rl.drops.capacity = MAX_FILEPATH_CAPACITY;
    }
    else if (rl.drops.count >= rl.drops.capacity) {
        TRACELOG(LOG_WARNING, "No free drop paths");
        return;
    }
    rl.drops.paths[rl.drops.count] = fp;
    rl.drops.count++;
#endif
}

RLCAPI bool IsFileDropped(void) {
#ifdef SUPPORT_FILES_DROPPING
    return (bool)rl.drops.count;
#else
    return false;
#endif
}

RLCAPI FilePathList LoadDroppedFiles(void) {
#ifdef SUPPORT_FILES_DROPPING
    return rl.drops;
#else
    FilePathList result = { 0 };
    return result;
#endif
}

RLCAPI void UnloadDroppedFiles(FilePathList files) {
#ifdef SUPPORT_FILES_DROPPING
    if (files.capacity <= 0 || files.paths == NULL) {
        TRACELOG(LOG_WARNING, "Passed inavid FilePathList");
        return;
    }
    for (size_t i = 0; i < files.count; i++) {
        if (files.paths[i] == NULL)
            continue;
        SDL_free(files.paths[i]);
    }
    SDL_free(files.paths);
    // Bruh
    files.capacity = rl.drops.capacity = 0;
    files.count = rl.drops.count = 0;
    files.paths = rl.drops.paths = NULL;
#endif
}

RLCAPI long GetFileModTime(const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return 0;
    }
    // TODO
    return 0;
}

RLCAPI unsigned char *CompressData(const unsigned char *data, int dataSize, int *compDataSize) {
    return NULL; // TODO
}

RLCAPI unsigned char *DecompressData(const unsigned char *compData, int compDataSize, int *dataSize) {
    return NULL;
}

RLCAPI char *EncodeDataBase64(const unsigned char *data, int dataSize, int *outputSize) {
    if (data == NULL || dataSize <= 0) {
        TRACELOG(LOG_WARNING, "Invalid data");
        return NULL;
    }
    static const unsigned char base64encodeTable[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };
    static const int modTable[] = { 0, 2, 1 };
    *outputSize = 4 * ((dataSize + 2) / 3);
    char *encodedData = (char *)SDL_malloc(*outputSize);
    if (encodedData == NULL) {
        TRACELOG(LOG_WARNING, "Failed to allocate memory for encoded data");
        return NULL;
    }
    for (int i = 0, j = 0; i < dataSize;)
    {
        unsigned int octetA = (i < dataSize)? (unsigned char)data[i++] : 0;
        unsigned int octetB = (i < dataSize)? (unsigned char)data[i++] : 0;
        unsigned int octetC = (i < dataSize)? (unsigned char)data[i++] : 0;

        unsigned int triple = (octetA << 0x10) + (octetB << 0x08) + octetC;

        encodedData[j++] = base64encodeTable[(triple >> 3 * 6) & 0x3F];
        encodedData[j++] = base64encodeTable[(triple >> 2 * 6) & 0x3F];
        encodedData[j++] = base64encodeTable[(triple >> 1 * 6) & 0x3F];
        encodedData[j++] = base64encodeTable[(triple >> 0 * 6) & 0x3F];
    }
    for (int i = 0; i < modTable[dataSize % 3]; i++) encodedData[*outputSize - 1 - i] = '=';
    return encodedData;
}

RLCAPI unsigned char *DecodeDataBase64(const unsigned char *data, int *outputSize) {
    if (data == NULL) {
        TRACELOG(LOG_WARNING, "Invalid data");
        return NULL;
    }
    static const unsigned char base64decodeTable[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 62, 0, 0, 0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
        37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
    };
    int outSize = 0;
    for (int i = 0; data[4 * i] != 0; i++)
    {
        if (data[4 * i + 3] == '=')
        {
            if (data[4 * i + 2] == '=') outSize += 1;
            else outSize += 2;
        }
        else outSize += 3;
    }
    unsigned char *decodedData = (unsigned char *)SDL_malloc(outSize);
    if (decodedData == NULL) {
        TRACELOG(LOG_WARNING, "Failed to allocate memory for decoded data");
        return NULL;
    }
    for (int i = 0; i < outSize / 3; i++)
    {
        unsigned char a = base64decodeTable[(int)data[4 * i]];
        unsigned char b = base64decodeTable[(int)data[4 * i + 1]];
        unsigned char c = base64decodeTable[(int)data[4 * i + 2]];
        unsigned char d = base64decodeTable[(int)data[4 * i + 3]];

        decodedData[3 * i] = (a << 2) | (b >> 4);
        decodedData[3 * i + 1] = (b << 4) | (c >> 2);
        decodedData[3 * i + 2] = (c << 6) | d;
    }
    if (outSize % 3 == 1)
    {
        int n = outSize / 3;
        unsigned char a = base64decodeTable[(int)data[4 * n]];
        unsigned char b = base64decodeTable[(int)data[4 * n + 1]];
        decodedData[outSize - 1] = (a << 2) | (b >> 4);
    }
    else if (outSize % 3 == 2)
    {
        int n = outSize / 3;
        unsigned char a = base64decodeTable[(int)data[4 * n]];
        unsigned char b = base64decodeTable[(int)data[4 * n + 1]];
        unsigned char c = base64decodeTable[(int)data[4 * n + 2]];
        decodedData[outSize - 2] = (a << 2) | (b >> 4);
        decodedData[outSize - 1] = (b << 4) | (c >> 2);
    }
    *outputSize = outSize;
    return decodedData;
}
