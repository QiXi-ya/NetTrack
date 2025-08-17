#include "Main.h"
#include "trd.h"
#include "resource.h"

#define IDI_ICON1 101

/// <summary>
/// 托盘操作类
/// </summary>
NOTIFYICONDATA nid = { 0 };

    void trd::InitTrayIcon(HWND hwnd, const wchar_t* tooltip) {
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = hwnd;
        nid.uID = 1;
        nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        nid.uCallbackMessage = WM_USER + 1;

        nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        wcsncpy(nid.szTip, tooltip, ARRAYSIZE(nid.szTip) - 1);
        if (Shell_NotifyIconW(NIM_ADD, &nid))
        {
            if (debug) cout << endl << "add success" << endl;
        }
        else
        {
            if (debug) cout << endl << "add failed" << endl;
        }
    }

    void trd::ShowTrayMessage(HWND hwnd, const wchar_t* text, const wchar_t* title, int type) {
        if (debug) cout << endl << "showMessage : text -> ";
        if (debug) wprintf(L"%s", text);
        if (debug) cout << "    title ->";
        if (debug) wprintf(L"%s", title);

        NOTIFYICONDATAW nid = { sizeof(NOTIFYICONDATAW) };
        nid.hWnd = hwnd;
        nid.uID = 1;
        nid.uFlags = NIF_INFO;

        // 安全拷贝 text 和 title
        wcsncpy(nid.szInfo, text, ARRAYSIZE(nid.szInfo) - 1);
        nid.szInfo[ARRAYSIZE(nid.szInfo) - 1] = L'\0';

        wcsncpy(nid.szInfoTitle, title, ARRAYSIZE(nid.szInfoTitle) - 1);
        nid.szInfoTitle[ARRAYSIZE(nid.szInfoTitle) - 1] = L'\0';

        switch (type) {
        case 0: nid.dwInfoFlags = NIIF_INFO; break;
        case 1: nid.dwInfoFlags = NIIF_WARNING; break;
        case 2: nid.dwInfoFlags = NIIF_ERROR; break;
        default: nid.dwInfoFlags = NIIF_NONE; break;
        }

        if (!Shell_NotifyIconW(NIM_MODIFY, &nid)) {
            if (debug) wcerr << L"错误: 无法显示托盘消息 (" << GetLastError() << L")" << endl;
        }
    }

    void trd::RemoveTrayIcon(HWND hwnd) {
        nid.hWnd = hwnd;
        Shell_NotifyIcon(NIM_DELETE, &nid);
    }
