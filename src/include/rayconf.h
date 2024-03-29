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
#define IMG_FORMATS IMG_INIT_JPG | IMG_INIT_PNG
#define MIX_IMPL
#define MIX_SUPPORT
#define MIX_FORMATS MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG
#define MIX_FREQ MIX_DEFAULT_FREQUENCY
#define MIX_FORMAT AUDIO_F32
#define MIX_CHANNELS MIX_DEFAULT_CHANNELS
#define MIX_CHUNK_SIZE 2048
#define MIX_DEVICE NULL
#define DEFAULT_FONT_PATH "assets/segoeuib.ttf"
#define MIX_ALLOWED_CHANGES 0
#define TTF_SUPPORT
#define DEFAULT_FONT_SIZE 32
#define TTF_AA
#define DEFAULT_BPP 32
#define TEXTURE_NONE_BLEND SDL_BLENDMODE_BLEND
#define DEFAULT_BLEND SDL_BLENDMODE_BLEND
#define DRAW_RGB_FORMAT SDL_PIXELFORMAT_RGB888
#define DRAW_RGBA_FORMAT SDL_PIXELFORMAT_RGBA8888
#define DRAW_TEXTURE_FORMAT DRAW_RGB_FORMAT
#define DRAW_TEXTURE_FORMAT_ALPHA DRAW_RGBA_FORMAT
#define RENDER_TEXTURE_DEFAULT_ACCESS SDL_TEXTUREACCESS_TARGET
#define ENABLE_IMAGE_RLE
#define RENDERER_DEFAULT_ID -1
#define RENDERER_NAME "opengl"
#define MAX_STR_LENGTH SDL_MAX_SINT16
