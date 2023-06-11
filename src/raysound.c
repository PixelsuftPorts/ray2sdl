#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>

#ifdef MIX_IMPL
Sound GetDummySound() {
    Sound result = { 0 };
    return result;
}

Wave GetDummyWave() {
    Wave result = { 0 };
    return result;
}

RLCAPI Wave LoadWave(const char *fileName) {
    return GetDummyWave();
}

RLCAPI Wave LoadWaveFromMemory(const char *fileType, const unsigned char *fileData, int dataSize) {
    return GetDummyWave();
}

RLCAPI bool IsWaveReady(Wave wave) {
    return false;
}

RLCAPI Sound LoadSound(const char *fileName) {
#ifdef MIX_SUPPORT
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummySound();
    }
    return GetDummySound();
#else
    return GetDummySound();
#endif
}

RLCAPI Sound LoadSoundFromWave(Wave wave) {
    // TODO
    return GetDummySound();
}

RLCAPI bool IsSoundReady(Sound sound) {
#ifdef MIX_SUPPORT
    return (bool)sound.chunk;
#else
    return false;
#endif
}

RLCAPI void UpdateSound(Sound sound, const void *data, int sampleCount) {
    if (data == NULL) {
        NULLPTR_WARN();
        return;
    }
}

RLCAPI void UnloadWave(Wave wave) {}

RLCAPI void UnloadSound(Sound sound) {}

RLCAPI bool ExportWave(Wave wave, const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return false;
}

RLCAPI bool ExportWaveAsCode(Wave wave, const char *fileName) {
    if (fileName == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return false;
}
#endif
