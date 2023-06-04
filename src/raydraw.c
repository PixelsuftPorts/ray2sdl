#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>
#include <raygfx.h>

// TODO: make raygfx.c normal

RLCAPI void ClearBackground(Color color) {
    if (SDL_SetRenderDrawColor(rl.r, color.r, color.g, color.b, color.a) < 0)
        TRACELOG(LOG_WARNING, "Failed to set draw color (%s)", SDL_GetError());
    if (SDL_RenderClear(rl.r) < 0)
        TRACELOG(LOG_WARNING, "Failed to clear render (%s)", SDL_GetError());
}

RLCAPI void DrawPixel(int posX, int posY, Color color) {
    DrawPixelV(VECLITERAL(Vector2){ (float)posX, (float)posY }, color);
}

RLCAPI void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color) {
    DrawLineV(
        VECLITERAL(Vector2){ (float)startPosX, (float)startPosY },
        VECLITERAL(Vector2){ (float)endPosX, (float)endPosY }, color
    );
}

RLCAPI void DrawPixelV(Vector2 position, Color color) {
    if (APPLY_BLEND(color) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawPointF(rl.r, position.x, position.y) < 0)
        DRAW_WARN();
}

RLCAPI void DrawLineV(Vector2 startPos, Vector2 endPos, Color color) {
    if (APPLY_BLEND(color) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawLineF(rl.r, startPos.x, startPos.y, endPos.x, endPos.y) < 0)
        DRAW_WARN();
}

void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color) {
    if (thickLineRGBA(
        (Sint16)startPos.x, (Sint16)startPos.y, (Sint16)endPos.x, (Sint16)endPos.y,
        (Uint8)thick, color.r, color.g, color.b, color.a
    ) < 0)
        DRAW_WARN();
}
