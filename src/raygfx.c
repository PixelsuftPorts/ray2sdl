/*

raygfx.h: graphics primitives for SDL

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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <raydef.h>
#include <raygfx.h>

/* ---- Structures */

/*!
\brief The structure passed to the internal Bresenham iterator.
*/
typedef struct
{
    Sint16 x, y;
    int dx, dy, s1, s2, swapdir, error;
    Uint32 count;
} SDL2_gfxBresenhamIterator;

/* ---- Pixel */

/*!
\brief Draw pixel  in currently set color.

\param renderer The renderer to draw on.
\param x X (horizontal) coordinate of the pixel.
\param y Y (vertical) coordinate of the pixel.

\returns Returns 0 on success, -1 on failure.
*/
int pixel(Sint16 x, Sint16 y)
{
    return SDL_RenderDrawPoint(rl.r, x, y);
}

/*!
\brief Draw pixel with blending enabled if a<255.

\param renderer The renderer to draw on.
\param x X (horizontal) coordinate of the pixel.
\param y Y (vertical) coordinate of the pixel.
\param r The red color value of the pixel to draw.
\param g The green color value of the pixel to draw.
\param b The blue color value of the pixel to draw.
\param a The alpha value of the pixel to draw.

\returns Returns 0 on success, -1 on failure.
*/
int pixelRGBA(Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);
    result |= SDL_RenderDrawPoint(rl.r, x, y);
    return result;
}

/*!
\brief Draw pixel with blending enabled and using alpha weight on color.

\param renderer The renderer to draw on.
\param x The horizontal coordinate of the pixel.
\param y The vertical position of the pixel.
\param r The red color value of the pixel to draw.
\param g The green color value of the pixel to draw.
\param b The blue color value of the pixel to draw.
\param a The alpha value of the pixel to draw.
\param weight The weight multiplied into the alpha value of the pixel.

\returns Returns 0 on success, -1 on failure.
*/
int pixelRGBAWeight(Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint32 weight)
{
    /*
     * Modify Alpha by weight
     */
    Uint32 ax = a;
    ax = ((ax * weight) >> 8);
    if (ax > 255)
    {
        a = 255;
    }
    else
    {
        a = (Uint8)(ax & 0x000000ff);
    }

    return pixelRGBA(x, y, r, g, b, a);
}

/* ---- Hline */

/*!
\brief Draw horizontal line in currently set color

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.

\returns Returns 0 on success, -1 on failure.
*/
int hline(Sint16 x1, Sint16 x2, Sint16 y)
{
    return SDL_RenderDrawLine(rl.r, x1, y, x2, y);
    ;
}

/*!
\brief Draw horizontal line with blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.
\param r The red value of the line to draw.
\param g The green value of the line to draw.
\param b The blue value of the line to draw.
\param a The alpha value of the line to draw.

\returns Returns 0 on success, -1 on failure.
*/
int hlineRGBA(Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);
    result |= SDL_RenderDrawLine(rl.r, x1, y, x2, y);
    return result;
}

/* ---- Vline */

/*!
\brief Draw vertical line in currently set color

\param renderer The renderer to draw on.
\param x X coordinate of points of the line.
\param y1 Y coordinate of the first point (i.e. top) of the line.
\param y2 Y coordinate of the second point (i.e. bottom) of the line.

\returns Returns 0 on success, -1 on failure.
*/
int vline(Sint16 x, Sint16 y1, Sint16 y2)
{
    return SDL_RenderDrawLine(rl.r, x, y1, x, y2);
    ;
}

/*!
\brief Draw vertical line with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the points of the line.
\param y1 Y coordinate of the first point (i.e. top) of the line.
\param y2 Y coordinate of the second point (i.e. bottom) of the line.
\param r The red value of the line to draw.
\param g The green value of the line to draw.
\param b The blue value of the line to draw.
\param a The alpha value of the line to draw.

\returns Returns 0 on success, -1 on failure.
*/
int vlineRGBA(Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);
    result |= SDL_RenderDrawLine(rl.r, x, y1, x, y2);
    return result;
}

/* ---- Rectangle */

/*!
\brief Draw rectangle with blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the rectangle.
\param y1 Y coordinate of the first point (i.e. top right) of the rectangle.
\param x2 X coordinate of the second point (i.e. bottom left) of the rectangle.
\param y2 Y coordinate of the second point (i.e. bottom left) of the rectangle.
\param r The red value of the rectangle to draw.
\param g The green value of the rectangle to draw.
\param b The blue value of the rectangle to draw.
\param a The alpha value of the rectangle to draw.

\returns Returns 0 on success, -1 on failure.
*/
int rectangleRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    Sint16 tmp;
    SDL_Rect rect;

    /*
     * Test for special cases of straight lines or single point
     */
    if (x1 == x2)
    {
        if (y1 == y2)
        {
            return (pixelRGBA(x1, y1, r, g, b, a));
        }
        else
        {
            return (vlineRGBA(x1, y1, y2, r, g, b, a));
        }
    }
    else
    {
        if (y1 == y2)
        {
            return (hlineRGBA(x1, x2, y1, r, g, b, a));
        }
    }

    /*
     * Swap x1, x2 if required
     */
    if (x1 > x2)
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    /*
     * Swap y1, y2 if required
     */
    if (y1 > y2)
    {
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    /*
     * Create destination rect
     */
    rect.x = x1;
    rect.y = y1;
    rect.w = x2 - x1;
    rect.h = y2 - y1;

    /*
     * Draw
     */
    result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);
    result |= SDL_RenderDrawRect(rl.r, &rect);
    return result;
}

/* ---- Rounded Rectangle */

/*!
\brief Draw rounded-corner rectangle with blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the rectangle.
\param y1 Y coordinate of the first point (i.e. top right) of the rectangle.
\param x2 X coordinate of the second point (i.e. bottom left) of the rectangle.
\param y2 Y coordinate of the second point (i.e. bottom left) of the rectangle.
\param rad The radius of the corner arc.
\param r The red value of the rectangle to draw.
\param g The green value of the rectangle to draw.
\param b The blue value of the rectangle to draw.
\param a The alpha value of the rectangle to draw.

\returns Returns 0 on success, -1 on failure.
*/
int roundedRectangleRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result = 0;
    Sint16 tmp;
    Sint16 w, h;
    Sint16 xx1, xx2;
    Sint16 yy1, yy2;

    /*
     * Check radius vor valid range
     */
    if (rad < 0)
    {
        return -1;
    }

    /*
     * Special case - no rounding
     */
    if (rad <= 1)
    {
        return rectangleRGBA(x1, y1, x2, y2, r, g, b, a);
    }

    /*
     * Test for special cases of straight lines or single point
     */
    if (x1 == x2)
    {
        if (y1 == y2)
        {
            return (pixelRGBA(x1, y1, r, g, b, a));
        }
        else
        {
            return (vlineRGBA(x1, y1, y2, r, g, b, a));
        }
    }
    else
    {
        if (y1 == y2)
        {
            return (hlineRGBA(x1, x2, y1, r, g, b, a));
        }
    }

    /*
     * Swap x1, x2 if required
     */
    if (x1 > x2)
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    /*
     * Swap y1, y2 if required
     */
    if (y1 > y2)
    {
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    /*
     * Calculate width&height
     */
    w = x2 - x1;
    h = y2 - y1;

    /*
     * Maybe adjust radius
     */
    if ((rad * 2) > w)
    {
        rad = w / 2;
    }
    if ((rad * 2) > h)
    {
        rad = h / 2;
    }

    /*
     * Draw corners
     */
    xx1 = x1 + rad;
    xx2 = x2 - rad;
    yy1 = y1 + rad;
    yy2 = y2 - rad;
    result |= arcRGBA(xx1, yy1, rad, 180, 270, r, g, b, a);
    result |= arcRGBA(xx2, yy1, rad, 270, 360, r, g, b, a);
    result |= arcRGBA(xx1, yy2, rad, 90, 180, r, g, b, a);
    result |= arcRGBA(xx2, yy2, rad, 0, 90, r, g, b, a);

    /*
     * Draw lines
     */
    if (xx1 <= xx2)
    {
        result |= hlineRGBA(xx1, xx2, y1, r, g, b, a);
        result |= hlineRGBA(xx1, xx2, y2, r, g, b, a);
    }
    if (yy1 <= yy2)
    {
        result |= vlineRGBA(x1, yy1, yy2, r, g, b, a);
        result |= vlineRGBA(x2, yy1, yy2, r, g, b, a);
    }

    return result;
}

/* ---- Rounded Box */

/*!
\brief Draw rounded-corner box (filled rectangle) with blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the box.
\param y1 Y coordinate of the first point (i.e. top right) of the box.
\param x2 X coordinate of the second point (i.e. bottom left) of the box.
\param y2 Y coordinate of the second point (i.e. bottom left) of the box.
\param rad The radius of the corner arcs of the box.
\param r The red value of the box to draw.
\param g The green value of the box to draw.
\param b The blue value of the box to draw.
\param a The alpha value of the box to draw.

\returns Returns 0 on success, -1 on failure.
*/
int roundedBoxRGBA(Sint16 x1, Sint16 y1, Sint16 x2,
                   Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    Sint16 w, h, r2, tmp;
    Sint16 cx = 0;
    Sint16 cy = rad;
    Sint16 ocx = (Sint16)0xffff;
    Sint16 ocy = (Sint16)0xffff;
    Sint16 df = 1 - rad;
    Sint16 d_e = 3;
    Sint16 d_se = -2 * rad + 5;
    Sint16 xpcx, xmcx, xpcy, xmcy;
    Sint16 ypcy, ymcy, ypcx, ymcx;
    Sint16 x, y, dx, dy;

    /*
     * Check radius vor valid range
     */
    if (rad < 0)
    {
        return -1;
    }

    /*
     * Special case - no rounding
     */
    if (rad <= 1)
    {
        return boxRGBA(x1, y1, x2, y2, r, g, b, a);
    }

    /*
     * Test for special cases of straight lines or single point
     */
    if (x1 == x2)
    {
        if (y1 == y2)
        {
            return (pixelRGBA(x1, y1, r, g, b, a));
        }
        else
        {
            return (vlineRGBA(x1, y1, y2, r, g, b, a));
        }
    }
    else
    {
        if (y1 == y2)
        {
            return (hlineRGBA(x1, x2, y1, r, g, b, a));
        }
    }

    /*
     * Swap x1, x2 if required
     */
    if (x1 > x2)
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    /*
     * Swap y1, y2 if required
     */
    if (y1 > y2)
    {
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    /*
     * Calculate width&height
     */
    w = x2 - x1 + 1;
    h = y2 - y1 + 1;

    /*
     * Maybe adjust radius
     */
    r2 = rad + rad;
    if (r2 > w)
    {
        rad = w / 2;
        r2 = rad + rad;
    }
    if (r2 > h)
    {
        rad = h / 2;
    }

    /* Setup filled circle drawing for corners */
    x = x1 + rad;
    y = y1 + rad;
    dx = x2 - x1 - rad - rad;
    dy = y2 - y1 - rad - rad;

    /*
     * Set color
     */
    result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);

    /*
     * Draw corners
     */
    do
    {
        xpcx = x + cx;
        xmcx = x - cx;
        xpcy = x + cy;
        xmcy = x - cy;
        if (ocy != cy)
        {
            if (cy > 0)
            {
                ypcy = y + cy;
                ymcy = y - cy;
                result |= hline(xmcx, xpcx + dx, ypcy + dy);
                result |= hline(xmcx, xpcx + dx, ymcy);
            }
            else
            {
                result |= hline(xmcx, xpcx + dx, y);
            }
            ocy = cy;
        }
        if (ocx != cx)
        {
            if (cx != cy)
            {
                if (cx > 0)
                {
                    ypcx = y + cx;
                    ymcx = y - cx;
                    result |= hline(xmcy, xpcy + dx, ymcx);
                    result |= hline(xmcy, xpcy + dx, ypcx + dy);
                }
                else
                {
                    result |= hline(xmcy, xpcy + dx, y);
                }
            }
            ocx = cx;
        }

        /*
         * Update
         */
        if (df < 0)
        {
            df += d_e;
            d_e += 2;
            d_se += 2;
        }
        else
        {
            df += d_se;
            d_e += 2;
            d_se += 4;
            cy--;
        }
        cx++;
    } while (cx <= cy);

    /* Inside */
    if (dx > 0 && dy > 0)
    {
        result |= boxRGBA(x1, y1 + rad + 1, x2, y2 - rad, r, g, b, a);
    }

    return (result);
}

/* ---- Box */

/*!
\brief Draw box (filled rectangle) with blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. top right) of the box.
\param y1 Y coordinate of the first point (i.e. top right) of the box.
\param x2 X coordinate of the second point (i.e. bottom left) of the box.
\param y2 Y coordinate of the second point (i.e. bottom left) of the box.
\param r The red value of the box to draw.
\param g The green value of the box to draw.
\param b The blue value of the box to draw.
\param a The alpha value of the box to draw.

\returns Returns 0 on success, -1 on failure.
*/
int boxRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    Sint16 tmp;
    SDL_Rect rect;

    /*
     * Test for special cases of straight lines or single point
     */
    if (x1 == x2)
    {
        if (y1 == y2)
        {
            return (pixelRGBA(x1, y1, r, g, b, a));
        }
        else
        {
            return (vlineRGBA(x1, y1, y2, r, g, b, a));
        }
    }
    else
    {
        if (y1 == y2)
        {
            return (hlineRGBA(x1, x2, y1, r, g, b, a));
        }
    }

    /*
     * Swap x1, x2 if required
     */
    if (x1 > x2)
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    /*
     * Swap y1, y2 if required
     */
    if (y1 > y2)
    {
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    /*
     * Create destination rect
     */
    rect.x = x1;
    rect.y = y1;
    rect.w = x2 - x1 + 1;
    rect.h = y2 - y1 + 1;

    /*
     * Draw
     */
    result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);
    result |= SDL_RenderFillRect(rl.r, &rect);
    return result;
}

/* ----- Line */

/*!
\brief Draw line with alpha blending using the currently set color.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.

\returns Returns 0 on success, -1 on failure.
*/
int line(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2)
{
    /*
     * Draw
     */
    return SDL_RenderDrawLine(rl.r, x1, y1, x2, y2);
}

/*!
\brief Draw line with alpha blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.
\param r The red value of the line to draw.
\param g The green value of the line to draw.
\param b The blue value of the line to draw.
\param a The alpha value of the line to draw.

\returns Returns 0 on success, -1 on failure.
*/
int lineRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    /*
     * Draw
     */
    int result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);
    result |= SDL_RenderDrawLine(rl.r, x1, y1, x2, y2);
    return result;
}

/* ---- AA Line */

#define AAlevels 256
#define AAbits 8

/*!
\brief Internal function to draw anti-aliased line with alpha blending and endpoint control.

This implementation of the Wu antialiasing code is based on Mike Abrash's
DDJ article which was reprinted as Chapter 42 of his Graphics Programming
Black Book, but has been optimized to work with SDL and utilizes 32-bit
fixed-point arithmetic by A. Schiffler. The endpoint control allows the
supression to draw the last pixel useful for rendering continous aa-lines
with alpha<255.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the aa-line.
\param y1 Y coordinate of the first point of the aa-line.
\param x2 X coordinate of the second point of the aa-line.
\param y2 Y coordinate of the second point of the aa-line.
\param r The red value of the aa-line to draw.
\param g The green value of the aa-line to draw.
\param b The blue value of the aa-line to draw.
\param a The alpha value of the aa-line to draw.
\param draw_endpoint Flag indicating if the endpoint should be drawn; draw if non-zero.

\returns Returns 0 on success, -1 on failure.
*/
int _aalineRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int draw_endpoint)
{
    Sint32 xx0, yy0, xx1, yy1;
    int result;
    Uint32 intshift, erracc, erradj;
    Uint32 erracctmp, wgt;
    int dx, dy, tmp, xdir, y0p1, x0pxdir;

    /*
     * Keep on working with 32bit numbers
     */
    xx0 = x1;
    yy0 = y1;
    xx1 = x2;
    yy1 = y2;

    /*
     * Reorder points to make dy positive
     */
    if (yy0 > yy1)
    {
        tmp = yy0;
        yy0 = yy1;
        yy1 = tmp;
        tmp = xx0;
        xx0 = xx1;
        xx1 = tmp;
    }

    /*
     * Calculate distance
     */
    dx = xx1 - xx0;
    dy = yy1 - yy0;

    /*
     * Adjust for negative dx and set xdir
     */
    if (dx >= 0)
    {
        xdir = 1;
    }
    else
    {
        xdir = -1;
        dx = (-dx);
    }

    /*
     * Check for special cases
     */
    if (dx == 0)
    {
        /*
         * Vertical line
         */
        if (draw_endpoint)
        {
            return (vlineRGBA(x1, y1, y2, r, g, b, a));
        }
        else
        {
            if (dy > 0)
            {
                return (vlineRGBA(x1, yy0, yy0 + dy, r, g, b, a));
            }
            else
            {
                return (pixelRGBA(x1, y1, r, g, b, a));
            }
        }
    }
    else if (dy == 0)
    {
        /*
         * Horizontal line
         */
        if (draw_endpoint)
        {
            return (hlineRGBA(x1, x2, y1, r, g, b, a));
        }
        else
        {
            if (dx > 0)
            {
                return (hlineRGBA(xx0, xx0 + (xdir * dx), y1, r, g, b, a));
            }
            else
            {
                return (pixelRGBA(x1, y1, r, g, b, a));
            }
        }
    }
    else if ((dx == dy) && (draw_endpoint))
    {
        /*
         * Diagonal line (with endpoint)
         */
        return (lineRGBA(x1, y1, x2, y2, r, g, b, a));
    }

    /*
     * Line is not horizontal, vertical or diagonal (with endpoint)
     */
    result = 0;

    /*
     * Zero accumulator
     */
    erracc = 0;

    /*
     * # of bits by which to shift erracc to get intensity level
     */
    intshift = 32 - AAbits;

    /*
     * Draw the initial pixel in the foreground color
     */
    result |= pixelRGBA(x1, y1, r, g, b, a);

    /*
     * x-major or y-major?
     */
    if (dy > dx)
    {

        /*
         * y-major.  Calculate 16-bit fixed point fractional part of a pixel that
         * X advances every time Y advances 1 pixel, truncating the result so that
         * we won't overrun the endpoint along the X axis
         */
        /*
         * Not-so-portable version: erradj = ((Uint64)dx << 32) / (Uint64)dy;
         */
        erradj = ((dx << 16) / dy) << 16;

        /*
         * draw all pixels other than the first and last
         */
        x0pxdir = xx0 + xdir;
        while (--dy)
        {
            erracctmp = erracc;
            erracc += erradj;
            if (erracc <= erracctmp)
            {
                /*
                 * rollover in error accumulator, x coord advances
                 */
                xx0 = x0pxdir;
                x0pxdir += xdir;
            }
            yy0++; /* y-major so always advance Y */

            /*
             * the AAbits most significant bits of erracc give us the intensity
             * weighting for this pixel, and the complement of the weighting for
             * the paired pixel.
             */
            wgt = (erracc >> intshift) & 255;
            result |= pixelRGBAWeight(xx0, yy0, r, g, b, a, 255 - wgt);
            result |= pixelRGBAWeight(x0pxdir, yy0, r, g, b, a, wgt);
        }
    }
    else
    {

        /*
         * x-major line.  Calculate 16-bit fixed-point fractional part of a pixel
         * that Y advances each time X advances 1 pixel, truncating the result so
         * that we won't overrun the endpoint along the X axis.
         */
        /*
         * Not-so-portable version: erradj = ((Uint64)dy << 32) / (Uint64)dx;
         */
        erradj = ((dy << 16) / dx) << 16;

        /*
         * draw all pixels other than the first and last
         */
        y0p1 = yy0 + 1;
        while (--dx)
        {

            erracctmp = erracc;
            erracc += erradj;
            if (erracc <= erracctmp)
            {
                /*
                 * Accumulator turned over, advance y
                 */
                yy0 = y0p1;
                y0p1++;
            }
            xx0 += xdir; /* x-major so always advance X */
            /*
             * the AAbits most significant bits of erracc give us the intensity
             * weighting for this pixel, and the complement of the weighting for
             * the paired pixel.
             */
            wgt = (erracc >> intshift) & 255;
            result |= pixelRGBAWeight(xx0, yy0, r, g, b, a, 255 - wgt);
            result |= pixelRGBAWeight(xx0, y0p1, r, g, b, a, wgt);
        }
    }

    /*
     * Do we have to draw the endpoint
     */
    if (draw_endpoint)
    {
        /*
         * Draw final pixel, always exactly intersected by the line and doesn't
         * need to be weighted.
         */
        result |= pixelRGBA(x2, y2, r, g, b, a);
    }

    return (result);
}

/*!
\brief Draw anti-aliased line with alpha blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the aa-line.
\param y1 Y coordinate of the first point of the aa-line.
\param x2 X coordinate of the second point of the aa-line.
\param y2 Y coordinate of the second point of the aa-line.
\param r The red value of the aa-line to draw.
\param g The green value of the aa-line to draw.
\param b The blue value of the aa-line to draw.
\param a The alpha value of the aa-line to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aalineRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return _aalineRGBA(x1, y1, x2, y2, r, g, b, a, 1);
}

/* ----- Circle */

/*!
\brief Draw circle with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the circle.
\param y Y coordinate of the center of the circle.
\param rad Radius in pixels of the circle.
\param r The red value of the circle to draw.
\param g The green value of the circle to draw.
\param b The blue value of the circle to draw.
\param a The alpha value of the circle to draw.

\returns Returns 0 on success, -1 on failure.
*/
int circleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return ellipseRGBA(x, y, rad, rad, r, g, b, a);
}

/* ----- Arc */

/*!
\brief Arc with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the arc.
\param y Y coordinate of the center of the arc.
\param rad Radius in pixels of the arc.
\param start Starting radius in degrees of the arc. 0 degrees is right, increasing clockwise.
\param end Ending radius in degrees of the arc. 0 degrees is right, increasing clockwise.
\param r The red value of the arc to draw.
\param g The green value of the arc to draw.
\param b The blue value of the arc to draw.
\param a The alpha value of the arc to draw.

\returns Returns 0 on success, -1 on failure.
*/
/* TODO: rewrite algorithm; arc endpoints are not always drawn */
int arcRGBA(Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    Sint16 cx = 0;
    Sint16 cy = rad;
    Sint16 df = 1 - rad;
    Sint16 d_e = 3;
    Sint16 d_se = -2 * rad + 5;
    Sint16 xpcx, xmcx, xpcy, xmcy;
    Sint16 ypcy, ymcy, ypcx, ymcx;
    Uint8 drawoct;
    int startoct, endoct, oct, stopval_start = 0, stopval_end = 0;
    double dstart, dend, temp = 0.;

    /*
     * Sanity check radius
     */
    if (rad < 0)
    {
        return (-1);
    }

    /*
     * Special case for rad=0 - draw a point
     */
    if (rad == 0)
    {
        return (pixelRGBA(x, y, r, g, b, a));
    }

    /*
     Octant labelling

      \ 5 | 6 /
       \  |  /
      4 \ | / 7
         \|/
    ------+------ +x
         /|\
      3 / | \ 0
       /  |  \
      / 2 | 1 \
          +y

     Initially reset bitmask to 0x00000000
     the set whether or not to keep drawing a given octant.
     For example: 0x00111100 means we're drawing in octants 2-5
    */
    drawoct = 0;

    /*
     * Fixup angles
     */
    start %= 360;
    end %= 360;
    /* 0 <= start & end < 360; note that sometimes start > end - if so, arc goes back through 0. */
    while (start < 0)
        start += 360;
    while (end < 0)
        end += 360;
    start %= 360;
    end %= 360;

    /* now, we find which octants we're drawing in. */
    startoct = start / 45;
    endoct = end / 45;
    oct = startoct - 1;

    /* stopval_start, stopval_end; what values of cx to stop at. */
    do
    {
        oct = (oct + 1) % 8;

        if (oct == startoct)
        {
            /* need to compute stopval_start for this octant.  Look at picture above if this is unclear */
            dstart = (double)start;
            switch (oct)
            {
            case 0:
            case 3:
                temp = sin(dstart * M_PI / 180.);
                break;
            case 1:
            case 6:
                temp = cos(dstart * M_PI / 180.);
                break;
            case 2:
            case 5:
                temp = -cos(dstart * M_PI / 180.);
                break;
            case 4:
            case 7:
                temp = -sin(dstart * M_PI / 180.);
                break;
            }
            temp *= rad;
            stopval_start = (int)temp;

            /*
            This isn't arbitrary, but requires graph paper to explain well.
            The basic idea is that we're always changing drawoct after we draw, so we
            stop immediately after we render the last sensible pixel at x = ((int)temp).
            and whether to draw in this octant initially
            */
            if (oct % 2)
                drawoct |= (1 << oct); /* this is basically like saying drawoct[oct] = true, if drawoct were a bool array */
            else
                drawoct &= 255 - (1 << oct); /* this is basically like saying drawoct[oct] = false */
        }
        if (oct == endoct)
        {
            /* need to compute stopval_end for this octant */
            dend = (double)end;
            switch (oct)
            {
            case 0:
            case 3:
                temp = sin(dend * M_PI / 180);
                break;
            case 1:
            case 6:
                temp = cos(dend * M_PI / 180);
                break;
            case 2:
            case 5:
                temp = -cos(dend * M_PI / 180);
                break;
            case 4:
            case 7:
                temp = -sin(dend * M_PI / 180);
                break;
            }
            temp *= rad;
            stopval_end = (int)temp;

            /* and whether to draw in this octant initially */
            if (startoct == endoct)
            {
                /* note:      we start drawing, stop, then start again in this case */
                /* otherwise: we only draw in this octant, so initialize it to false, it will get set back to true */
                if (start > end)
                {
                    /* unfortunately, if we're in the same octant and need to draw over the whole circle, */
                    /* we need to set the rest to true, because the while loop will end at the bottom. */
                    drawoct = 255;
                }
                else
                {
                    drawoct &= 255 - (1 << oct);
                }
            }
            else if (oct % 2)
                drawoct &= 255 - (1 << oct);
            else
                drawoct |= (1 << oct);
        }
        else if (oct != startoct)
        {                          /* already verified that it's != endoct */
            drawoct |= (1 << oct); /* draw this entire segment */
        }
    } while (oct != endoct);

    /* so now we have what octants to draw and when to draw them. all that's left is the actual raster code. */

    /*
     * Set color
     */
    result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);

    /*
     * Draw arc
     */
    do
    {
        ypcy = y + cy;
        ymcy = y - cy;
        if (cx > 0)
        {
            xpcx = x + cx;
            xmcx = x - cx;

            /* always check if we're drawing a certain octant before adding a pixel to that octant. */
            if (drawoct & 4)
                result |= pixel(xmcx, ypcy);
            if (drawoct & 2)
                result |= pixel(xpcx, ypcy);
            if (drawoct & 32)
                result |= pixel(xmcx, ymcy);
            if (drawoct & 64)
                result |= pixel(xpcx, ymcy);
        }
        else
        {
            if (drawoct & 96)
                result |= pixel(x, ymcy);
            if (drawoct & 6)
                result |= pixel(x, ypcy);
        }

        xpcy = x + cy;
        xmcy = x - cy;
        if (cx > 0 && cx != cy)
        {
            ypcx = y + cx;
            ymcx = y - cx;
            if (drawoct & 8)
                result |= pixel(xmcy, ypcx);
            if (drawoct & 1)
                result |= pixel(xpcy, ypcx);
            if (drawoct & 16)
                result |= pixel(xmcy, ymcx);
            if (drawoct & 128)
                result |= pixel(xpcy, ymcx);
        }
        else if (cx == 0)
        {
            if (drawoct & 24)
                result |= pixel(xmcy, y);
            if (drawoct & 129)
                result |= pixel(xpcy, y);
        }

        /*
         * Update whether we're drawing an octant
         */
        if (stopval_start == cx)
        {
            /* works like an on-off switch. */
            /* This is just in case start & end are in the same octant. */
            if (drawoct & (1 << startoct))
                drawoct &= 255 - (1 << startoct);
            else
                drawoct |= (1 << startoct);
        }
        if (stopval_end == cx)
        {
            if (drawoct & (1 << endoct))
                drawoct &= 255 - (1 << endoct);
            else
                drawoct |= (1 << endoct);
        }

        /*
         * Update pixels
         */
        if (df < 0)
        {
            df += d_e;
            d_e += 2;
            d_se += 2;
        }
        else
        {
            df += d_se;
            d_e += 2;
            d_se += 4;
            cy--;
        }
        cx++;
    } while (cx <= cy);

    return (result);
}

/* ----- AA Circle */

/*!
\brief Draw anti-aliased circle with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the aa-circle.
\param y Y coordinate of the center of the aa-circle.
\param rad Radius in pixels of the aa-circle.
\param r The red value of the aa-circle to draw.
\param g The green value of the aa-circle to draw.
\param b The blue value of the aa-circle to draw.
\param a The alpha value of the aa-circle to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aacircleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    /*
     * Draw
     */
    return aaellipseRGBA(x, y, rad, rad, r, g, b, a);
}

/* ----- Filled Circle */

/*!
\brief Draw filled circle with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the filled circle.
\param y Y coordinate of the center of the filled circle.
\param rad Radius in pixels of the filled circle.
\param r The red value of the filled circle to draw.
\param g The green value of the filled circle to draw.
\param b The blue value of the filled circle to draw.
\param a The alpha value of the filled circle to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledCircleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return filledEllipseRGBA(x, y, rad, rad, r, g, b, a);
}

/* ----- Ellipse */

/*!
\brief Draw ellipse with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the ellipse.
\param y Y coordinate of the center of the ellipse.
\param rx Horizontal radius in pixels of the ellipse.
\param ry Vertical radius in pixels of the ellipse.
\param r The red value of the ellipse to draw.
\param g The green value of the ellipse to draw.
\param b The blue value of the ellipse to draw.
\param a The alpha value of the ellipse to draw.

\returns Returns 0 on success, -1 on failure.
*/
int ellipseRGBA(Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int ix, iy;
    int h, i, j, k;
    int oh, oi, oj, ok;
    int xmh, xph, ypk, ymk;
    int xmi, xpi, ymj, ypj;
    int xmj, xpj, ymi, ypi;
    int xmk, xpk, ymh, yph;

    /*
     * Sanity check radii
     */
    if ((rx < 0) || (ry < 0))
    {
        return (-1);
    }

    /*
     * Special case for rx=0 - draw a vline
     */
    if (rx == 0)
    {
        return (vlineRGBA(x, y - ry, y + ry, r, g, b, a));
    }
    /*
     * Special case for ry=0 - draw a hline
     */
    if (ry == 0)
    {
        return (hlineRGBA(x - rx, x + rx, y, r, g, b, a));
    }

    /*
     * Set color
     */
    result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);

    /*
     * Init vars
     */
    oh = oi = oj = ok = 0xFFFF;

    /*
     * Draw
     */
    if (rx > ry)
    {
        ix = 0;
        iy = rx * 64;

        do
        {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * ry) / rx;
            k = (i * ry) / rx;

            if (((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j))
            {
                xph = x + h;
                xmh = x - h;
                if (k > 0)
                {
                    ypk = y + k;
                    ymk = y - k;
                    result |= pixel(xmh, ypk);
                    result |= pixel(xph, ypk);
                    result |= pixel(xmh, ymk);
                    result |= pixel(xph, ymk);
                }
                else
                {
                    result |= pixel(xmh, y);
                    result |= pixel(xph, y);
                }
                ok = k;
                xpi = x + i;
                xmi = x - i;
                if (j > 0)
                {
                    ypj = y + j;
                    ymj = y - j;
                    result |= pixel(xmi, ypj);
                    result |= pixel(xpi, ypj);
                    result |= pixel(xmi, ymj);
                    result |= pixel(xpi, ymj);
                }
                else
                {
                    result |= pixel(xmi, y);
                    result |= pixel(xpi, y);
                }
                oj = j;
            }

            ix = ix + iy / rx;
            iy = iy - ix / rx;

        } while (i > h);
    }
    else
    {
        ix = 0;
        iy = ry * 64;

        do
        {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * rx) / ry;
            k = (i * rx) / ry;

            if (((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h)))
            {
                xmj = x - j;
                xpj = x + j;
                if (i > 0)
                {
                    ypi = y + i;
                    ymi = y - i;
                    result |= pixel(xmj, ypi);
                    result |= pixel(xpj, ypi);
                    result |= pixel(xmj, ymi);
                    result |= pixel(xpj, ymi);
                }
                else
                {
                    result |= pixel(xmj, y);
                    result |= pixel(xpj, y);
                }
                oi = i;
                xmk = x - k;
                xpk = x + k;
                if (h > 0)
                {
                    yph = y + h;
                    ymh = y - h;
                    result |= pixel(xmk, yph);
                    result |= pixel(xpk, yph);
                    result |= pixel(xmk, ymh);
                    result |= pixel(xpk, ymh);
                }
                else
                {
                    result |= pixel(xmk, y);
                    result |= pixel(xpk, y);
                }
                oh = h;
            }

            ix = ix + iy / ry;
            iy = iy - ix / ry;

        } while (i > h);
    }

    return (result);
}

/* ----- AA Ellipse */

/*!
\brief Draw anti-aliased ellipse with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the aa-ellipse.
\param y Y coordinate of the center of the aa-ellipse.
\param rx Horizontal radius in pixels of the aa-ellipse.
\param ry Vertical radius in pixels of the aa-ellipse.
\param r The red value of the aa-ellipse to draw.
\param g The green value of the aa-ellipse to draw.
\param b The blue value of the aa-ellipse to draw.
\param a The alpha value of the aa-ellipse to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aaellipseRGBA(Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int i;
    int a2, b2, ds, dt, dxt, t, s, d;
    Sint16 xp, yp, xs, ys, dyt, od, xx, yy, xc2, yc2;
    float cp;
    double sab;
    Uint8 weight, iweight;

    /*
     * Sanity check radii
     */
    if ((rx < 0) || (ry < 0))
    {
        return (-1);
    }

    /*
     * Special cases for rx=0 and/or ry=0: draw a hline/vline/pixel
     */
    if (rx == 0)
    {
        if (ry == 0)
        {
            return (pixel(x, y));
        }
        else
        {
            return (vline(x, y - ry, y + ry));
        }
    }
    else
    {
        if (ry == 0)
        {
            return (hline(x - rx, x + rx, y));
        }
    }

    /* Variable setup */
    a2 = rx * rx;
    b2 = ry * ry;

    ds = 2 * a2;
    dt = 2 * b2;

    xc2 = 2 * x;
    yc2 = 2 * y;

    sab = sqrt((double)(a2 + b2));
    od = (Sint16)lrint(sab * 0.01) + 1; /* introduce some overdraw */
    dxt = (Sint16)lrint((double)a2 / sab) + od;

    t = 0;
    s = -2 * a2 * ry;
    d = 0;

    xp = x;
    yp = y - ry;

    /* Draw */
    result = 0;
    if (a != 255)
        result |= SDL_SetRenderDrawBlendMode(rl.r, SDL_BLENDMODE_BLEND);

    /* "End points" */
    result |= pixelRGBA(xp, yp, r, g, b, a);
    result |= pixelRGBA(xc2 - xp, yp, r, g, b, a);
    result |= pixelRGBA(xp, yc2 - yp, r, g, b, a);
    result |= pixelRGBA(xc2 - xp, yc2 - yp, r, g, b, a);

    for (i = 1; i <= dxt; i++)
    {
        xp--;
        d += t - b2;

        if (d >= 0)
            ys = yp - 1;
        else if ((d - s - a2) > 0)
        {
            if ((2 * d - s - a2) >= 0)
                ys = yp + 1;
            else
            {
                ys = yp;
                yp++;
                d -= s + a2;
                s += ds;
            }
        }
        else
        {
            yp++;
            ys = yp + 1;
            d -= s + a2;
            s += ds;
        }

        t -= dt;

        /* Calculate alpha */
        if (s != 0)
        {
            cp = (float)abs(d) / (float)abs(s);
            if (cp > 1.0)
            {
                cp = 1.0;
            }
        }
        else
        {
            cp = 1.0;
        }

        /* Calculate weights */
        weight = (Uint8)(cp * 255);
        iweight = 255 - weight;

        /* Upper half */
        xx = xc2 - xp;
        result |= pixelRGBAWeight(xp, yp, r, g, b, a, iweight);
        result |= pixelRGBAWeight(xx, yp, r, g, b, a, iweight);

        result |= pixelRGBAWeight(xp, ys, r, g, b, a, weight);
        result |= pixelRGBAWeight(xx, ys, r, g, b, a, weight);

        /* Lower half */
        yy = yc2 - yp;
        result |= pixelRGBAWeight(xp, yy, r, g, b, a, iweight);
        result |= pixelRGBAWeight(xx, yy, r, g, b, a, iweight);

        yy = yc2 - ys;
        result |= pixelRGBAWeight(xp, yy, r, g, b, a, weight);
        result |= pixelRGBAWeight(xx, yy, r, g, b, a, weight);
    }

    /* Replaces original approximation code dyt = abs(yp - yc); */
    dyt = (Sint16)lrint((double)b2 / sab) + od;

    for (i = 1; i <= dyt; i++)
    {
        yp++;
        d -= s + a2;

        if (d <= 0)
            xs = xp + 1;
        else if ((d + t - b2) < 0)
        {
            if ((2 * d + t - b2) <= 0)
                xs = xp - 1;
            else
            {
                xs = xp;
                xp--;
                d += t - b2;
                t -= dt;
            }
        }
        else
        {
            xp--;
            xs = xp - 1;
            d += t - b2;
            t -= dt;
        }

        s += ds;

        /* Calculate alpha */
        if (t != 0)
        {
            cp = (float)abs(d) / (float)abs(t);
            if (cp > 1.0)
            {
                cp = 1.0;
            }
        }
        else
        {
            cp = 1.0;
        }

        /* Calculate weight */
        weight = (Uint8)(cp * 255);
        iweight = 255 - weight;

        /* Left half */
        xx = xc2 - xp;
        yy = yc2 - yp;
        result |= pixelRGBAWeight(xp, yp, r, g, b, a, iweight);
        result |= pixelRGBAWeight(xx, yp, r, g, b, a, iweight);

        result |= pixelRGBAWeight(xp, yy, r, g, b, a, iweight);
        result |= pixelRGBAWeight(xx, yy, r, g, b, a, iweight);

        /* Right half */
        xx = xc2 - xs;
        result |= pixelRGBAWeight(xs, yp, r, g, b, a, weight);
        result |= pixelRGBAWeight(xx, yp, r, g, b, a, weight);

        result |= pixelRGBAWeight(xs, yy, r, g, b, a, weight);
        result |= pixelRGBAWeight(xx, yy, r, g, b, a, weight);
    }

    return (result);
}

/* ---- Filled Ellipse */

/*!
\brief Draw filled ellipse with blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the filled ellipse.
\param y Y coordinate of the center of the filled ellipse.
\param rx Horizontal radius in pixels of the filled ellipse.
\param ry Vertical radius in pixels of the filled ellipse.
\param r The red value of the filled ellipse to draw.
\param g The green value of the filled ellipse to draw.
\param b The blue value of the filled ellipse to draw.
\param a The alpha value of the filled ellipse to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledEllipseRGBA(Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int ix, iy;
    int h, i, j, k;
    int oh, oi, oj, ok;
    int xmh, xph;
    int xmi, xpi;
    int xmj, xpj;
    int xmk, xpk;

    /*
     * Sanity check radii
     */
    if ((rx < 0) || (ry < 0))
    {
        return (-1);
    }

    /*
     * Special case for rx=0 - draw a vline
     */
    if (rx == 0)
    {
        return (vlineRGBA(x, y - ry, y + ry, r, g, b, a));
    }
    /*
     * Special case for ry=0 - draw a hline
     */
    if (ry == 0)
    {
        return (hlineRGBA(x - rx, x + rx, y, r, g, b, a));
    }

    /*
     * Set color
     */
    result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);

    /*
     * Init vars
     */
    oh = oi = oj = ok = 0xFFFF;

    /*
     * Draw
     */
    if (rx > ry)
    {
        ix = 0;
        iy = rx * 64;

        do
        {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * ry) / rx;
            k = (i * ry) / rx;

            if ((ok != k) && (oj != k))
            {
                xph = x + h;
                xmh = x - h;
                if (k > 0)
                {
                    result |= hline(xmh, xph, y + k);
                    result |= hline(xmh, xph, y - k);
                }
                else
                {
                    result |= hline(xmh, xph, y);
                }
                ok = k;
            }
            if ((oj != j) && (ok != j) && (k != j))
            {
                xmi = x - i;
                xpi = x + i;
                if (j > 0)
                {
                    result |= hline(xmi, xpi, y + j);
                    result |= hline(xmi, xpi, y - j);
                }
                else
                {
                    result |= hline(xmi, xpi, y);
                }
                oj = j;
            }

            ix = ix + iy / rx;
            iy = iy - ix / rx;

        } while (i > h);
    }
    else
    {
        ix = 0;
        iy = ry * 64;

        do
        {
            h = (ix + 32) >> 6;
            i = (iy + 32) >> 6;
            j = (h * rx) / ry;
            k = (i * rx) / ry;

            if ((oi != i) && (oh != i))
            {
                xmj = x - j;
                xpj = x + j;
                if (i > 0)
                {
                    result |= hline(xmj, xpj, y + i);
                    result |= hline(xmj, xpj, y - i);
                }
                else
                {
                    result |= hline(xmj, xpj, y);
                }
                oi = i;
            }
            if ((oh != h) && (oi != h) && (i != h))
            {
                xmk = x - k;
                xpk = x + k;
                if (h > 0)
                {
                    result |= hline(xmk, xpk, y + h);
                    result |= hline(xmk, xpk, y - h);
                }
                else
                {
                    result |= hline(xmk, xpk, y);
                }
                oh = h;
            }

            ix = ix + iy / ry;
            iy = iy - ix / ry;

        } while (i > h);
    }

    return (result);
}

/* ----- Pie */

/*!
\brief Internal float (low-speed) pie-calc implementation by drawing polygons.

Note: Determines vertex array and uses polygon or filledPolygon drawing routines to render.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the pie.
\param y Y coordinate of the center of the pie.
\param rad Radius in pixels of the pie.
\param start Starting radius in degrees of the pie.
\param end Ending radius in degrees of the pie.
\param r The red value of the pie to draw.
\param g The green value of the pie to draw.
\param b The blue value of the pie to draw.
\param a The alpha value of the pie to draw.
\param filled Flag indicating if the pie should be filled (=1) or not (=0).

\returns Returns 0 on success, -1 on failure.
*/
/* TODO: rewrite algorithm; pie is not always accurate */
int _pieRGBA(Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 filled)
{
    int result;
    double angle, start_angle, end_angle;
    double deltaAngle;
    double dr;
    int numpoints, i;
    Sint16 *vx, *vy;

    /*
     * Sanity check radii
     */
    if (rad < 0)
    {
        return (-1);
    }

    /*
     * Fixup angles
     */
    start = start % 360;
    end = end % 360;

    /*
     * Special case for rad=0 - draw a point
     */
    if (rad == 0)
    {
        return (pixelRGBA(x, y, r, g, b, a));
    }

    /*
     * Variable setup
     */
    dr = (double)rad;
    deltaAngle = 3.0 / dr;
    start_angle = (double)start * (2.0 * M_PI / 360.0);
    end_angle = (double)end * (2.0 * M_PI / 360.0);
    if (start > end)
    {
        end_angle += (2.0 * M_PI);
    }

    /* We will always have at least 2 points */
    numpoints = 2;

    /* Count points (rather than calculating it) */
    angle = start_angle;
    while (angle < end_angle)
    {
        angle += deltaAngle;
        numpoints++;
    }

    /* Allocate combined vertex array */
    vx = vy = (Sint16 *)malloc(2 * sizeof(Sint16) * numpoints);
    if (vx == NULL)
    {
        return (-1);
    }

    /* Update point to start of vy */
    vy += numpoints;

    /* Center */
    vx[0] = x;
    vy[0] = y;

    /* First vertex */
    angle = start_angle;
    vx[1] = x + (int)(dr * cos(angle));
    vy[1] = y + (int)(dr * sin(angle));

    if (numpoints < 3)
    {
        result = lineRGBA(vx[0], vy[0], vx[1], vy[1], r, g, b, a);
    }
    else
    {
        /* Calculate other vertices */
        i = 2;
        angle = start_angle;
        while (angle < end_angle)
        {
            angle += deltaAngle;
            if (angle > end_angle)
            {
                angle = end_angle;
            }
            vx[i] = x + (int)(dr * cos(angle));
            vy[i] = y + (int)(dr * sin(angle));
            i++;
        }

        /* Draw */
        if (filled)
        {
            result = filledPolygonRGBA(vx, vy, numpoints, r, g, b, a);
        }
        else
        {
            result = polygonRGBA(vx, vy, numpoints, r, g, b, a);
        }
    }

    /* Free combined vertex array */
    free(vx);

    return (result);
}

/*!
\brief Draw pie (outline) with alpha blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the pie.
\param y Y coordinate of the center of the pie.
\param rad Radius in pixels of the pie.
\param start Starting radius in degrees of the pie.
\param end Ending radius in degrees of the pie.
\param r The red value of the pie to draw.
\param g The green value of the pie to draw.
\param b The blue value of the pie to draw.
\param a The alpha value of the pie to draw.

\returns Returns 0 on success, -1 on failure.
*/
int pieRGBA(Sint16 x, Sint16 y, Sint16 rad,
            Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return _pieRGBA(x, y, rad, start, end, r, g, b, a, 0);
}

/*!
\brief Draw filled pie with alpha blending.

\param renderer The renderer to draw on.
\param x X coordinate of the center of the filled pie.
\param y Y coordinate of the center of the filled pie.
\param rad Radius in pixels of the filled pie.
\param start Starting radius in degrees of the filled pie.
\param end Ending radius in degrees of the filled pie.
\param r The red value of the filled pie to draw.
\param g The green value of the filled pie to draw.
\param b The blue value of the filled pie to draw.
\param a The alpha value of the filled pie to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledPieRGBA(Sint16 x, Sint16 y, Sint16 rad,
                  Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return _pieRGBA(x, y, rad, start, end, r, g, b, a, 1);
}

/* ------ Trigon */

/*!
\brief Draw trigon (triangle outline) with alpha blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the trigon.
\param y1 Y coordinate of the first point of the trigon.
\param x2 X coordinate of the second point of the trigon.
\param y2 Y coordinate of the second point of the trigon.
\param x3 X coordinate of the third point of the trigon.
\param y3 Y coordinate of the third point of the trigon.
\param r The red value of the trigon to draw.
\param g The green value of the trigon to draw.
\param b The blue value of the trigon to draw.
\param a The alpha value of the trigon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int trigonRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
               Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    Sint16 vx[3];
    Sint16 vy[3];

    vx[0] = x1;
    vx[1] = x2;
    vx[2] = x3;
    vy[0] = y1;
    vy[1] = y2;
    vy[2] = y3;

    return (polygonRGBA(vx, vy, 3, r, g, b, a));
}

/* ------ AA-Trigon */

/*!
\brief Draw anti-aliased trigon (triangle outline) with alpha blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the aa-trigon.
\param y1 Y coordinate of the first point of the aa-trigon.
\param x2 X coordinate of the second point of the aa-trigon.
\param y2 Y coordinate of the second point of the aa-trigon.
\param x3 X coordinate of the third point of the aa-trigon.
\param y3 Y coordinate of the third point of the aa-trigon.
\param r The red value of the aa-trigon to draw.
\param g The green value of the aa-trigon to draw.
\param b The blue value of the aa-trigon to draw.
\param a The alpha value of the aa-trigon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aatrigonRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
                 Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    Sint16 vx[3];
    Sint16 vy[3];

    vx[0] = x1;
    vx[1] = x2;
    vx[2] = x3;
    vy[0] = y1;
    vy[1] = y2;
    vy[2] = y3;

    return (aapolygonRGBA(vx, vy, 3, r, g, b, a));
}

/* ------ Filled Trigon */

/*!
\brief Draw filled trigon (triangle) with alpha blending.

Note: Creates vertex array and uses aapolygon routine to render.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the filled trigon.
\param y1 Y coordinate of the first point of the filled trigon.
\param x2 X coordinate of the second point of the filled trigon.
\param y2 Y coordinate of the second point of the filled trigon.
\param x3 X coordinate of the third point of the filled trigon.
\param y3 Y coordinate of the third point of the filled trigon.
\param r The red value of the filled trigon to draw.
\param g The green value of the filled trigon to draw.
\param b The blue value of the filled trigon to draw.
\param a The alpha value of the filled trigon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledTrigonRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
                     Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    Sint16 vx[3];
    Sint16 vy[3];

    vx[0] = x1;
    vx[1] = x2;
    vx[2] = x3;
    vy[0] = y1;
    vy[1] = y2;
    vy[2] = y3;

    return (filledPolygonRGBA(vx, vy, 3, r, g, b, a));
}

/* ---- Polygon */

/*!
\brief Draw polygon with the currently set color and blend mode.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the polygon.
\param vy Vertex array containing Y coordinates of the points of the polygon.
\param n Number of points in the vertex array. Minimum number is 3.

\returns Returns 0 on success, -1 on failure.
*/
int polygon(const Sint16 *vx, const Sint16 *vy, int n)
{
    /*
     * Draw
     */
    int result = 0;
    int i, nn;
    SDL_Point *points;

    /*
     * Vertex array NULL check
     */
    if (vx == NULL)
    {
        return (-1);
    }
    if (vy == NULL)
    {
        return (-1);
    }

    /*
     * Sanity check
     */
    if (n < 3)
    {
        return (-1);
    }

    /*
     * Create array of points
     */
    nn = n + 1;
    points = (SDL_Point *)malloc(sizeof(SDL_Point) * nn);
    if (points == NULL)
    {
        return -1;
    }
    for (i = 0; i < n; i++)
    {
        points[i].x = vx[i];
        points[i].y = vy[i];
    }
    points[n].x = vx[0];
    points[n].y = vy[0];

    /*
     * Draw
     */
    result |= SDL_RenderDrawLines(rl.r, points, nn);
    free(points);

    return (result);
}

/*!
\brief Draw polygon with alpha blending.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the polygon.
\param vy Vertex array containing Y coordinates of the points of the polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the polygon to draw.
\param g The green value of the polygon to draw.
\param b The blue value of the polygon to draw.
\param a The alpha value of the polygon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int polygonRGBA(const Sint16 *vx, const Sint16 *vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    /*
     * Draw
     */
    int result;

    /*
     * Vertex array NULL check
     */
    if (vx == NULL)
    {
        return (-1);
    }
    if (vy == NULL)
    {
        return (-1);
    }

    /*
     * Sanity check
     */
    if (n < 3)
    {
        return (-1);
    }

    /*
     * Set color
     */
    result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);

    /*
     * Draw
     */
    result |= polygon(vx, vy, n);

    return (result);
}

/* ---- AA-Polygon */

/*!
\brief Draw anti-aliased polygon with alpha blending.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the aa-polygon.
\param vy Vertex array containing Y coordinates of the points of the aa-polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the aa-polygon to draw.
\param g The green value of the aa-polygon to draw.
\param b The blue value of the aa-polygon to draw.
\param a The alpha value of the aa-polygon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aapolygonRGBA(const Sint16 *vx, const Sint16 *vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int i;
    const Sint16 *x1, *y1, *x2, *y2;

    /*
     * Vertex array NULL check
     */
    if (vx == NULL)
    {
        return (-1);
    }
    if (vy == NULL)
    {
        return (-1);
    }

    /*
     * Sanity check
     */
    if (n < 3)
    {
        return (-1);
    }

    /*
     * Pointer setup
     */
    x1 = x2 = vx;
    y1 = y2 = vy;
    x2++;
    y2++;

    /*
     * Draw
     */
    result = 0;
    for (i = 1; i < n; i++)
    {
        result |= _aalineRGBA(*x1, *y1, *x2, *y2, r, g, b, a, 0);
        x1 = x2;
        y1 = y2;
        x2++;
        y2++;
    }

    result |= _aalineRGBA(*x1, *y1, *vx, *vy, r, g, b, a, 0);

    return (result);
}

/* ---- Filled Polygon */

/*!
\brief Internal helper qsort callback functions used in filled polygon drawing.

\param a The surface to draw on.
\param b Vertex array containing X coordinates of the points of the polygon.

\returns Returns 0 if a==b, a negative number if a<b or a positive number if a>b.
*/
int _gfxPrimitivesCompareInt(const void *a, const void *b)
{
    return (*(const int *)a) - (*(const int *)b);
}

/*!
\brief Global vertex array to use if optional parameters are not given in filledPolygonMT calls.

Note: Used for non-multithreaded (default) operation of filledPolygonMT.
*/
static int *gfxPrimitivesPolyIntsGlobal = NULL;

/*!
\brief Flag indicating if global vertex array was already allocated.

Note: Used for non-multithreaded (default) operation of filledPolygonMT.
*/
static int gfxPrimitivesPolyAllocatedGlobal = 0;

/*!
\brief Draw filled polygon with alpha blending (multi-threaded capable).

Note: The last two parameters are optional; but are required for multithreaded operation.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the filled polygon to draw.
\param g The green value of the filled polygon to draw.
\param b The blue value of the filled polygon to draw.
\param a The alpha value of the filled polygon to draw.
\param polyInts Preallocated, temporary vertex array used for sorting vertices. Required for multithreaded operation; set to NULL otherwise.
\param polyAllocated Flag indicating if temporary vertex array was allocated. Required for multithreaded operation; set to NULL otherwise.

\returns Returns 0 on success, -1 on failure.
*/
int filledPolygonRGBAMT(const Sint16 *vx, const Sint16 *vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int **polyInts, int *polyAllocated)
{
    int result;
    int i;
    int y, xa, xb;
    int miny, maxy;
    int x1, y1;
    int x2, y2;
    int ind1, ind2;
    int ints;
    int *gfxPrimitivesPolyInts = NULL;
    int *gfxPrimitivesPolyIntsNew = NULL;
    int gfxPrimitivesPolyAllocated = 0;

    /*
     * Vertex array NULL check
     */
    if (vx == NULL)
    {
        return (-1);
    }
    if (vy == NULL)
    {
        return (-1);
    }

    /*
     * Sanity check number of edges
     */
    if (n < 3)
    {
        return -1;
    }

    /*
     * Map polygon cache
     */
    if ((polyInts == NULL) || (polyAllocated == NULL))
    {
        /* Use global cache */
        gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsGlobal;
        gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
    }
    else
    {
        /* Use local cache */
        gfxPrimitivesPolyInts = *polyInts;
        gfxPrimitivesPolyAllocated = *polyAllocated;
    }

    /*
     * Allocate temp array, only grow array
     */
    if (!gfxPrimitivesPolyAllocated)
    {
        gfxPrimitivesPolyInts = (int *)malloc(sizeof(int) * n);
        gfxPrimitivesPolyAllocated = n;
    }
    else
    {
        if (gfxPrimitivesPolyAllocated < n)
        {
            gfxPrimitivesPolyIntsNew = (int *)realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
            if (!gfxPrimitivesPolyIntsNew)
            {
                if (!gfxPrimitivesPolyInts)
                {
                    free(gfxPrimitivesPolyInts);
                    gfxPrimitivesPolyInts = NULL;
                }
                gfxPrimitivesPolyAllocated = 0;
            }
            else
            {
                gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsNew;
                gfxPrimitivesPolyAllocated = n;
            }
        }
    }

    /*
     * Check temp array
     */
    if (gfxPrimitivesPolyInts == NULL)
    {
        gfxPrimitivesPolyAllocated = 0;
    }

    /*
     * Update cache variables
     */
    if ((polyInts == NULL) || (polyAllocated == NULL))
    {
        gfxPrimitivesPolyIntsGlobal = gfxPrimitivesPolyInts;
        gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
    }
    else
    {
        *polyInts = gfxPrimitivesPolyInts;
        *polyAllocated = gfxPrimitivesPolyAllocated;
    }

    /*
     * Check temp array again
     */
    if (gfxPrimitivesPolyInts == NULL)
    {
        return (-1);
    }

    /*
     * Determine Y maxima
     */
    miny = vy[0];
    maxy = vy[0];
    for (i = 1; (i < n); i++)
    {
        if (vy[i] < miny)
        {
            miny = vy[i];
        }
        else if (vy[i] > maxy)
        {
            maxy = vy[i];
        }
    }

    /*
     * Draw, scanning y
     */
    result = 0;
    for (y = miny; (y <= maxy); y++)
    {
        ints = 0;
        for (i = 0; (i < n); i++)
        {
            if (!i)
            {
                ind1 = n - 1;
                ind2 = 0;
            }
            else
            {
                ind1 = i - 1;
                ind2 = i;
            }
            y1 = vy[ind1];
            y2 = vy[ind2];
            if (y1 < y2)
            {
                x1 = vx[ind1];
                x2 = vx[ind2];
            }
            else if (y1 > y2)
            {
                y2 = vy[ind1];
                y1 = vy[ind2];
                x2 = vx[ind1];
                x1 = vx[ind2];
            }
            else
            {
                continue;
            }
            if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)))
            {
                gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
            }
        }

        qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);

        /*
         * Set color
         */
        result = 0;
        result |= APPLY_BLEND_RGBA(r, g, b, a);

        for (i = 0; (i < ints); i += 2)
        {
            xa = gfxPrimitivesPolyInts[i] + 1;
            xa = (xa >> 16) + ((xa & 32768) >> 15);
            xb = gfxPrimitivesPolyInts[i + 1] - 1;
            xb = (xb >> 16) + ((xb & 32768) >> 15);
            result |= hline(xa, xb, y);
        }
    }

    return (result);
}

/*!
\brief Draw filled polygon with alpha blending.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the filled polygon to draw.
\param g The green value of the filled polygon to draw.
\param b The blue value of the filed polygon to draw.
\param a The alpha value of the filled polygon to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledPolygonRGBA(const Sint16 *vx, const Sint16 *vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return filledPolygonRGBAMT(vx, vy, n, r, g, b, a, NULL, NULL);
}

/* ---- Textured Polygon */

/*!
\brief Internal function to draw a textured horizontal line.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point (i.e. left) of the line.
\param x2 X coordinate of the second point (i.e. right) of the line.
\param y Y coordinate of the points of the line.
\param texture The texture to retrieve color information from.
\param texture_w The width of the texture.
\param texture_h The height of the texture.
\param texture_dx The X offset for the texture lookup.
\param texture_dy The Y offset for the textured lookup.

\returns Returns 0 on success, -1 on failure.
*/
int _HLineTextured(Sint16 x1, Sint16 x2, Sint16 y, SDL_Texture *texture, int texture_w, int texture_h, int texture_dx, int texture_dy)
{
    Sint16 w;
    Sint16 xtmp;
    int result = 0;
    int texture_x_walker;
    int texture_y_start;
    SDL_Rect source_rect, dst_rect;
    int pixels_written, write_width;

    /*
     * Swap x1, x2 if required to ensure x1<=x2
     */
    if (x1 > x2)
    {
        xtmp = x1;
        x1 = x2;
        x2 = xtmp;
    }

    /*
     * Calculate width to draw
     */
    w = x2 - x1 + 1;

    /*
     * Determine where in the texture we start drawing
     */
    texture_x_walker = (x1 - texture_dx) % texture_w;
    if (texture_x_walker < 0)
    {
        texture_x_walker = texture_w + texture_x_walker;
    }

    texture_y_start = (y + texture_dy) % texture_h;
    if (texture_y_start < 0)
    {
        texture_y_start = texture_h + texture_y_start;
    }

    /* setup the source rectangle; we are only drawing one horizontal line */
    source_rect.y = texture_y_start;
    source_rect.x = texture_x_walker;
    source_rect.h = 1;

    /* we will draw to the current y */
    dst_rect.y = y;
    dst_rect.h = 1;

    /* if there are enough pixels left in the current row of the texture */
    /* draw it all at once */
    if (w <= texture_w - texture_x_walker)
    {
        source_rect.w = w;
        source_rect.x = texture_x_walker;
        dst_rect.x = x1;
        dst_rect.w = source_rect.w;
        result = (SDL_RenderCopy(rl.r, texture, &source_rect, &dst_rect) == 0);
    }
    else
    {
        /* we need to draw multiple times */
        /* draw the first segment */
        pixels_written = texture_w - texture_x_walker;
        source_rect.w = pixels_written;
        source_rect.x = texture_x_walker;
        dst_rect.x = x1;
        dst_rect.w = source_rect.w;
        result |= (SDL_RenderCopy(rl.r, texture, &source_rect, &dst_rect) == 0);
        write_width = texture_w;

        /* now draw the rest */
        /* set the source x to 0 */
        source_rect.x = 0;
        while (pixels_written < w)
        {
            if (write_width >= w - pixels_written)
            {
                write_width = w - pixels_written;
            }
            source_rect.w = write_width;
            dst_rect.x = x1 + pixels_written;
            dst_rect.w = source_rect.w;
            result |= (SDL_RenderCopy(rl.r, texture, &source_rect, &dst_rect) == 0);
            pixels_written += write_width;
        }
    }

    return result;
}

/*!
\brief Draws a polygon filled with the given texture (Multi-Threading Capable).

\param renderer The renderer to draw on.
\param vx array of x vector components
\param vy array of x vector components
\param n the amount of vectors in the vx and vy array
\param texture the sdl surface to use to fill the polygon
\param texture_dx the offset of the texture relative to the screeen. If you move the polygon 10 pixels
to the left and want the texture to apear the same you need to increase the texture_dx value
\param texture_dy see texture_dx
\param polyInts Preallocated temp array storage for vertex sorting (used for multi-threaded operation)
\param polyAllocated Flag indicating oif the temp array was allocated (used for multi-threaded operation)

\returns Returns 0 on success, -1 on failure.
*/
int texturedPolygonMT(const Sint16 *vx, const Sint16 *vy, int n,
                      SDL_Surface *texture, int texture_dx, int texture_dy, int **polyInts, int *polyAllocated)
{
    int result;
    int i;
    int y, xa, xb;
    int minx, maxx, miny, maxy;
    int x1, y1;
    int x2, y2;
    int ind1, ind2;
    int ints;
    int *gfxPrimitivesPolyInts = NULL;
    int *gfxPrimitivesPolyIntsTemp = NULL;
    int gfxPrimitivesPolyAllocated = 0;
    SDL_Texture *textureAsTexture = NULL;

    /*
     * Sanity check number of edges
     */
    if (n < 3)
    {
        return -1;
    }

    /*
     * Map polygon cache
     */
    if ((polyInts == NULL) || (polyAllocated == NULL))
    {
        /* Use global cache */
        gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsGlobal;
        gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
    }
    else
    {
        /* Use local cache */
        gfxPrimitivesPolyInts = *polyInts;
        gfxPrimitivesPolyAllocated = *polyAllocated;
    }

    /*
     * Allocate temp array, only grow array
     */
    if (!gfxPrimitivesPolyAllocated)
    {
        gfxPrimitivesPolyInts = (int *)malloc(sizeof(int) * n);
        gfxPrimitivesPolyAllocated = n;
    }
    else
    {
        if (gfxPrimitivesPolyAllocated < n)
        {
            gfxPrimitivesPolyIntsTemp = (int *)realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
            if (gfxPrimitivesPolyIntsTemp == NULL)
            {
                /* Realloc failed - keeps original memory block, but fails this operation */
                return (-1);
            }
            gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsTemp;
            gfxPrimitivesPolyAllocated = n;
        }
    }

    /*
     * Check temp array
     */
    if (gfxPrimitivesPolyInts == NULL)
    {
        gfxPrimitivesPolyAllocated = 0;
    }

    /*
     * Update cache variables
     */
    if ((polyInts == NULL) || (polyAllocated == NULL))
    {
        gfxPrimitivesPolyIntsGlobal = gfxPrimitivesPolyInts;
        gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
    }
    else
    {
        *polyInts = gfxPrimitivesPolyInts;
        *polyAllocated = gfxPrimitivesPolyAllocated;
    }

    /*
     * Check temp array again
     */
    if (gfxPrimitivesPolyInts == NULL)
    {
        return (-1);
    }

    /*
     * Determine X,Y minima,maxima
     */
    miny = vy[0];
    maxy = vy[0];
    minx = vx[0];
    maxx = vx[0];
    for (i = 1; (i < n); i++)
    {
        if (vy[i] < miny)
        {
            miny = vy[i];
        }
        else if (vy[i] > maxy)
        {
            maxy = vy[i];
        }
        if (vx[i] < minx)
        {
            minx = vx[i];
        }
        else if (vx[i] > maxx)
        {
            maxx = vx[i];
        }
    }

    /* Create texture for drawing */
    textureAsTexture = SDL_CreateTextureFromSurface(rl.r, texture);
    if (textureAsTexture == NULL)
    {
        return -1;
    }
    SDL_SetTextureBlendMode(textureAsTexture, SDL_BLENDMODE_BLEND);

    /*
     * Draw, scanning y
     */
    result = 0;
    for (y = miny; (y <= maxy); y++)
    {
        ints = 0;
        for (i = 0; (i < n); i++)
        {
            if (!i)
            {
                ind1 = n - 1;
                ind2 = 0;
            }
            else
            {
                ind1 = i - 1;
                ind2 = i;
            }
            y1 = vy[ind1];
            y2 = vy[ind2];
            if (y1 < y2)
            {
                x1 = vx[ind1];
                x2 = vx[ind2];
            }
            else if (y1 > y2)
            {
                y2 = vy[ind1];
                y1 = vy[ind2];
                x2 = vx[ind1];
                x1 = vx[ind2];
            }
            else
            {
                continue;
            }
            if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)))
            {
                gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
            }
        }

        qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);

        for (i = 0; (i < ints); i += 2)
        {
            xa = gfxPrimitivesPolyInts[i] + 1;
            xa = (xa >> 16) + ((xa & 32768) >> 15);
            xb = gfxPrimitivesPolyInts[i + 1] - 1;
            xb = (xb >> 16) + ((xb & 32768) >> 15);
            result |= _HLineTextured(xa, xb, y, textureAsTexture, texture->w, texture->h, texture_dx, texture_dy);
        }
    }

    SDL_RenderPresent(rl.r);
    SDL_DestroyTexture(textureAsTexture);

    return (result);
}

/*!
\brief Draws a polygon filled with the given texture.

This standard version is calling multithreaded versions with NULL cache parameters.

\param renderer The renderer to draw on.
\param vx array of x vector components
\param vy array of x vector components
\param n the amount of vectors in the vx and vy array
\param texture the sdl surface to use to fill the polygon
\param texture_dx the offset of the texture relative to the screeen. if you move the polygon 10 pixels
to the left and want the texture to apear the same you need to increase the texture_dx value
\param texture_dy see texture_dx

\returns Returns 0 on success, -1 on failure.
*/
int texturedPolygon(const Sint16 *vx, const Sint16 *vy, int n, SDL_Surface *texture, int texture_dx, int texture_dy)
{
    /*
     * Draw
     */
    return (texturedPolygonMT(vx, vy, n, texture, texture_dx, texture_dy, NULL, NULL));
}

/* ---- Bezier curve */

/*!
\brief Internal function to calculate bezier interpolator of data array with ndata values at position 't'.

\param data Array of values.
\param ndata Size of array.
\param t Position for which to calculate interpolated value. t should be between [0, ndata].

\returns Interpolated value at position t, value[0] when t<0, value[n-1] when t>n.
*/
double _evaluateBezier(double *data, int ndata, double t)
{
    double mu, result;
    int n, k, kn, nn, nkn;
    double blend, muk, munk;

    /* Sanity check bounds */
    if (t < 0.0)
    {
        return (data[0]);
    }
    if (t >= (double)ndata)
    {
        return (data[ndata - 1]);
    }

    /* Adjust t to the range 0.0 to 1.0 */
    mu = t / (double)ndata;

    /* Calculate interpolate */
    n = ndata - 1;
    result = 0.0;
    muk = 1;
    munk = pow(1 - mu, (double)n);
    for (k = 0; k <= n; k++)
    {
        nn = n;
        kn = k;
        nkn = n - k;
        blend = muk * munk;
        muk *= mu;
        munk /= (1 - mu);
        while (nn >= 1)
        {
            blend *= nn;
            nn--;
            if (kn > 1)
            {
                blend /= (double)kn;
                kn--;
            }
            if (nkn > 1)
            {
                blend /= (double)nkn;
                nkn--;
            }
        }
        result += data[k] * blend;
    }

    return (result);
}

/*!
\brief Draw a bezier curve with alpha blending.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the bezier curve.
\param vy Vertex array containing Y coordinates of the points of the bezier curve.
\param n Number of points in the vertex array. Minimum number is 3.
\param s Number of steps for the interpolation. Minimum number is 2.
\param r The red value of the bezier curve to draw.
\param g The green value of the bezier curve to draw.
\param b The blue value of the bezier curve to draw.
\param a The alpha value of the bezier curve to draw.

\returns Returns 0 on success, -1 on failure.
*/
int bezierRGBA(const Sint16 *vx, const Sint16 *vy, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int i;
    double *x, *y, t, stepsize;
    Sint16 x1, y1, x2, y2;

    /*
     * Sanity check
     */
    if (n < 3)
    {
        return (-1);
    }
    if (s < 2)
    {
        return (-1);
    }

    /*
     * Variable setup
     */
    stepsize = (double)1.0 / (double)s;

    /* Transfer vertices into float arrays */
    if ((x = (double *)malloc(sizeof(double) * (n + 1))) == NULL)
    {
        return (-1);
    }
    if ((y = (double *)malloc(sizeof(double) * (n + 1))) == NULL)
    {
        free(x);
        return (-1);
    }
    for (i = 0; i < n; i++)
    {
        x[i] = (double)vx[i];
        y[i] = (double)vy[i];
    }
    x[n] = (double)vx[0];
    y[n] = (double)vy[0];

    /*
     * Set color
     */
    result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);

    /*
     * Draw
     */
    t = 0.0;
    x1 = (Sint16)lrint(_evaluateBezier(x, n + 1, t));
    y1 = (Sint16)lrint(_evaluateBezier(y, n + 1, t));
    for (i = 0; i <= (n * s); i++)
    {
        t += stepsize;
        x2 = (Sint16)_evaluateBezier(x, n, t);
        y2 = (Sint16)_evaluateBezier(y, n, t);
        result |= line(x1, y1, x2, y2);
        x1 = x2;
        y1 = y2;
    }

    /* Clean up temporary array */
    free(x);
    free(y);

    return (result);
}

/* ---- Thick Line */

/*!
\brief Internal function to initialize the Bresenham line iterator.

Example of use:
SDL2_gfxBresenhamIterator b;
_bresenhamInitialize (&b, x1, y1, x2, y2);
do {
plot(b.x, b.y);
} while (_bresenhamIterate(&b)==0);

\param b Pointer to struct for bresenham line drawing state.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.

\returns Returns 0 on success, -1 on failure.
*/
int _bresenhamInitialize(SDL2_gfxBresenhamIterator *b, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2)
{
    int temp;

    if (b == NULL)
    {
        return (-1);
    }

    b->x = x1;
    b->y = y1;

    /* dx = abs(x2-x1), s1 = sign(x2-x1) */
    if ((b->dx = x2 - x1) != 0)
    {
        if (b->dx < 0)
        {
            b->dx = -b->dx;
            b->s1 = -1;
        }
        else
        {
            b->s1 = 1;
        }
    }
    else
    {
        b->s1 = 0;
    }

    /* dy = abs(y2-y1), s2 = sign(y2-y1)    */
    if ((b->dy = y2 - y1) != 0)
    {
        if (b->dy < 0)
        {
            b->dy = -b->dy;
            b->s2 = -1;
        }
        else
        {
            b->s2 = 1;
        }
    }
    else
    {
        b->s2 = 0;
    }

    if (b->dy > b->dx)
    {
        temp = b->dx;
        b->dx = b->dy;
        b->dy = temp;
        b->swapdir = 1;
    }
    else
    {
        b->swapdir = 0;
    }

    b->count = (b->dx < 0) ? 0 : (unsigned int)b->dx;
    b->dy <<= 1;
    b->error = b->dy - b->dx;
    b->dx <<= 1;

    return (0);
}

/*!
\brief Internal function to move Bresenham line iterator to the next position.

Maybe updates the x and y coordinates of the iterator struct.

\param b Pointer to struct for bresenham line drawing state.

\returns Returns 0 on success, 1 if last point was reached, 2 if moving past end-of-line, -1 on failure.
*/
int _bresenhamIterate(SDL2_gfxBresenhamIterator *b)
{
    if (b == NULL)
    {
        return (-1);
    }

    /* last point check */
    if (b->count == 0)
    {
        return (2);
    }

    while (b->error >= 0)
    {
        if (b->swapdir)
        {
            b->x += b->s1;
        }
        else
        {
            b->y += b->s2;
        }

        b->error -= b->dx;
    }

    if (b->swapdir)
    {
        b->y += b->s2;
    }
    else
    {
        b->x += b->s1;
    }

    b->error += b->dy;
    b->count--;

    /* count==0 indicates "end-of-line" */
    return ((b->count) ? 0 : 1);
}

/* Code for Murphy thick line algorithm from http://kt8216.unixcab.org/murphy/ */

/***********************************************************************
 *                                                                     *
 *                            X BASED LINES                            *
 *                                                                     *
 ***********************************************************************/

static void x_perpendicular(int x0, int y0, int dx, int dy, int xstep, int ystep,
                            int einit, int w_left, int w_right, int winit)
{
    int x, y, threshold, E_diag, E_square;
    int tk;
    int error;
    int p, q;

    threshold = dx - 2 * dy;
    E_diag = -2 * dx;
    E_square = 2 * dy;
    p = q = 0;

    y = y0;
    x = x0;
    error = einit;
    tk = dx + dy - winit;

    while (tk <= w_left)
    {
        SDL_RenderDrawPoint(rl.r, x, y);
        if (error >= threshold)
        {
            x = x + xstep;
            error = error + E_diag;
            tk = tk + 2 * dy;
        }
        error = error + E_square;
        y = y + ystep;
        tk = tk + 2 * dx;
        q++;
    }

    y = y0;
    x = x0;
    error = -einit;
    tk = dx + dy + winit;

    while (tk <= w_right)
    {
        if (p)
            SDL_RenderDrawPoint(rl.r, x, y);
        if (error > threshold)
        {
            x = x - xstep;
            error = error + E_diag;
            tk = tk + 2 * dy;
        }
        error = error + E_square;
        y = y - ystep;
        tk = tk + 2 * dx;
        p++;
    }

    if (q == 0 && p < 2)
        SDL_RenderDrawPoint(rl.r, x0, y0); // we need this for very thin lines
}

static void x_varthick_line(int style,
                            int x0, int y0, int dx, int dy, int xstep, int ystep,
                            double thickness, int pxstep, int pystep)
{
    int p_error, error, x, y, threshold, E_diag, E_square, length, p;
    int w_left, w_right;
    double D;

    p_error = 0;
    error = 0;
    y = y0;
    x = x0;
    threshold = dx - 2 * dy;
    E_diag = -2 * dx;
    E_square = 2 * dy;
    length = dx + 1;
    D = sqrt(dx * dx + dy * dy);
    w_left = thickness * D + 0.5;
    w_right = 2.0 * thickness * D + 0.5;
    w_right -= w_left;

    for (p = 0; p < length; p++)
    {
        style = (style << 1) | (style < 0);

        if (style < 0)
            x_perpendicular(x, y, dx, dy, pxstep, pystep,
                            p_error, w_left, w_right, error);
        if (error >= threshold)
        {
            y = y + ystep;
            error = error + E_diag;
            if (p_error >= threshold)
            {
                if (style < 0)
                    x_perpendicular(x, y, dx, dy, pxstep, pystep,
                                    (p_error + E_diag + E_square),
                                    w_left, w_right, error);
                p_error = p_error + E_diag;
            }
            p_error = p_error + E_square;
        }
        error = error + E_square;
        x = x + xstep;
    }
}

/***********************************************************************
 *                                                                     *
 *                            Y BASED LINES                            *
 *                                                                     *
 ***********************************************************************/

static void y_perpendicular(int x0, int y0, int dx, int dy, int xstep, int ystep,
                            int einit, int w_left, int w_right, int winit)
{
    int x, y, threshold, E_diag, E_square;
    int tk;
    int error;
    int p, q;

    p = q = 0;
    threshold = dy - 2 * dx;
    E_diag = -2 * dy;
    E_square = 2 * dx;

    y = y0;
    x = x0;
    error = -einit;
    tk = dx + dy + winit;

    while (tk <= w_left)
    {
        SDL_RenderDrawPoint(rl.r, x, y);
        if (error > threshold)
        {
            y = y + ystep;
            error = error + E_diag;
            tk = tk + 2 * dx;
        }
        error = error + E_square;
        x = x + xstep;
        tk = tk + 2 * dy;
        q++;
    }

    y = y0;
    x = x0;
    error = einit;
    tk = dx + dy - winit;

    while (tk <= w_right)
    {
        if (p)
            SDL_RenderDrawPoint(rl.r, x, y);
        if (error >= threshold)
        {
            y = y - ystep;
            error = error + E_diag;
            tk = tk + 2 * dx;
        }
        error = error + E_square;
        x = x - xstep;
        tk = tk + 2 * dy;
        p++;
    }

    if (q == 0 && p < 2)
        SDL_RenderDrawPoint(rl.r, x0, y0); // we need this for very thin lines
}

static void y_varthick_line(int style,
                            int x0, int y0, int dx, int dy, int xstep, int ystep,
                            double thickness, int pxstep, int pystep)
{
    int p_error, error, x, y, threshold, E_diag, E_square, length, p;
    int w_left, w_right;
    double D;

    p_error = 0;
    error = 0;
    y = y0;
    x = x0;
    threshold = dy - 2 * dx;
    E_diag = -2 * dy;
    E_square = 2 * dx;
    length = dy + 1;
    D = sqrt(dx * dx + dy * dy);
    w_left = thickness * D + 0.5;
    w_right = 2.0 * thickness * D + 0.5;
    w_right -= w_left;

    for (p = 0; p < length; p++)
    {
        style = (style << 1) | (style < 0);

        if (style < 0)
            y_perpendicular(x, y, dx, dy, pxstep, pystep,
                            p_error, w_left, w_right, error);
        if (error >= threshold)
        {
            x = x + xstep;
            error = error + E_diag;
            if (p_error >= threshold)
            {
                if (style < 0)
                    y_perpendicular(x, y, dx, dy, pxstep, pystep,
                                    p_error + E_diag + E_square,
                                    w_left, w_right, error);
                p_error = p_error + E_diag;
            }
            p_error = p_error + E_square;
        }
        error = error + E_square;
        y = y + ystep;
    }
}

/***********************************************************************
 *                                                                     *
 *                                ENTRY                                *
 *                                                                     *
 ***********************************************************************/

void draw_varthick_line(int style, int x0, int y0, int x1, int y1, double thickness)
{
    int dx, dy, xstep, ystep;
    int pxstep = 0, pystep = 0;

    dx = x1 - x0;
    dy = y1 - y0;
    xstep = ystep = 1;

    if (dx < 0)
    {
        dx = -dx;
        xstep = -1;
    }
    if (dy < 0)
    {
        dy = -dy;
        ystep = -1;
    }

    if (dx == 0)
        xstep = 0;
    if (dy == 0)
        ystep = 0;

    switch (xstep + ystep * 4)
    {
    case -1 + -1 * 4:
        pystep = -1;
        pxstep = 1;
        break; // -5
    case -1 + 0 * 4:
        pystep = -1;
        pxstep = 0;
        break; // -1
    case -1 + 1 * 4:
        pystep = 1;
        pxstep = 1;
        break; // 3
    case 0 + -1 * 4:
        pystep = 0;
        pxstep = -1;
        break; // -4
    case 0 + 0 * 4:
        pystep = 0;
        pxstep = 0;
        break; // 0
    case 0 + 1 * 4:
        pystep = 0;
        pxstep = 1;
        break; // 4
    case 1 + -1 * 4:
        pystep = -1;
        pxstep = -1;
        break; // -3
    case 1 + 0 * 4:
        pystep = -1;
        pxstep = 0;
        break; // 1
    case 1 + 1 * 4:
        pystep = 1;
        pxstep = -1;
        break; // 5
    }

    if (dx > dy)
        x_varthick_line(style, x0, y0, dx, dy, xstep, ystep,
                        thickness + 1.0,
                        pxstep, pystep);
    else
        y_varthick_line(style, x0, y0, dx, dy, xstep, ystep,
                        thickness + 1.0,
                        pxstep, pystep);
    return;
}

static int LineStyle = -1;

/*!
\brief Draw a thick line with alpha blending.

\param renderer The renderer to draw on.
\param x1 X coordinate of the first point of the line.
\param y1 Y coordinate of the first point of the line.
\param x2 X coordinate of the second point of the line.
\param y2 Y coordinate of the second point of the line.
\param width Width of the line in pixels. Must be >0.
\param r The red value of the character to draw.
\param g The green value of the character to draw.
\param b The blue value of the character to draw.
\param a The alpha value of the character to draw.

\returns Returns 0 on success, -1 on failure.
*/
int thickLineRGBA(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int result;
    int wh;

    /* Special case: thick "point" */
    if ((x1 == x2) && (y1 == y2))
    {
        wh = width / 2;
        return boxRGBA(x1 - wh, y1 - wh, x2 + wh, y2 + wh, r, g, b, a);
    }

    /*
     * Set color
     */
    result = 0;
    result |= APPLY_BLEND_RGBA(r, g, b, a);

    /*
     * Draw
     */
    draw_varthick_line(LineStyle, x1, y1, x2, y2, (double)width);
    return (result);
}

// Extensions for thick outline ellipses and arcs by Richard Russell 19-Feb-2019

// SDL_RenderDrawLine() is documented as including both end points, but this isn't
// reliable in Linux so use SDL_RenderDrawPoints() instead, despite being slower.
static int renderdrawline(int x1, int y1, int x2, int y2)
{
    int result;
#ifndef __EMSCRIPTEN__
    if ((x1 == x2) && (y1 == y2))
        result = SDL_RenderDrawPoint(rl.r, x1, y1);
    else if (y1 == y2)
    {
        int x;
        if (x1 > x2)
        {
            x = x1;
            x1 = x2;
            x2 = x;
        }
        SDL_Point *points = (SDL_Point *)malloc((x2 - x1 + 1) * sizeof(SDL_Point));
        if (points == NULL)
            return -1;
        for (x = x1; x <= x2; x++)
        {
            points[x - x1].x = x;
            points[x - x1].y = y1;
        }
        result = SDL_RenderDrawPoints(rl.r, points, x2 - x1 + 1);
        free(points);
    }
    else if (x1 == x2)
    {
        int y;
        if (y1 > y2)
        {
            y = y1;
            y1 = y2;
            y2 = y;
        }
        SDL_Point *points = (SDL_Point *)malloc((y2 - y1 + 1) * sizeof(SDL_Point));
        if (points == NULL)
            return -1;
        for (y = y1; y <= y2; y++)
        {
            points[y - y1].x = x1;
            points[y - y1].y = y;
        }
        result = SDL_RenderDrawPoints(rl.r, points, y2 - y1 + 1);
        free(points);
    }
    else
#endif
        result = SDL_RenderDrawLine(rl.r, x1, y1, x2, y2);
    return result;
}

static int hlinecliparc(int x1, int x2, int y, int xc, int yc, double s, double f)
{
    int result = 0;
    double a1, a2;
    a1 = atan2(y, x1);
    a2 = atan2(y, x2);
    if (a1 > a2)
    {
        double a = a1;
        a1 = a2;
        a2 = a;
        int x = x1;
        x1 = x2;
        x2 = x;
    }
    if (f < s)
    {
        if ((a1 > f) && (a2 < s))
            return result;
        if ((a1 < s) && (a1 > f))
            x1 = y / tan(s);
        if ((a2 > f) && (a2 < s))
            x2 = y / tan(f);
        if ((a1 < f) && (a2 > s))
        {
            result |= renderdrawline(x1 + xc, y + yc, y / tan(f) + xc, y + yc);
            result |= renderdrawline(y / tan(s) + xc, y + yc, x2 + xc, y + yc);
            return result;
        }
    }
    else
    {
        if ((a1 > f) || (a2 < s))
            return result;
        if (a1 < s)
            x1 = y / tan(s);
        if (a2 > f)
            x2 = y / tan(f);
    }
    result |= renderdrawline(x1 + xc, y + yc, x2 + xc, y + yc);
    return result;
}

/*!
\brief Draw thick ellipse with blending.

\param renderer The renderer to draw on.
\param xc X coordinate of the center of the ellipse.
\param yc Y coordinate of the center of the ellipse.
\param xr Horizontal radius in pixels of the ellipse.
\param yr Vertical radius in pixels of the ellipse.
\param r The red value of the ellipse to draw.
\param g The green value of the ellipse to draw.
\param b The blue value of the ellipse to draw.
\param a The alpha value of the ellipse to draw.
\param thick The line thickness in pixels

\returns Returns 0 on success, -1 on failure.
*/
int thickEllipseRGBA(Sint16 xc, Sint16 yc, Sint16 xr, Sint16 yr, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick)
{
    int result = 0;
    int xi, yi, xo, yo, x, y, z;
    double xi2, yi2, xo2, yo2;

    if (thick <= 1)
        return ellipseRGBA(xc, yc, xr, yr, r, g, b, a);

    xi = xr - thick / 2;
    xo = xi + thick - 1;
    yi = yr - thick / 2;
    yo = yi + thick - 1;

    if ((xi <= 0) || (yi <= 0))
        return -1;

    xi2 = xi * xi;
    yi2 = yi * yi;
    xo2 = xo * xo;
    yo2 = yo * yo;

    result |= APPLY_BLEND_RGBA(r, g, b, a);

    if (xr < yr)
    {
        for (x = -xo; x <= -xi; x++)
        {
            y = sqrt(yo2 * (1.0 - x * x / xo2)) + 0.5;
            result |= renderdrawline(xc + x, yc - y, xc + x, yc + y);
        }
        for (x = -xi + 1; x <= xi - 1; x++)
        {
            y = sqrt(yo2 * (1.0 - x * x / xo2)) + 0.5;
            z = sqrt(yi2 * (1.0 - x * x / xi2)) + 0.5;
            result |= renderdrawline(xc + x, yc + z, xc + x, yc + y);
            result |= renderdrawline(xc + x, yc - z, xc + x, yc - y);
        }
        for (x = xo; x >= xi; x--)
        {
            y = sqrt(yo2 * (1.0 - x * x / xo2)) + 0.5;
            result |= renderdrawline(xc + x, yc - y, xc + x, yc + y);
        }
    }
    else
    {
        for (y = -yo; y <= -yi; y++)
        {
            x = sqrt(xo2 * (1.0 - y * y / yo2)) + 0.5;
            result |= renderdrawline(xc - x, yc + y, xc + x, yc + y);
        }
        for (y = -yi + 1; y <= yi - 1; y++)
        {
            x = sqrt(xo2 * (1.0 - y * y / yo2)) + 0.5;
            z = sqrt(xi2 * (1.0 - y * y / yi2)) + 0.5;
            result |= renderdrawline(xc + z, yc + y, xc + x, yc + y);
            result |= renderdrawline(xc - z, yc + y, xc - x, yc + y);
        }
        for (y = yo; y >= yi; y--)
        {
            x = sqrt(xo2 * (1.0 - y * y / yo2)) + 0.5;
            result |= renderdrawline(xc - x, yc + y, xc + x, yc + y);
        }
    }
    return result;
}

/*!
\brief thick Arc with blending.

\param renderer The renderer to draw on.
\param xc X coordinate of the center of the arc.
\param yc Y coordinate of the center of the arc.
\param rad Radius in pixels of the arc.
\param start Starting radius in degrees of the arc. 0 degrees is right, increasing clockwise.
\param end Ending radius in degrees of the arc. 0 degrees is right, increasing clockwise.
\param r The red value of the arc to draw.
\param g The green value of the arc to draw.
\param b The blue value of the arc to draw.
\param a The alpha value of the arc to draw.
\param thick The line thickness in pixels.

\returns Returns 0 on success, -1 on failure.
*/
int thickArcRGBA(Sint16 xc, Sint16 yc, Sint16 rad, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick)
{
    int result = 0;
    int ri, ro, x, y, z;
    double ri2, ro2, s, f;

    if (thick <= 1)
        return arcRGBA(xc, yc, rad, start, end, r, g, b, a);

    while (start < -180)
        start += 360;
    while (start >= 180)
        start -= 360;
    while (end < -180)
        end += 360;
    while (end >= 180)
        end -= 360;
    s = M_PI * (double)start / 180.0;
    f = M_PI * (double)end / 180.0;
    if (start == end)
        return 0;

    ri = rad - thick / 2;
    ro = ri + thick - 1;
    if (ri <= 0)
        return -1;

    ri2 = ri * ri;
    ro2 = ro * ro;

    result |= APPLY_BLEND_RGBA(r, g, b, a);

    for (y = -ro; y <= -ri; y++)
    {
        x = sqrt(ro2 * (1.0 - y * y / ro2)) + 0.5;
        result |= hlinecliparc(-x, x, y, xc, yc, s, f);
    }
    for (y = -ri + 1; y <= ri - 1; y++)
    {
        x = sqrt(ro2 * (1.0 - y * y / ro2)) + 0.5;
        z = sqrt(ri2 * (1.0 - y * y / ri2)) + 0.5;
        result |= hlinecliparc(z, x, y, xc, yc, s, f);
        result |= hlinecliparc(-z, -x, y, xc, yc, s, f);
    }
    for (y = ro; y >= ri; y--)
    {
        x = sqrt(ro2 * (1.0 - y * y / ro2)) + 0.5;
        result |= hlinecliparc(-x, x, y, xc, yc, s, f);
    }
    return result;
}

// returns Returns 0 on success, -1 on failure.
int thickCircleRGBA(Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick)
{
    return thickEllipseRGBA(x, y, rad, rad, r, g, b, a, thick);
}

/*!
\brief Fill a region bounded by cubic Bezier curves, with alpha blending.

\param renderer The renderer to draw on.
\param x Vertex array containing X coordinates of the points of the bezier curves.
\param y Vertex array containing Y coordinates of the points of the bezier curves.
\param n Number of points in the vertex array. Should be 3n + 1 for n bezier curves.
\param s Number of steps for the interpolation. Minimum number is 2.
\param r The red value of the bezier curve to draw.
\param g The green value of the bezier curve to draw.
\param b The blue value of the bezier curve to draw.
\param a The alpha value of the bezier curve to draw.

\returns Returns 0 on success, -1 on failure.
*/
int filledPolyBezierRGBA(const Sint16 *x, const Sint16 *y, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int i, j, nbeziers, nverts, result;
    double t, stepsize;
    double x1, y1, x2, y2;
    double *dx, *dy;
    Sint16 *vx, *vy;

    // Sanity check
    if ((n < 7) || (s < 2))
        return -1;

    /* Transfer vertices into float arrays */
    if ((dx = (double *)malloc(sizeof(double) * n)) == NULL)
    {
        return (-1);
    }
    if ((dy = (double *)malloc(sizeof(double) * n)) == NULL)
    {
        free(dx);
        return (-1);
    }
    for (i = 0; i < n; i++)
    {
        dx[i] = (double)x[i];
        dy[i] = (double)y[i];
    }

    // Create combined vertex array:
    nbeziers = (n - 1) / 3;
    nverts = nbeziers * 4 * s + 1;
    vx = (Sint16 *)malloc(nverts * 2 * sizeof(Sint16));
    if (vx == NULL)
    {
        free(dy);
        free(dx);
        return -1;
    }
    vy = vx + nverts;

    // Draw Beziers
    stepsize = 1.0 / (double)s;
    for (j = 0; j < nbeziers; j++)
    {
        t = 0.0;
        x1 = _evaluateBezier(dx + j * 3, 4, t);
        y1 = _evaluateBezier(dy + j * 3, 4, t);
        for (i = 0; i < 4 * s; i++)
        {
            t += stepsize;
            x2 = _evaluateBezier(dx + j * 3, 4, t);
            y2 = _evaluateBezier(dy + j * 3, 4, t);

            vx[i + j * s * 4] = floor(x1 + 0.5);
            vy[i + j * s * 4] = floor(y1 + 0.5);

            x1 = x2;
            y1 = y2;
        }
    }

    vx[j * s * 4] = floor(x1 + 0.5);
    vy[j * s * 4] = floor(y1 + 0.5);

    free(dy);
    free(dx);

    result = filledPolygonRGBA(vx, vy, nverts, r, g, b, a);

    free(vx);
    return (result);
}

// Extensions for anti-aliased filled ellipses and polygons by Richard Russell 20-Aug-2019

/*!
\brief Draw anti-aliased filled ellipse with blending.

\param renderer The renderer to draw on.
\param cx X coordinate of the center of the filled ellipse.
\param cy Y coordinate of the center of the filled ellipse.
\param rx Horizontal radius in pixels of the filled ellipse.
\param ry Vertical radius in pixels of the filled ellipse.
\param r The red value of the filled ellipse to draw.
\param g The green value of the filled ellipse to draw.
\param b The blue value of the filled ellipse to draw.
\param a The alpha value of the filled ellipse to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aaFilledEllipseRGBA(float cx, float cy, float rx, float ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int n, xi, yi, result = 0;
    double s, v, x, y, dx, dy;

    if ((rx <= 0.0) || (ry <= 0.0))
        return -1;

    result |= SDL_SetRenderDrawBlendMode(rl.r, SDL_BLENDMODE_BLEND);
    if (rx >= ry)
    {
        n = ry + 1;
        for (yi = cy - n - 1; yi <= cy + n + 1; yi++)
        {
            if (yi < (cy - 0.5))
                y = yi;
            else
                y = yi + 1;
            s = (y - cy) / ry;
            s = s * s;
            x = 0.5;
            if (s < 1.0)
            {
                x = rx * sqrt(1.0 - s);
                if (x >= 0.5)
                {
                    result |= SDL_SetRenderDrawColor(rl.r, r, g, b, a);
                    result |= renderdrawline(cx - x + 1, yi, cx + x - 1, yi);
                }
            }
            s = 8 * ry * ry;
            dy = fabs(y - cy) - 1.0;
            xi = cx - x; // left
            while (1)
            {
                dx = (cx - xi - 1) * ry / rx;
                v = s - 4 * (dx - dy) * (dx - dy);
                if (v < 0)
                    break;
                v = (sqrt(v) - 2 * (dx + dy)) / 4;
                if (v < 0)
                    break;
                if (v > 1.0)
                    v = 1.0;
                result |= SDL_SetRenderDrawColor(rl.r, r, g, b, (double)a * v);
                result |= SDL_RenderDrawPoint(rl.r, xi, yi);
                xi -= 1;
            }
            xi = cx + x; // right
            while (1)
            {
                dx = (xi - cx) * ry / rx;
                v = s - 4 * (dx - dy) * (dx - dy);
                if (v < 0)
                    break;
                v = (sqrt(v) - 2 * (dx + dy)) / 4;
                if (v < 0)
                    break;
                if (v > 1.0)
                    v = 1.0;
                result |= SDL_SetRenderDrawColor(rl.r, r, g, b, (double)a * v);
                result |= SDL_RenderDrawPoint(rl.r, xi, yi);
                xi += 1;
            }
        }
    }
    else
    {
        n = rx + 1;
        for (xi = cx - n - 1; xi <= cx + n + 1; xi++)
        {
            if (xi < (cx - 0.5))
                x = xi;
            else
                x = xi + 1;
            s = (x - cx) / rx;
            s = s * s;
            y = 0.5;
            if (s < 1.0)
            {
                y = ry * sqrt(1.0 - s);
                if (y >= 0.5)
                {
                    result |= SDL_SetRenderDrawColor(rl.r, r, g, b, a);
                    result |= renderdrawline(xi, cy - y + 1, xi, cy + y - 1);
                }
            }
            s = 8 * rx * rx;
            dx = fabs(x - cx) - 1.0;
            yi = cy - y; // top
            while (1)
            {
                dy = (cy - yi - 1) * rx / ry;
                v = s - 4 * (dy - dx) * (dy - dx);
                if (v < 0)
                    break;
                v = (sqrt(v) - 2 * (dy + dx)) / 4;
                if (v < 0)
                    break;
                if (v > 1.0)
                    v = 1.0;
                result |= SDL_SetRenderDrawColor(rl.r, r, g, b, (double)a * v);
                result |= SDL_RenderDrawPoint(rl.r, xi, yi);
                yi -= 1;
            }
            yi = cy + y; // bottom
            while (1)
            {
                dy = (yi - cy) * rx / ry;
                v = s - 4 * (dy - dx) * (dy - dx);
                if (v < 0)
                    break;
                v = (sqrt(v) - 2 * (dy + dx)) / 4;
                if (v < 0)
                    break;
                if (v > 1.0)
                    v = 1.0;
                result |= SDL_SetRenderDrawColor(rl.r, r, g, b, (double)a * v);
                result |= SDL_RenderDrawPoint(rl.r, xi, yi);
                yi += 1;
            }
        }
    }
    return result;
}

static int _gfxPrimitivesCompareFloat2(const void *a, const void *b)
{
    float diff = *((float *)a + 1) - *((float *)b + 1);
    if (diff != 0.0)
        return (diff > 0) - (diff < 0);
    diff = *(float *)a - *(float *)b;
    return (diff > 0) - (diff < 0);
}

// This constant determines the maximum size and/or complexity of polygon that can be
// drawn. Set to 16K the maximum aaArc height is approximately 1100 lines.
#define POLYSIZE 16384

/*!
\brief Draw anti-aliased filled polygon with alpha blending.

\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the filled polygon to draw.
\param g The green value of the filled polygon to draw.
\param b The blue value of the filed polygon to draw.
\param a The alpha value of the filled polygon to draw.

\returns Returns 0 on success, -1 on failure, or -2 if the polygon is too large and/or complex.
*/
int aaFilledPolygonRGBA(const double *vx, const double *vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int i, j, xi, yi, result;
    double x1, x2, y0, y1, y2, minx, maxx, prec;
    float *list, *strip;

    if (n < 3)
        return -1;

    result = SDL_SetRenderDrawBlendMode(rl.r, SDL_BLENDMODE_BLEND);

    // Find extrema:
    minx = 99999.0;
    maxx = -99999.0;
    prec = 0.00001;
    for (i = 0; i < n; i++)
    {
        double x = vx[i];
        double y = fabs(vy[i]);
        if (x < minx)
            minx = x;
        if (x > maxx)
            maxx = x;
        if (y > prec)
            prec = y;
    }
    minx = floor(minx);
    maxx = floor(maxx);
    prec = floor(pow(2, 19) / prec);

    // Allocate main array, this determines the maximum polygon size and complexity:
    list = (float *)malloc(POLYSIZE * sizeof(float));
    if (list == NULL)
        return -2;

    // Build vertex list.  Special x-values used to indicate vertex type:
    // x = -100001.0 indicates /\, x = -100003.0 indicates \/, x = -100002.0 neither
    yi = 0;
    y0 = floor(vy[n - 1] * prec) / prec;
    y1 = floor(vy[0] * prec) / prec;
    for (i = 1; i <= n; i++)
    {
        if (yi > POLYSIZE - 4)
        {
            free(list);
            return -2;
        }
        y2 = floor(vy[i % n] * prec) / prec;
        if (((y1 < y2) - (y1 > y2)) == ((y0 < y1) - (y0 > y1)))
        {
            list[yi++] = -100002.0;
            list[yi++] = y1;
            list[yi++] = -100002.0;
            list[yi++] = y1;
        }
        else
        {
            if (y0 != y1)
            {
                list[yi++] = (y1 < y0) - (y1 > y0) - 100002.0;
                list[yi++] = y1;
            }
            if (y1 != y2)
            {
                list[yi++] = (y1 < y2) - (y1 > y2) - 100002.0;
                list[yi++] = y1;
            }
        }
        y0 = y1;
        y1 = y2;
    }
    xi = yi;

    // Sort vertex list:
    qsort(list, yi / 2, sizeof(float) * 2, _gfxPrimitivesCompareFloat2);

    // Append line list to vertex list:
    for (i = 1; i <= n; i++)
    {
        double x, y;
        double d = 0.5 / prec;

        x1 = vx[i - 1];
        y1 = floor(vy[i - 1] * prec) / prec;
        x2 = vx[i % n];
        y2 = floor(vy[i % n] * prec) / prec;

        if (y2 < y1)
        {
            double tmp;
            tmp = x1;
            x1 = x2;
            x2 = tmp;
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }
        if (y2 != y1)
            y0 = (x2 - x1) / (y2 - y1);

        for (j = 1; j < xi; j += 4)
        {
            y = list[j];
            if (((y + d) <= y1) || (y == list[j + 4]))
                continue;
            if ((y -= d) >= y2)
                break;
            if (yi > POLYSIZE - 4)
            {
                free(list);
                return -2;
            }
            if (y > y1)
            {
                list[yi++] = x1 + y0 * (y - y1);
                list[yi++] = y;
            }
            y += d * 2.0;
            if (y < y2)
            {
                list[yi++] = x1 + y0 * (y - y1);
                list[yi++] = y;
            }
        }

        y = floor(y1) + 1.0;
        while (y <= y2)
        {
            x = x1 + y0 * (y - y1);
            if (yi > POLYSIZE - 2)
            {
                free(list);
                return -2;
            }
            list[yi++] = x;
            list[yi++] = y;
            y += 1.0;
        }
    }

    // Sort combined list:
    qsort(list, yi / 2, sizeof(float) * 2, _gfxPrimitivesCompareFloat2);

    // Plot lines:
    strip = (float *)malloc((maxx - minx + 2) * sizeof(float));
    if (strip == NULL)
    {
        free(list);
        return -1;
    }
    memset(strip, 0, (maxx - minx + 2) * sizeof(float));
    n = yi;
    yi = list[1];
    j = 0;

    for (i = 0; i < n - 7; i += 4)
    {
        float x1 = list[i + 0];
        float y1 = list[i + 1];
        float x3 = list[i + 2];
        float x2 = list[i + j + 0];
        float y2 = list[i + j + 1];
        float x4 = list[i + j + 2];

        if (x1 + x3 == -200002.0)
            j += 4;
        else if (x1 + x3 == -200006.0)
            j -= 4;
        else if ((x1 >= minx) && (x2 >= minx))
        {
            if (x1 > x2)
            {
                float tmp = x1;
                x1 = x2;
                x2 = tmp;
            }
            if (x3 > x4)
            {
                float tmp = x3;
                x3 = x4;
                x4 = tmp;
            }

            for (xi = x1 - minx; xi <= x4 - minx; xi++)
            {
                float u, v;
                float x = minx + xi;
                if (x < x2)
                    u = (x - x1 + 1) / (x2 - x1 + 1);
                else
                    u = 1.0;
                if (x >= x3 - 1)
                    v = (x4 - x) / (x4 - x3 + 1);
                else
                    v = 1.0;
                if ((u > 0.0) && (v > 0.0))
                    strip[xi] += (y2 - y1) * (u + v - 1.0);
            }
        }

        if ((yi == (list[i + 5] - 1.0)) || (i == n - 8))
        {
            for (xi = 0; xi <= maxx - minx; xi++)
            {
                if (strip[xi] != 0.0)
                {
                    if (strip[xi] >= 0.996)
                    {
                        int x0 = xi;
                        while (strip[++xi] >= 0.996)
                            ;
                        xi--;
                        result |= SDL_SetRenderDrawColor(rl.r, r, g, b, a);
                        result |= renderdrawline(minx + x0, yi, minx + xi, yi);
                    }
                    else
                    {
                        result |= SDL_SetRenderDrawColor(rl.r, r, g, b, a * strip[xi]);
                        result |= SDL_RenderDrawPoint(rl.r, minx + xi, yi);
                    }
                }
            }
            memset(strip, 0, (maxx - minx + 2) * sizeof(float));
            yi++;
        }
    }

    // Free arrays:
    free(list);
    free(strip);
    return result;
}

/*!
\brief Draw anti-aliased filled ellipical pie (or chord) with alpha blending.

\param renderer The renderer to draw on.
\param cx X coordinate of the center of the filled pie.
\param cy Y coordinate of the center of the filled pie.
\param rx Horizontal radius in pixels of the filled pie.
\param ry Vertical radius in pixels of the filled pie.
\param start Starting angle in degrees of the filled pie; zero is right, increasing clockwise.
\param end Ending angle in degrees of the filled pie; zero is right, increasing clockwise.
\param chord Set to 0 for a pie (sector) or 1 for a chord (segment).
\param r The red value of the filled pie to draw.
\param g The green value of the filled pie to draw.
\param b The blue value of the filled pie to draw.
\param a The alpha value of the filled pie to draw.
/
\returns Returns 0 on success, -1 on failure.
*/
int aaFilledPieRGBA(float cx, float cy, float rx, float ry,
                    float start, float end, Uint32 chord, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int nverts, i, result;
    double *vx, *vy;

    // Sanity check radii
    if ((rx <= 0) || (ry <= 0) || (start == end))
        return -1;

    // Convert degrees to radians
    start = fmod(start, 360.0) * 2.0 * M_PI / 360.0;
    end = fmod(end, 360.0) * 2.0 * M_PI / 360.0;
    while (start >= end)
        end += 2.0 * M_PI;

    // Calculate number of vertices on perimeter
    nverts = (end - start) * sqrt(rx * ry) / M_PI;
    if (nverts < 2)
        nverts = 2;
    if (nverts > 180)
        nverts = 180;

    // Allocate combined vertex array
    vx = vy = (double *)malloc(2 * sizeof(double) * (nverts + 1));
    if (vx == NULL)
        return (-1);

    // Update pointer to start of vy
    vy += nverts + 1;

    // Calculate vertices:
    for (i = 0; i < nverts; i++)
    {
        double angle = start + (end - start) * (double)i / (double)(nverts - 1);
        vx[i] = cx + rx * cos(angle);
        vy[i] = cy + ry * sin(angle);
    }

    // Center:
    vx[i] = cx;
    vy[i] = cy;

    result = aaFilledPolygonRGBA(vx, vy, nverts + 1 - (chord != 0), r, g, b, a);

    // Free combined vertex array
    free(vx);

    return (result);
}

/*!
\brief Draw anti-aliased ellipical arc with alpha blending.

\param renderer The renderer to draw on.
\param cx X coordinate of the center of the filled pie.
\param cy Y coordinate of the center of the filled pie.
\param rx Horizontal radius in pixels of the filled pie.
\param ry Vertical radius in pixels of the filled pie.
\param start Starting angle in degrees of the filled pie; zero is right, increasing clockwise.
\param end Ending angle in degrees of the filled pie; zero is right, increasing clockwise.
\param thick The thickness of the line in pixels.
\param r The red value of the filled pie to draw.
\param g The green value of the filled pie to draw.
\param b The blue value of the filled pie to draw.
\param a The alpha value of the filled pie to draw.
/
\returns Returns 0 on success, -1 on failure.
*/
int aaArcRGBA(float cx, float cy, float rx, float ry,
              float start, float end, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int nverts, i, result;
    double *vx, *vy;

    // Sanity check radii and thickness
    if ((rx <= 0) || (ry <= 0) || (start == end) || (thick <= 0))
        return -1;

    // Convert degrees to radians
    start = fmod(start, 360.0) * 2.0 * M_PI / 360.0;
    end = fmod(end, 360.0) * 2.0 * M_PI / 360.0;
    while (start >= end)
        end += 2.0 * M_PI;

    // Calculate number of vertices
    nverts = 2 * floor((end - start) * sqrt(rx * ry) / M_PI);
    if (nverts < 2)
        nverts = 2;
    if (nverts > 360)
        nverts = 360;

    // Allocate combined vertex array
    vx = vy = (double *)malloc(2 * sizeof(double) * nverts);
    if (vx == NULL)
        return (-1);

    // Update pointer to start of vy
    vy += nverts;

    // Calculate vertices:
    for (i = 0; i < nverts / 2; i++)
    {
        double angle = start + (end - start) * (double)i / (double)(nverts / 2 - 1);
        vx[i] = cx + (rx + thick / 2) * cos(angle);
        vy[i] = cy + (ry + thick / 2) * sin(angle);
        vx[nverts - 1 - i] = cx + (rx - thick / 2) * cos(angle);
        vy[nverts - 1 - i] = cy + (ry - thick / 2) * sin(angle);
    }

    result = aaFilledPolygonRGBA(vx, vy, nverts, r, g, b, a);

    // Free combined vertex array
    free(vx);

    return (result);
}

/*!
\brief Draw an anti-aliased bezier curve with alpha blending.

\param renderer The renderer to draw on.
\param x Vertex array containing X coordinates of the points of the bezier curve.
\param y Vertex array containing Y coordinates of the points of the bezier curve.
\param n Number of points in the vertex array. Minimum number is 3.
\param s Number of steps for the interpolation. Minimum number is 2.
\param thick Thickness of line in pixels.
\param r The red value of the bezier curve to draw.
\param g The green value of the bezier curve to draw.
\param b The blue value of the bezier curve to draw.
\param a The alpha value of the bezier curve to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aaBezierRGBA(double *x, double *y, int n, int s, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int i, nverts, result;
    double d, t, stepsize;
    double x1, y1, x2, y2, dx = 0.0, dy = 0.0;
    double *vx, *vy;

    // Sanity check
    if ((n < 3) || (s < 2))
        return -1;

    // Create combined vertex array:
    nverts = n * s * 2 + 2;
    vx = (double *)malloc(nverts * 2 * sizeof(double));
    if (vx == NULL)
        return -1;
    vy = vx + nverts;

    // Draw Bezier
    t = 0.0;
    stepsize = 1.0 / (double)s;
    x1 = _evaluateBezier(x, n, t);
    y1 = _evaluateBezier(y, n, t);
    for (i = 0; i < n * s; i++)
    {
        t += stepsize;
        x2 = _evaluateBezier(x, n, t);
        y2 = _evaluateBezier(y, n, t);

        dx = x2 - x1;
        dy = y2 - y1;
        d = thick * 0.5L / sqrt(dx * dx + dy * dy);
        dx *= d;
        dy *= d;

        vx[i] = x1 + dy;
        vy[i] = y1 - dx;
        vx[nverts - 1 - i] = x1 - dy;
        vy[nverts - 1 - i] = y1 + dx;

        x1 = x2;
        y1 = y2;
    }

    vx[i] = x1 + dy;
    vy[i] = y1 - dx;
    vx[nverts - 1 - i] = x1 - dy;
    vy[nverts - 1 - i] = y1 + dx;

    result = aaFilledPolygonRGBA(vx, vy, nverts, r, g, b, a);

    free(vx);
    return (result);
}

/*!
\brief Fill an anti-aliased region bounded by cubic Bezier curves, with alpha blending.

\param renderer The renderer to draw on.
\param x Vertex array containing X coordinates of the points of the bezier curves.
\param y Vertex array containing Y coordinates of the points of the bezier curves.
\param n Number of points in the vertex array. Should be 3n + 1 for n bezier curves.
\param s Number of steps for the interpolation. Minimum number is 2.
\param r The red value of the bezier curve to draw.
\param g The green value of the bezier curve to draw.
\param b The blue value of the bezier curve to draw.
\param a The alpha value of the bezier curve to draw.

\returns Returns 0 on success, -1 on failure.
*/
int aaFilledPolyBezierRGBA(double *x, double *y, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int i, j, nbeziers, nverts, result;
    double t, stepsize;
    double x1, y1, x2, y2;
    double *vx, *vy;

    // Sanity check
    if ((n < 7) || (s < 2))
        return -1;

    // Create combined vertex array:
    nbeziers = (n - 1) / 3;
    nverts = nbeziers * 4 * s + 1;
    vx = (double *)malloc(nverts * 2 * sizeof(double));
    if (vx == NULL)
        return -1;
    vy = vx + nverts;

    // Draw Beziers
    stepsize = 1.0 / (double)s;
    for (j = 0; j < nbeziers; j++)
    {
        t = 0.0;
        x1 = _evaluateBezier(x + j * 3, 4, t);
        y1 = _evaluateBezier(y + j * 3, 4, t);
        for (i = 0; i < 4 * s; i++)
        {
            t += stepsize;
            x2 = _evaluateBezier(x + j * 3, 4, t);
            y2 = _evaluateBezier(y + j * 3, 4, t);

            vx[i + j * s * 4] = x1;
            vy[i + j * s * 4] = y1;

            x1 = x2;
            y1 = y2;
        }
    }

    vx[j * s * 4] = x1;
    vy[j * s * 4] = y1;

    result = aaFilledPolygonRGBA(vx, vy, nverts, r, g, b, a);

    free(vx);
    return (result);
}
