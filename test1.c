#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#if defined(_WIN32) && !defined(__imp__wassert)
void __imp__wassert() {}
#endif

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "raylib [core] example - basic window");

    unsigned int bytes_read;
    unsigned char* data = LoadFileData(".gitignore", &bytes_read);
    data[bytes_read - 1] = '\0';
    printf("%s\n", data);

    while (!WindowShouldClose())
    {
        if (IsWindowResized()) {
            printf("Window Resized: [%ix%i]\n", GetScreenWidth(), GetScreenHeight());
        }
        BeginDrawing();
        ClearBackground(BLACK);
            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
