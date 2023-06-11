#include <stdio.h>
#include <raylib.h>
#include <raydef.h>
#include <raydark.h>
#include <rayconf.h>
#ifdef SUPPORT_TRACELOG
#include <stdarg.h>
#endif
#ifdef IMG_SUPPORT
#include <SDL2/SDL_image.h>
#endif
#ifdef MIX_SUPPORT
#include <SDL2/SDL_mixer.h>
#endif

RLCAPI void InitWindow(int width, int height, const char *title) {
    if (!rl.not_first_init) {
        // TODO: clean need things when not first init maybe?
        unsigned int fl_temp = rl.fl;
        SDL_memset(&rl, 0, sizeof(rl));
        rl.fl = fl_temp;
        rl.not_first_init = true;
        rl.log_level = LOG_INFO;
        rl.exit_key = KEY_ESCAPE;
        TRACELOG(LOG_INFO, "Initializing ray2sdl %s", RAYLIB_VERSION);
        TRACELOG(LOG_INFO, "Supported raylib modules:");
        TRACELOG(LOG_INFO, "    > bruh:...... loaded (mandatory)");
    }
    if (!rl.was_init) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
            TRACELOG(LOG_ERROR, "Failed to initialize SDL (%s)", SDL_GetError());
        }
#ifdef IMG_SUPPORT
        rl.img_enabled = true;
        int img_formats = IMG_FORMATS;
        int img_init_formats = IMG_Init(img_formats);
        if (img_init_formats == 0) {
            rl.img_enabled = false;
            TRACELOG(LOG_WARNING, "Failed to init SDL2_image (%s)", IMG_GetError());
        }
        else if (img_init_formats != img_formats) {
            TRACELOG(LOG_WARNING, "Failed to init some image formats (%s)", IMG_GetError());
        }
        else TRACELOG(LOG_INFO, "SDL2_image initialized successfully");
#endif
#ifdef MIX_SUPPORT
        rl.mix_enabled = true;
        int mix_formats = MIX_FORMATS;
        int mix_init_formats = Mix_Init(img_formats);
        if (mix_init_formats == 0) {
            rl.mix_enabled = false;
            TRACELOG(LOG_WARNING, "Failed to init SDL2_mixer (%s)", Mix_GetError());
        }
        else if (mix_init_formats != mix_formats) {
            TRACELOG(LOG_WARNING, "Failed to init some mixer formats (%s)", Mix_GetError());
        }
        else TRACELOG(LOG_INFO, "SDL2_mixer initialized successfully");
        rl.mix_device_opened = false;
#endif
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
        RENDER_ID, // TODO
        SDL_RENDERER_ACCELERATED | (rl.fl & FLAG_VSYNC_HINT ? SDL_RENDERER_PRESENTVSYNC : 0)
    );
    CheckDarkMode(rl.w);
    if (rl.r == NULL) {
        TRACELOG(LOG_ERROR, "Failed to create renderer (%s)", SDL_GetError());
    }
    else {
        int mon = GetCurrentMonitor();
        SDL_RendererInfo info;
        TRACELOG(LOG_INFO, "DISPLAY: Device initialized successfully");
        TRACELOG(LOG_INFO, "    > Display size: %ix%i", GetMonitorWidth(mon), GetMonitorHeight(mon));
        TRACELOG(LOG_INFO, "    > Screen size:  %ix%i", GetScreenWidth(), GetScreenHeight());
        TRACELOG(LOG_INFO, "    > Render size:  %ix%i", GetRenderWidth(), GetRenderHeight());
        if (SDL_GetRendererInfo(rl.r, &info) < 0)
            TRACELOG(LOG_WARNING, "Failed to get renderer info (%s)", SDL_GetError());
        else {
            TRACELOG(LOG_INFO, "RENDER: Information:");
            TRACELOG(LOG_INFO, "    > Name:             %s", info.name);
            TRACELOG(LOG_INFO, "    > Max texture size: %ix%i", info.max_texture_width, info.max_texture_width);
        }
    }
    rl.kbd_array = SDL_GetKeyboardState(&rl.num_kbd_keys);
#ifdef HANDLE_KEY_PRESS
    rl.keypress_array = SDL_malloc(rl.num_kbd_keys);
    if (rl.keypress_array == NULL) {
        TRACELOG(LOG_WARNING, "Failed to allocate keypress array");
    }
#endif
    rl.touch_dev = SDL_GetTouchDevice(TOUCH_DEVICE_ID);
    if (rl.touch_dev > 0)
        TRACELOG(LOG_INFO, "TOUCH: Device with id %lld and index %i initialized successfully", (long long)rl.touch_dev, TOUCH_DEVICE_ID);
    rl.should_close = false;
    rl.clip_ptr = NULL;
#ifdef ENABLE_CLOCK
    ClockReset();
#endif
    if (!(rl.fl & FLAG_WINDOW_HIDDEN))
        SDL_ShowWindow(rl.w);
}

void PollEvents() {
#ifdef ENABLE_CLOCK
    ClockStep();
#endif
    rl.w_resized = false;
    rl.wheel_move.x = rl.wheel_move.y = 0.0f;
    SDL_memset(rl.mousepress_array, 0, 8);
#ifdef HANDLE_KEY_PRESS
    if (rl.keypress_array)
        SDL_memset(rl.keypress_array, 0, rl.num_kbd_keys);
#endif
    while (SDL_PollEvent(&rl.event)) {
        switch (rl.event.type) {
            case SDL_QUIT: {
                // rl.should_close = true;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
#ifndef EMULATE_MOUSE_WITH_TOUCH
                if (rl.event.button.which == SDL_TOUCH_MOUSEID)
                    break;
#endif 
                rl.mousepress_array[rl.event.button.button] = 1;
                break;
            }
            case SDL_MOUSEBUTTONUP: {
#ifndef EMULATE_MOUSE_WITH_TOUCH
                if (rl.event.button.which == SDL_TOUCH_MOUSEID)
                    break;
#endif 
                rl.mousepress_array[rl.event.button.button] = 2; // TODO: handle in different array in case if released at the same frame
                break;
            }
            case SDL_KEYDOWN: {
                if (GET_KEY_INT(rl.event.key.keysym) == rl.exit_key)
                    rl.should_close = true;
#ifdef HANDLE_KEY_PRESS
                if (rl.keypress_array && !rl.event.key.repeat)
                    rl.keypress_array[rl.event.key.keysym.scancode] = 1;
#endif
                break;
            }
            case SDL_KEYUP: {
#ifdef HANDLE_KEY_PRESS
                if (rl.keypress_array)
                    rl.keypress_array[rl.event.key.keysym.scancode] = 2;
#endif
                break;
            }
            case SDL_WINDOWEVENT: {
                switch (rl.event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED: {
                        rl.w_resized = true;
                        break;
                    }
                    case SDL_WINDOWEVENT_SIZE_CHANGED: {
                        if (rl.z_en)
                            UpdateCameraTexture();
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
            case SDL_MOUSEWHEEL: {
#if SDL_VERSION_ATLEAST(2, 0, 18)
                rl.wheel_move.x += rl.event.wheel.preciseX;
                rl.wheel_move.y += rl.event.wheel.preciseY;
#else
                rl.wheel_move.x += (float)rl.event.wheel.x;
                rl.wheel_move.y += (float)rl.event.wheel.y;
#endif
                break;
            }
            case SDL_DROPFILE: {
#ifdef SUPPORT_FILES_DROPPING
                RegisterFileDrop(rl.event.drop.file);
#else
                if (rl.event.drop.file)
                    SDL_free(rl.event.drop.file);
                rl.event.drop.file = NULL;
#endif
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

RLCAPI bool WindowShouldClose(void) {
    if (!rl.event_waiting)
        WaitEvents();
    if (rl.should_close) {
        rl.should_close = false;
        return true;
    }
    return false;
}

RLCAPI void CloseWindow(void) {
#ifdef SUPPORT_FILES_DROPPING
    if (rl.drops.capacity && rl.drops.paths)
        UnloadDroppedFiles(rl.drops);
#endif
#ifdef HANDLE_KEY_PRESS
    if (rl.keypress_array) {
        SDL_free(rl.keypress_array);
        rl.keypress_array = NULL;
    }
#endif
    if (rl.cursor) {
        SDL_FreeCursor(rl.cursor);
        rl.cursor = NULL;
    }
    if (rl.clip_ptr) {
        SDL_free(rl.clip_ptr);
        rl.clip_ptr = NULL;
    }
    if (rl.w) {
        SDL_DestroyWindow(rl.w);
        rl.w = NULL;
        rl.should_close = false;
    }
#ifdef MIX_SUPPORT
    if (rl.mix_enabled) {
        rl.mix_enabled = false;
        Mix_Quit();
    }
#endif
#ifdef IMG_SUPPORT
    if (rl.img_enabled) {
        rl.img_enabled = false;
        IMG_Quit();
    }
#endif
    if (rl.was_init) {
        SDL_Quit();
        rl.was_init = false;
    }
    TRACELOG(LOG_INFO, "Window closed successfully");
}

RLCAPI bool IsWindowReady(void) {
    return (bool)rl.w;
}

RLCAPI bool IsWindowFullscreen(void) {
    return (bool)(rl.fl & FLAG_FULLSCREEN_MODE);
}

RLCAPI bool IsWindowHidden(void) {
    return (bool)(rl.fl & FLAG_WINDOW_HIDDEN);
}

RLCAPI bool IsWindowMinimized(void) {
    return (bool)(rl.fl & FLAG_WINDOW_MINIMIZED);
}

RLCAPI bool IsWindowMaximized(void) {
    return (bool)(rl.fl & FLAG_WINDOW_MAXIMIZED);
}

RLCAPI bool IsWindowFocused(void) {
    return SDL_GetMouseFocus() == rl.w && SDL_GetKeyboardFocus() == rl.w;
}

RLCAPI bool IsWindowResized(void) {
    return rl.w_resized;
}

RLCAPI bool IsWindowState(unsigned int flag) {
    return (bool)(rl.fl & flag);
}

RLCAPI void SetWindowState(unsigned int flags) {
    unsigned int diff = rl.fl ^ flags;
    rl.fl |= flags;
    if (diff & FLAG_WINDOW_HIDDEN)
        SDL_HideWindow(rl.w);
    if (diff & FLAG_FULLSCREEN_MODE) {
        if (SDL_SetWindowFullscreen(rl.w, SDL_WINDOW_FULLSCREEN) < 0) {
            TRACELOG(LOG_WARNING, "Failed to enable fullscreen (%s)", SDL_GetError());
        }
    }
    if (diff & FLAG_VSYNC_HINT) {
        if (SDL_RenderSetVSync(rl.r, 1) != 0)
            TRACELOG(LOG_WARNING, "Failed to turn on vertical sync");
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

RLCAPI void ClearWindowState(unsigned int flags) {
    rl.fl &= ~flags;
    unsigned int diff = rl.fl ^ flags;
    if (diff & FLAG_WINDOW_HIDDEN)
        SDL_ShowWindow(rl.w);
    if (diff & FLAG_FULLSCREEN_MODE) {
        if (SDL_SetWindowFullscreen(rl.w, 0) < 0)
            TRACELOG(LOG_WARNING, "Failed to disable fullscreen (%s)", SDL_GetError());
    }
    if (diff & FLAG_VSYNC_HINT) {
        if (SDL_RenderSetVSync(rl.r, 0) != 0)
            TRACELOG(LOG_WARNING, "Failed to turn off vertical sync");
    }
    if (diff & FLAG_WINDOW_RESIZABLE)
        SDL_SetWindowResizable(rl.w, SDL_FALSE);
    if (diff & SDL_WINDOW_BORDERLESS)
        SDL_SetWindowBordered(rl.w, SDL_TRUE);
    if ((diff & FLAG_WINDOW_MINIMIZED) || ((diff & FLAG_WINDOW_MAXIMIZED) && !(rl.fl & FLAG_WINDOW_MINIMIZED)))
        SDL_RestoreWindow(rl.w);
    if (diff & FLAG_WINDOW_TOPMOST)
        SDL_SetWindowAlwaysOnTop(rl.w, SDL_FALSE);
    if (diff & FLAG_WINDOW_UNFOCUSED) {
        if (SDL_SetWindowInputFocus(rl.w) < 0)
            TRACELOG(LOG_WARNING, "Failed to set input focus (%s)", SDL_GetError());
    }
}

RLCAPI void ToggleFullscreen(void) {
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

RLCAPI void MaximizeWindow(void) {
    rl.fl |= FLAG_WINDOW_MAXIMIZED;
    SDL_MaximizeWindow(rl.w);
}

RLCAPI void MinimizeWindow(void) {
    rl.fl |= FLAG_WINDOW_MINIMIZED;
    SDL_MinimizeWindow(rl.w);
}

RLCAPI void RestoreWindow(void) {
    SDL_RestoreWindow(rl.w);
    rl.fl &= ~FLAG_WINDOW_MAXIMIZED;
    rl.fl &= ~FLAG_WINDOW_MINIMIZED;
}

RLCAPI void SetWindowTitle(const char *title) {
    SDL_SetWindowTitle(rl.w, title);
}

RLCAPI void SetWindowPosition(int x, int y) {
    SDL_SetWindowPosition(rl.w, x, y);
}

RLCAPI void SetWindowMonitor(int monitor) {
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(monitor, &dm) < 0) { // Or SDL_GetDesktopDisplayMode?
        TRACELOG(LOG_WARNING, "Failed to get display mode (%s)", SDL_GetError());
        return;
    }
    if (SDL_SetWindowDisplayMode(rl.w, &dm) < 0) // Is this right? I have no second monitor to test.
        TRACELOG(LOG_WARNING, "Failed to set display mode (%s)", SDL_GetError());
}

RLCAPI void SetWindowMinSize(int width, int height) {
    SDL_SetWindowMinimumSize(rl.w, width, height);
}

RLCAPI void SetWindowSize(int width, int height) {
    SDL_SetWindowSize(rl.w, width, height);
}

RLCAPI void SetWindowOpacity(float opacity) {
    if (SDL_SetWindowOpacity(rl.w, opacity) < 0)
        TRACELOG(LOG_WARNING, "Failed to set window opacity (%s)", SDL_GetError());
}

RLCAPI void *GetWindowHandle(void) {
    void* handle = GetHandleBySDLWindow(rl.w);
    if (handle == NULL)
        TRACELOG(LOG_WARNING, "Failed to get window handle (%s)", SDL_GetError());
    return handle;
}

RLCAPI int GetScreenWidth(void) {
    int width;
    SDL_GetWindowSize(rl.w, &width, NULL);
    return width;
}

RLCAPI int GetScreenHeight(void) {
    int height;
    SDL_GetWindowSize(rl.w, NULL, &height);
    return height;
}

RLCAPI int GetRenderWidth(void) {
    int width;
    if (SDL_GetRendererOutputSize(rl.r, &width, NULL) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get renderer output size (%s)", SDL_GetError());
        return 0;
    }
    return width;
}

RLCAPI int GetRenderHeight(void) {
    int height;
    if (SDL_GetRendererOutputSize(rl.r, NULL, &height) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get renderer output size (%s)", SDL_GetError());
        return 0;
    }
    return height;
}

RLCAPI int GetMonitorCount(void) {
    int result = SDL_GetNumVideoDisplays();
    if (result < 0) {
        TRACELOG(LOG_WARNING, "Failed to get monitor count (%s)", SDL_GetError());
        return 0;
    }
    return result;
}

RLCAPI int GetCurrentMonitor(void) {
    int result = SDL_GetWindowDisplayIndex(rl.w);
    if (result < 0) {
        TRACELOG(LOG_WARNING, "Failed to get current monitor index (%s)", SDL_GetError());
        return 0;
    }
    return result;
}

RLCAPI Vector2 GetMonitorPosition(int monitor) {
    SDL_Rect rect;
    if (SDL_GetDisplayBounds(monitor, &rect) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get monitor bounds (%s)", SDL_GetError());
    }
    return VECLITERAL(Vector2){
        .x = (float)rect.x, .y = (float)rect.y
    };
}

RLCAPI int GetMonitorWidth(int monitor) {
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(monitor, &dm) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
        return 0;
    }
    return dm.w;
}

RLCAPI int GetMonitorHeight(int monitor) {
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(monitor, &dm) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
        return 0;
    }
    return dm.h;
}

RLCAPI int GetMonitorPhysicalWidth(int monitor) {
    float hdpi;
    if (SDL_GetDisplayDPI(monitor, NULL, &hdpi, NULL) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get monitor horizontal DPI (%s)", SDL_GetError());
        return 0;
    }
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(monitor, &dm) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
        return 0;
    }
    return (int)((float)dm.w * 25.35f / hdpi);
}

RLCAPI int GetMonitorPhysicalHeight(int monitor) {
    float vdpi;
    if (SDL_GetDisplayDPI(monitor, NULL, NULL, &vdpi) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get monitor vertical DPI (%s)", SDL_GetError());
        return 0;
    }
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(monitor, &dm) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
        return 0;
    }
    return (int)((float)dm.h * 25.35f / vdpi);
}

RLCAPI int GetMonitorRefreshRate(int monitor) {
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(monitor, &dm) < 0) {
        TRACELOG(LOG_WARNING, "Failed to get monitor display mode (%s)", SDL_GetError());
        return 0;
    }
    return dm.refresh_rate;
}

RLCAPI Vector2 GetWindowPosition(void) {
    int x, y;
    SDL_GetWindowPosition(rl.w, &x, &y);
    return VECLITERAL(Vector2){
        .x = (float)x, .y = (float)y
    };
}

RLCAPI Vector2 GetWindowScaleDPI(void) {
    return VECLITERAL(Vector2){
        .x = GetRenderWidth() / GetScreenWidth(),
        .y = GetRenderHeight() / GetScreenHeight()
    };
}

RLCAPI const char *GetMonitorName(int monitor) {
    const char* result = SDL_GetDisplayName(monitor);
    if (result == NULL) {
        TRACELOG(LOG_WARNING, "Failed to get monitor name (%s)", SDL_GetError());
        return "";
    }
    return result;
}

RLCAPI void SetClipboardText(const char *text) {
    if (SDL_SetClipboardText(text) < 0)
        TRACELOG(LOG_WARNING, "Failed to set clipboard text (%s)", SDL_GetError());
}

RLCAPI const char *GetClipboardText(void) {
    if (rl.clip_ptr)
        SDL_free(rl.clip_ptr);
    rl.clip_ptr = SDL_GetClipboardText();
    if (SDL_strlen(rl.clip_ptr) <= 0) {
        TRACELOG(LOG_WARNING, "Failed to get clipboard text (%s)", SDL_GetError());
        return NULL;
    }
    return rl.clip_ptr;
}

RLCAPI void EnableEventWaiting(void) {
    rl.event_waiting = true;
}

RLCAPI void DisableEventWaiting(void) {
    rl.event_waiting = false;
}

RLCAPI void SwapScreenBuffer(void) {
    rl.need_to_swap = false;
    SDL_RenderPresent(rl.r);
}

RLCAPI void PollInputEvents(void) {
    PollEvents(); // Crude.
}

RLCAPI void ShowCursor(void) {
    if (SDL_ShowCursor(SDL_ENABLE) < 0)
        TRACELOG(LOG_WARNING, "Failed to show cursor (%s)", SDL_GetError());
}

RLCAPI void HideCursor(void) {
    if (SDL_ShowCursor(SDL_DISABLE) < 0)
        TRACELOG(LOG_WARNING, "Failed to hide cursor (%s)", SDL_GetError());
}

RLCAPI bool IsCursorHidden(void) {
    int result = SDL_ShowCursor(SDL_QUERY);
    if (result < 0) {
        TRACELOG(LOG_WARNING, "Failed to get if cursor is hidden (%s)", SDL_GetError());
        return false;
    }
    return result == SDL_DISABLE;
}

RLCAPI void EnableCursor(void) {
    if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0) { // I don't this it's good. Maybe add to config different things?
        TRACELOG(LOG_WARNING, "Failed to enable cursor (%s)", SDL_GetError());
    }
}

RLCAPI void DisableCursor(void) {
    if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
        TRACELOG(LOG_WARNING, "Failed to disable cursor (%s)", SDL_GetError());
}

RLCAPI bool IsCursorOnScreen(void) {
    int x, y, w, h;
    SDL_GetMouseState(&x, &y);
    SDL_GetWindowSize(rl.w, &w, &h);
    return (x >= 0) && (y >= 0) && (x < w) && (y < h);
}

RLCAPI double GetTime(void) {
    return (double)SDL_GetTicks64() / 1000.0;
}

RLCAPI int GetRandomValue(int min, int max) {
    if (min > max) {
        int tmp = max;
        max = min;
        min = tmp;
    }
    return (int)((float)((double)rand() / (double)(RAND_MAX + 1)) * (float)(max - min + 1)) + min;
}

RLCAPI void SetRandomSeed(unsigned int seed) {
    srand(seed);
}

RLCAPI void TakeScreenshot(const char *fileName) {
    // TODO
}

RLCAPI void SetConfigFlags(unsigned int flags) {
    rl.fl |= flags;
}

RLCAPI void TraceLog(int logType, const char *text, ...) {
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
        case LOG_TRACE: SDL_strlcpy(buffer, "TRACE: ", MAX_TRACELOG_MSG_LENGTH); break;
        case LOG_DEBUG: SDL_strlcpy(buffer, "DEBUG: ", MAX_TRACELOG_MSG_LENGTH); break;
        case LOG_INFO: SDL_strlcpy(buffer, "INFO: ", MAX_TRACELOG_MSG_LENGTH); break;
        case LOG_WARNING: SDL_strlcpy(buffer, "WARNING: ", MAX_TRACELOG_MSG_LENGTH); break;
        case LOG_ERROR: SDL_strlcpy(buffer, "ERROR: ", MAX_TRACELOG_MSG_LENGTH); break;
        case LOG_FATAL: SDL_strlcpy(buffer, "FATAL: ", MAX_TRACELOG_MSG_LENGTH); break;
        default: break;
    }

    unsigned int textSize = (unsigned int)SDL_strlen(text);
    SDL_memcpy(buffer + SDL_strlen(buffer), text, (textSize < (MAX_TRACELOG_MSG_LENGTH - 12))? textSize : (MAX_TRACELOG_MSG_LENGTH - 12));
    SDL_strlcat(buffer, "\n", MAX_TRACELOG_MSG_LENGTH);
    vprintf(buffer, args);
    fflush(stdout);

    if (logType == LOG_FATAL)
        exit(EXIT_FAILURE);
#endif
}

RLCAPI void SetTraceLogLevel(int logLevel) {
    rl.log_level = logLevel;
}

RLCAPI void *MemAlloc(unsigned int size) {
    void *ptr = SDL_calloc(size, 1);
    if (ptr == NULL)
        TRACELOG(LOG_ERROR, "Failed to allocate memory");
    return ptr;
}

RLCAPI void *MemRealloc(void *ptr, unsigned int size) {
    void *ret = SDL_realloc(ptr, size);
    if (ret == NULL)
        TRACELOG(LOG_ERROR, "Failed to reallocate memory");
    return ret;
}

RLCAPI void MemFree(void *ptr) {
    SDL_free(ptr);
}

RLCAPI void OpenURL(const char *url) {
    if (SDL_OpenURL(url) < 0)
        TRACELOG(LOG_WARNING, "Failed to open URL (%s)", SDL_GetError());
}

RLCAPI void BeginDrawing(void) {
    rl.need_to_swap = true;
    if (SDL_SetRenderTarget(rl.r, NULL) < 0)
        TRACELOG(LOG_WARNING, "Failed to set main target (%s)", SDL_GetError());
}

RLCAPI void EndDrawing(void) {
    if (rl.need_to_swap)
        SDL_RenderPresent(rl.r);
    if (rl.event_waiting)
        WaitEvents();
}
