#pragma once
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "Main.h"

class WindowProc_class
{
public:
    //mainHwnd 主程序窗口过程
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    //mainWindow 主图形界面(EGE)过程函数
    static LRESULT CALLBACK EGE_WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    //右键松开钩子;
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

    //子窗口过程函数
    static LRESULT CALLBACK SubWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // 在程序初始化时调用
    static void InstallMouseHook();

    // 程序退出时调用
    static void UninstallMouseHook();
};