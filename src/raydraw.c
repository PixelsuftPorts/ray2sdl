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

RLCAPI void DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color) {
    if (filledEllipseRGBA(
        (Sint16)centerX, (Sint16)centerY, radiusH, radiusV, color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, Color color) {
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
#ifdef PREFER_GPU_FUNCTIONS
    SDL_Texture* tex = CREATE_DRAW_TEXTURE(rec.width, rec.height, color.a);
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
#else
    // TODO
#endif
}

RLCAPI void DrawRectangleGradientV(int posX, int posY, int width, int height, Color color1, Color color2) {
#ifdef PREFER_GPU_FUNCTIONS
    SDL_Texture* tex = CREATE_DRAW_TEXTURE(width, 2, SDL_min(color1.a, color2.a));
    if (tex == NULL) {
        CREATE_TEXTURE_WARN();
        return;
    }
    if (SDL_SetTextureScaleMode(tex, SDL_ScaleModeLinear) < 0)
        SCALE_MODE_WARN();
    SDL_Texture* target_before = SDL_GetRenderTarget(rl.r);
    if (SDL_SetRenderTarget(rl.r, tex) < 0)
        RENDER_TARGET_WARN();
    if (APPLY_BLEND(color1) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawLineF(rl.r, 0.0f, 0.0f, (float)width, 0.0f) < 0)
        DRAW_WARN();
    if (APPLY_BLEND(color2) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawLineF(rl.r, 0.0f, 1.0f, (float)width, 1.0f) < 0)
        DRAW_WARN();
    if (SDL_SetRenderTarget(rl.r, target_before) < 0)
        RENDER_TARGET_WARN();
    SDL_FRect dst_rect = { (float)posX, (float)posY, (float)width, (float)height };
    if (SDL_RenderCopyF(rl.r, tex, NULL, &dst_rect) < 0)
        RENDER_COPY_WARN();
    SDL_DestroyTexture(tex);
#else
    // TODO
#endif
}

RLCAPI void DrawRectangleGradientH(int posX, int posY, int width, int height, Color color1, Color color2) {
#ifdef PREFER_GPU_FUNCTIONS
    SDL_Texture* tex = CREATE_DRAW_TEXTURE(2, height, SDL_min(color1.a, color2.a));
    if (tex == NULL) {
        CREATE_TEXTURE_WARN();
        return;
    }
    if (SDL_SetTextureScaleMode(tex, SDL_ScaleModeLinear) < 0)
        SCALE_MODE_WARN();
    SDL_Texture* target_before = SDL_GetRenderTarget(rl.r);
    if (SDL_SetRenderTarget(rl.r, tex) < 0)
        RENDER_TARGET_WARN();
    if (APPLY_BLEND(color1) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawLineF(rl.r, 0.0f, 0.0f, 0.0f, (float)height) < 0)
        DRAW_WARN();
    if (APPLY_BLEND(color2) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawLineF(rl.r, 1.0f, 0.0f, 1.0f, (float)height) < 0)
        DRAW_WARN();
    if (SDL_SetRenderTarget(rl.r, target_before) < 0)
        RENDER_TARGET_WARN();
    SDL_FRect dst_rect = { (float)posX, (float)posY, (float)width, (float)height };
    if (SDL_RenderCopyF(rl.r, tex, NULL, &dst_rect) < 0)
        RENDER_COPY_WARN();
    SDL_DestroyTexture(tex);
#else
    // TODO
#endif
}

RLCAPI void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4) {
#ifdef PREFER_GPU_FUNCTIONS
    SDL_Texture* tex = CREATE_DRAW_TEXTURE(
        2, 2,
        SDL_min(SDL_min(SDL_min(col1.a, col2.a), col3.a), col4.a)
    );
    if (tex == NULL) {
        CREATE_TEXTURE_WARN();
        return;
    }
    if (SDL_SetTextureScaleMode(tex, SDL_ScaleModeLinear) < 0)
        SCALE_MODE_WARN();
    SDL_Texture* target_before = SDL_GetRenderTarget(rl.r);
    if (SDL_SetRenderTarget(rl.r, tex) < 0)
        RENDER_TARGET_WARN();
    if (APPLY_BLEND(col1) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawPointF(rl.r, 0.0f, 0.0f) < 0)
        DRAW_WARN();
    if (APPLY_BLEND(col2) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawPointF(rl.r, 0.0f, 1.0f) < 0)
        DRAW_WARN();
    if (APPLY_BLEND(col3) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawPointF(rl.r, 1.0f, 1.0f) < 0)
        DRAW_WARN();
    if (APPLY_BLEND(col4) < 0)
        BLEND_WARN();
    if (SDL_RenderDrawPointF(rl.r, 1.0f, 0.0f) < 0)
        DRAW_WARN();
    if (SDL_SetRenderTarget(rl.r, target_before) < 0)
        RENDER_TARGET_WARN();
    if (SDL_RenderCopyF(rl.r, tex, NULL, (const SDL_FRect*)&rec) < 0)
        RENDER_COPY_WARN();
    SDL_DestroyTexture(tex);
#else
    // TODO
#endif
}

RLCAPI void DrawRectangleLines(int posX, int posY, int width, int height, Color color) {
    if (APPLY_BLEND(color) < 0)
        BLEND_WARN();
    SDL_FRect draw_rect = { (float)posX, (float)posY, (float)width, (float)height };
    if (SDL_RenderDrawRectF(rl.r, &draw_rect) < 0)
        DRAW_WARN();
}

RLCAPI void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color) {
    if (lineThick == 1.0f) {
        if (APPLY_BLEND(color) < 0)
            BLEND_WARN();
        if (SDL_RenderDrawRectF(rl.r, (const SDL_FRect*)&rec) < 0)
            DRAW_WARN();
        return;
    }
    if (APPLY_BLEND(color) < 0)
        BLEND_WARN();
    SDL_FRect draw_rects[4] = {
        { rec.x, rec.y, rec.width, lineThick },
        { rec.x + rec.width - lineThick, rec.y + lineThick, lineThick, rec.height - lineThick - lineThick },
        { rec.x, rec.y + rec.height - lineThick, rec.width, lineThick },
        { rec.x, rec.y + lineThick, lineThick, rec.height - lineThick - lineThick }
    };
    if (SDL_RenderFillRectsF(rl.r, draw_rects, 4) < 0)
        DRAW_WARN();
}

RLCAPI void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color) {
    if ((roundness <= 0.0f) || (rec.width < 1.0f) || (rec.height < 1.0f))
    {
        DrawRectangleRec(rec, color);
        return;
    }
    if (roundness >= 1.0f) roundness = 1.0f;
    if (roundedBoxRGBA(
        (Sint16)rec.x, (Sint16)rec.y, (Sint16)(rec.width + rec.x), (Sint16)(rec.height + rec.y),
        roundness * SDL_min(rec.width, rec.height) / 2.0f, color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, float lineThick, Color color) {
    // TODO: thick
    if ((roundness <= 0.0f) || (rec.width < 1.0f) || (rec.height < 1.0f))
    {
        DrawRectangleLinesEx(rec, lineThick, color);
        return;
    }
    if (roundness >= 1.0f) roundness = 1.0f;
    if (roundedRectangleRGBA(
        (Sint16)rec.x, (Sint16)rec.y, (Sint16)(rec.width + rec.x), (Sint16)(rec.height + rec.y),
        roundness * SDL_min(rec.width, rec.height) / 2.0f, color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
    if (filledTrigonRGBA(
        (Sint16)v1.x, (Sint16)v1.y, (Sint16)v2.x, (Sint16)v2.y, (Sint16)v3.x, (Sint16)v3.y,
        color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
    if (trigonRGBA(
        (Sint16)v1.x, (Sint16)v1.y, (Sint16)v2.x, (Sint16)v2.y, (Sint16)v3.x, (Sint16)v3.y,
        color.r, color.g, color.b, color.a
    ) < 0)
        GFX_WARN();
}

RLCAPI void DrawTriangleFan(Vector2 *points, int pointCount, Color color) {
    if (pointCount < 3)
        return;
    // What this does?
    for (int i = 0; i < pointCount; i++) {
        if (filledTrigonRGBA(
            (Sint16)points[i].x, (Sint16)points[i].y, (Sint16)points[i + 1].x, (Sint16)points[i + 1].y,
            (Sint16)points[i + 2].x, (Sint16)points[i + 2].y, color.r, color.g, color.b, color.a
        ) < 0)
            GFX_WARN();
    }
}

RLCAPI void DrawTriangleStrip(Vector2 *points, int pointCount, Color color) {
    for (int i = 0; i < pointCount; i++) {
        if (filledTrigonRGBA(
            (Sint16)points[i].x, (Sint16)points[i].y, (Sint16)points[i + 1].x, (Sint16)points[i + 1].y,
            (Sint16)points[i + 2].x, (Sint16)points[i + 2].y, color.r, color.g, color.b, color.a
        ) < 0)
            GFX_WARN();
    }
}

RLCAPI void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color) {}

RLCAPI void DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color) {}

RLCAPI void DrawPolyLinesEx(Vector2 center, int sides, float radius, float rotation, float lineThick, Color color) {}
