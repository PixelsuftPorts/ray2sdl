#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>

// TODO: most of the functions work with channels, so we must cache values if sound is not playing 

#ifdef MIX_IMPL
#define AFK_CHANNEL -2

Sound GetDummySound() {
    Sound result = { 0 };
    return result;
}

Wave GetDummyWave() {
    Wave result = { 0 };
    return result;
}

bool HasSoundChannel(Sound* sound) {
    if (sound->extra->channel == AFK_CHANNEL)
        return false;
    if (!Mix_Playing(sound->extra->channel)) {
        sound->extra->channel = AFK_CHANNEL;
        return false;
    }
    return true;
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
    result.extra = SDL_malloc(sizeof(SoundExtra));
    if (result.extra == NULL) {
        Mix_FreeChunk(result.chunk);
        TRACELOG(LOG_WARNING, "Failed to allocate memory for extra sound data");
        return GetDummySound();
    }
    result.stream.buffer = (rAudioBuffer*)chunk->abuf;
    result.stream.processor = NULL;
    result.stream.channels = MIX_CHANNELS;
    result.stream.sampleRate = MIX_FREQ;
    result.stream.sampleSize = MIX_CHUNK_SIZE;
    result.chunk = chunk;
    result.frameCount = (unsigned int)chunk->alen;
    result.extra->channel = AFK_CHANNEL;
    result.extra->pan = 0;
    result.extra->has_pan = false;
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
    return (bool)sound.chunk && (bool)sound.extra;
#else
    return false;
#endif
}

RLCAPI void UpdateSound(Sound sound, const void *data, int sampleCount) {
#ifdef MIX_SUPPORT
    if (sound.chunk == NULL || sound.extra == NULL || data == NULL) {
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
    }
    else {
        Mix_FreeChunk(sound.chunk);
        sound.chunk = NULL;
    }
    if (sound.extra == NULL) {
        NULLPTR_WARN();
    }
    else {
        SDL_free(sound.extra);
        sound.extra = NULL;
    }
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
    if (sound.chunk == NULL || sound.extra == NULL) {
        NULLPTR_WARN();
        return;
    }
    if (HasSoundChannel(&sound)) {
        if (Mix_HaltChannel(sound.extra->channel) < 0)
            TRACELOG(LOG_WARNING, "Failed to stop sound (%s)", Mix_GetError());
    }
    sound.extra->channel = Mix_PlayChannel(-1, sound.chunk, 0);
    if (sound.extra->channel == -1) {
        sound.extra->channel = AFK_CHANNEL;
        TRACELOG(LOG_WARNING, "Failed to play sound (%s)", Mix_GetError());
        return;
    }
    if (sound.extra->has_pan) {
        Mix_SetPanning(sound.extra->channel, sound.extra->pan, 255 - sound.extra->pan);
    }
#endif
}

RLCAPI void StopSound(Sound sound) {
#ifdef MIX_SUPPORT
    if (sound.chunk == NULL || sound.extra == NULL) {
        NULLPTR_WARN();
        return;
    }
    if (HasSoundChannel(&sound)) {
        if (Mix_HaltChannel(sound.extra->channel) < 0)
            TRACELOG(LOG_WARNING, "Failed to stop sound (%s)", Mix_GetError());
        sound.extra->channel = AFK_CHANNEL;
    }
#endif
}

RLCAPI void PauseSound(Sound sound) {
#ifdef MIX_SUPPORT
    if (sound.chunk == NULL || sound.extra == NULL) {
        NULLPTR_WARN();
        return;
    }
    if (HasSoundChannel(&sound)) {
        Mix_Pause(sound.extra->channel);
    }
#endif
}

RLCAPI void ResumeSound(Sound sound) {
#ifdef MIX_SUPPORT
    if (sound.chunk == NULL || sound.extra == NULL) {
        NULLPTR_WARN();
        return;
    }
    if (HasSoundChannel(&sound)) {
        Mix_Resume(sound.extra->channel);
    }
#endif
}

RLCAPI bool IsSoundPlaying(Sound sound) {
#ifdef MIX_SUPPORT
    if (sound.chunk == NULL || sound.extra == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return HasSoundChannel(&sound) && Mix_Paused(sound.extra->channel) <= 0;
#endif
}

RLCAPI void SetSoundVolume(Sound sound, float volume) {
#ifdef MIX_SUPPORT
    if (sound.chunk == NULL) {
        NULLPTR_WARN();
        return;
    }
    HasSoundChannel(&sound); // Just to ensure if it's state is not changed
    Mix_VolumeChunk(sound.chunk, (int)(volume * (float)MIX_MAX_VOLUME));
#endif
}

RLCAPI void SetSoundPitch(Sound sound, float pitch) {
#ifdef MIX_SUPPORT
    if (sound.chunk == NULL || sound.extra == NULL) {
        NULLPTR_WARN();
        return;
    }
#endif
}

RLCAPI void SetSoundPan(Sound sound, float pan) {
#ifdef MIX_SUPPORT
    if (sound.chunk == NULL || sound.extra == NULL) {
        NULLPTR_WARN();
        return;
    }
    // Lowers the volume, but in original too
    sound.extra->pan = (Uint8)(SDL_clamp(pan, 0.0f, 1.0f) * 255.0f);
    sound.extra->has_pan = true;
    if (HasSoundChannel(&sound)) {
        Mix_SetPanning(sound.extra->channel, sound.extra->pan, 255 - sound.extra->pan);
    }
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
        return;
    }
#endif
}
