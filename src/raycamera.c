#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>
#include <raygfx.h>

RLCAPI void UpdateCameraTexture() {
    int w = GetScreenWidth();
    int h = GetScreenHeight();
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
    if (camera.zoom < 0.0f) {
        rl.z = -camera.zoom;
        rl.cam_rot = (double)camera.rotation + 180.0;
    }
    else {
        rl.z = camera.zoom;
        rl.cam_rot = (double)camera.rotation;
    }
    while (rl.cam_rot >= 360.0)
        rl.cam_rot -= 360.0;
    while (rl.cam_rot < 0.0)
        rl.cam_rot += 360.0;
    rl.rot_origin = *(SDL_FPoint*)&camera.target;
    rl.co = camera.offset;
    UpdateCameraTexture();
}

RLCAPI void EndMode2D(void) {
    if (SDL_SetRenderTarget(rl.r, NULL) < 0)
        RENDER_TARGET_WARN();
    if (rl.screen_tex) {
        if (SDL_RenderCopyExF(
            rl.r, rl.screen_tex, NULL, NULL,
            rl.cam_rot, &rl.rot_origin, SDL_FLIP_NONE
        ) < 0)
            RENDER_COPY_WARN();
        SDL_DestroyTexture(rl.screen_tex);
        rl.screen_tex = NULL;
    }
    rl.z_en = false;
}
