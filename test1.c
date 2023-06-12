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
    Image img1 = LoadImage("assets/image.png");
    SetWindowIcon(img1);
    UnloadImage(img1);
    InitAudioDevice();
    Music mus1 = LoadMusicStream("assets/music1.mp3");
    Sound sound1 = LoadSound("assets/sound1.ogg");
    SetMusicVolume(mus1, 0.2f);
    Texture tex1 = LoadTexture("assets/win7.png");
    SetExitKey(KEY_Q);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(mus1);
        if (IsWindowResized()) {
            printf("Window Resized: %ix%i\n", GetScreenWidth(), GetScreenHeight());
        }
        if (IsFileDropped()) {
            FilePathList files = LoadDroppedFiles();
            for (size_t i = 0; i < files.count; i++) {
                printf("File dropped: %s\n", files.paths[i]);
            }
            UnloadDroppedFiles(files);
        }
        if (IsKeyPressed(KEY_SPACE)) {
            IsMusicStreamPlaying(mus1) ? StopMusicStream(mus1) : PlayMusicStream(mus1);
            printf("Space Down\n");
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            SetSoundPan(sound1, 1.0f - (float)GetMouseX() / (float)GetScreenWidth());
            PlaySound(sound1);
            printf("Click at %ix%i\n", GetMouseX(), GetMouseY());
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
        else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        float speed_mul = GetFrameTime() * 60.0f;
        if (IsKeyDown(KEY_RIGHT))
            camera.offset.x += 4.0f * speed_mul;
        else if (IsKeyDown(KEY_LEFT))
            camera.offset.x -= 4.0f * speed_mul;
        if (IsKeyDown(KEY_UP))
            camera.offset.y -= 4.0f * speed_mul;
        else if (IsKeyDown(KEY_DOWN))
            camera.offset.y += 4.0f * speed_mul;
        if (IsKeyDown(KEY_D))
            camera.target.x += 4.0f * speed_mul;
        else if (IsKeyDown(KEY_A))
            camera.target.x -= 4.0f * speed_mul;
        if (IsKeyDown(KEY_W))
            camera.target.y -= 4.0f * speed_mul;
        else if (IsKeyDown(KEY_S))
            camera.target.y += 4.0f * speed_mul;
        if (IsKeyDown(KEY_Z))
            camera.zoom += 0.05f * speed_mul;
        else if (IsKeyDown(KEY_X))
            camera.zoom -= 0.05f * speed_mul;
        if (IsKeyDown(KEY_C))
            camera.rotation += 1.0f * speed_mul;
        else if (IsKeyDown(KEY_V))
            camera.rotation -= 1.0f * speed_mul;
        if (IsKeyPressed(KEY_R)) {
            memset(&camera, 0, sizeof(Camera2D));
            camera.zoom = 1.0f;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        if (IsKeyUp(KEY_B))
            BeginMode2D(camera);
        ClearBackground(BLACK);
        char fps_buf[15] = "FPS: ";
        itoa(GetFPS(), fps_buf + 5, 10);
        SetWindowTitle(fps_buf);
        int screenWidth = GetRenderWidth();
        DrawRectangle(0, 0, screenWidth, GetRenderHeight(), (Color){ 50, 50, 50, 255 });
        DrawTexturePro(
            tex1, (Rectangle){0.0f, 0.0f, 1000.0f, 5000.0f},
            (Rectangle){0.0f, 0.0f, 2000.0f, 1000.0f}, (Vector2){0.0f, 0.0f}, 20.0f, WHITE
        );
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
        Vector2 screen_pos = GetScreenToWorld2D((Vector2){GetMouseX(), GetMouseY()}, camera);
        //BeginBlendMode(BLEND_MULTIPLIED);
        DrawCircleGradient((int)screen_pos.x, (int)screen_pos.y, 25.0f, RED, (Color){ 0, 0, 0, 0 });
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        //EndBlendMode();
        DrawFPS(0, 0);
        EndMode2D();
        DrawRectangleRec((Rectangle){
            GetMusicTimePlayed(mus1) / GetMusicTimeLength(mus1) * ((float)GetRenderWidth() - 50.0f),
            (float)GetRenderHeight() - 50.0f, 50.0f, 50.0f
        }, RED);
        EndDrawing();
    }

    UnloadTexture(tex1);
    UnloadSound(sound1);
    UnloadMusicStream(mus1);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
