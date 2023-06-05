#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>

#if defined(_WIN32) && !defined(__imp__wassert)
void __imp__wassert() {}
#endif

int main(void)
{
    Camera2D camera;
    memset(&camera, 0, sizeof(Camera2D));
    camera.zoom = 1.0f;
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
        if (IsKeyDown(KEY_RIGHT))
            camera.offset.x += 1.0f;
        else if (IsKeyDown(KEY_LEFT))
            camera.offset.x -= 1.0f;
        if (IsKeyDown(KEY_UP))
            camera.offset.y -= 1.0f;
        else if (IsKeyDown(KEY_DOWN))
            camera.offset.y += 1.0f;
        if (IsKeyDown(KEY_D))
            camera.target.x += 1.0f;
        else if (IsKeyDown(KEY_A))
            camera.target.x -= 1.0f;
        if (IsKeyDown(KEY_W))
            camera.target.y -= 1.0f;
        else if (IsKeyDown(KEY_S))
            camera.target.y += 1.0f;
        if (IsKeyDown(KEY_Z))
            camera.zoom += 0.05f;
        else if (IsKeyDown(KEY_X))
            camera.zoom -= 0.05f;
        if (IsKeyDown(KEY_C))
            camera.rotation += 1.0f;
        else if (IsKeyDown(KEY_V))
            camera.rotation -= 1.0f;
        if (IsKeyPressed(KEY_R)) {
            memset(&camera, 0, sizeof(Camera2D));
            camera.zoom = 1.0f;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        if (IsKeyUp(KEY_B))
            BeginMode2D(camera);
        int screenWidth = GetScreenWidth();
        DrawRectangle(0, 0, screenWidth, GetScreenHeight(), (Color){ 50, 50, 50, 255 });
        DrawCircle(screenWidth/5, 120, 35, DARKBLUE);
        DrawCircleGradient(screenWidth/5, 220, 60, GREEN, SKYBLUE);
        DrawCircleLines(screenWidth/5, 340, 80, DARKBLUE);
        DrawRectangleRounded((Rectangle){screenWidth/4*2 - 60, 100, 120, 60}, 0.4f, 0, RED);
        DrawRectangleGradientH(screenWidth/4*2 - 90, 170, 180, 130, MAROON, GOLD);
        DrawRectangleLines(screenWidth/4*2 - 40, 320, 80, 60, ORANGE);
        DrawTriangle((Vector2){ screenWidth/4.0f *3.0f, 80.0f },
                        (Vector2){ screenWidth/4.0f *3.0f - 60.0f, 150.0f },
                        (Vector2){ screenWidth/4.0f *3.0f + 60.0f, 150.0f }, VIOLET);

        DrawTriangleLines((Vector2){ screenWidth/4.0f*3.0f, 160.0f },
                            (Vector2){ screenWidth/4.0f*3.0f - 20.0f, 230.0f },
                            (Vector2){ screenWidth/4.0f*3.0f + 20.0f, 230.0f }, DARKBLUE);
        DrawPixel(10, 10, RED);
        DrawCircleGradient(GetMouseX(), GetMouseY(), 25.0f, RED, (Color){ 0, 0, 0, 0 });
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
