#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>
#include <raygfx.h>


// TODO: support rendering outside of the bounds [0, 0, render_width, render_height]
RLCAPI void UpdateCameraTexture() {
    int w = GetRenderWidth();
    int h = GetRenderHeight();
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
    rl.co.x = camera.offset.x - camera.target.x;
    rl.co.y = camera.offset.y - camera.target.y;
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
