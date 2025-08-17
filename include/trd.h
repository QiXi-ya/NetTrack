#pragma once
#include "Main.h"
#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

class trd
{
public:
    static void InitTrayIcon(HWND hwnd, const wchar_t* tooltip);
    static void ShowTrayMessage(HWND hwnd, const wchar_t* text, const wchar_t* title, int type);
    static void RemoveTrayIcon(HWND hwnd);
};