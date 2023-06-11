#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <raydef.h>
#include <rayconf.h>
#include <raylib.h>
#ifdef MIX_SUPPORT
#include <SDL2/SDL_mixer.h>
#endif

RLCAPI void InitAudioDevice(void) {
#ifdef MIX_SUPPORT
    if (rl.mix_enabled)
        rl.mix_device_opened = Mix_OpenAudioDevice(
            MIX_FREQ, MIX_FORMAT, MIX_CHANNELS, MIX_CHANNELS, MIX_DEVICE, MIX_ALLOWED_CHANGES
        ) == 0;
    if (rl.mix_device_opened)
        TRACELOG(LOG_INFO, "Audio device opened");
    else
        TRACELOG(LOG_WARNING, "Failed to open audio device");
#endif
}

RLCAPI void CloseAudioDevice(void) {
#ifdef MIX_SUPPORT
    if (rl.mix_device_opened) {
        rl.mix_device_opened = false;
        Mix_CloseAudio();
    }
#endif
}

RLCAPI bool IsAudioDeviceReady(void) {
    return rl.mix_device_opened;
}

RLCAPI void SetMasterVolume(float volume) {
    Mix_MasterVolume((int)(volume * (float)MIX_MAX_VOLUME));
}
