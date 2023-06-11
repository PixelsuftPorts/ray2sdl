#include <raylib.h>
#include <raydef.h>
#include <raymath.h>
#include <rayconf.h>
#include <raygfx.h>

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
    return GetDummyMusic();
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
    return GetDummyMusic();
#else
    return GetDummyMusic();
#endif
}

RLCAPI bool IsMusicReady(Music music) {
    return false;
}

RLCAPI void UnloadMusicStream(Music music) {}

RLCAPI void PlayMusicStream(Music music) {}

RLCAPI bool IsMusicStreamPlaying(Music music) {
    return false;
}

RLCAPI void UpdateMusicStream(Music music) {}

RLCAPI void StopMusicStream(Music music) {}

RLCAPI void PauseMusicStream(Music music) {}

RLCAPI void ResumeMusicStream(Music music) {}

RLCAPI void SeekMusicStream(Music music, float position) {}

RLCAPI void SetMusicVolume(Music music, float volume) {}

RLCAPI void SetMusicPitch(Music music, float pitch) {}

RLCAPI void SetMusicPan(Music music, float pan) {}

RLCAPI float GetMusicTimeLength(Music music) {
    return 0.0f;
}

RLCAPI float GetMusicTimePlayed(Music music) {
    return 0.0f;
}
#endif
