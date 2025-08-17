#pragma once
#include <Windows.h>
#include <graphics.h>
class DrawManager
{
public:
    static void setFillRectColor();
    static void setFont(int size);
    static void centerText();
    static void startText();
    static void fillroundrectwithrect(int x, int y, int w, int h, int r);
};