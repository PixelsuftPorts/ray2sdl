#pragma once

#define SUPPORT_TRACELOG
#define MAX_TRACELOG_MSG_LENGTH 1024
#define MAX_FILE_EXTENSION_SIZE  16

#ifdef PATH_MAX
#define MAX_FILEPATH_LENGTH PATH_MAX
#else
#define MAX_FILEPATH_LENGTH 4096
#endif
#define MAX_FILENAME_LENGTH MAX_FILEPATH_LENGTH
#define MAX_FILEPATH_CAPACITY 8192
// #define SUPPORT_FILES_DROPPING
#define TOUCH_DEVICE_ID 0
#define EMULATE_MOUSE_WITH_TOUCH
#define ENABLE_CLOCK
#define USE_SCANCODES
#define HANDLE_KEY_PRESS
#define PREFER_HW_FUNCTIONS
#define IMG_SUPPORT
#define DEFAULT_BPP 32
#define DRAW_RGB_FORMAT SDL_PIXELFORMAT_RGB888
#define DRAW_RGBA_FORMAT SDL_PIXELFORMAT_RGBA8888
#define DRAW_TEXTURE_FORMAT DRAW_RGB_FORMAT
#define DRAW_TEXTURE_FORMAT_ALPHA DRAW_RGBA_FORMAT
#define ENABLE_IMAGE_RLE
#define RENDER_ID 2
