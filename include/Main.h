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

//==================================����ͼƬ==================================
extern PIMAGE backImage;
extern PIMAGE backImage_i;

extern HINSTANCE hInstanceTop;
extern HWND mainWindow;
extern HWND mainHwnd;
extern POINT mousePos;
extern POINT windowSize;
extern POINT screen;

//�浵-----------------------------------------------------------------------------------|
const int saveMax = 256;
const int saveConfigMax = 15;
const int lastUploadMax = 128;
const int lastDownloadMax = 128;
extern std::string saveData_link[saveMax];
extern std::string userset[saveConfigMax];
extern std::string lastUpload[lastUploadMax];
extern std::string lastDownload[lastDownloadMax];


//userset
// 0����
//  1 ���ܷ�ʽ
// 2����
// 3 ��ɫ
// 4����λ��
//5 ����ģ����
// 6������͸����
//7 ���ʷ�ʽ
// 8 �ٶ�����
// 9 ����˿�

extern std::vector<std::string> trustList; //�����б�
extern std::vector<std::string> noList; //������

extern bool DarkMode;
extern int BKcolor;
extern bool debug;

//==================================����==========================================
extern UINT WM_TASKBARCREATED;

//===================================�Ӵ���=======================================
extern bool noChange;
extern HWND subWindow;
extern int subAlpha;

//==============================����ģʽ=========================================
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
const int easeTM = 30;//ease ֡��

//==================================�ļ������б�==================================
struct SendFileInfo {
    std::string filePath; // Net��ʵ�ʷ��͵��ļ�·��
    std::string displayPath; // NetTrack��ʾ�˵�ӳ��·��
};
extern std::vector<SendFileInfo> sendFileList;
extern int whle_temp;
extern int whle;

extern bool isTrd;