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
    rl.w = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_HIDDEN
    );
    if (rl.w == NULL) {
        // TODO
    }
    rl.r = SDL_CreateRenderer(
        rl.w,
        -1, // TODO
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (rl.r == NULL) {
        // TODO
    }
    CheckDarkMode(rl.w);
    rl.should_close = false;
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

RLAPI void ClearBackground(Color color) {
    SDL_SetRenderDrawColor(rl.r, color.r, color.g, color.b, color.a);
    SDL_RenderClear(rl.r);
}

RLAPI void BeginDrawing(void) {

}

RLAPI void EndDrawing(void) {
    SDL_RenderPresent(rl.r);
}
