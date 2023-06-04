#include <raylib.h>
#include <raydef.h>
#include <rayconf.h>

RLCAPI bool IsKeyPressed(int key) {
    if (key >= 0 && key < rl.num_kbd_keys) {
#ifdef HANDLE_KEY_PRESS
        return rl.keypress_array[key] == 1;
#else
        return false;
#endif
    }
    TRACELOG(LOG_WARNING, "Got Invalid Key %i", key);
    return false;
}

RLCAPI bool IsKeyDown(int key) {
    if (key >= 0 && key < rl.num_kbd_keys) {
        return (bool)rl.kbd_array[CONVERT_KEY_CODE(key)];
    }
    TRACELOG(LOG_WARNING, "Got Invalid Key %i", key);
    return false;
}

RLCAPI bool IsKeyReleased(int key) {
    if (key >= 0 && key < rl.num_kbd_keys) {
#ifdef HANDLE_KEY_PRESS
        return rl.keypress_array[key] == 2;
#else
        return false;
#endif
    }
    TRACELOG(LOG_WARNING, "Got Invalid Key %i", key);
    return false;
}

RLCAPI bool IsKeyUp(int key) {
    if (key >= 0 && key < rl.num_kbd_keys) {
        return !rl.kbd_array[CONVERT_KEY_CODE(key)];
    }
    TRACELOG(LOG_WARNING, "Got Invalid Key %i", key);
    return true;
}

RLCAPI void SetExitKey(int key) {
    rl.exit_key = key;
}

RLCAPI int GetKeyPressed(void) {
    return 0; // TODO
}

RLCAPI int GetCharPressed(void) {
    return 0;
}

RLCAPI bool IsMouseButtonPressed(int button) {}

RLCAPI bool IsMouseButtonDown(int button) {}

RLCAPI bool IsMouseButtonReleased(int button) {}

RLCAPI bool IsMouseButtonUp(int button) {}

RLCAPI int GetMouseX(void) {}

RLCAPI int GetMouseY(void) {}

RLCAPI Vector2 GetMousePosition(void) {}

RLCAPI Vector2 GetMouseDelta(void) {}

RLCAPI void SetMousePosition(int x, int y) {}

RLCAPI void SetMouseOffset(int offsetX, int offsetY) {}

RLCAPI void SetMouseScale(float scaleX, float scaleY) {}

RLCAPI float GetMouseWheelMove(void) {}

RLCAPI Vector2 GetMouseWheelMoveV(void) {}

RLCAPI void SetMouseCursor(int cursor) {}
