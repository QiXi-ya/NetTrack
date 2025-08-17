#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <graphics.h>
#include "Main.h"
using namespace std;

class msg {
public:
    static int msgBoxManager();
    static int Message(int type, int color[3], int fontColor[3], int overtime, std::string text);
    static void KillBox(std::string text);
    static void MessageBar(int color[3], int fontColor[3], std::string text, std::string bottom[3]);
    static int DrawMessageBox();
    static std::vector<std::string> splitByZero(const std::string& input);
    static void DrawMessageBar();
    static bool isBox(std::string text);
    static int GetMessageBarChoose();
	static bool noBarH();
};
extern int msgBarChoose;
extern bool noBar;
