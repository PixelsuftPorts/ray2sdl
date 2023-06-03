#include <raylib.h>
#include <raydef.h>
#include <raydark.h>

RLAPI void InitWindow(int width, int height, const char *title) {
    if (!rl.was_init) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
            // TODO
        } 
        rl.was_init = true;
    }
    // TODO: FLAG_WINDOW_ALWAYS_RUN emulation
    rl.w = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_HIDDEN | (rl.fl & FLAG_FULLSCREEN_MODE ? SDL_WINDOW_FULLSCREEN : 0) |
        (rl.fl & FLAG_WINDOW_RESIZABLE ? SDL_WINDOW_RESIZABLE : 0) | (rl.fl & FLAG_WINDOW_RESIZABLE ? SDL_WINDOW_RESIZABLE : 0) |
        (rl.fl & FLAG_WINDOW_UNDECORATED ? SDL_WINDOW_BORDERLESS : 0) | (rl.fl & FLAG_WINDOW_MINIMIZED ? SDL_WINDOW_MINIMIZED : 0) |
        (rl.fl & FLAG_WINDOW_MAXIMIZED ? SDL_WINDOW_MAXIMIZED : 0) | (rl.fl & FLAG_WINDOW_TOPMOST ? SDL_WINDOW_ALWAYS_ON_TOP : 0) | 
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
    while (SDL_PollEvent(&rl.event)) {
        switch (rl.event.type) {
            case SDL_QUIT: {
                rl.should_close = true;
                break;
            }
        }
    }
}

RLAPI bool WindowShouldClose(void) {
    PollEvents(); // hack
    return rl.should_close;
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

RLAPI bool IsWindowState(unsigned int flag) {
    return (bool)(rl.fl & flag);
}

RLAPI void SetWindowState(unsigned int flags) {
    rl.fl |= flags;
}

RLAPI void ClearWindowState(unsigned int flags) {
    rl.fl &= ~flags;
}

RLAPI void ClearBackground(Color color) {
    SDL_SetRenderDrawColor(rl.r, color.r, color.g, color.b, color.a);
    SDL_RenderClear(rl.r);
}

RLAPI void BeginDrawing(void) {

}

RLAPI void EndDrawing(void) {
    SDL_RenderPresent(rl.r);
}
