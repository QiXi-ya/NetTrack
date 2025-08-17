#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#pragma warning(disable: 4244)
#pragma warning(disable: 4566)
#pragma warning(disable: 4838)
#pragma warning(disable: 4267)
#include <iostream>
#include <filesystem>
#include <string>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <thread>
#include <graphics.h>
using namespace std;
namespace fs = filesystem;
#include "Main.h"

class Window
{
public:
    static POINT GetMousePos();
    static POINT GetMousePosX();
    static POINT GetWindowSize();
    static  bool IsDarkModeEnabled();
    static bool isForegroundWindow();
    static POINT GetWindowPos();
    static POINT GetScreenSize();
};
