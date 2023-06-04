/*

raygfx.c: graphics primitives for SDL

Copyright (C) 2012-2014  Andreas Schiffler
Additions for BBC BASIC (C) 2016-2020 Richard Russell

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.

Andreas Schiffler -- aschiffler at ferzkopp dot net
Richard Russell -- richard at rtrussell dot co dot uk
Pixelsuf's Hands -- pixelsuft.github.io

*/

#pragma once
#include <math.h>
#include <SDL2/SDL.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C"
{
#endif

    /* ---- Function Prototypes */

/*#ifdef _MSC_VER
#  if defined(DLL_EXPORT) && !defined(LIBSDL2_GFX_DLL_IMPORT)
#    define RAYGFXAPI __declspec(dllexport)
#  else
#    ifdef LIBSDL2_GFX_DLL_IMPORT
#      define RAYGFXAPI __declspec(dllimport)
#    endif
#  endif
#endif
#ifndef RAYGFXAPI
#  define RAYGFXAPI extern
#endif*/
#define RAYGFXAPI
    /* Pixel */

    RAYGFXAPI int pixelRGBA(Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Horizontal line */

    RAYGFXAPI int hlineRGBA(Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Vertical line */

    RAYGFXAPI int vlineRGBA(Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Rectangle */

    RAYGFXAPI int rectangleRGBA(Sint16 x1, Sint16 y1,
                                Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Rounded-Corner Rectangle */

    RAYGFXAPI int roundedRectangleRGBA(Sint16 x1, Sint16 y1,
                                       Sint16 x2, Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Filled rectangle (Box) */

    RAYGFXAPI int boxRGBA(Sint16 x1, Sint16 y1, Sint16 x2,
                          Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Rounded-Corner Filled rectangle (Box) */

    RAYGFXAPI int roundedBoxRGBA(Sint16 x1, Sint16 y1, Sint16 x2,
                                 Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Line */

    RAYGFXAPI int lineRGBA(Sint16 x1, Sint16 y1,
                           Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* AA Line */

    RAYGFXAPI int aalineRGBA(Sint16 x1, Sint16 y1,
                             Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Thick Line */
    RAYGFXAPI int thickLineRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,
                                float width, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Circle */

    RAYGFXAPI int circleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Arc */

    RAYGFXAPI int arcRGBA(Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end,
                          Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* AA Circle */

    RAYGFXAPI int aacircleRGBA(Sint16 x, Sint16 y,
                               Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Filled Circle */

    RAYGFXAPI int filledCircleRGBA(Sint16 x, Sint16 y,
                                   float rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Ellipse */

    RAYGFXAPI int ellipseRGBA(Sint16 x, Sint16 y,
                              Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* AA Ellipse */

    RAYGFXAPI int aaellipseRGBA(Sint16 x, Sint16 y,
                                Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Filled Ellipse */

    RAYGFXAPI int filledEllipseRGBA(Sint16 x, Sint16 y,
                                    float rx, float ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Pie */

    RAYGFXAPI int pieRGBA(Sint16 x, Sint16 y, Sint16 rad,
                          Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Filled Pie */

    RAYGFXAPI int filledPieRGBA(Sint16 x, Sint16 y, Sint16 rad,
                                Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Trigon */

    RAYGFXAPI int trigonRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
                             Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* AA-Trigon */

    RAYGFXAPI int aatrigonRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
                               Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Filled Trigon */

    RAYGFXAPI int filledTrigonRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
                                   Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Polygon */

    RAYGFXAPI int polygonRGBA(const Sint16 *vx, const Sint16 *vy,
                              int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* AA-Polygon */

    RAYGFXAPI int aapolygonRGBA(const Sint16 *vx, const Sint16 *vy,
                                int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Filled Polygon */

    RAYGFXAPI int filledPolygonRGBA(const Sint16 *vx,
                                    const Sint16 *vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Textured Polygon */

    RAYGFXAPI int texturedPolygon(const Sint16 *vx, const Sint16 *vy, int n, SDL_Surface *texture, int texture_dx, int texture_dy);

    /* Bezier */

    RAYGFXAPI int bezierRGBA(const Sint16 *vx, const Sint16 *vy,
                             int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    RAYGFXAPI double _evaluateBezier(double *data, int ndata, double t);
    /* Richard Russell's additions */

    RAYGFXAPI int thickEllipseRGBA(Sint16 xc, Sint16 yc, Sint16 xr, Sint16 yr, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick);
    RAYGFXAPI int thickArcRGBA(Sint16 xc, Sint16 yc, Sint16 rad, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick);
    RAYGFXAPI int thickCircleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick);
    RAYGFXAPI int filledPolyBezierRGBA(const Sint16 *x, const Sint16 *y, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    RAYGFXAPI int aaFilledEllipseRGBA(float cx, float cy, float rx, float ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    RAYGFXAPI int aaFilledPolygonRGBA(const double *vx, const double *vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    RAYGFXAPI int aaFilledPieRGBA(float cx, float cy, float rx, float ry,
                                  float start, float end, Uint32 chord, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    RAYGFXAPI int aaArcRGBA(float cx, float cy, float rx, float ry,
                            float start, float end, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    RAYGFXAPI int aaBezierRGBA(double *x, double *y, int n, int s, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    RAYGFXAPI int aaFilledPolyBezierRGBA(double *x, double *y, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
