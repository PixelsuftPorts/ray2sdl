#include <stdio.h>
#include <raylib.h>
#include <raydef.h>
#include <raydark.h>
#include <rayconf.h>
#ifdef SUPPORT_TRACELOG
#include <stdarg.h>
#endif

// TODO: Error Handling For Every Function

RLAPI void InitWindow(int width, int height, const char *title) {
    if (rl.first_init) {
        rl.first_init = false;
        TRACELOG(LOG_INFO, "Initializing ray2sdl %s", RAYLIB_VERSION);
        TRACELOG(LOG_INFO, "Supported raylib modules:");
        TRACELOG(LOG_INFO, "     > bruh:...... loaded (mandatory)");
    }
    if (!rl.was_init) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
            TRACELOG(LOG_ERROR, "Failed to initialize SDL (%s)", SDL_GetError());
        } 
        rl.was_init = true;
    }
    rl.w = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_HIDDEN | ((rl.fl & FLAG_FULLSCREEN_MODE) ? SDL_WINDOW_FULLSCREEN : 0) |
        (rl.fl & FLAG_WINDOW_RESIZABLE ? SDL_WINDOW_RESIZABLE : 0) |
        (rl.fl & FLAG_WINDOW_UNDECORATED ? SDL_WINDOW_BORDERLESS : 0) |
        (rl.fl & FLAG_WINDOW_MINIMIZED ? SDL_WINDOW_MINIMIZED : 0) |
        (rl.fl & FLAG_WINDOW_MAXIMIZED ? SDL_WINDOW_MAXIMIZED : 0) |
        (rl.fl & FLAG_WINDOW_TOPMOST ? SDL_WINDOW_ALWAYS_ON_TOP : 0) | 
        (rl.fl & FLAG_WINDOW_HIGHDPI ? SDL_WINDOW_ALLOW_HIGHDPI : 0) | 
        (rl.fl & FLAG_WINDOW_UNFOCUSED ? 0 : (SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS))
    );
    if (rl.w == NULL) {
        TRACELOG(LOG_ERROR, "Failed to create window (%s)", SDL_GetError());
    }
    rl.r = SDL_CreateRenderer(
        rl.w,
        2, // TODO
        SDL_RENDERER_ACCELERATED | (rl.fl & FLAG_VSYNC_HINT ? SDL_RENDERER_PRESENTVSYNC : 0)
    );
    CheckDarkMode(rl.w);
    if (rl.r == NULL) {
        TRACELOG(LOG_ERROR, "Failed to create renderer (%s)", SDL_GetError());
    }
    else {
        int mon = GetCurrentMonitor();
        SDL_RendererInfo info;
        if (SDL_GetRendererInfo(rl.r, &info) < 0)
            TRACELOG(LOG_WARNING, "Failed to get renderer info (%s)", SDL_GetError());
        TRACELOG(LOG_INFO, "DISPLAY: Device initialized successfully");
        TRACELOG(LOG_INFO, "     > Display size: %ix%i", GetMonitorWidth(mon), GetMonitorHeight(mon));
        TRACELOG(LOG_INFO, "     > Screen size:  %ix%i", GetScreenWidth(), GetScreenHeight());
        TRACELOG(LOG_INFO, "     > Render size:  %ix%i", GetRenderWidth(), GetRenderHeight());
        TRACELOG(LOG_INFO, "RENDER: Information:");
        TRACELOG(LOG_INFO, "     > Name:             %s", info.name);
        TRACELOG(LOG_INFO, "     > Max texture size: %ix%i", info.max_texture_width, info.max_texture_width);
    }
    rl.should_close = false;
    rl.clip_ptr = NULL;
    if (!(rl.fl & FLAG_WINDOW_HIDDEN))
        SDL_ShowWindow(rl.w);
}

void PollEvents() {
    rl.w_resized = false;
    while (SDL_PollEvent(&rl.event)) {
        switch (rl.event.type) {
            case SDL_QUIT: {
                // rl.should_close = true;
                break;
            }
            case SDL_KEYDOWN: {
                if (rl.event.key.keysym.sym == SDLK_ESCAPE)
                    rl.should_close = true;
                break;
            }
            case SDL_WINDOWEVENT: {
                switch (rl.event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED: {
                        rl.w_resized = true;
                        break;
                    }
                    case SDL_WINDOWEVENT_MAXIMIZED: {
                        rl.fl |= FLAG_WINDOW_MAXIMIZED;
                        break;
                    }
                    case SDL_WINDOWEVENT_CLOSE: {
                        rl.should_close = true;
                        break;
                    }
                    case SDL_WINDOWEVENT_MINIMIZED: {
                        rl.fl |= FLAG_WINDOW_MINIMIZED;
                        break;
                    }
                    case SDL_WINDOWEVENT_RESTORED: {
                        rl.fl &= ~(IsWindowMinimized() ? FLAG_WINDOW_MINIMIZED : FLAG_WINDOW_MAXIMIZED);
                        break;
                    }
                    case SDL_WINDOWEVENT_SHOWN: {
                        rl.fl &= ~FLAG_WINDOW_HIDDEN;
                        break;
                    }
                    case SDL_WINDOWEVENT_HIDDEN: {
                        rl.fl |= FLAG_WINDOW_HIDDEN;
                        break;
                    }
                }
                break;
            }
        }
    }
}

void WaitEvents() {
    while (!(rl.fl & FLAG_WINDOW_ALWAYS_RUN) && IsWindowMinimized()) {
        PollEvents();
    }
    PollEvents();
}

RLAPI bool WindowShouldClose(void) {
    if (!rl.event_waiting)
        WaitEvents();
    if (rl.should_close) {
        rl.should_close = false;
        return true;
    }
    return false;
}

RLAPI void CloseWindow(void) {
    if (rl.clip_ptr) {
        RL_FREE(rl.clip_ptr);
        rl.clip_ptr = NULL;
    }
    if (rl.w) {
        SDL_DestroyWindow(rl.w);
        rl.w = NULL;
        rl.should_close = false;
    }
    if (rl.was_init) {
        SDL_Quit();
        rl.was_init = false;
    }
    TRACELOG(LOG_INFO, "Window closed successfully");
}

RLAPI bool IsWindowReady(void) {
    return (bool)rl.w;
}

RLAPI bool IsWindowFullscreen(void) {
    return (bool)(rl.fl & FLAG_FULLSCREEN_MODE);
}

RLAPI bool IsWindowHidden(void) {
    return (bool)(rl.fl & FLAG_WINDOW_HIDDEN);
}

RLAPI bool IsWindowMinimized(void) {
    return (bool)(rl.fl & FLAG_WINDOW_MINIMIZED);
}

RLAPI bool IsWindowMaximized(void) {
    return (bool)(rl.fl & FLAG_WINDOW_MAXIMIZED);
}

RLAPI bool IsWindowFocused(void) {
    return SDL_GetMouseFocus() == rl.w && SDL_GetKeyboardFocus() == rl.w;
}

RLAPI bool IsWindowResized(void) {
    return rl.w_resized;
}

RLAPI bool IsWindowState(unsigned int flag) {
    return (bool)(rl.fl & flag);
}

RLAPI void SetWindowState(unsigned int flags) {
    unsigned int diff = rl.fl ^ flags;
    rl.fl |= flags;
    if (diff & FLAG_WINDOW_HIDDEN)
        SDL_HideWindow(rl.w);
    if (diff & FLAG_FULLSCREEN_MODE) {
        if (SDL_SetWindowFullscreen(rl.w, SDL_WINDOW_FULLSCREEN) < 0) {
            TRACELOG(LOG_WARNING, "Failed to enable fullscreen (%s)", SDL_GetError());
        }
    }
    if (diff & FLAG_WINDOW_RESIZABLE)
        SDL_SetWindowResizable(rl.w, SDL_TRUE);
    if (diff & SDL_WINDOW_BORDERLESS)
        SDL_SetWindowBordered(rl.w, SDL_FALSE);
    if (diff & FLAG_WINDOW_MINIMIZED)
        SDL_MinimizeWindow(rl.w);
    if (diff & FLAG_WINDOW_MAXIMIZED)
        SDL_MaximizeWindow(rl.w);
    if (diff & FLAG_WINDOW_TOPMOST)
        SDL_SetWindowAlwaysOnTop(rl.w, SDL_TRUE);
}

RLAPI void ClearWindowState(unsigned int flags) {
    rl.fl &= ~flags;
    unsigned int diff = rl.fl ^ flags;
    if (diff & FLAG_WINDOW_HIDDEN)
        SDL_ShowWindow(rl.w);
    if (diff & FLAG_FULLSCREEN_MODE) {
        if (SDL_SetWindowFullscreen(rl.w, 0) < 0)
            TRACELOG(LOG_WARNING, "Failed to disable fullscreen (%s)", SDL_GetError());
    }
    if (diff & FLAG_WINDOW_RESIZABLE)
        SDL_SetWindowResizable(rl.w, SDL_FALSE);
    if (diff & SDL_WINDOW_BORDERLESS)
        SDL_SetWindowBordered(rl.w, SDL_TRUE);
    if ((diff & FLAG_WINDOW_MINIMIZED) || ((diff & FLAG_WINDOW_MAXIMIZED) && !(rl.fl & FLAG_WINDOW_MINIMIZED)))
        SDL_RestoreWindow(rl.w);
    if (diff & FLAG_WINDOW_TOPMOST)
        SDL_SetWindowAlwaysOnTop(rl.w, SDL_FALSE);
    if (diff & FLAG_WINDOW_UNFOCUSED)
        SDL_SetWindowInputFocus(rl.w);
}

RLAPI void ToggleFullscreen(void) {
    if (IsWindowFullscreen()) {
        rl.fl &= ~FLAG_FULLSCREEN_MODE;
        if (SDL_SetWindowFullscreen(rl.w, 0) < 0)
            TRACELOG(LOG_WARNING, "Failed to disable fullscreen (%s)", SDL_GetError());
    }
    else {
        rl.fl |= FLAG_FULLSCREEN_MODE;
        if (SDL_SetWindowFullscreen(rl.w, SDL_WINDOW_FULLSCREEN) < 0)
            TRACELOG(LOG_WARNING, "Failed to enable fullscreen (%s)", SDL_GetError());
    }
}

RLAPI void MaximizeWindow(void) {
    rl.fl |= FLAG_WINDOW_MAXIMIZED;
    SDL_MaximizeWindow(rl.w);
}

RLAPI void MinimizeWindow(void) {
    rl.fl |= FLAG_WINDOW_MINIMIZED;
    SDL_MinimizeWindow(rl.w);
}

RLAPI void RestoreWindow(void) {
    SDL_RestoreWindow(rl.w);
    rl.fl &= ~FLAG_WINDOW_MAXIMIZED;
    rl.fl &= ~FLAG_WINDOW_MINIMIZED;
}

RLAPI void SetWindowTitle(const char *title) {
    SDL_SetWindowTitle(rl.w, title);
}

RLAPI void SetWindowPosition(int x, int y) {
    SDL_SetWindowPosition(rl.w, x, y);
}

RLAPI void SetWindowMonitor(int monitor) {
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(monitor, &dm) < 0) { // Or SDL_GetDesktopDisplayMode?
        TRACELOG(LOG_WARNING, "Failed to get display mode (%s)", SDL_GetError());
        return;
    }
    if (SDL_SetWindowDisplayMode(rl.w, &dm) < 0) { // Is this right? I have no second monitor to test.
        TRACELOG(LOG_WARNING, "Failed to set display mode (%s)", SDL_GetError());
    }
}

RLAPI void SetWindowMinSize(int width, int height) {
    SDL_SetWindowMinimumSize(rl.w, width, height);
}

RLAPI void SetWindowSize(int width, int height) {
    SDL_SetWindowSize(rl.w, width, height);
}

RLAPI void SetWindowOpacity(float opacity) {
    if (SDL_SetWindowOpacity(rl.w, opacity) < 0) {
        TRACELOG(LOG_WARNING, "Failed to set window opacity (%s)", SDL_GetError());
    }
}

RLAPI void *GetWindowHandle(void) {
    void* handle = GetHandleBySDLWindow(rl.w);
    if (handle == NULL)
        TRACELOG(LOG_WARNING, "Failed to get window handle (%s)", SDL_GetError());
    return handle;
}

RLAPI int GetScreenWidth(void) {
    int width;
    SDL_GetWindowSize(rl.w, &width, NULL);
    return width;
}

RLAPI int GetScreenHeight(void) {
    int height;
    SDL_GetWindowSize(rl.w, NULL, &height);
    return height;
}

RLAPI int GetRenderWidth(void) {
    int width;
    if (SDL_GetRendererOutputSize(rl.r, &width, NULL) < 0)
        TRACELOG(LOG_WARNING, "Failed to get renderer output size (%s)", SDL_GetError());
    return width;
}

RLAPI int GetRenderHeight(void) {
    int height;
    if (SDL_GetRendererOutputSize(rl.r, NULL, &height) < 0)
        TRACELOG(LOG_WARNING, "Failed to get renderer output size (%s)", SDL_GetError());
    return height;
}

RLAPI int GetMonitorCount(void) {
    int result = SDL_GetNumVideoDisplays();
    if (result < 0)
        TRACELOG(LOG_WARNING, "Failed to get monitor count (%s)", SDL_GetError());
    return result;
}

RLAPI int GetCurrentMonitor(void) {
    int result = SDL_GetWindowDisplayIndex(rl.w);
    if (result < 0)
        TRACELOG(LOG_WARNING, "Failed to get current monitor index (%s)", SDL_GetError());
    return result;
}

RLAPI Vector2 GetMonitorPosition(int monitor) {
    SDL_Rect rect;
    if (SDL_GetDisplayBounds(monitor, &rect) < 0)
        TRACELOG(LOG_WARNING, "Failed to get monitor bounds (%s)", SDL_GetError());
    return VECLITERAL(Vector2){
        .x = (float)rect.x, .y = (float)rect.y
    };
}

RLAPI int GetMonitorWidth(int monitor) {
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(monitor, &dm) < 0)
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
    return dm.w;
}

RLAPI int GetMonitorHeight(int monitor) {
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(monitor, &dm) < 0)
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
    return dm.h;
}

RLAPI int GetMonitorPhysicalWidth(int monitor) {
    float hdpi;
    if (SDL_GetDisplayDPI(monitor, NULL, &hdpi, NULL) < 0)
        TRACELOG(LOG_WARNING, "Failed to get monitor DPI (%s)", SDL_GetError());
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(monitor, &dm) < 0)
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
    return (int)((float)dm.w * 25.35f / hdpi);
}

RLAPI int GetMonitorPhysicalHeight(int monitor) {
    float vdpi;
    if (SDL_GetDisplayDPI(monitor, NULL, NULL, &vdpi) < 0)
        TRACELOG(LOG_WARNING, "Failed to get monitor DPI (%s)", SDL_GetError());
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(monitor, &dm) < 0)
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
    return (int)((float)dm.h * 25.35f / vdpi);
}

RLAPI int GetMonitorRefreshRate(int monitor) {
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(monitor, &dm) < 0)
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
    return dm.refresh_rate;
}

RLAPI Vector2 GetWindowPosition(void) {
    int x, y;
    SDL_GetWindowPosition(rl.w, &x, &y);
    return VECLITERAL(Vector2){
        .x = (float)x, .y = (float)y
    };
}

RLAPI Vector2 GetWindowScaleDPI(void) {
    return VECLITERAL(Vector2){
        .x = GetRenderWidth() / GetScreenWidth(),
        .y = GetRenderHeight() / GetScreenHeight()
    };
}

RLAPI const char *GetMonitorName(int monitor) {
    const char* result = SDL_GetDisplayName(monitor);
    if (result == NULL)
        TRACELOG(LOG_WARNING, "Failed to get monitor name (%s)", SDL_GetError());
    return result;
}

RLAPI void SetClipboardText(const char *text) {
    if (SDL_SetClipboardText(text) < 0)
        TRACELOG(LOG_WARNING, "Failed to set clipboard text (%s)", SDL_GetError());
}

RLAPI const char *GetClipboardText(void) {
    if (rl.clip_ptr)
        RL_FREE(rl.clip_ptr);
    rl.clip_ptr = SDL_GetClipboardText();
    if (SDL_strlen(rl.clip_ptr) <= 0)
        TRACELOG(LOG_WARNING, "Failed to get clipboard text (%s)", SDL_GetError());
    return rl.clip_ptr;
}

RLAPI void EnableEventWaiting(void) {
    rl.event_waiting = true;
}

RLAPI void DisableEventWaiting(void) {
    rl.event_waiting = false;
}

RLAPI void SwapScreenBuffer(void) {
    rl.need_to_swap = false;
    SDL_RenderPresent(rl.r);
}

RLAPI void PollInputEvents(void) {
    PollEvents(); // Crude.
}

RLAPI void WaitTime(double seconds) {
    Uint64 timer_start = SDL_GetTicks64();
    while ((double)(SDL_GetTicks64() - timer_start) < seconds * 1000.0) {}
}

RLAPI void ShowCursor(void) {
    if (SDL_ShowCursor(SDL_ENABLE) < 0)
        TRACELOG(LOG_WARNING, "Failed to show cursor (%s)", SDL_GetError());
}

RLAPI void HideCursor(void) {
    if (SDL_ShowCursor(SDL_DISABLE) < 0)
        TRACELOG(LOG_WARNING, "Failed to hide cursor (%s)", SDL_GetError());
}

RLAPI bool IsCursorHidden(void) {
    int result = SDL_ShowCursor(SDL_QUERY);
    if (result < 0)
        TRACELOG(LOG_WARNING, "Failed to get if cursor is hidden (%s)", SDL_GetError());
    return result == SDL_DISABLE;
}

RLAPI void EnableCursor(void) {
    if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0) { // I don't this it's good. Maybe add to config different things?
        TRACELOG(LOG_WARNING, "Failed to enable cursor (%s)", SDL_GetError());
    }
}

RLAPI void DisableCursor(void) {
    if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
        TRACELOG(LOG_WARNING, "Failed to disable cursor (%s)", SDL_GetError());
}

RLAPI bool IsCursorOnScreen(void) {
    int x, y, w, h;
    SDL_GetMouseState(&x, &y);
    SDL_GetWindowSize(rl.w, &w, &h);
    return (x >= 0) && (y >= 0) && (x < w) && (y < h);
}

RLAPI void SetTargetFPS(int fps) {} // TODO this
RLAPI int GetFPS(void) {return 0;}
RLAPI float GetFrameTime(void) {return 1.0f / 60.0f;}

RLAPI double GetTime(void) {
    return (double)SDL_GetTicks64() / 1000.0; // TODO: from InitWindow
}

RLAPI int GetRandomValue(int min, int max) {
    if (min > max) {
        int tmp = max;
        max = min;
        min = tmp;
    }
    return (int)((float)rand() / (float)(RAND_MAX + 1) * (float)(max - min + 1)) + min;
}

RLAPI void SetRandomSeed(unsigned int seed) {
    srand(seed);
}

RLAPI void TakeScreenshot(const char *fileName) {
    // TODO
}

RLAPI void SetConfigFlags(unsigned int flags) {
    rl.fl |= flags;
}

RLAPI void TraceLog(int logType, const char *text, ...) {
    // TODO: log callbacks
#ifdef SUPPORT_TRACELOG
    if (logType < rl.log_level) return;

    va_list args;
    va_start(args, text);

    if (rl.traceLog) {
        rl.traceLog(logType, text, args);
        va_end(args);
        return;
    }

    char buffer[MAX_TRACELOG_MSG_LENGTH] = { 0 };
    switch (logType)
    {
        case LOG_TRACE: strcpy(buffer, "TRACE: "); break;
        case LOG_DEBUG: strcpy(buffer, "DEBUG: "); break;
        case LOG_INFO: strcpy(buffer, "INFO: "); break;
        case LOG_WARNING: strcpy(buffer, "WARNING: "); break;
        case LOG_ERROR: strcpy(buffer, "ERROR: "); break;
        case LOG_FATAL: strcpy(buffer, "FATAL: "); break;
        default: break;
    }

    unsigned int textSize = (unsigned int)strlen(text);
    memcpy(buffer + strlen(buffer), text, (textSize < (MAX_TRACELOG_MSG_LENGTH - 12))? textSize : (MAX_TRACELOG_MSG_LENGTH - 12));
    strcat(buffer, "\n");
    vprintf(buffer, args);
    fflush(stdout);

    if (logType == LOG_FATAL)
        exit(EXIT_FAILURE);
#endif
}

RLAPI void SetTraceLogLevel(int logLevel) {
    rl.log_level = logLevel;
}

RLAPI void ClearBackground(Color color) {
    if (SDL_SetRenderDrawColor(rl.r, color.r, color.g, color.b, color.a) < 0)
        TRACELOG(LOG_WARNING, "Failed to set draw color (%s)", SDL_GetError());
    if (SDL_RenderClear(rl.r) < 0)
        TRACELOG(LOG_WARNING, "Failed to clear render (%s)", SDL_GetError());
}

RLAPI void BeginDrawing(void) {
    rl.need_to_swap = true;
    if (SDL_SetRenderTarget(rl.r, NULL) < 0)
        TRACELOG(LOG_WARNING, "Failed to set main target (%s)", SDL_GetError());
}

RLAPI void EndDrawing(void) {
    if (rl.need_to_swap)
        SDL_RenderPresent(rl.r);
    if (rl.event_waiting)
        WaitEvents();
}
