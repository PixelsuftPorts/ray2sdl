#include <raydef.h>
#include <raylib.h>

// TODO
RLCAPI Shader LoadShader(const char *vsFileName, const char *fsFileName) {
    Shader result = { 0 };
    return result;
}

RLCAPI Shader LoadShaderFromMemory(const char *vsCode, const char *fsCode) {
    Shader result = { 0 };
    return result;
}

RLCAPI bool IsShaderReady(Shader shader) {
    return false;
}

RLCAPI int GetShaderLocation(Shader shader, const char *uniformName) {
    return 0;
}

RLCAPI int GetShaderLocationAttrib(Shader shader, const char *attribName) {
    return 0;
}

RLCAPI void SetShaderValue(Shader shader, int locIndex, const void *value, int uniformType) {

}

RLCAPI void SetShaderValueV(Shader shader, int locIndex, const void *value, int uniformType, int count) {
    
}

RLCAPI void SetShaderValueMatrix(Shader shader, int locIndex, Matrix mat) {
    
}

RLCAPI void SetShaderValueTexture(Shader shader, int locIndex, Texture2D texture) {
    
}

RLCAPI void UnloadShader(Shader shader) {
    
}

RLCAPI void BeginShaderMode(Shader shader) {
    
}

RLCAPI void EndShaderMode(void) {

}
