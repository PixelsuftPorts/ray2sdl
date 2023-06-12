#include <raydef.h>
#include <raylib.h>

// TODO
RLAPI Shader LoadShader(const char *vsFileName, const char *fsFileName) {
    Shader result = { 0 };
    return result;
}

RLAPI Shader LoadShaderFromMemory(const char *vsCode, const char *fsCode) {
    Shader result = { 0 };
    return result;
}

RLAPI bool IsShaderReady(Shader shader) {
    return false;
}

RLAPI int GetShaderLocation(Shader shader, const char *uniformName) {
    return 0;
}

RLAPI int GetShaderLocationAttrib(Shader shader, const char *attribName) {
    return 0;
}

RLAPI void SetShaderValue(Shader shader, int locIndex, const void *value, int uniformType) {

}

RLAPI void SetShaderValueV(Shader shader, int locIndex, const void *value, int uniformType, int count) {
    
}

RLAPI void SetShaderValueMatrix(Shader shader, int locIndex, Matrix mat) {
    
}

RLAPI void SetShaderValueTexture(Shader shader, int locIndex, Texture2D texture) {
    
}

RLAPI void UnloadShader(Shader shader) {
    
}
