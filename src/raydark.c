#ifdef _WIN32
#include <SDL2/SDL_syswm.h>
#include <windows.h>
#include <stdbool.h>
#endif

void CheckDarkMode(SDL_Window* window) {
#ifdef _WIN32
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
    if (!DwmSetWindowAttribute || !ShouldAppsUseDarkMode || !ShouldAppsUseDarkMode()) {
        FreeLibrary(uxtheme);
        FreeLibrary(dwm);
        return;
    }
    SDL_SysWMinfo wm_info;
    SDL_VERSION(&wm_info.version);
    SDL_GetWindowWMInfo(window, &wm_info);
    HWND hwnd = (HWND)wm_info.info.win.window;
    BOOL dark_mode = 1;
    if (!DwmSetWindowAttribute(hwnd, 20, &dark_mode, sizeof(BOOL))) {
        dark_mode = 1;
        DwmSetWindowAttribute(hwnd, 19, &dark_mode, sizeof(BOOL));
    }
    FreeLibrary(uxtheme);
    FreeLibrary(dwm);
#endif
}
