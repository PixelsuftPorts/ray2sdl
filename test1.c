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
    SetExitKey(KEY_Q);

    while (!WindowShouldClose())
    {
        if (IsWindowResized()) {
            printf("Window Resized: [%ix%i]\n", GetScreenWidth(), GetScreenHeight());
        }
        if (IsFileDropped()) {
            FilePathList files = LoadDroppedFiles();
            for (size_t i = 0; i < files.count; i++) {
                printf("File dropped: %s\n", files.paths[i]);
            }
            UnloadDroppedFiles(files);
        }
        if (IsKeyPressed(KEY_SPACE)) {
            printf("Space Down\n");
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            printf("Click at %ix%i\n", GetMouseX(), GetMouseY());
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
        else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircleGradient(GetMouseX(), GetMouseY(), 50.0f, RED, (Color){ 0, 0, 0, 0 });
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
