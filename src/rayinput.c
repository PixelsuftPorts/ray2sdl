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

RLCAPI bool IsMouseButtonPressed(int button) { // TODO: I don't think it's good to make these functions to be safe
    return rl.mousepress_array[button] == 1;
}

RLCAPI bool IsMouseButtonDown(int button) {
    return (bool)(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button));
}

RLCAPI bool IsMouseButtonReleased(int button) {
    return rl.mousepress_array[button] == 2;
}

RLCAPI bool IsMouseButtonUp(int button) {
    return !(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button));
}

RLCAPI int GetMouseX(void) {
    int x;
    SDL_GetMouseState(&x, NULL);
    return x;
}

RLCAPI int GetMouseY(void) {
    int y;
    SDL_GetMouseState(NULL, &y);
    return y;
}

RLCAPI Vector2 GetMousePosition(void) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return VECLITERAL(Vector2) {.x = y, .y = y};
}

RLCAPI Vector2 GetMouseDelta(void) {
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    return VECLITERAL(Vector2) {.x = y, .y = y};
}

RLCAPI void SetMousePosition(int x, int y) {
    SDL_WarpMouseInWindow(rl.w, x, y);
}

RLCAPI void SetMouseOffset(int offsetX, int offsetY) {
//TODO
}

RLCAPI void SetMouseScale(float scaleX, float scaleY) {}

RLCAPI float GetMouseWheelMove(void) {
    return rl.wheel_move.y;
}

RLCAPI Vector2 GetMouseWheelMoveV(void) {
    return rl.wheel_move;
}

RLCAPI void SetMouseCursor(int cursor) {
    SDL_Cursor* cursor_handle = cursor >= 0 ? SDL_CreateSystemCursor(cursor) : SDL_GetDefaultCursor();
    if (cursor_handle == NULL) {
        TRACELOG(LOG_WARNING, "Failed to set get system cursor (%s)", SDL_GetError());
        return;
    }
    SDL_SetCursor(cursor_handle);
    if (rl.cursor)
        SDL_FreeCursor(rl.cursor);
    rl.cursor = cursor >= 0 ? cursor_handle : NULL;
}
