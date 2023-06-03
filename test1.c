#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#ifndef __imp__wassert
void __imp__wassert() {}
#endif

int main(void)
{
    SetWindowState(FLAG_WINDOW_ALWAYS_RUN | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(800, 450, "raylib [core] example - basic window");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
