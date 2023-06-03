#include <SDL2/SDL_syswm.h>
#ifdef SDL_VIDEO_DRIVER_WINDOWS
#include <windows.h>
#include <stdbool.h>
#endif

void* GetHandleBySDLWindow(SDL_Window* window) {
    SDL_SysWMinfo wm_info;
    SDL_VERSION(&wm_info.version);
    if (SDL_GetWindowWMInfo(window, &wm_info) == SDL_FALSE)
        return NULL;
#ifdef SDL_VIDEO_DRIVER_WINDOWS
    return (void*)wm_info.info.win.window;
#endif
#ifdef SDL_VIDEO_DRIVER_WINRT
    return (void*)wm_info.info.winrt.window;
#endif
#ifdef SDL_VIDEO_DRIVER_X11
    return (void*)wm_info.info.x11.window; // Right?
#endif
#ifdef SDL_VIDEO_DRIVER_WAYLAND
    return (void*)wm_info.info.wl.egl_window; // Right?
#endif
#ifdef SDL_VIDEO_DRIVER_DIRECTFB
    return (void*)wm_info.info.directfb.window;
#endif
#ifdef SDL_VIDEO_DRIVER_COCOA
    return (void*)wm_info.info.cocoa.window;
#endif
#ifdef SDL_VIDEO_DRIVER_UIKIT
    return (void*)wm_info.info.uikit.window;
#endif
#ifdef SDL_VIDEO_DRIVER_ANDROID
    return (void*)wm_info.info.android.window;
#endif
#ifdef SDL_VIDEO_DRIVER_OS2
    return (void*)wm_info.info.os2.hwnd;
#endif
#ifdef SDL_VIDEO_DRIVER_VIVANTE
    return (void*)wm_info.info.vivante.window;
#endif
#ifdef SDL_VIDEO_DRIVER_KMSDRM
    return (void*)wm_info.info.kmsdrm.gbm_dev;
#endif
    return NULL;
}

void CheckDarkMode(SDL_Window* window) {
#ifdef SDL_VIDEO_DRIVER_WINDOWS
    HMODULE dwm = LoadLibraryA("dwmapi.dll");
    if (!dwm)
        return;
    HMODULE uxtheme = LoadLibraryA("uxtheme.dll");
    if (!uxtheme) {
        FreeLibrary(dwm);
        return;
    }
    typedef HRESULT (*DwmSetWindowAttributePTR)(HWND, DWORD, LPCVOID, DWORD);
    DwmSetWindowAttributePTR DwmSetWindowAttribute = (DwmSetWindowAttributePTR)GetProcAddress(dwm, "DwmSetWindowAttribute");
    typedef bool (WINAPI *ShouldAppsUseDarkModePTR)();
    ShouldAppsUseDarkModePTR ShouldAppsUseDarkMode = (ShouldAppsUseDarkModePTR)GetProcAddress(uxtheme, MAKEINTRESOURCEA(132));
    void* handle = GetHandleBySDLWindow(window);
    if (handle == NULL || !DwmSetWindowAttribute || !ShouldAppsUseDarkMode || !ShouldAppsUseDarkMode()) {
        FreeLibrary(uxtheme);
        FreeLibrary(dwm);
        return;
    }
    HWND hwnd = (HWND)handle;
    BOOL dark_mode = 1;
    if (!DwmSetWindowAttribute(hwnd, 20, &dark_mode, sizeof(BOOL))) {
        dark_mode = 1;
        DwmSetWindowAttribute(hwnd, 19, &dark_mode, sizeof(BOOL));
    }
    FreeLibrary(uxtheme);
    FreeLibrary(dwm);
#endif
}
