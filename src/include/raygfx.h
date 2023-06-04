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
extern "C" {
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

RAYGFXAPI int pixelRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Horizontal line */

RAYGFXAPI int hlineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Vertical line */

RAYGFXAPI int vlineRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Rectangle */

RAYGFXAPI int rectangleRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1,
    Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Rounded-Corner Rectangle */

RAYGFXAPI int roundedRectangleRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1,
    Sint16 x2, Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled rectangle (Box) */

RAYGFXAPI int boxRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2,
    Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Rounded-Corner Filled rectangle (Box) */

RAYGFXAPI int roundedBoxRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2,
    Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Line */

RAYGFXAPI int lineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1,
    Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA Line */

RAYGFXAPI int aalineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1,
    Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Thick Line */
RAYGFXAPI int thickLineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, 
    Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Circle */

RAYGFXAPI int circleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Arc */

RAYGFXAPI int arcRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, 
    Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA Circle */

RAYGFXAPI int aacircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y,
    Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Circle */

RAYGFXAPI int filledCircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y,
    Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Ellipse */

RAYGFXAPI int ellipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y,
    Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA Ellipse */

RAYGFXAPI int aaellipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y,
    Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Ellipse */

RAYGFXAPI int filledEllipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y,
    Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Pie */

RAYGFXAPI int pieRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad,
    Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Pie */

RAYGFXAPI int filledPieRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad,
    Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Trigon */

RAYGFXAPI int trigonRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
    Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA-Trigon */

RAYGFXAPI int aatrigonRGBA(SDL_Renderer * renderer,  Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
    Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Trigon */

RAYGFXAPI int filledTrigonRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
    Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Polygon */

RAYGFXAPI int polygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy,
    int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* AA-Polygon */

RAYGFXAPI int aapolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy,
    int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Filled Polygon */

RAYGFXAPI int filledPolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx,
    const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* Textured Polygon */

RAYGFXAPI int texturedPolygon(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, SDL_Surface * texture,int texture_dx,int texture_dy);

/* Bezier */

RAYGFXAPI int bezierRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy,
    int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
RAYGFXAPI double _evaluateBezier(double *data, int ndata, double t);
/* Richard Russell's additions */

RAYGFXAPI int thickEllipseRGBA(SDL_Renderer * renderer, Sint16 xc, Sint16 yc, Sint16 xr, Sint16 yr, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick);
RAYGFXAPI int thickArcRGBA(SDL_Renderer * renderer, Sint16 xc, Sint16 yc, Sint16 rad, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick);
RAYGFXAPI int thickCircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick);
RAYGFXAPI int filledPolyBezierRGBA(SDL_Renderer * renderer, const Sint16 *x, const Sint16 *y, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

RAYGFXAPI int aaFilledEllipseRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a); 
RAYGFXAPI int aaFilledPolygonRGBA(SDL_Renderer * renderer, const double * vx, const double * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
RAYGFXAPI int aaFilledPieRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry,
    float start, float end, Uint32 chord, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

RAYGFXAPI int aaArcRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry,
    float start, float end, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
RAYGFXAPI int aaBezierRGBA(SDL_Renderer * renderer, double *x, double *y, int n, int s, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
RAYGFXAPI int aaFilledPolyBezierRGBA(SDL_Renderer * renderer, double *x, double *y, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
