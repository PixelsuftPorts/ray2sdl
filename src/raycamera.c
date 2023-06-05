#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>
#include <raygfx.h>

RLCAPI void BeginMode2D(Camera2D camera) {
    if (camera.zoom < 0.0f) {
        rl.z = -camera.zoom;
    }
    else {
        rl.z = camera.zoom;
    }
    rl.co = camera.offset;
    rl.z_en = true;
}

RLCAPI void EndMode2D(void) {
    rl.z_en = false;
}
