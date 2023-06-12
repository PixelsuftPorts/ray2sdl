#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>

// TODO: nullptr checks

#ifdef MIX_IMPL
Music GetDummyMusic() {
    Music result = { 0 };
    return result;
}

RLCAPI Music LoadMusicStream(const char *fileName) {
#ifdef MIX_SUPPORT
    if (fileName == NULL) {
        NULLPTR_WARN();
        return GetDummyMusic();
    }
    Mix_Music* mus = Mix_LoadMUS(fileName);
    if (mus == NULL) {
        TRACELOG(LOG_WARNING, "Failed to load music %s (%s)", fileName, Mix_GetError());
        return GetDummyMusic();
    }
    Music result = { 0 };
    result.stream.buffer = NULL;
    result.stream.processor = NULL;
    result.stream.channels = MIX_CHANNELS;
    result.stream.sampleRate = MIX_FREQ;
    result.stream.sampleSize = MIX_CHUNK_SIZE;
    result.mus = mus;
    result.looping = false;
    result.ctxType = Mix_GetMusicType(mus);
#if SDL_MIXER_VERSION_ATLEAST(2, 6, 0)
    result.looping = Mix_GetMusicLoopStartTime(mus) >= 0.0;
    double duration = Mix_MusicDuration(mus);
    if (duration < 0.0) {
        duration = 0.0;
        TRACELOG(LOG_WARNING, "Failed to get music duration (%s)", Mix_GetError());
    }
#else
    result.looping = false;
    result.duration = 0.0;
#endif
    result.duration = (float)duration;
    result.frameCount = (unsigned int)(duration * (double)result.stream.sampleRate);
    result.ctxData = NULL;
    return result;
#else
    return GetDummyMusic();
#endif
}

RLCAPI Music LoadMusicStreamFromMemory(const char *fileType, const unsigned char *data, int dataSize) {
#ifdef MIX_SUPPORT
    if (fileType == NULL || data == NULL) {
        NULLPTR_WARN();
        return GetDummyMusic();
    }
    // TODO
    return GetDummyMusic();
#else
    return GetDummyMusic();
#endif
}

RLCAPI bool IsMusicReady(Music music) {
#ifdef MIX_SUPPORT
    return (bool)music.mus;
#else
    return false;
#endif
}

RLCAPI void UnloadMusicStream(Music music) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    Mix_FreeMusic(music.mus);
    music.mus = NULL;
#endif
}

RLCAPI void PlayMusicStream(Music music) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    if (Mix_PlayMusic(music.mus, 0) < 0)
        TRACELOG(LOG_WARNING, "Failed to play music (%s)", Mix_GetError());
#endif
}

RLCAPI bool IsMusicStreamPlaying(Music music) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return false;
    }
    return (bool)Mix_PlayingMusic(); // Only 1 music channel is supported
#else
    return false;
#endif
}

RLCAPI void UpdateMusicStream(Music music) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    // Lol we don't need this
#endif
}

RLCAPI void StopMusicStream(Music music) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    Mix_HaltMusic();
#endif
}

RLCAPI void PauseMusicStream(Music music) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    Mix_PauseMusic();
#endif
}

RLCAPI void ResumeMusicStream(Music music) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    Mix_ResumeMusic();
#endif
}

RLCAPI void SeekMusicStream(Music music, float position) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    if (Mix_SetMusicPosition((double)position) < 0)
        TRACELOG(LOG_WARNING, "Failed to set music position (%s)", Mix_GetError());
#endif
}

RLCAPI void SetMusicVolume(Music music, float volume) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    Mix_VolumeMusic((int)(volume * (float)MIX_MAX_VOLUME));
#endif
}

RLCAPI void SetMusicPitch(Music music, float pitch) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    // TODO: speed change
#endif
}

RLCAPI void SetMusicPan(Music music, float pan) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return;
    }
    // Only for sound lol
#endif
}

RLCAPI float GetMusicTimeLength(Music music) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return 0.0f;
    }
    return music.duration;
#else
    return 0.0f;
#endif
}

RLCAPI float GetMusicTimePlayed(Music music) {
#ifdef MIX_SUPPORT
    if (music.mus == NULL) {
        NULLPTR_WARN();
        return 0.0f;
    }
#if SDL_MIXER_VERSION_ATLEAST(2, 6, 0)
    double result = Mix_GetMusicPosition(music.mus);
    if (result < 0.0) {
        TRACELOG(LOG_WARNING, "Failed to get music position (Feature is not supported)");
        return 0.0f;
    }
#else
    double result = 0.0;
#endif
    return (float)result;
#else
    return 0.0f;
#endif
}
#endif
