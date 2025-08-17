
#include "Main.h"
#include "WindowsEv.h"


using namespace std;


    POINT Window::GetMousePos()
    {
        POINT pt_temp;
        GetCursorPos(&pt_temp);
        RECT windowRect;
        GetWindowRect(mainWindow, &windowRect);
        POINT returnA = { 0,0 };
        returnA.x = pt_temp.x - windowRect.left;
        returnA.y = pt_temp.y - windowRect.top - 40;
        return returnA;
    }
    POINT Window::GetMousePosX()
    {
        int x, y;
        mousepos(&x, &y);
        POINT returnA = { x,y };
        return returnA;
    }
    POINT Window::GetWindowSize()
    {
        int size_temp[4] = { 0,0,0,0 };
        window_getviewport(&size_temp[0], &size_temp[1], &size_temp[2], &size_temp[3]);
        POINT returnA = { 0,0 };
        returnA.x = size_temp[2] - size_temp[0];
        returnA.y = size_temp[3] - size_temp[0];
        return returnA;
    }
     bool Window::IsDarkModeEnabled() {
        if (!userset[3].empty())
        {
            if (stoi(userset[3]) == 0)
            {
                DWORD value = 0;
                DWORD valueSize = sizeof(value);
                HKEY hKey;

                // 打开注册表项
                if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                    // 读取AppsUseLightTheme值
                    if (RegGetValue(hKey, NULL, L"AppsUseLightTheme", RRF_RT_REG_DWORD, NULL, &value, &valueSize) == ERROR_SUCCESS) {
                        RegCloseKey(hKey);
                        return value == 0; // 0表示深色模式,1表示浅色模式
                    }
                    RegCloseKey(hKey);
                }

                return false; // 默认返回浅色模式
            }
            else return stoi(userset[3]) == 1;
        }
        else
        {
            DWORD value = 0;
            DWORD valueSize = sizeof(value);
            HKEY hKey;

            // 打开注册表项
            if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                // 读取AppsUseLightTheme值
                if (RegGetValue(hKey, NULL, L"AppsUseLightTheme", RRF_RT_REG_DWORD, NULL, &value, &valueSize) == ERROR_SUCCESS) {
                    RegCloseKey(hKey);
                    return value == 0; // 0表示深色模式,1表示浅色模式
                }
                RegCloseKey(hKey);
            }

            return false; // 默认返回浅色模式
        }
    }
    bool Window::isForegroundWindow()
    {
        HWND ForeWindow = GetForegroundWindow();
        return ForeWindow == mainWindow;
    }
    POINT Window::GetWindowPos()
    {
        RECT temp = { 0 };
        GetWindowRect(mainWindow, &temp);
        POINT returnA = { temp.left,temp.top };
        return returnA;
    }
    POINT Window::GetScreenSize()
    { 
        POINT screenSize = { 0,0 };
        screenSize.x = GetSystemMetrics(SM_CXSCREEN);
        screenSize.y = GetSystemMetrics(SM_CYSCREEN);
		return screenSize;
    }
