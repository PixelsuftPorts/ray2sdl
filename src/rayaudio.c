#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <raydef.h>
#include <rayconf.h>
#include <raylib.h>
#ifdef MIX_SUPPORT
#include <SDL2/SDL_mixer.h>
#endif

#ifdef MIX_IMPL
RLCAPI void InitAudioDevice(void) {
#ifdef MIX_SUPPORT
    if (rl.mix_enabled)
        rl.mix_device_opened = !Mix_OpenAudioDevice(
            MIX_FREQ, MIX_FORMAT, MIX_CHANNELS, MIX_CHUNK_SIZE, MIX_DEVICE, MIX_ALLOWED_CHANGES
        );
    if (rl.mix_device_opened) {
        TRACELOG(LOG_INFO, "AUDIO: Device initialized successfully");
        int freq, channels;
        if (Mix_QuerySpec(&freq, NULL, &channels) < 0)
            TRACELOG(LOG_WARNING, "Failed to query audio device specs (%s)", Mix_GetError());
        else {
            TRACELOG(LOG_INFO, "    > Backend:       %s", SDL_GetCurrentAudioDriver());
            TRACELOG(LOG_INFO, "    > Channels:      %i", channels);
            TRACELOG(LOG_INFO, "    > Sample rate:   %i", freq);
            TRACELOG(LOG_INFO, "    > Periods size:  %i", MIX_CHUNK_SIZE);
        }
        char* dev_name;
        SDL_AudioSpec dev_spec;
        if (!SDL_GetDefaultAudioInfo(&dev_name, &dev_spec, 0) && dev_name != NULL) {
            TRACELOG(LOG_INFO, "    > Device name:   %s", dev_name);
            SDL_free(dev_name);
        }
        else {
            TRACELOG(LOG_WARNING, "Failed to query default audio device specs (%s)", SDL_GetError());
        }
    }
    else
        TRACELOG(LOG_WARNING, "Failed to open audio device (%s)", Mix_GetError());
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
#ifdef MIX_SUPPORT
#if SDL_MIXER_VERSION_ATLEAST(2, 6, 0)
    Mix_MasterVolume((int)(volume * (float)MIX_MAX_VOLUME));
#endif
#endif
}
#endif
