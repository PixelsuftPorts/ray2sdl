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

RLCAPI int GetTouchX(void) {
    SDL_Finger* result = SDL_GetTouchFinger(rl.touch_dev, 0);
    if (result == NULL)
        TRACELOG(LOG_WARNING, "Failed to get touch finger");
    return result->x;
}

RLCAPI int GetTouchY(void) {
    if (!rl.touch_dev) {
        TRACELOG(LOG_WARNING, "Device is not opened");
        return 0;
    }
    SDL_Finger* result = SDL_GetTouchFinger(rl.touch_dev, 0);
    if (result == NULL)
        TRACELOG(LOG_WARNING, "Failed to get touch finger");
    return result->y;
}

RLCAPI Vector2 GetTouchPosition(int index) {
    if (!rl.touch_dev) {
        TRACELOG(LOG_WARNING, "Device is not opened");
        return VECLITERAL(Vector2){.x = 0.0f, .y = 0.0f};
    }
    SDL_Finger* result = SDL_GetTouchFinger(rl.touch_dev, 0);
    if (result == NULL)
        TRACELOG(LOG_WARNING, "Failed to get touch finger");
    return VECLITERAL(Vector2){.x = (float)result->x, .y = (float)result->y};
}

RLCAPI int GetTouchPointId(int index) {
    if (!rl.touch_dev) {
        TRACELOG(LOG_WARNING, "Device is not opened");
        return 0;
    }
    SDL_Finger* result = SDL_GetTouchFinger(rl.touch_dev, index);
    if (result == NULL)
        TRACELOG(LOG_WARNING, "Failed to get touch finger");
    return result->id;
}

RLCAPI int GetTouchPointCount(void) {
    if (!rl.touch_dev) {
        TRACELOG(LOG_WARNING, "Device is not opened");
        return 0;
    }
    int result = SDL_GetNumTouchFingers(rl.touch_dev);
    if (!result)
        TRACELOG(LOG_WARNING, "Failed to get num of touch fingers (%s)", SDL_GetError());
    return result;
}

RLCAPI bool IsGamepadAvailable(int gamepad) { // TODO
    return false;
}

RLCAPI const char *GetGamepadName(int gamepad) {
    return "";
}

RLCAPI bool IsGamepadButtonPressed(int gamepad, int button) {
    return false;
}

RLCAPI bool IsGamepadButtonDown(int gamepad, int button) {
    return false;
}

RLCAPI bool IsGamepadButtonReleased(int gamepad, int button) {
    return false;
}

RLCAPI bool IsGamepadButtonUp(int gamepad, int button) {
    return false;
}

RLCAPI int GetGamepadButtonPressed(void) {
    return 0;
}

RLCAPI int GetGamepadAxisCount(int gamepad) {
    return 0;
}

RLCAPI float GetGamepadAxisMovement(int gamepad, int axis) {
    return 0.0f;
}

RLCAPI int SetGamepadMappings(const char *mappings) {
    return 0;
}

RLCAPI void SetGesturesEnabled(unsigned int flags) {

}

RLCAPI bool IsGestureDetected(int gesture) {
    return false;
}

RLCAPI int GetGestureDetected(void) {
    return 0;
}

RLCAPI float GetGestureHoldDuration(void) {
    return 0.0f;
}

RLCAPI Vector2 GetGestureDragVector(void) {
    Vector2 result = { 0 };
    return result;
}

RLCAPI float GetGestureDragAngle(void) {
    return 0.0f;
}

RLCAPI Vector2 GetGesturePinchVector(void) {
    Vector2 result = { 0 };
    return result;
}

RLCAPI float GetGesturePinchAngle(void) {
    return 0.0f;
}
