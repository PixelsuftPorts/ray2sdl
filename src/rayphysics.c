#include <raydef.h>
#include <raylib.h>
#include <rayconf.h>
#include <raymath.h>

// WARNING: much code was copy pasted from raylib

RLCAPI bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2)
{
    return ((rec1.x < (rec2.x + rec2.width) && (rec1.x + rec1.width) > rec2.x) &&
            (rec1.y < (rec2.y + rec2.height) && (rec1.y + rec1.height) > rec2.y));
}

RLCAPI bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)
{
    float dx = center2.x - center1.x;
    float dy = center2.y - center1.y;
    // Lol who said that we need to compare square roots?
    return (dx * dx + dy * dy) <= (radius1 * radius1 + radius2 * radius2);
}

RLCAPI bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec)
{
    int recCenterX = (int)(rec.x + rec.width / 2.0f);
    int recCenterY = (int)(rec.y + rec.height / 2.0f);

    float dx = SDL_fabsf(center.x - (float)recCenterX);
    float dy = SDL_fabsf(center.y - (float)recCenterY);

    if (dx > (rec.width / 2.0f + radius))
        return false;
    if (dy > (rec.height / 2.0f + radius))
        return false;

    if (dx <= (rec.width / 2.0f))
        return true;
    if (dy <= (rec.height / 2.0f))
        return true;

    float cornerDistanceSq = (dx - rec.width / 2.0f) * (dx - rec.width / 2.0f) +
                             (dy - rec.height / 2.0f) * (dy - rec.height / 2.0f);
    return cornerDistanceSq <= (radius * radius);
}

RLCAPI bool CheckCollisionPointRec(Vector2 point, Rectangle rec)
{
    return SDL_PointInFRect((const SDL_FPoint *)&point, (const SDL_FRect *)&rec);
}

RLCAPI bool CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius)
{
    return CheckCollisionCircles(point, 0.0f, center, radius);
}

RLCAPI bool CheckCollisionPointTriangle(Vector2 point, Vector2 p1, Vector2 p2, Vector2 p3)
{
    float alpha = ((p2.y - p3.y) * (point.x - p3.x) + (p3.x - p2.x) * (point.y - p3.y)) /
                  ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));

    float beta = ((p3.y - p1.y) * (point.x - p3.x) + (p1.x - p3.x) * (point.y - p3.y)) /
                 ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));

    float gamma = 1.0f - alpha - beta;

    return (alpha > 0) && (beta > 0) && (gamma > 0);
}

RLCAPI bool CheckCollisionPointPoly(Vector2 point, Vector2 *points, int pointCount)
{
    if (pointCount < 3)
        return false;
    bool collision = false;
    for (int i = 0; i < pointCount - 1; i++)
    {
        Vector2 vc = points[i];
        Vector2 vn = points[i + 1];

        if ((((vc.y >= point.y) && (vn.y < point.y)) || ((vc.y < point.y) && (vn.y >= point.y))) &&
            (point.x < ((vn.x - vc.x) * (point.y - vc.y) / (vn.y - vc.y) + vc.x)))
            collision = !collision;
    }
    return collision;
}

RLCAPI bool CheckCollisionLines(Vector2 startPos1, Vector2 endPos1, Vector2 startPos2, Vector2 endPos2, Vector2 *collisionPoint)
{
    float div = (endPos2.y - startPos2.y) * (endPos1.x - startPos1.x) - (endPos2.x - startPos2.x) * (endPos1.y - startPos1.y);
    if (SDL_fabs(div) < EPSILON)
        return false;
    float xi = ((startPos2.x - endPos2.x) * (startPos1.x * endPos1.y - startPos1.y * endPos1.x) - (startPos1.x - endPos1.x) * (startPos2.x * endPos2.y - startPos2.y * endPos2.x)) / div;
    float yi = ((startPos2.y - endPos2.y) * (startPos1.x * endPos1.y - startPos1.y * endPos1.x) - (startPos1.y - endPos1.y) * (startPos2.x * endPos2.y - startPos2.y * endPos2.x)) / div;

    if (
        ((SDL_fabsf(startPos1.x - endPos1.x) > EPSILON) && (xi < SDL_min(startPos1.x, endPos1.x) || (xi > SDL_max(startPos1.x, endPos1.x)))) ||
        ((SDL_fabsf(startPos2.x - endPos2.x) > EPSILON) && (xi < SDL_min(startPos2.x, endPos2.x) || (xi > SDL_max(startPos2.x, endPos2.x)))) ||
        ((SDL_fabsf(startPos1.y - endPos1.y) > EPSILON) && (yi < SDL_min(startPos1.y, endPos1.y) || (yi > SDL_max(startPos1.y, endPos1.y)))) ||
        ((SDL_fabsf(startPos2.y - endPos2.y) > EPSILON) && (yi < SDL_min(startPos2.y, endPos2.y) || (yi > SDL_max(startPos2.y, endPos2.y)))))
        return false;
    if (collisionPoint)
    {
        collisionPoint->x = xi;
        collisionPoint->y = yi;
    }
    return true;
}

RLCAPI bool CheckCollisionPointLine(Vector2 point, Vector2 p1, Vector2 p2, int threshold)
{
    float dxc = point.x - p1.x;
    float dyc = point.y - p1.y;
    float dxl = p2.x - p1.x;
    float dyl = p2.y - p1.y;
    float cross = dxc * dyl - dyc * dxl;

    if (SDL_fabsf(cross) >= (threshold * SDL_max(SDL_fabsf(dxl), SDL_fabsf(dyl))))
        return false;
    if (SDL_fabsf(dxl) >= SDL_fabsf(dyl))
        return (dxl > 0) ? ((p1.x <= point.x) && (point.x <= p2.x)) : ((p2.x <= point.x) && (point.x <= p1.x));
    else
        return (dyl > 0) ? ((p1.y <= point.y) && (point.y <= p2.y)) : ((p2.y <= point.y) && (point.y <= p1.y));
}

RLCAPI Rectangle GetCollisionRec(Rectangle rec1, Rectangle rec2)
{
    Rectangle overlap = {0};
    float left = (rec1.x > rec2.x) ? rec1.x : rec2.x;
    float right1 = rec1.x + rec1.width;
    float right2 = rec2.x + rec2.width;
    float right = (right1 < right2) ? right1 : right2;
    float top = (rec1.y > rec2.y) ? rec1.y : rec2.y;
    float bottom1 = rec1.y + rec1.height;
    float bottom2 = rec2.y + rec2.height;
    float bottom = (bottom1 < bottom2) ? bottom1 : bottom2;
    if ((left < right) && (top < bottom))
    {
        overlap.x = left;
        overlap.y = top;
        overlap.width = right - left;
        overlap.height = bottom - top;
    }
    return overlap;
}
