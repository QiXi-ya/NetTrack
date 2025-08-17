#ifndef WINPROMPT_H
#define WINPROMPT_H
#include "Main.h"
#include <windows.h>
#include <string>
#include <tchar.h>
#include <gdiplus.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
using namespace Gdiplus;

class WinPrompt {
public:
    enum Language {
        LANG_C = 0,
        LANG_E = 1,
        LANG_J = 2,
        LANG_K = 3,
        LANG_F = 4,
        LANG_G = 5,
        LANG_R = 6,
        LANG_S = 7
    };
    enum IconType {
        ICON_ERROR,
        ICON_WARNING,
        ICON_INFORMATION,
        ICON_QUESTION
    };
    enum ButtonType {
        BTN_OK,
        BTN_OKCANCEL,
        BTN_YESNO,
        BTN_YESNOCANCEL
    };
    enum WindowMode {
        MODE_DEFAULT,
        MODE_INFO,
        MODE_WARNING,
        MODE_ERROR
    };
    enum ButtonState {
        STATE_NORMAL,
        STATE_HOVER,
        STATE_PRESSED
    };
    struct Button {
        RECT rect;
        int id;
        TCHAR text[20];
        ButtonState state;
    };
    static void setLanguage(Language lang);
    static WinPrompt::Language getLanguage();
    WinPrompt(HINSTANCE hInstance, HWND hParent = NULL);
    ~WinPrompt();
    int show(const TCHAR* title, const TCHAR* message,
        IconType icon, ButtonType buttons, WindowMode mode = MODE_DEFAULT);
private:
    static Language s_language;
    HINSTANCE m_hInstance;
    HWND m_hParent;
    HWND m_hWnd;
    const TCHAR* m_title;
    const TCHAR* m_message;
    IconType m_iconType;
    ButtonType m_buttonType;
    WindowMode m_mode;
    ULONG_PTR m_gdiplusToken;
    bool m_isDarkMode;
    Button* m_buttons;
    int m_buttonCount;
    int m_selectedResult;
    SIZE m_textSize;
    int m_iconSize;
    int m_margin;
    COLORREF m_bgColor;
    COLORREF m_textColor;
    COLORREF m_buttonNormal;
    COLORREF m_buttonHover;
    COLORREF m_buttonPressed;
    COLORREF m_buttonBorder;
    COLORREF GetInverseColor(COLORREF color);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void initializeGDIPlus();
    void shutdownGDIPlus();
    void detectDarkMode();
    void calculateTextSize();
    void createButtons();
    void positionWindow();
    void setTitleBarColor();
    void drawWindow(Graphics* graphics); // Graphics* 改为 void* 以避免头文件依赖
    void drawIcon(Graphics* graphics);
    void drawText(Graphics* graphics);
    void drawButtons(Graphics* graphics);
    int hitTestButtons(int x, int y);
    void updateButtonState(int x, int y, bool isPressed);
    void FillRoundedRectangle(Graphics* graphics, Brush* brush, Rect rect, int radius);
    void DrawRoundedRectangle(Graphics* graphics, Pen* pen, Rect rect, int radius);
    int runMessageLoop();
};

#endif // WINPROMPT_H