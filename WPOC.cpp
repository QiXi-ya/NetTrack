#include "Main.h"
#include "WPOC.h"
#include "WindowsEv.h"
#include "trd.h"
#include <windowsx.h>

#define WM_TRAYICON (WM_USER + 1)


    //mainHwnd 主程序窗口过程
    LRESULT CALLBACK WindowProc_class::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (message == WM_TASKBARCREATED) {
            // Explorer重启，重新添加托盘图标
            trd::InitTrayIcon(mainHwnd, L"NetTrack");
        }
        mainHwnd = hWnd;
        switch (message) {
        case WM_DESTROY:
            trd::RemoveTrayIcon(mainHwnd);
            PostQuitMessage(0);
            break;
        case WM_TRAYICON:
            if (lParam == WM_LBUTTONUP) { // 鼠标左键弹起
                /*
                if (!g_lastReceivedFilePath.empty()) {
                    // 打开文件夹并选中文件
                    PIDLIST_ABSOLUTE pidl = ILCreateFromPathW(g_lastReceivedFilePath.c_str());
                    if (pidl) {
                        SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);
                        ILFree(pidl);
                    }
                }
                */
            }
            if (lParam == WM_RBUTTONUP) {
                // 右键菜单等
            }
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
    //mainWindow 主图形界面(EGE)过程函数
    LRESULT CALLBACK WindowProc_class::EGE_WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (message == WM_SIZE) {
            if (wParam == SIZE_MAXIMIZED && max_EGE == false)
            {
                max_EGE = true;
                DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &lastPos, sizeof(lastPos));
                int rb = 0;
                RECT A2 = { 0 };
                GetClientRect(hWnd, &A2);
                rb = A2.top - lastPos.top;
                RECT desktopRect;
                SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopRect, 0);
                int sw = desktopRect.right - desktopRect.left;
                int sh = desktopRect.bottom - desktopRect.top;
                resizewindow(sw, sh - rb);
            }
            else if (wParam == SIZE_RESTORED && max_EGE == true)
            {
                max_EGE = false;
                resizeRect = { 100,100,screen.x * 2 / 3,screen.y * 2 / 3 };
                resizewindow(resizeRect.right - resizeRect.left, resizeRect.bottom - resizeRect.top);
                movewindow(resizeRect.left, resizeRect.top, false);
            }
        }
        // 其他消息交给原窗口过程
        return CallWindowProc(g_EGE_OldWndProc, hWnd, message, wParam, lParam);
    }

    //右键松开钩子;
    LRESULT CALLBACK WindowProc_class::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode == HC_ACTION) {
            MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;
            if (wParam == WM_LBUTTONUP) {
                if (subWindow == GetForegroundWindow() && !resizing) {
                    resizing = true;
                    rs = true;
                    DwmGetWindowAttribute(subWindow, DWMWA_EXTENDED_FRAME_BOUNDS, &resizeRect, sizeof(resizeRect));
                    
                    temp_GS = max(abs(resizeRect.right - resizeRect.left - windowSize.x), abs(resizeRect.bottom - resizeRect.top - windowSize.y)) / easeTM;
                    if (temp_GS < 1) temp_GS = 1;
                    GS = easeTM;
                    ReleaseCapture();
                }
            }
            if (wParam == WM_LBUTTONDOWN) {
                resizing = false;
                if (subWindow == GetForegroundWindow() && !resizing) {
                    noChange = true;
                }
            }
        }
        return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
    }

    //子窗口过程函数
    LRESULT CALLBACK WindowProc_class::SubWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        int borderWidth = 6; // 虚线宽度
        switch (msg) {
        case WM_SIZE: {
            RECT rc;
            GetClientRect(hWnd, &rc);
            int w = rc.right, h = rc.bottom;
            if (w <= 0 || h <= 0) break;

            BITMAPINFO bmi = { 0 };
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = w;
            bmi.bmiHeader.biHeight = -h;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;
            void* bits = nullptr;
            HDC hdc = GetDC(hWnd);
            HDC memdc = CreateCompatibleDC(hdc);
            HBITMAP hBmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
            HBITMAP oldBmp = (HBITMAP)SelectObject(memdc, hBmp);

            COLORREF rgb = RGB(155, 255, 200);
            int borderWidth = 6;
            int dashLen = 18, gapLen = 10;
            int r = 18; // 圆角半径
            BYTE alpha = subAlpha;

            memset(bits, 0, w * h * 4);

            if (alpha > 0) {
                auto putpixel_alpha = [&](int x, int y) {
                    if (x >= 0 && x < w && y >= 0 && y < h)
                        ((DWORD*)bits)[y * w + x] = (alpha << 24) | (rgb & 0xFFFFFF);
                    };



                // 画四条边（虚线）


                        // 上边
                for (int x = r; x < w - r; x += dashLen + gapLen) {
                    for (int dx = 0; dx < dashLen && x + dx < w - r; ++dx) {
                        for (int bw = 0; bw < borderWidth; ++bw) {
                            putpixel_alpha(x + dx, bw);
                        }
                    }
                }
                // 下边
                for (int x = r; x < w - r; x += dashLen + gapLen) {
                    for (int dx = 0; dx < dashLen && x + dx < w - r; ++dx) {
                        for (int bw = 0; bw < borderWidth; ++bw) {
                            putpixel_alpha(x + dx, h - 1 - bw);
                        }
                    }
                }
                // 左边
                for (int y = r; y < h - r; y += dashLen + gapLen) {
                    for (int dy = 0; dy < dashLen && y + dy < h - r; ++dy) {
                        for (int bw = 0; bw < borderWidth; ++bw) {
                            putpixel_alpha(bw, y + dy);
                        }
                    }
                }
                // 右边
                for (int y = r; y < h - r; y += dashLen + gapLen) {
                    for (int dy = 0; dy < dashLen && y + dy < h - r; ++dy) {
                        for (int bw = 0; bw < borderWidth; ++bw) {
                            putpixel_alpha(w - 1 - bw, y + dy);
                        }
                    }
                }
                // 四个圆角（实线）
                // 左上角
                for (int t = 0; t < 90; ++t) {
                    double rad = t * 3.14159265 / 180.0;
                    int cx = r, cy = r;
                    for (int bw = 0; bw < borderWidth; ++bw) {
                        int px = static_cast<int>(cx + (r - bw) * cos(3.14159265 - rad));
                        int py = static_cast<int>(cy - (r - bw) * sin(3.14159265 - rad));
                        putpixel_alpha(px, py);
                    }
                }
                // 右上角
                for (int t = 0; t < 90; ++t) {
                    double rad = t * 3.14159265 / 180.0;
                    int cx = w - r - 1, cy = r;
                    for (int bw = 0; bw < borderWidth; ++bw) {
                        int px = static_cast<int>(cx + (r - bw) * cos(rad));
                        int py = static_cast<int>(cy - (r - bw) * sin(rad));
                        putpixel_alpha(px, py);
                    }
                }
                // 右下角
                for (int t = 0; t < 90; ++t) {
                    double rad = (t + 360) * 3.14159265 / 180.0; // 270~360度
                    int cx = w - r - 1, cy = h - r - 1;
                    for (int bw = 0; bw < borderWidth; ++bw) {
                        int px = static_cast<int>(cx + (r - bw) * cos(rad));
                        int py = static_cast<int>(cy + (r - bw) * sin(rad));
                        putpixel_alpha(px, py);
                    }
                }
                // 左下角
                for (int t = 0; t < 90; ++t) {
                    double rad = t * 3.14159265 / 180.0;
                    int cx = r, cy = h - r - 1;
                    for (int bw = 0; bw < borderWidth; ++bw) {
                        int px = static_cast<int>(cx - (r - bw) * cos(rad));
                        int py = static_cast<int>(cy + (r - bw) * sin(rad));
                        putpixel_alpha(px, py);
                    }
                }


            }

            // 层叠窗口更新
            POINT ptSrc = { 0, 0 };
            SIZE sizeWnd = { w, h };
            BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
            POINT ptDst;
            RECT winrc;
            GetWindowRect(hWnd, &winrc);
            ptDst.x = winrc.left;
            ptDst.y = winrc.top;
            HDC screenDC = GetDC(NULL);
            UpdateLayeredWindow(hWnd, screenDC, &ptDst, &sizeWnd, memdc, &ptSrc, 0, &blend, ULW_ALPHA);
            ReleaseDC(NULL, screenDC);
            SelectObject(memdc, oldBmp);
            DeleteObject(hBmp);
            DeleteDC(memdc);
            ReleaseDC(hWnd, hdc);
            break;
        }
        case WM_LBUTTONDOWN: {
            // 判断是否在边缘，开始调整大小
            //MessageBoxA(NULL, "子窗口", "提示", MB_OK | MB_ICONINFORMATION);
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            RECT rc;
            GetClientRect(hWnd, &rc);
            int border = 10;
            if (pt.x > rc.right - border && pt.y > rc.bottom - border) {
                SetCapture(hWnd);
            }
            break;
        }
        case WM_DESTROY:
        {
            break;
        }
        default:
        {
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        }
        return 0;
    }

    // 在程序初始化时调用
    void WindowProc_class::InstallMouseHook() {
        g_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    }

    // 程序退出时调用
    void WindowProc_class::UninstallMouseHook() {
        if (g_mouseHook) UnhookWindowsHookEx(g_mouseHook);
    }
