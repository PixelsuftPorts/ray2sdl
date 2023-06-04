#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>

RLCAPI void ClearBackground(Color color) {
    if (SDL_SetRenderDrawColor(rl.r, color.r, color.g, color.b, color.a) < 0)
        TRACELOG(LOG_WARNING, "Failed to set draw color (%s)", SDL_GetError());
    if (SDL_RenderClear(rl.r) < 0)
        TRACELOG(LOG_WARNING, "Failed to clear render (%s)", SDL_GetError());
}

#define APPLY_BLEND(color) (\
    SDL_SetRenderDrawBlendMode(rl.r, color.a >= 255 ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND) |\
    SDL_SetRenderDrawColor(rl.r, color.r, color.g, color.b, color.a))

#define BLEND_WARN() TRACELOG(LOG_WARNING, "Failed to apply blend with color (%s)", SDL_GetError())
#define DRAW_WARN() TRACELOG(LOG_WARNING, "Failed to draw (%s)", SDL_GetError())

RLCAPI void DrawPixel(int posX, int posY, Color color) {
    DrawPixelV((Vector2){ (float)posX, (float)posY }, color);
}

RLCAPI void DrawPixelV(Vector2 position, Color color) {
    if (APPLY_BLEND(color) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawPointF(rl.r, position.x, position.y) < 0)
        DRAW_WARN();
}
