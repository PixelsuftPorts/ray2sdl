#include <raylib.h>
#include <raydef.h>
#include <raydark.h>

RLAPI void InitWindow(int width, int height, const char *title) {
    if (!rl.was_init) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
            // TODO
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
        // TODO
    }
    rl.r = SDL_CreateRenderer(
        rl.w,
        -1, // TODO
        SDL_RENDERER_ACCELERATED | (rl.fl & FLAG_VSYNC_HINT ? SDL_RENDERER_PRESENTVSYNC : 0)
    );
    if (rl.r == NULL) {
        // TODO
    }
    CheckDarkMode(rl.w);
    rl.should_close = false;
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

RLAPI bool WindowShouldClose(void) {
    while (!(rl.fl & FLAG_WINDOW_ALWAYS_RUN) && IsWindowMinimized()) {
        PollEvents();
    }
    PollEvents();
    if (rl.should_close) {
        rl.should_close = false;
        return true;
    }
    return false;
}

RLAPI void CloseWindow(void) {
    if (rl.w) {
        SDL_DestroyWindow(rl.w);
        rl.w = NULL;
        rl.should_close = false;
    }
    if (rl.was_init) {
        SDL_Quit();
        rl.was_init = false;
    }
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
    if (diff & FLAG_FULLSCREEN_MODE)
        SDL_SetWindowFullscreen(rl.w, SDL_WINDOW_FULLSCREEN);
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
    if (diff & FLAG_FULLSCREEN_MODE)
        SDL_SetWindowFullscreen(rl.w, 0);
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
        SDL_SetWindowFullscreen(rl.w, 0);
    }
    else {
        rl.fl |= FLAG_FULLSCREEN_MODE;
        SDL_SetWindowFullscreen(rl.w, SDL_WINDOW_FULLSCREEN);
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

RLAPI void ClearBackground(Color color) {
    SDL_SetRenderDrawColor(rl.r, color.r, color.g, color.b, color.a);
    SDL_RenderClear(rl.r);
}

RLAPI void BeginDrawing(void) {
    SDL_SetRenderTarget(rl.r, NULL);
}

RLAPI void EndDrawing(void) {
    SDL_RenderPresent(rl.r);
}
