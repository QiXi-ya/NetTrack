#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#pragma warning(disable: 4244)
#pragma warning(disable: 4566)
#pragma warning(disable: 4838)
#pragma warning(disable: 4267)
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <graphics.h>

//==================================背景图片==================================
extern PIMAGE backImage;
extern PIMAGE backImage_i;

extern HINSTANCE hInstanceTop;
extern HWND mainWindow;
extern HWND mainHwnd;
extern POINT mousePos;
extern POINT windowSize;
extern POINT screen;

//存档-----------------------------------------------------------------------------------|
const int saveMax = 256;
const int saveConfigMax = 15;
const int lastUploadMax = 128;
const int lastDownloadMax = 128;
extern std::string saveData_link[saveMax];
extern std::string userset[saveConfigMax];
extern std::string lastUpload[lastUploadMax];
extern std::string lastDownload[lastDownloadMax];


//userset
// 0语言
//  1 加密方式
// 2密码
// 3 颜色
// 4下载位置
//5 背景模糊度
// 6背景不透明度
//7 访问方式
// 8 速度上限
// 9 随机端口

extern std::vector<std::string> trustList; //信任列表
extern std::vector<std::string> noList; //黑名单

extern bool DarkMode;
extern int BKcolor;
extern bool debug;

//==================================托盘==========================================
extern UINT WM_TASKBARCREATED;

//===================================子窗口=======================================
extern bool noChange;
extern HWND subWindow;
extern int subAlpha;

//==============================狗子模式=========================================
extern HHOOK g_mouseHook;
extern bool max_EGE;
extern RECT lastPos;
extern WNDPROC g_EGE_OldWndProc;
extern bool resizing;
extern RECT resizeRect;
extern bool a;
extern int GS;
extern int temp_GS;
extern bool rs;
const int easeTM = 30;//ease 帧数

//==================================文件发送列表==================================
struct SendFileInfo {
    std::string filePath; // Net端实际发送的文件路径
    std::string displayPath; // NetTrack显示端的映像路径
};
extern std::vector<SendFileInfo> sendFileList;
extern int whle_temp;
extern int whle;

extern bool isTrd;