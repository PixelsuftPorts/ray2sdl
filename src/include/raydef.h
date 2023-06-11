#pragma once
#include <rayconf.h>
#include <SDL2/SDL.h>
#ifdef MIX_SUPPORT
#include <SDL2/SDL_mixer.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define RAYLIB_VERSION_MAJOR 228
#define RAYLIB_VERSION_MINOR 0
#define RAYLIB_VERSION_PATCH 0
#define RAYLIB_VERSION  "228.0"

#if defined(_WIN32) && 0
    #if defined(BUILD_LIBTYPE_SHARED)
        #if defined(__TINYC__)
            #define __declspec(x) __attribute__((x))
        #endif
        #define RLAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define RLAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

#ifndef RLAPI
    #define RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

#ifndef RLCAPI
    #define RLCAPI
#endif

#if defined(SUPPORT_TRACELOG)
    #define TRACELOG(level, msg, ...) TraceLog(level, "[%s:%i at %s] " msg, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
    #define TRACELOG(level, msg, ...) (void)0
#endif

#ifndef PI
    #define PI M_PI
#endif
#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

#ifndef RL_MALLOC
    #define RL_MALLOC(sz)       SDL_malloc(sz)
#endif
#ifndef RL_CALLOC
    #define RL_CALLOC(n,sz)     SDL_calloc(n,sz)
#endif
#ifndef RL_REALLOC
    #define RL_REALLOC(ptr,sz)  SDL_realloc(ptr,sz)
#endif
#ifndef RL_FREE
    #define RL_FREE(ptr)        SDL_free(ptr)
#endif

#if defined(__cplusplus)
    #define CLITERAL(type)      type
#else
    #define CLITERAL(type)      (type)
#endif
#define VECLITERAL(type) CLITERAL(type)

#define RL_COLOR_TYPE
#define RL_RECTANGLE_TYPE
#define RL_VECTOR2_TYPE
#define RL_VECTOR3_TYPE
#define RL_VECTOR4_TYPE
#define RL_QUATERNION_TYPE
#define RL_MATRIX_TYPE

#define LIGHTGRAY  CLITERAL(Color){ 200, 200, 200, 255 }   // Light Gray
#define GRAY       CLITERAL(Color){ 130, 130, 130, 255 }   // Gray
#define DARKGRAY   CLITERAL(Color){ 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     CLITERAL(Color){ 253, 249, 0, 255 }     // Yellow
#define GOLD       CLITERAL(Color){ 255, 203, 0, 255 }     // Gold
#define ORANGE     CLITERAL(Color){ 255, 161, 0, 255 }     // Orange
#define PINK       CLITERAL(Color){ 255, 109, 194, 255 }   // Pink
#define RED        CLITERAL(Color){ 230, 41, 55, 255 }     // Red
#define MAROON     CLITERAL(Color){ 190, 33, 55, 255 }     // Maroon
#define GREEN      CLITERAL(Color){ 0, 228, 48, 255 }      // Green
#define LIME       CLITERAL(Color){ 0, 158, 47, 255 }      // Lime
#define DARKGREEN  CLITERAL(Color){ 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    CLITERAL(Color){ 102, 191, 255, 255 }   // Sky Blue
#define BLUE       CLITERAL(Color){ 0, 121, 241, 255 }     // Blue
#define DARKBLUE   CLITERAL(Color){ 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     CLITERAL(Color){ 200, 122, 255, 255 }   // Purple
#define VIOLET     CLITERAL(Color){ 135, 60, 190, 255 }    // Violet
#define DARKPURPLE CLITERAL(Color){ 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      CLITERAL(Color){ 211, 176, 131, 255 }   // Beige
#define BROWN      CLITERAL(Color){ 127, 106, 79, 255 }    // Brown
#define DARKBROWN  CLITERAL(Color){ 76, 63, 47, 255 }      // Dark Brown

#define WHITE      CLITERAL(Color){ 255, 255, 255, 255 }   // White
#define BLACK      CLITERAL(Color){ 0, 0, 0, 255 }         // Black
#define BLANK      CLITERAL(Color){ 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    CLITERAL(Color){ 255, 0, 255, 255 }     // Magenta
#define RAYWHITE   CLITERAL(Color){ 245, 245, 245, 255 }   // My own White (raylib logo)

#define BLEND_WARN() TRACELOG(LOG_WARNING, "Failed to apply blend or color (%s)", SDL_GetError())
#define DRAW_WARN() TRACELOG(LOG_WARNING, "Failed to draw (%s)", SDL_GetError())
#define GFX_WARN() TRACELOG(LOG_WARNING, "Failed to draw complex graphics (%s)", SDL_GetError())
#define CREATE_SURF_WARN() TRACELOG(LOG_WARNING, "Failed to create surface (%s)", SDL_GetError())
#define CREATE_TEXTURE_WARN() TRACELOG(LOG_WARNING, "Failed to create texture (%s)", SDL_GetError())
#define RENDER_TARGET_WARN() TRACELOG(LOG_WARNING, "Failed to set render target (%s)", SDL_GetError())
#define RENDER_COPY_WARN() TRACELOG(LOG_WARNING, "Failed to copy texture (%s)", SDL_GetError())
#define SCALE_MODE_WARN() TRACELOG(LOG_WARNING, "Failed to set scale mode (%s)", SDL_GetError())
#define SCALE_WARN() TRACELOG(LOG_WARNING, "Failed to set scale (%s)", SDL_GetError())
#define SET_RLE_WARN() TRACELOG(LOG_WARNING, "Failed to set surface RLE (%s)", SDL_GetError())
#define QUERY_TEXTURE_WARN() TRACELOG(LOG_WARNING, "Failed to query texture (%s)", SDL_GetError())
#define TINT_WARN() TRACELOG(LOG_WARNING, "Failed to set texture color/alpha mod (%s)", SDL_GetError())
#define GET_COLOR_MASK_WARN() TRACELOG(LOG_WARNING, "Failed to get color masks for pixel format (%s)", SDL_GetError())
#define FILENAME_WARN() TRACELOG(LOG_WARNING, "File name provided is not valid")
#define NULLPTR_WARN() TRACELOG(LOG_WARNING, "NULL pointer passed")

#define APPLY_BLEND_RGBA(color_r, color_g, color_b, color_a) (\
    SDL_SetRenderDrawBlendMode(rl.r, (color_a) >= 255 ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND) |\
    SDL_SetRenderDrawColor(rl.r, (color_r), (color_g), (color_b), (color_a)))
#define APPLY_BLEND(color) APPLY_BLEND_RGBA((color.r), (color.g), (color.b), (color.a))
#define CREATE_DRAW_TEXTURE(width, height, color_a) SDL_CreateTexture(\
    rl.r, (color_a) >= 255 ? DRAW_TEXTURE_FORMAT : DRAW_TEXTURE_FORMAT_ALPHA, SDL_TEXTUREACCESS_TARGET,\
    width, height)
#define APPLY_TEXTURE_BLEND(texture, color) SDL_SetTextureBlendMode(\
    texture, color.a >= 255 ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND)
#define RENDER_ENABLE_SCALE() SDL_RenderSetScale(rl.r, rl.z, rl.z)
#define RENDER_DISABLE_SCALE() SDL_RenderSetScale(rl.r, 1.0f, 1.0f)
#define APPLY_TEXTURE_TINT(texture, tint) (SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b) |\
    SDL_SetTextureBlendMode(texture, tint.a >= 255 ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND) |\
    SDL_SetTextureAlphaMod(texture, tint.a))

#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false = 0, true = !false } bool;
    #define RL_BOOL_TYPE
#endif

// Vector2, 2 components
typedef struct Vector2 {
    float x;                // Vector x component
    float y;                // Vector y component
} Vector2;

// Vector3, 3 components
typedef struct Vector3 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
} Vector3;

// Vector4, 4 components
typedef struct Vector4 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
    float w;                // Vector w component
} Vector4;

// Quaternion, 4 components (Vector4 alias)
typedef Vector4 Quaternion;

// Matrix, 4x4 components, column major, OpenGL style, right-handed
typedef struct Matrix {
    float m0, m4, m8, m12;  // Matrix first row (4 components)
    float m1, m5, m9, m13;  // Matrix second row (4 components)
    float m2, m6, m10, m14; // Matrix third row (4 components)
    float m3, m7, m11, m15; // Matrix fourth row (4 components)
} Matrix;

// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;

// Rectangle, 4 components
typedef struct Rectangle {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height
} Rectangle;

// Image, pixel data stored in CPU memory (RAM)
typedef struct Image {
    SDL_Surface* surf;
    void *data;             // Image raw data
    int width;              // Image base width
    int height;             // Image base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Image;

// Texture, tex data stored in GPU memory (VRAM)
typedef struct Texture {
    SDL_Texture* tex;
    unsigned int id;        // OpenGL texture id
    int width;              // Texture base width
    int height;             // Texture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Texture;

// Texture2D, same as Texture
typedef Texture Texture2D;

// TextureCubemap, same as Texture
typedef Texture TextureCubemap;

// RenderTexture, fbo for texture rendering
typedef struct RenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    Texture texture;        // Color buffer attachment texture
    Texture depth;          // Depth buffer attachment texture
} RenderTexture;

// RenderTexture2D, same as RenderTexture
typedef RenderTexture RenderTexture2D;

// NPatchInfo, n-patch layout info
typedef struct NPatchInfo {
    Rectangle source;       // Texture source rectangle
    int left;               // Left border offset
    int top;                // Top border offset
    int right;              // Right border offset
    int bottom;             // Bottom border offset
    int layout;             // Layout of the n-patch: 3x3, 1x3 or 3x1
} NPatchInfo;

// GlyphInfo, font characters glyphs info
typedef struct GlyphInfo {
    int value;              // Character value (Unicode)
    int offsetX;            // Character offset X when drawing
    int offsetY;            // Character offset Y when drawing
    int advanceX;           // Character advance position X
    Image image;            // Character image data
} GlyphInfo;

// Font, font texture and GlyphInfo array data
typedef struct Font {
    int baseSize;           // Base size (default chars height)
    int glyphCount;         // Number of glyph characters
    int glyphPadding;       // Padding around the glyph characters
    Texture2D texture;      // Texture atlas containing the glyphs
    Rectangle *recs;        // Rectangles in texture for the glyphs
    GlyphInfo *glyphs;      // Glyphs info data
} Font;

// Camera, defines position/orientation in 3d space
typedef struct Camera3D {
    Vector3 position;       // Camera position
    Vector3 target;         // Camera target it looks-at
    Vector3 up;             // Camera up vector (rotation over its axis)
    float fovy;             // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
    int projection;         // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
} Camera3D;

typedef Camera3D Camera;    // Camera type fallback, defaults to Camera3D

// Camera2D, defines position/orientation in 2d space
typedef struct Camera2D {
    Vector2 offset;         // Camera offset (displacement from target)
    Vector2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    float zoom;             // Camera zoom (scaling), should be 1.0f by default
} Camera2D;

// Mesh, vertex data and vao/vbo
typedef struct Mesh {
    int vertexCount;        // Number of vertices stored in arrays
    int triangleCount;      // Number of triangles stored (indexed or not)

    // Vertex attributes data
    float *vertices;        // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;       // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    float *texcoords2;      // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
    float *normals;         // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    float *tangents;        // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
    unsigned char *colors;      // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned short *indices;    // Vertex indices (in case vertex data comes indexed)

    // Animation vertex data
    float *animVertices;    // Animated vertex positions (after bones transformations)
    float *animNormals;     // Animated normals (after bones transformations)
    unsigned char *boneIds; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning)
    float *boneWeights;     // Vertex bone weight, up to 4 bones influence by vertex (skinning)

    // OpenGL identifiers
    unsigned int vaoId;     // OpenGL Vertex Array Object id
    unsigned int *vboId;    // OpenGL Vertex Buffer Objects id (default vertex data)
} Mesh;

// Shader
typedef struct Shader {
    unsigned int id;        // Shader program id
    int *locs;              // Shader locations array (RL_MAX_SHADER_LOCATIONS)
} Shader;

// MaterialMap
typedef struct MaterialMap {
    Texture2D texture;      // Material map texture
    Color color;            // Material map color
    float value;            // Material map value
} MaterialMap;

// Material, includes shader and maps
typedef struct Material {
    Shader shader;          // Material shader
    MaterialMap *maps;      // Material maps array (MAX_MATERIAL_MAPS)
    float params[4];        // Material generic parameters (if required)
} Material;

// Transform, vertex transformation data
typedef struct Transform {
    Vector3 translation;    // Translation
    Quaternion rotation;    // Rotation
    Vector3 scale;          // Scale
} Transform;

// Bone, skeletal animation bone
typedef struct BoneInfo {
    char name[32];          // Bone name
    int parent;             // Bone parent
} BoneInfo;

// Model, meshes, materials and animation data
typedef struct Model {
    Matrix transform;       // Local transform matrix

    int meshCount;          // Number of meshes
    int materialCount;      // Number of materials
    Mesh *meshes;           // Meshes array
    Material *materials;    // Materials array
    int *meshMaterial;      // Mesh material number

    // Animation data
    int boneCount;          // Number of bones
    BoneInfo *bones;        // Bones information (skeleton)
    Transform *bindPose;    // Bones base transformation (pose)
} Model;

// ModelAnimation
typedef struct ModelAnimation {
    int boneCount;          // Number of bones
    int frameCount;         // Number of animation frames
    BoneInfo *bones;        // Bones information (skeleton)
    Transform **framePoses; // Poses array by frame
} ModelAnimation;

// Ray, ray for raycasting
typedef struct Ray {
    Vector3 position;       // Ray position (origin)
    Vector3 direction;      // Ray direction
} Ray;

// RayCollision, ray hit information
typedef struct RayCollision {
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to the nearest hit
    Vector3 point;          // Point of the nearest hit
    Vector3 normal;         // Surface normal of hit
} RayCollision;

// BoundingBox
typedef struct BoundingBox {
    Vector3 min;            // Minimum vertex box-corner
    Vector3 max;            // Maximum vertex box-corner
} BoundingBox;

// Wave, audio wave data
typedef struct Wave {
    unsigned int frameCount;    // Total number of frames (considering channels)
    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
    void *data;                 // Buffer data pointer
} Wave;

#ifdef MIX_IMPL
// Opaque structs declaration
// NOTE: Actual structs are defined internally in raudio module
typedef struct rAudioBuffer rAudioBuffer;
typedef struct rAudioProcessor rAudioProcessor;

// AudioStream, custom audio stream
typedef struct AudioStream {
    rAudioBuffer *buffer;       // Pointer to internal data used by the audio system
    rAudioProcessor *processor; // Pointer to internal data processor, useful for audio effects

    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
} AudioStream;

// Sound
typedef struct Sound {
    AudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
} Sound;

// Music, audio stream, anything longer than ~10 seconds should be streamed
typedef struct Music {
#ifdef MIX_SUPPORT
    Mix_Music* mus;
#endif
    AudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
    bool looping;               // Music looping enable

    int ctxType;                // Type of music context (audio filetype)
    void *ctxData;              // Audio context data, depends on type
} Music;
#endif

// VrDeviceInfo, Head-Mounted-Display device parameters
typedef struct VrDeviceInfo {
    int hResolution;                // Horizontal resolution in pixels
    int vResolution;                // Vertical resolution in pixels
    float hScreenSize;              // Horizontal size in meters
    float vScreenSize;              // Vertical size in meters
    float vScreenCenter;            // Screen center in meters
    float eyeToScreenDistance;      // Distance between eye and display in meters
    float lensSeparationDistance;   // Lens separation distance in meters
    float interpupillaryDistance;   // IPD (distance between pupils) in meters
    float lensDistortionValues[4];  // Lens distortion constant parameters
    float chromaAbCorrection[4];    // Chromatic aberration correction parameters
} VrDeviceInfo;

// VrStereoConfig, VR stereo rendering configuration for simulator
typedef struct VrStereoConfig {
    Matrix projection[2];           // VR projection matrices (per eye)
    Matrix viewOffset[2];           // VR view offset matrices (per eye)
    float leftLensCenter[2];        // VR left lens center
    float rightLensCenter[2];       // VR right lens center
    float leftScreenCenter[2];      // VR left screen center
    float rightScreenCenter[2];     // VR right screen center
    float scale[2];                 // VR distortion scale
    float scaleIn[2];               // VR distortion scale in
} VrStereoConfig;

// File path list
typedef struct FilePathList {
    unsigned int capacity;          // Filepaths max entries
    unsigned int count;             // Filepaths entries count
    char **paths;                   // Filepaths entries
} FilePathList;

//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// System/Window config flags
// NOTE: Every bit registers one state (use it with bit masks)
// By default all flags are set to 0
typedef enum {
    FLAG_VSYNC_HINT         = 0x00000040,   // Set to try enabling V-Sync on GPU
    FLAG_FULLSCREEN_MODE    = 0x00000002,   // Set to run program in fullscreen
    FLAG_WINDOW_RESIZABLE   = 0x00000004,   // Set to allow resizable window
    FLAG_WINDOW_UNDECORATED = 0x00000008,   // Set to disable window decoration (frame and buttons)
    FLAG_WINDOW_HIDDEN      = 0x00000080,   // Set to hide window
    FLAG_WINDOW_MINIMIZED   = 0x00000200,   // Set to minimize window (iconify)
    FLAG_WINDOW_MAXIMIZED   = 0x00000400,   // Set to maximize window (expanded to monitor)
    FLAG_WINDOW_UNFOCUSED   = 0x00000800,   // Set to window non focused
    FLAG_WINDOW_TOPMOST     = 0x00001000,   // Set to window always on top
    FLAG_WINDOW_ALWAYS_RUN  = 0x00000100,   // Set to allow windows running while minimized
    FLAG_WINDOW_TRANSPARENT = 0x00000010,   // Set to allow transparent framebuffer
    FLAG_WINDOW_HIGHDPI     = 0x00002000,   // Set to support HighDPI
    FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000, // Set to support mouse passthrough, only supported when FLAG_WINDOW_UNDECORATED
    FLAG_MSAA_4X_HINT       = 0x00000020,   // Set to try enabling MSAA 4X
    FLAG_INTERLACED_HINT    = 0x00010000    // Set to try enabling interlaced video format (for V3D)
} ConfigFlags;

// Trace log level
// NOTE: Organized by priority level
typedef enum {
    LOG_ALL = 0,        // Display all logs
    LOG_TRACE,          // Trace logging, intended for internal use only
    LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
    LOG_INFO,           // Info logging, used for program execution info
    LOG_WARNING,        // Warning logging, used on recoverable failures
    LOG_ERROR,          // Error logging, used on unrecoverable failures
    LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    LOG_NONE            // Disable logging
} TraceLogLevel;

// Add backwards compatibility support for deprecated names
#define MOUSE_LEFT_BUTTON   MOUSE_BUTTON_LEFT
#define MOUSE_RIGHT_BUTTON  MOUSE_BUTTON_RIGHT
#define MOUSE_MIDDLE_BUTTON MOUSE_BUTTON_MIDDLE

// Mouse buttons
typedef enum {
    MOUSE_BUTTON_LEFT    = SDL_BUTTON_LEFT,       // Mouse button left
    MOUSE_BUTTON_RIGHT   = SDL_BUTTON_RIGHT,       // Mouse button right
    MOUSE_BUTTON_MIDDLE  = SDL_BUTTON_MIDDLE,       // Mouse button middle (pressed wheel)
    MOUSE_BUTTON_SIDE    = SDL_BUTTON_X1,       // Mouse button side (advanced mouse device)
    MOUSE_BUTTON_EXTRA   = SDL_BUTTON_X2,       // Mouse button extra (advanced mouse device)
    MOUSE_BUTTON_FORWARD = SDL_BUTTON_X1,       // Mouse button forward (advanced mouse device)
    MOUSE_BUTTON_BACK    = SDL_BUTTON_X2,       // Mouse button back (advanced mouse device)
} MouseButton;

// Mouse cursor
typedef enum {
    MOUSE_CURSOR_DEFAULT       = -1,     // Default pointer shape
    MOUSE_CURSOR_ARROW         = SDL_SYSTEM_CURSOR_ARROW,     // Arrow shape
    MOUSE_CURSOR_IBEAM         = SDL_SYSTEM_CURSOR_IBEAM,     // Text writing cursor shape
    MOUSE_CURSOR_CROSSHAIR     = SDL_SYSTEM_CURSOR_CROSSHAIR,     // Cross shape
    MOUSE_CURSOR_POINTING_HAND = SDL_SYSTEM_CURSOR_HAND,     // Pointing hand cursor
    MOUSE_CURSOR_RESIZE_EW     = SDL_SYSTEM_CURSOR_SIZEWE,     // Horizontal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NS     = SDL_SYSTEM_CURSOR_SIZENS,     // Vertical resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NWSE   = SDL_SYSTEM_CURSOR_SIZENWSE,     // Top-left to bottom-right diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NESW   = SDL_SYSTEM_CURSOR_SIZENESW,     // The top-right to bottom-left diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_ALL    = SDL_SYSTEM_CURSOR_SIZEALL,     // The omnidirectional resize/move cursor shape
    MOUSE_CURSOR_NOT_ALLOWED   = SDL_SYSTEM_CURSOR_NO     // The operation-not-allowed shape
} MouseCursor;

// Gamepad buttons
typedef enum {
    GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
    GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
    GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
    GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
    GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
    GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Square, Xbox: X)
    GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Circle, Xbox: B)
    GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
    GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (one), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
    GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
    GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
    GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
    GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
} GamepadButton;

// Gamepad axis
typedef enum {
    GAMEPAD_AXIS_LEFT_X        = 0,     // Gamepad left stick X axis
    GAMEPAD_AXIS_LEFT_Y        = 1,     // Gamepad left stick Y axis
    GAMEPAD_AXIS_RIGHT_X       = 2,     // Gamepad right stick X axis
    GAMEPAD_AXIS_RIGHT_Y       = 3,     // Gamepad right stick Y axis
    GAMEPAD_AXIS_LEFT_TRIGGER  = 4,     // Gamepad back trigger left, pressure level: [1..-1]
    GAMEPAD_AXIS_RIGHT_TRIGGER = 5      // Gamepad back trigger right, pressure level: [1..-1]
} GamepadAxis;

// Material map index
typedef enum {
    MATERIAL_MAP_ALBEDO = 0,        // Albedo material (same as: MATERIAL_MAP_DIFFUSE)
    MATERIAL_MAP_METALNESS,         // Metalness material (same as: MATERIAL_MAP_SPECULAR)
    MATERIAL_MAP_NORMAL,            // Normal material
    MATERIAL_MAP_ROUGHNESS,         // Roughness material
    MATERIAL_MAP_OCCLUSION,         // Ambient occlusion material
    MATERIAL_MAP_EMISSION,          // Emission material
    MATERIAL_MAP_HEIGHT,            // Heightmap material
    MATERIAL_MAP_CUBEMAP,           // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_IRRADIANCE,        // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_PREFILTER,         // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_BRDF               // Brdf material
} MaterialMapIndex;

#define MATERIAL_MAP_DIFFUSE      MATERIAL_MAP_ALBEDO
#define MATERIAL_MAP_SPECULAR     MATERIAL_MAP_METALNESS

// Shader location index
typedef enum {
    SHADER_LOC_VERTEX_POSITION = 0, // Shader location: vertex attribute: position
    SHADER_LOC_VERTEX_TEXCOORD01,   // Shader location: vertex attribute: texcoord01
    SHADER_LOC_VERTEX_TEXCOORD02,   // Shader location: vertex attribute: texcoord02
    SHADER_LOC_VERTEX_NORMAL,       // Shader location: vertex attribute: normal
    SHADER_LOC_VERTEX_TANGENT,      // Shader location: vertex attribute: tangent
    SHADER_LOC_VERTEX_COLOR,        // Shader location: vertex attribute: color
    SHADER_LOC_MATRIX_MVP,          // Shader location: matrix uniform: model-view-projection
    SHADER_LOC_MATRIX_VIEW,         // Shader location: matrix uniform: view (camera transform)
    SHADER_LOC_MATRIX_PROJECTION,   // Shader location: matrix uniform: projection
    SHADER_LOC_MATRIX_MODEL,        // Shader location: matrix uniform: model (transform)
    SHADER_LOC_MATRIX_NORMAL,       // Shader location: matrix uniform: normal
    SHADER_LOC_VECTOR_VIEW,         // Shader location: vector uniform: view
    SHADER_LOC_COLOR_DIFFUSE,       // Shader location: vector uniform: diffuse color
    SHADER_LOC_COLOR_SPECULAR,      // Shader location: vector uniform: specular color
    SHADER_LOC_COLOR_AMBIENT,       // Shader location: vector uniform: ambient color
    SHADER_LOC_MAP_ALBEDO,          // Shader location: sampler2d texture: albedo (same as: SHADER_LOC_MAP_DIFFUSE)
    SHADER_LOC_MAP_METALNESS,       // Shader location: sampler2d texture: metalness (same as: SHADER_LOC_MAP_SPECULAR)
    SHADER_LOC_MAP_NORMAL,          // Shader location: sampler2d texture: normal
    SHADER_LOC_MAP_ROUGHNESS,       // Shader location: sampler2d texture: roughness
    SHADER_LOC_MAP_OCCLUSION,       // Shader location: sampler2d texture: occlusion
    SHADER_LOC_MAP_EMISSION,        // Shader location: sampler2d texture: emission
    SHADER_LOC_MAP_HEIGHT,          // Shader location: sampler2d texture: height
    SHADER_LOC_MAP_CUBEMAP,         // Shader location: samplerCube texture: cubemap
    SHADER_LOC_MAP_IRRADIANCE,      // Shader location: samplerCube texture: irradiance
    SHADER_LOC_MAP_PREFILTER,       // Shader location: samplerCube texture: prefilter
    SHADER_LOC_MAP_BRDF             // Shader location: sampler2d texture: brdf
} ShaderLocationIndex;

#define SHADER_LOC_MAP_DIFFUSE      SHADER_LOC_MAP_ALBEDO
#define SHADER_LOC_MAP_SPECULAR     SHADER_LOC_MAP_METALNESS

// Shader uniform data type
typedef enum {
    SHADER_UNIFORM_FLOAT = 0,       // Shader uniform type: float
    SHADER_UNIFORM_VEC2,            // Shader uniform type: vec2 (2 float)
    SHADER_UNIFORM_VEC3,            // Shader uniform type: vec3 (3 float)
    SHADER_UNIFORM_VEC4,            // Shader uniform type: vec4 (4 float)
    SHADER_UNIFORM_INT,             // Shader uniform type: int
    SHADER_UNIFORM_IVEC2,           // Shader uniform type: ivec2 (2 int)
    SHADER_UNIFORM_IVEC3,           // Shader uniform type: ivec3 (3 int)
    SHADER_UNIFORM_IVEC4,           // Shader uniform type: ivec4 (4 int)
    SHADER_UNIFORM_SAMPLER2D        // Shader uniform type: sampler2d
} ShaderUniformDataType;

// Shader attribute data types
typedef enum {
    SHADER_ATTRIB_FLOAT = 0,        // Shader attribute type: float
    SHADER_ATTRIB_VEC2,             // Shader attribute type: vec2 (2 float)
    SHADER_ATTRIB_VEC3,             // Shader attribute type: vec3 (3 float)
    SHADER_ATTRIB_VEC4              // Shader attribute type: vec4 (4 float)
} ShaderAttributeDataType;

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
typedef enum {
    PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = SDL_PIXELFORMAT_INDEX8,
    PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA = SDL_PIXELFORMAT_INDEX8,
    PIXELFORMAT_UNCOMPRESSED_R5G6B5 = SDL_PIXELFORMAT_RGB555,
    PIXELFORMAT_UNCOMPRESSED_R8G8B8 = SDL_PIXELFORMAT_RGB888,
    PIXELFORMAT_UNCOMPRESSED_R5G5B5A1 = SDL_PIXELFORMAT_RGBA5551,
    PIXELFORMAT_UNCOMPRESSED_R4G4B4A4 = SDL_PIXELFORMAT_RGBA4444,
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 = SDL_PIXELFORMAT_RGBA8888,
    PIXELFORMAT_UNCOMPRESSED_R32 = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_UNCOMPRESSED_R32G32B32 = SDL_PIXELFORMAT_RGB332,
    PIXELFORMAT_UNCOMPRESSED_R32G32B32A32 = SDL_PIXELFORMAT_RGBA32,
    PIXELFORMAT_COMPRESSED_DXT1_RGB = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_DXT1_RGBA = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_DXT3_RGBA = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_DXT5_RGBA = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_ETC1_RGB = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_ETC2_RGB = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_PVRT_RGB = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_PVRT_RGBA = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA = SDL_PIXELFORMAT_UNKNOWN,
    PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA = SDL_PIXELFORMAT_UNKNOWN
} PixelFormat;

// Texture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
typedef enum {
    TEXTURE_FILTER_POINT = SDL_ScaleModeNearest,               // No filter, just pixel approximation
    TEXTURE_FILTER_BILINEAR = SDL_ScaleModeLinear,                // Linear filtering
    TEXTURE_FILTER_TRILINEAR = SDL_ScaleModeLinear,               // Trilinear filtering (linear with mipmaps)
    TEXTURE_FILTER_ANISOTROPIC_4X = SDL_ScaleModeLinear,          // Anisotropic filtering 4x
    TEXTURE_FILTER_ANISOTROPIC_8X = SDL_ScaleModeLinear,          // Anisotropic filtering 8x
    TEXTURE_FILTER_ANISOTROPIC_16X = SDL_ScaleModeLinear         // Anisotropic filtering 16x
} TextureFilter;

// Texture parameters: wrap mode
typedef enum {
    TEXTURE_WRAP_REPEAT = 0,                // Repeats texture in tiled mode
    TEXTURE_WRAP_CLAMP,                     // Clamps texture to edge pixel in tiled mode
    TEXTURE_WRAP_MIRROR_REPEAT,             // Mirrors and repeats the texture in tiled mode
    TEXTURE_WRAP_MIRROR_CLAMP               // Mirrors and clamps to border the texture in tiled mode
} TextureWrap;

// Cubemap layouts
typedef enum {
    CUBEMAP_LAYOUT_AUTO_DETECT = 0,         // Automatically detect layout type
    CUBEMAP_LAYOUT_LINE_VERTICAL,           // Layout is defined by a vertical line with faces
    CUBEMAP_LAYOUT_LINE_HORIZONTAL,         // Layout is defined by a horizontal line with faces
    CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,     // Layout is defined by a 3x4 cross with cubemap faces
    CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE,     // Layout is defined by a 4x3 cross with cubemap faces
    CUBEMAP_LAYOUT_PANORAMA                 // Layout is defined by a panorama image (equirrectangular map)
} CubemapLayout;

// Font type, defines generation method
typedef enum {
    FONT_DEFAULT = 0,               // Default font generation, anti-aliased
    FONT_BITMAP,                    // Bitmap font generation, no anti-aliasing
    FONT_SDF                        // SDF font generation, requires external shader
} FontType;

// Color blending modes (pre-defined)
typedef enum {
    BLEND_ALPHA = SDL_BLENDMODE_BLEND,
    BLEND_ADDITIVE = SDL_BLENDMODE_ADD,
    BLEND_MULTIPLIED = SDL_BLENDMODE_MUL,
    BLEND_ADD_COLORS = SDL_BLENDMODE_BLEND,
    BLEND_SUBTRACT_COLORS = SDL_BLENDMODE_MOD,
    BLEND_ALPHA_PREMULTIPLY = SDL_BLENDMODE_MUL,
    BLEND_CUSTOM,
    BLEND_CUSTOM_SEPARATE
} BlendMode;

// Gesture
// NOTE: Provided as bit-wise flags to enable only desired gestures
typedef enum {
    GESTURE_NONE        = 0,        // No gesture
    GESTURE_TAP         = 1,        // Tap gesture
    GESTURE_DOUBLETAP   = 2,        // Double tap gesture
    GESTURE_HOLD        = 4,        // Hold gesture
    GESTURE_DRAG        = 8,        // Drag gesture
    GESTURE_SWIPE_RIGHT = 16,       // Swipe right gesture
    GESTURE_SWIPE_LEFT  = 32,       // Swipe left gesture
    GESTURE_SWIPE_UP    = 64,       // Swipe up gesture
    GESTURE_SWIPE_DOWN  = 128,      // Swipe down gesture
    GESTURE_PINCH_IN    = 256,      // Pinch in gesture
    GESTURE_PINCH_OUT   = 512       // Pinch out gesture
} Gesture;

// Camera system modes
typedef enum {
    CAMERA_CUSTOM = 0,              // Custom camera
    CAMERA_FREE,                    // Free camera
    CAMERA_ORBITAL,                 // Orbital camera
    CAMERA_FIRST_PERSON,            // First person camera
    CAMERA_THIRD_PERSON             // Third person camera
} CameraMode;

// Camera projection
typedef enum {
    CAMERA_PERSPECTIVE = 0,         // Perspective projection
    CAMERA_ORTHOGRAPHIC             // Orthographic projection
} CameraProjection;

// N-patch layout
typedef enum {
    NPATCH_NINE_PATCH = 0,          // Npatch layout: 3x3 tiles
    NPATCH_THREE_PATCH_VERTICAL,    // Npatch layout: 1x3 tiles
    NPATCH_THREE_PATCH_HORIZONTAL   // Npatch layout: 3x1 tiles
} NPatchLayout;

typedef enum {
    KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
#ifdef USE_SCANCODES
    KEY_APOSTROPHE      = SDL_SCANCODE_APOSTROPHE,       // Key: '
    KEY_COMMA           = SDL_SCANCODE_COMMA,       // Key: ,
    KEY_MINUS           = SDL_SCANCODE_MINUS,       // Key: -
    KEY_PERIOD          = SDL_SCANCODE_PERIOD,       // Key: .
    KEY_SLASH           = SDL_SCANCODE_SLASH,       // Key: /
    KEY_ZERO            = SDL_SCANCODE_0,       // Key: 0
    KEY_ONE             = SDL_SCANCODE_1,       // Key: 1
    KEY_TWO             = SDL_SCANCODE_2,       // Key: 2
    KEY_THREE           = SDL_SCANCODE_3,       // Key: 3
    KEY_FOUR            = SDL_SCANCODE_4,       // Key: 4
    KEY_FIVE            = SDL_SCANCODE_5,       // Key: 5
    KEY_SIX             = SDL_SCANCODE_6,       // Key: 6
    KEY_SEVEN           = SDL_SCANCODE_7,       // Key: 7
    KEY_EIGHT           = SDL_SCANCODE_8,       // Key: 8
    KEY_NINE            = SDL_SCANCODE_9,       // Key: 9
    KEY_SEMICOLON       = SDL_SCANCODE_SEMICOLON,       // Key: ;
    KEY_EQUAL           = SDL_SCANCODE_EQUALS,       // Key: =
    KEY_A               = SDL_SCANCODE_A,       // Key: A | a
    KEY_B               = SDL_SCANCODE_B,       // Key: B | b
    KEY_C               = SDL_SCANCODE_C,       // Key: C | c
    KEY_D               = SDL_SCANCODE_D,       // Key: D | d
    KEY_E               = SDL_SCANCODE_E,       // Key: E | e
    KEY_F               = SDL_SCANCODE_F,       // Key: F | f
    KEY_G               = SDL_SCANCODE_G,       // Key: G | g
    KEY_H               = SDL_SCANCODE_H,       // Key: H | h
    KEY_I               = SDL_SCANCODE_I,       // Key: I | i
    KEY_J               = SDL_SCANCODE_J,       // Key: J | j
    KEY_K               = SDL_SCANCODE_K,       // Key: K | k
    KEY_L               = SDL_SCANCODE_L,       // Key: L | l
    KEY_M               = SDL_SCANCODE_M,       // Key: M | m
    KEY_N               = SDL_SCANCODE_N,       // Key: N | n
    KEY_O               = SDL_SCANCODE_O,       // Key: O | o
    KEY_P               = SDL_SCANCODE_P,       // Key: P | p
    KEY_Q               = SDL_SCANCODE_Q,       // Key: Q | q
    KEY_R               = SDL_SCANCODE_R,       // Key: R | r
    KEY_S               = SDL_SCANCODE_S,       // Key: S | s
    KEY_T               = SDL_SCANCODE_T,       // Key: T | t
    KEY_U               = SDL_SCANCODE_U,       // Key: U | u
    KEY_V               = SDL_SCANCODE_V,       // Key: V | v
    KEY_W               = SDL_SCANCODE_W,       // Key: W | w
    KEY_X               = SDL_SCANCODE_X,       // Key: X | x
    KEY_Y               = SDL_SCANCODE_Y,       // Key: Y | y
    KEY_Z               = SDL_SCANCODE_Z,       // Key: Z | z
    KEY_LEFT_BRACKET    = SDL_SCANCODE_LEFTBRACKET,       // Key: [
    KEY_BACKSLASH       = SDL_SCANCODE_BACKSLASH,       // Key: '\'
    KEY_RIGHT_BRACKET   = SDL_SCANCODE_RIGHTBRACKET,       // Key: ]
    KEY_GRAVE           = SDL_SCANCODE_GRAVE,       // Key: `
    // Function keys
    KEY_SPACE           = SDL_SCANCODE_SPACE,       // Key: Space
    KEY_ESCAPE          = SDL_SCANCODE_ESCAPE,      // Key: Esc
    KEY_ENTER           = SDL_SCANCODE_RETURN,      // Key: Enter
    KEY_TAB             = SDL_SCANCODE_TAB,      // Key: Tab
    KEY_BACKSPACE       = SDL_SCANCODE_BACKSPACE,      // Key: Backspace
    KEY_INSERT          = SDL_SCANCODE_INSERT,      // Key: Ins
    KEY_DELETE          = SDL_SCANCODE_DELETE,      // Key: Del
    KEY_RIGHT           = SDL_SCANCODE_RIGHT,      // Key: Cursor right
    KEY_LEFT            = SDL_SCANCODE_LEFT,      // Key: Cursor left
    KEY_DOWN            = SDL_SCANCODE_DOWN,      // Key: Cursor down
    KEY_UP              = SDL_SCANCODE_UP,      // Key: Cursor up
    KEY_PAGE_UP         = SDL_SCANCODE_PAGEUP,      // Key: Page up
    KEY_PAGE_DOWN       = SDL_SCANCODE_PAGEDOWN,      // Key: Page down
    KEY_HOME            = SDL_SCANCODE_HOME,      // Key: Home
    KEY_END             = SDL_SCANCODE_END,      // Key: End
    KEY_CAPS_LOCK       = SDL_SCANCODE_CAPSLOCK,      // Key: Caps lock
    KEY_SCROLL_LOCK     = SDL_SCANCODE_SCROLLLOCK,      // Key: Scroll down
    KEY_NUM_LOCK        = SDL_SCANCODE_NUMLOCKCLEAR,      // Key: Num lock
    KEY_PRINT_SCREEN    = SDL_SCANCODE_PRINTSCREEN,      // Key: Print screen
    KEY_PAUSE           = SDL_SCANCODE_PAUSE,      // Key: Pause
    KEY_F1              = SDL_SCANCODE_F1,      // Key: F1
    KEY_F2              = SDL_SCANCODE_F2,      // Key: F2
    KEY_F3              = SDL_SCANCODE_F3,      // Key: F3
    KEY_F4              = SDL_SCANCODE_F4,      // Key: F4
    KEY_F5              = SDL_SCANCODE_F5,      // Key: F5
    KEY_F6              = SDL_SCANCODE_F6,      // Key: F6
    KEY_F7              = SDL_SCANCODE_F7,      // Key: F7
    KEY_F8              = SDL_SCANCODE_F8,      // Key: F8
    KEY_F9              = SDL_SCANCODE_F9,      // Key: F9
    KEY_F10             = SDL_SCANCODE_F10,      // Key: F10
    KEY_F11             = SDL_SCANCODE_F11,      // Key: F11
    KEY_F12             = SDL_SCANCODE_F12,      // Key: F12
    KEY_LEFT_SHIFT      = SDL_SCANCODE_LSHIFT,      // Key: Shift left
    KEY_LEFT_CONTROL    = SDL_SCANCODE_LCTRL,      // Key: Control left
    KEY_LEFT_ALT        = SDL_SCANCODE_LALT,      // Key: Alt left
    KEY_LEFT_SUPER      = SDL_SCANCODE_LGUI,      // Key: Super left
    KEY_RIGHT_SHIFT     = SDL_SCANCODE_RSHIFT,      // Key: Shift right
    KEY_RIGHT_CONTROL   = SDL_SCANCODE_RCTRL,      // Key: Control right
    KEY_RIGHT_ALT       = SDL_SCANCODE_RALT,      // Key: Alt right
    KEY_RIGHT_SUPER     = SDL_SCANCODE_RGUI,      // Key: Super right
    KEY_KB_MENU         = SDL_SCANCODE_KBDILLUMTOGGLE,      // Key: KB menu
    // Keypad keys
    KEY_KP_0            = SDL_SCANCODE_KP_0,      // Key: Keypad 0
    KEY_KP_1            = SDL_SCANCODE_KP_1,      // Key: Keypad 1
    KEY_KP_2            = SDL_SCANCODE_KP_2,      // Key: Keypad 2
    KEY_KP_3            = SDL_SCANCODE_KP_3,      // Key: Keypad 3
    KEY_KP_4            = SDL_SCANCODE_KP_4,      // Key: Keypad 4
    KEY_KP_5            = SDL_SCANCODE_KP_5,      // Key: Keypad 5
    KEY_KP_6            = SDL_SCANCODE_KP_6,      // Key: Keypad 6
    KEY_KP_7            = SDL_SCANCODE_KP_7,      // Key: Keypad 7
    KEY_KP_8            = SDL_SCANCODE_KP_8,      // Key: Keypad 8
    KEY_KP_9            = SDL_SCANCODE_KP_9,      // Key: Keypad 9
    KEY_KP_DECIMAL      = SDL_SCANCODE_KP_DECIMAL,      // Key: Keypad .
    KEY_KP_DIVIDE       = SDL_SCANCODE_KP_DIVIDE,      // Key: Keypad /
    KEY_KP_MULTIPLY     = SDL_SCANCODE_KP_MULTIPLY,      // Key: Keypad *
    KEY_KP_SUBTRACT     = SDL_SCANCODE_KP_MINUS,      // Key: Keypad -
    KEY_KP_ADD          = SDL_SCANCODE_KP_PLUS,      // Key: Keypad +
    KEY_KP_ENTER        = SDL_SCANCODE_KP_ENTER,      // Key: Keypad Enter
    KEY_KP_EQUAL        = SDL_SCANCODE_KP_EQUALS,      // Key: Keypad =
    // Android key buttons
    KEY_BACK            = SDL_SCANCODE_AC_BACK,        // Key: Android back button
    KEY_MENU            = SDL_SCANCODE_APPLICATION,       // Key: Android menu button
    KEY_VOLUME_UP       = SDL_SCANCODE_VOLUMEUP,       // Key: Android volume up button
    KEY_VOLUME_DOWN     = SDL_SCANCODE_VOLUMEDOWN        // Key: Android volume down button
#else
    KEY_APOSTROPHE      = SDLK_QUOTE,       // Key: '
    KEY_COMMA           = SDLK_COMMA,       // Key: ,
    KEY_MINUS           = SDLK_MINUS,       // Key: -
    KEY_PERIOD          = SDLK_PERIOD,       // Key: .
    KEY_SLASH           = SDLK_SLASH,       // Key: /
    KEY_ZERO            = SDLK_0,       // Key: 0
    KEY_ONE             = SDLK_1,       // Key: 1
    KEY_TWO             = SDLK_2,       // Key: 2
    KEY_THREE           = SDLK_3,       // Key: 3
    KEY_FOUR            = SDLK_4,       // Key: 4
    KEY_FIVE            = SDLK_5,       // Key: 5
    KEY_SIX             = SDLK_6,       // Key: 6
    KEY_SEVEN           = SDLK_7,       // Key: 7
    KEY_EIGHT           = SDLK_8,       // Key: 8
    KEY_NINE            = SDLK_9,       // Key: 9
    KEY_SEMICOLON       = SDLK_SEMICOLON,       // Key: ;
    KEY_EQUAL           = SDLK_EQUALS,       // Key: =
    KEY_A               = SDLK_a,       // Key: A | a
    KEY_B               = SDLK_b,       // Key: B | b
    KEY_C               = SDLK_c,       // Key: C | c
    KEY_D               = SDLK_d,       // Key: D | d
    KEY_E               = SDLK_e,       // Key: E | e
    KEY_F               = SDLK_f,       // Key: F | f
    KEY_G               = SDLK_g,       // Key: G | g
    KEY_H               = SDLK_h,       // Key: H | h
    KEY_I               = SDLK_i,       // Key: I | i
    KEY_J               = SDLK_j,       // Key: J | j
    KEY_K               = SDLK_k,       // Key: K | k
    KEY_L               = SDLK_l,       // Key: L | l
    KEY_M               = SDLK_m,       // Key: M | m
    KEY_N               = SDLK_n,       // Key: N | n
    KEY_O               = SDLK_o,       // Key: O | o
    KEY_P               = SDLK_p,       // Key: P | p
    KEY_Q               = SDLK_q,       // Key: Q | q
    KEY_R               = SDLK_r,       // Key: R | r
    KEY_S               = SDLK_s,       // Key: S | s
    KEY_T               = SDLK_t,       // Key: T | t
    KEY_U               = SDLK_u,       // Key: U | u
    KEY_V               = SDLK_v,       // Key: V | v
    KEY_W               = SDLK_w,       // Key: W | w
    KEY_X               = SDLK_x,       // Key: X | x
    KEY_Y               = SDLK_y,       // Key: Y | y
    KEY_Z               = SDLK_z,       // Key: Z | z
    KEY_LEFT_BRACKET    = SDLK_LEFTBRACKET,       // Key: [
    KEY_BACKSLASH       = SDLK_BACKSLASH,       // Key: '\'
    KEY_RIGHT_BRACKET   = SDLK_RIGHTBRACKET,       // Key: ]
    KEY_GRAVE           = SDLK_BACKQUOTE,       // Key: `
    // Function keys
    KEY_SPACE           = SDLK_SPACE,       // Key: Space
    KEY_ESCAPE          = SDLK_ESCAPE,      // Key: Esc
    KEY_ENTER           = SDLK_RETURN,      // Key: Enter
    KEY_TAB             = SDLK_TAB,      // Key: Tab
    KEY_BACKSPACE       = SDLK_BACKSPACE,      // Key: Backspace
    KEY_INSERT          = SDLK_INSERT,      // Key: Ins
    KEY_DELETE          = SDLK_DELETE,      // Key: Del
    KEY_RIGHT           = SDLK_RIGHT,      // Key: Cursor right
    KEY_LEFT            = SDLK_LEFT,      // Key: Cursor left
    KEY_DOWN            = SDLK_DOWN,      // Key: Cursor down
    KEY_UP              = SDLK_UP,      // Key: Cursor up
    KEY_PAGE_UP         = SDLK_PAGEUP,      // Key: Page up
    KEY_PAGE_DOWN       = SDLK_PAGEDOWN,      // Key: Page down
    KEY_HOME            = SDLK_HOME,      // Key: Home
    KEY_END             = SDLK_END,      // Key: End
    KEY_CAPS_LOCK       = SDLK_CAPSLOCK,      // Key: Caps lock
    KEY_SCROLL_LOCK     = SDLK_SCROLLLOCK,      // Key: Scroll down
    KEY_NUM_LOCK        = SDLK_NUMLOCKCLEAR,      // Key: Num lock
    KEY_PRINT_SCREEN    = SDLK_PRINTSCREEN,      // Key: Print screen
    KEY_PAUSE           = SDLK_PAUSE,      // Key: Pause
    KEY_F1              = SDLK_F1,      // Key: F1
    KEY_F2              = SDLK_F2,      // Key: F2
    KEY_F3              = SDLK_F3,      // Key: F3
    KEY_F4              = SDLK_F4,      // Key: F4
    KEY_F5              = SDLK_F5,      // Key: F5
    KEY_F6              = SDLK_F6,      // Key: F6
    KEY_F7              = SDLK_F7,      // Key: F7
    KEY_F8              = SDLK_F8,      // Key: F8
    KEY_F9              = SDLK_F9,      // Key: F9
    KEY_F10             = SDLK_F10,      // Key: F10
    KEY_F11             = SDLK_F11,      // Key: F11
    KEY_F12             = SDLK_F12,      // Key: F12
    KEY_LEFT_SHIFT      = SDLK_LSHIFT,      // Key: Shift left
    KEY_LEFT_CONTROL    = SDLK_LCTRL,      // Key: Control left
    KEY_LEFT_ALT        = SDLK_LALT,      // Key: Alt left
    KEY_LEFT_SUPER      = SDLK_LGUI,      // Key: Super left
    KEY_RIGHT_SHIFT     = SDLK_RSHIFT,      // Key: Shift right
    KEY_RIGHT_CONTROL   = SDLK_RCTRL,      // Key: Control right
    KEY_RIGHT_ALT       = SDLK_RALT,      // Key: Alt right
    KEY_RIGHT_SUPER     = SDLK_RGUI,      // Key: Super right
    KEY_KB_MENU         = SDLK_KBDILLUMTOGGLE,      // Key: KB menu
    // Keypad keys
    KEY_KP_0            = SDLK_KP_0,      // Key: Keypad 0
    KEY_KP_1            = SDLK_KP_1,      // Key: Keypad 1
    KEY_KP_2            = SDLK_KP_2,      // Key: Keypad 2
    KEY_KP_3            = SDLK_KP_3,      // Key: Keypad 3
    KEY_KP_4            = SDLK_KP_4,      // Key: Keypad 4
    KEY_KP_5            = SDLK_KP_5,      // Key: Keypad 5
    KEY_KP_6            = SDLK_KP_6,      // Key: Keypad 6
    KEY_KP_7            = SDLK_KP_7,      // Key: Keypad 7
    KEY_KP_8            = SDLK_KP_8,      // Key: Keypad 8
    KEY_KP_9            = SDLK_KP_9,      // Key: Keypad 9
    KEY_KP_DECIMAL      = SDLK_KP_DECIMAL,      // Key: Keypad .
    KEY_KP_DIVIDE       = SDLK_KP_DIVIDE,      // Key: Keypad /
    KEY_KP_MULTIPLY     = SDLK_KP_MULTIPLY,      // Key: Keypad *
    KEY_KP_SUBTRACT     = SDLK_KP_MINUS,      // Key: Keypad -
    KEY_KP_ADD          = SDLK_KP_PLUS,      // Key: Keypad +
    KEY_KP_ENTER        = SDLK_KP_ENTER,      // Key: Keypad Enter
    KEY_KP_EQUAL        = SDLK_KP_EQUALS,      // Key: Keypad =
    // Android key buttons
    KEY_BACK            = SDLK_AC_BACK,        // Key: Android back button
    KEY_MENU            = SDLK_APPLICATION,       // Key: Android menu button
    KEY_VOLUME_UP       = SDLK_VOLUMEUP,       // Key: Android volume up button
    KEY_VOLUME_DOWN     = SDLK_VOLUMEDOWN        // Key: Android volume down button
#endif
} KeyboardKey;

#ifndef GET_KEY_INT
#ifdef USE_SCANCODES
#define GET_KEY_INT(keysym) (keysym.scancode)
#else
#define GET_KEY_INT(keysym) (keysym.sym)
#endif
#endif
#ifndef CONVERT_KEY_CODE
#ifdef USE_SCANCODES
#define CONVERT_KEY_CODE(scancode) (scancode)
#else
#define CONVERT_KEY_CODE(scancode) (SDL_GetScancodeFromKey(scancode))
#endif
#endif

typedef void (*TraceLogCallback)(int logLevel, const char *text, va_list args);
typedef unsigned char *(*LoadFileDataCallback)(const char *fileName, unsigned int *bytesRead);
typedef bool (*SaveFileDataCallback)(const char *fileName, void *data, unsigned int bytesToWrite);
typedef char *(*LoadFileTextCallback)(const char *fileName);
typedef bool (*SaveFileTextCallback)(const char *fileName, char *text);

struct rl_type {
#ifdef SUPPORT_FILES_DROPPING
    FilePathList drops;
#endif
    SDL_Event event;
    TraceLogCallback traceLog;
    LoadFileDataCallback loadFileData;
    SaveFileDataCallback saveFileData;
    LoadFileTextCallback loadFileText;
    SaveFileTextCallback saveFileText;
    Vector2 wheel_move;
    Vector2 co;
#ifdef ENABLE_CLOCK
    Uint64 clock_last_tick;
#endif
    SDL_TouchID touch_dev;
    SDL_FPoint cam_origin;
    SDL_Cursor* cursor;
    SDL_Window* w;
    SDL_Renderer* r;
    SDL_Texture* screen_tex;
    const Uint8* kbd_array;
    Uint8 mousepress_array[8];
#ifdef HANDLE_KEY_PRESS
    Uint8* keypress_array;
#endif
    char* clip_ptr;
#ifdef ENABLE_CLOCK
    float clock_freq;
    float clock_delta;
    float fps_limit;
#endif
    float z;
    double cam_rot;
    unsigned int fl;
    int cam_flip;
    int num_kbd_keys;
    int log_level;
    int exit_key;
#ifdef IMG_SUPPORT
    bool img_enabled;
#endif
#ifdef MIX_SUPPORT
    bool mix_enabled;
    bool mix_device_opened;
#endif
    bool z_en;
    bool not_first_init;
    bool event_waiting;
    bool w_resized;
    bool was_init;
    bool should_close;
    bool need_to_swap;
};
struct rl_type rl;


#ifdef __cplusplus
}
#endif
