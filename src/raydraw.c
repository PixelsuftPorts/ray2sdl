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

RLCAPI void DrawCircle(int centerX, int centerY, float radius, Color color) {
    DrawCircleV(VECLITERAL(Vector2) { (float)centerX, (float)centerY }, radius, color);
}

RLCAPI void DrawRectangle(int posX, int posY, int width, int height, Color color) {
    DrawRectangleRec(
        VECLITERAL(Rectangle){ (float)posX, (float)posY, (float)width, (float)height },
        color
    );
}

RLCAPI void DrawRectangleV(Vector2 position, Vector2 size, Color color) {
    DrawRectangleRec(VECLITERAL(Rectangle) { position.x, position.y, size.x, size.y }, color);
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

RLCAPI void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color) {
    if (thick <= 0.0f)
        return;
    if (thickLineRGBA(
        (Sint16)startPos.x, (Sint16)startPos.y, (Sint16)endPos.x, (Sint16)endPos.y,
        thick, color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawLineStrip(Vector2 *points, int pointCount, Color color) {
    if (APPLY_BLEND(color) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawLinesF(rl.r, (const SDL_FPoint*)points, pointCount) < 0)
        DRAW_WARN();
}

RLCAPI void DrawCircleGradient(int centerX, int centerY, float radius, Color color1, Color color2) {
    // TODO: what abaout transparency?
    if (radius <= 0.0f)
        radius = 0.1f;
    float speed[4] = { (float)(color1.r - color2.r) / radius, (float)(color1.g - color2.g) / radius,
                    (float)(color1.b - color2.b) / radius, (float)(color1.a - color2.a) / radius };
    float color[4] = { (float)color2.r, (float)color2.g, (float)color2.b, (float)color2.a };
    for (float i = radius; i >= 1.0f; i -= 1.0f) {
        DrawCircleV(
            VECLITERAL(Vector2){ (float)centerX, (float)centerY }, i,
            CLITERAL(Color) { (unsigned char)color[0], (unsigned char)color[1],
            (unsigned char)color[2], (unsigned char)color[3] }
        );
        color[0] += speed[0];
        color[1] += speed[1];
        color[2] += speed[2];
        color[3] += speed[3];
    }
}

RLCAPI void DrawCircleSector(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color) {
    if (radius <= 0.0f)
        radius = 0.1f;
    startAngle -= 135.0f;
    endAngle -= 135.0f;
    if (startAngle < 0.0f)
        startAngle += 360.0f;
    if (endAngle < 0.0f)
        endAngle += 360.0f;
    if (filledPieRGBA(
        (Sint16)center.x, (Sint16)center.y, radius, startAngle, endAngle,
        color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawCircleSectorLines(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color) {
    if (radius <= 0.0f)
        radius = 0.1f;
    startAngle -= 135.0f;
    endAngle -= 135.0f;
    if (startAngle < 0.0f)
        startAngle += 360.0f;
    if (endAngle < 0.0f)
        endAngle += 360.0f;
    if (pieRGBA(
        (Sint16)center.x, (Sint16)center.y, radius, startAngle, endAngle,
        color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawCircleV(Vector2 center, float radius, Color color) {
    if (radius <= 0.0f)
        radius = 0.1f;
    if (filledCircleRGBA(
        (Sint16)center.x, (Sint16)center.y, radius, color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawCircleLines(int centerX, int centerY, float radius, Color color) {
    if (radius <= 0.0f)
        radius = 0.1f;
    if (circleRGBA(
        (Sint16)centerX, (Sint16)centerY, radius, color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLAPI void DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color) {
    if (filledEllipseRGBA(
        (Sint16)centerX, (Sint16)centerY, radiusH, radiusV, color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLAPI void DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, Color color) {
    if (ellipseRGBA(
        (Sint16)centerX, (Sint16)centerY, radiusH, radiusV, color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color) {
    DrawRingLines(center, innerRadius, outerRadius, startAngle, endAngle, segments, color); // TODO
}

RLCAPI void DrawRingLines(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color) {
    if (startAngle == endAngle)
        return;
    startAngle -= 135.0f;
    endAngle -= 135.0f;
    if (startAngle < 0.0f)
        startAngle += 360.0f;
    if (endAngle < 0.0f)
        endAngle += 360.0f;
    if (outerRadius <= 0.0f)
        outerRadius = 0.1f;
    if (arcRGBA(
        (Sint16)center.x, (Sint16)center.y, (Sint16)innerRadius, (Sint16)startAngle, (Sint16)endAngle,
        color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
    // TODO: connect these lines
    if (arcRGBA(
        (Sint16)center.x, (Sint16)center.y, (Sint16)outerRadius, (Sint16)startAngle, (Sint16)endAngle,
        color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawRectangleRec(Rectangle rec, Color color) {
    if (APPLY_BLEND(color) < 0)
        BLEND_WARN();
    if (SDL_RenderFillRectF(rl.r, (const SDL_FRect*)&rec) < 0)
        DRAW_WARN();
}

RLCAPI void DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color) {
    if (rotation == 0.0f)
        return DrawRectangleRec(rec, color);
    SDL_Texture* tex = CREATE_DRAW_TEXTURE(rec.width, rec.height, color);
    if (tex == NULL) {
        CREATE_TEXTURE_WARN();
        return;
    }
    SDL_Texture* target_before = SDL_GetRenderTarget(rl.r);
    if (SDL_SetRenderTarget(rl.r, tex) < 0)
        RENDER_TARGET_WARN();
    if (APPLY_TEXTURE_BLEND(tex, color) < 0)
        BLEND_WARN();
    if (APPLY_BLEND(color) < 0)
        BLEND_WARN();
    if (color.a >= 255) {
        if (SDL_RenderClear(rl.r) < 0)
            DRAW_WARN();
    }
    else {
        SDL_FRect draw_rect = { 0.0f, 0.0f, rec.width, rec.height };
        if (SDL_RenderFillRectF(rl.r, &draw_rect) < 0)
            DRAW_WARN();
    }
    if (SDL_SetRenderTarget(rl.r, target_before) < 0)
        RENDER_TARGET_WARN();
    if (SDL_RenderCopyExF(
        rl.r, tex, NULL, (const SDL_FRect*)&rec,
        (double)rotation, (const SDL_FPoint*)&origin, SDL_FLIP_NONE
    ) < 0)
        DRAW_WARN();
    SDL_DestroyTexture(tex);
}
