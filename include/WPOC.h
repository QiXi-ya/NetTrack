#pragma once
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "Main.h"

class WindowProc_class
{
public:
    //mainHwnd �����򴰿ڹ���
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    //mainWindow ��ͼ�ν���(EGE)���̺���
    static LRESULT CALLBACK EGE_WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    //�Ҽ��ɿ�����;
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

    //�Ӵ��ڹ��̺���
    static LRESULT CALLBACK SubWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // �ڳ����ʼ��ʱ����
    static void InstallMouseHook();

    // �����˳�ʱ����
    static void UninstallMouseHook();
};