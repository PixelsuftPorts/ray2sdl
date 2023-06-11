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
    Mix_Chunk* chunk = Mix_LoadWAV(fileName);
    if (chunk == NULL) {
        TRACELOG(LOG_WARNING, "Failed to load sound %s (%s)", fileName, Mix_GetError());
        return GetDummySound();
    }
    Sound result = { 0 };
    result.stream.buffer = (rAudioBuffer*)chunk->abuf;
    result.stream.processor = NULL;
    result.stream.channels = MIX_CHANNELS;
    result.stream.sampleRate = MIX_FREQ;
    result.stream.sampleSize = MIX_CHUNK_SIZE;
    result.chunk = chunk;
    result.frameCount = (unsigned int)chunk->alen;
    return result;
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
#ifdef MIX_SUPPORT
    if (data == NULL) {
        NULLPTR_WARN();
        return;
    }
#endif
}

RLCAPI void UnloadWave(Wave wave) {
#ifdef MIX_SUPPORT
#endif
}

RLCAPI void UnloadSound(Sound sound) {
#ifdef MIX_SUPPORT
    if (sound.chunk == NULL) {
        NULLPTR_WARN();
        return;
    }
    Mix_FreeChunk(sound.chunk);
    sound.chunk = NULL;
#endif
}

RLCAPI bool ExportWave(Wave wave, const char *fileName) {
#ifdef MIX_SUPPORT
    if (fileName == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return false;
#else
    return false;
#endif
}

RLCAPI bool ExportWaveAsCode(Wave wave, const char *fileName) {
#ifdef MIX_SUPPORT
    if (fileName == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return false;
#else
    return false;
#endif
}
#endif

RLCAPI void PlaySound(Sound sound) {
#ifdef MIX_SUPPORT
#endif
}

RLCAPI void StopSound(Sound sound) {
#ifdef MIX_SUPPORT
#endif
}

RLCAPI void PauseSound(Sound sound) {
#ifdef MIX_SUPPORT
#endif
}

RLCAPI void ResumeSound(Sound sound) {
#ifdef MIX_SUPPORT
#endif
}

RLCAPI bool IsSoundPlaying(Sound sound) {
#ifdef MIX_SUPPORT
#endif
}

RLCAPI void SetSoundVolume(Sound sound, float volume) {
#ifdef MIX_SUPPORT
#endif
}

RLCAPI void SetSoundPitch(Sound sound, float pitch) {
#ifdef MIX_SUPPORT
#endif
}

RLCAPI void SetSoundPan(Sound sound, float pan) {
#ifdef MIX_SUPPORT
#endif
}

RLCAPI Wave WaveCopy(Wave wave) {
#ifdef MIX_SUPPORT
    return GetDummyWave();
#else
    return GetDummyWave();
#endif
}

RLCAPI void WaveCrop(Wave *wave, int initSample, int finalSample) {
#ifdef MIX_SUPPORT
    if (wave == NULL) {
        NULLPTR_WARN();
        return;
    }
#endif
}

RLCAPI void WaveFormat(Wave *wave, int sampleRate, int sampleSize, int channels) {
#ifdef MIX_SUPPORT
    if (wave == NULL) {
        NULLPTR_WARN();
        return;
    }
#endif
}

RLCAPI float *LoadWaveSamples(Wave wave) {
#ifdef MIX_SUPPORT
    return NULL;
#else
    return NULL;
#endif
}

RLCAPI void UnloadWaveSamples(float *samples) {
#ifdef MIX_SUPPORT
    if (samples == NULL) {
        NULLPTR_WARN();
        return
    }
#endif
}
