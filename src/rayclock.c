#include <raydef.h>
#include <raylib.h>
#include <rayconf.h>

RLCAPI void WaitTime(double seconds) {
    Uint64 timer_start = SDL_GetTicks64();
    while ((double)(SDL_GetTicks64() - timer_start) < seconds * 1000.0) {}
    // ClockReset();
}

RLCAPI void ClockReset(void) {
#ifdef ENABLE_CLOCK
    rl.clock_freq = (float)SDL_GetPerformanceFrequency();
    rl.clock_delta = 0.0f;
    rl.clock_last_tick = SDL_GetPerformanceCounter();
#endif
}

RLCAPI void ClockStep(void) {
#ifdef ENABLE_CLOCK
    Uint64 now = SDL_GetPerformanceCounter();
    while (rl.fps_limit > 0.0f && rl.clock_freq > rl.fps_limit * (float)(now - rl.clock_last_tick)) {
        now = SDL_GetPerformanceCounter();
    }
    rl.clock_delta = (float)(now - rl.clock_last_tick) / rl.clock_freq;
    rl.clock_last_tick = now;
#endif
    /*uint64_t now = SDL_GetPerformanceCounter();
    if (this->fps_limit && this->freq > this->fps_limit * (float)(now - this->last_tick))
        return false;
    this->dt = (float)(now - this->last_tick) / this->freq * this->speed_hack;
    this->last_tick = now;*/
}

RLCAPI void SetTargetFPS(int fps) {
#ifdef ENABLE_CLOCK
    rl.fps_limit = (float)fps;
#endif
}

RLCAPI int GetFPS(void) {
#ifdef ENABLE_CLOCK
    return (int)(1.0f / rl.clock_delta);
#else
    return 60;
#endif
}

RLCAPI float GetFrameTime(void) {
#ifdef ENABLE_CLOCK
    return rl.clock_delta;
#else
    return 1.0f / 60.0f;
#endif
}
