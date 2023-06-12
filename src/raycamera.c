#include <raylib.h>
#include <raydef.h>
#include <raymath.h>
#include <rayconf.h>
#include <raygfx.h>

// TODO:
//  - Support rendering outside of the bounds [0, 0, render_width, render_height]
//  - Fix with render texture (I think it's bugged, but not tested)
//  - Fix offset when scaling

RLCAPI void UpdateCameraTexture() {
    int w, h;
    if (rl.texture_backup == NULL) {
        w = GetRenderWidth();
        h = GetRenderHeight();
    }
    else if (SDL_QueryTexture(rl.texture_backup, NULL, NULL, &w, &h) < 0) {
        QUERY_TEXTURE_WARN();
        return;
    }
    if (rl.screen_tex)
        SDL_DestroyTexture(rl.screen_tex);
    rl.screen_tex = SDL_CreateTexture(
        rl.r,
        DRAW_TEXTURE_FORMAT_ALPHA,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );
    if (rl.screen_tex == NULL) {
        CREATE_TEXTURE_WARN();
        return;
    }
    if (SDL_SetRenderTarget(rl.r, rl.screen_tex) < 0)
        RENDER_TARGET_WARN();
    rl.z_en = true;
}

RLCAPI void BeginMode2D(Camera2D camera) {
    rl.texture_backup = SDL_GetRenderTarget(rl.r);
    if (camera.zoom < 0.0f) {
        rl.z = -camera.zoom;
        rl.cam_flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
    }
    else {
        rl.z = camera.zoom;
        rl.cam_flip = SDL_FLIP_NONE;
    }
    rl.cam_rot = (double)camera.rotation;
    while (rl.cam_rot >= 360.0)
        rl.cam_rot -= 360.0;
    while (rl.cam_rot < 0.0)
        rl.cam_rot += 360.0;
    rl.cam_origin.x = camera.target.x;
    rl.cam_origin.y = camera.target.y;
    // FIXME: how does with work?
    rl.co.x = camera.offset.x / rl.z - camera.target.x;
    rl.co.y = camera.offset.y / rl.z - camera.target.y;
    UpdateCameraTexture();
}

RLCAPI void EndMode2D(void) {
    if (SDL_SetRenderTarget(rl.r, NULL) < 0)
        RENDER_TARGET_WARN();
    if (rl.screen_tex) {
        //SDL_FRect dst_rect = { -rl.cam_origin.x, -rl.cam_origin.y,
        //    (float)GetRenderWidth(), (float)GetRenderHeight() };
        if (SDL_RenderCopyExF(
            rl.r, rl.screen_tex, NULL, NULL,
            rl.cam_rot, &rl.cam_origin, rl.cam_flip
        ) < 0)
            RENDER_COPY_WARN();
        SDL_DestroyTexture(rl.screen_tex);
        rl.screen_tex = NULL;
    }
    rl.z_en = false;
}

RLCAPI Matrix GetCameraMatrix2D(Camera2D camera) {
    Matrix matTransform = { 0 };
    Matrix matOrigin = MatrixTranslate(-camera.target.x, -camera.target.y, 0.0f);
    Matrix matRotation = MatrixRotate((Vector3){ 0.0f, 0.0f, 1.0f }, camera.rotation*DEG2RAD);
    Matrix matScale = MatrixScale(camera.zoom, camera.zoom, 1.0f);
    Matrix matTranslation = MatrixTranslate(camera.offset.x, camera.offset.y, 0.0f);
    matTransform = MatrixMultiply(MatrixMultiply(matOrigin, MatrixMultiply(matScale, matRotation)), matTranslation);
    return matTransform;
}

RLCAPI Vector2 GetScreenToWorld2D(Vector2 position, Camera2D camera) {
    Matrix invMatCamera = MatrixInvert(GetCameraMatrix2D(camera));
    Vector3 transform = Vector3Transform((Vector3){ position.x, position.y, 0 }, invMatCamera);

    return (Vector2){ transform.x, transform.y };
}

RLCAPI Vector2 GetWorldToScreen2D(Vector2 position, Camera2D camera) {
    Matrix matCamera = GetCameraMatrix2D(camera);
    Vector3 transform = Vector3Transform((Vector3){ position.x, position.y, 0 }, matCamera);

    return (Vector2){ transform.x, transform.y };
}
