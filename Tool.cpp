#define WIN32_LEAN_AND_MEAN
#define  _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <Windows.h>
#include "Tool.h"
#include <ShlDisp.h>
#include <Shellapi.h>

bool tool::IsWindows11OrGreater() {
    OSVERSIONINFOEXW osvi = { sizeof(osvi), 10, 0, 22000 };
    DWORDLONG mask = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
    return VerifyVersionInfoW(&osvi, VER_MAJORVERSION, mask);
}

bool tool::isValidChar(char c) {
    return (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') ||
        (c == '+') ||
        (c == '/') || (c == '=');
}

std::string tool::wstring_to_utf8(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, nullptr, nullptr);
    return str;
}

void tool::MoveNullToEnd(std::string arr[], int size) {
    std::stable_partition(arr, arr + size,
        [](const std::string& s) {
            return s != "NULL";
        });
}

std::wstring tool::string_to_wstring(const std::string& str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    wchar_t* wc_str = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wc_str, len);
    std::wstring result(wc_str);
    delete[] wc_str;
    return result;
}

void tool::RemoveAt_vector(std::vector<std::string>& vec, size_t pos) {
    if (pos < vec.size()) {
        vec.erase(vec.begin() + pos);
    }
}

std::wstring tool::truncateWithEllipsis(const std::wstring& input, size_t maxLength) {
    if (input.length() <= maxLength) {
        return input;
    }
    if (maxLength <= 3) {
        return input.substr(0, maxLength);
    }
    return input.substr(0, maxLength * 2 - 3) + L"...";
}

void tool::merge_vectors(std::vector<std::string>& dest, const std::vector<std::wstring>& src) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    dest.reserve(dest.size() + src.size());
    for (const auto& wstr : src) {
        try {
            dest.push_back(converter.to_bytes(wstr));
        }
        catch (const std::range_error& e) {
            dest.push_back("[CONVERSION_ERROR]");
            std::cerr << "Failed to convert wstring: " << e.what() << '\n';
        }
    }
}

std::string tool::wstring_to_string(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

bool tool::RunProgramWithShellExecute(const std::wstring& programPath) {
    HINSTANCE hInstance = ShellExecuteW(
        NULL,
        L"open",
        programPath.c_str(),
        NULL,
        NULL,
        SW_SHOWNORMAL
    );
    if ((int)hInstance <= 32) {
        std::wcerr << L"ShellExecuteW failed. Error: " << GetLastError() << std::endl;
        return false;
    }
    return true;
}

bool tool::IsRectEqual(const RECT& rect1, const RECT& rect2) {
    return rect1.left == rect2.left &&
        rect1.top == rect2.top &&
        rect1.right == rect2.right &&
        rect1.bottom == rect2.bottom;
}

RECT tool::GetClientRectDetails(HWND hwnd) {
    RECT rect = { 0 };
    if (!hwnd || !IsWindow(hwnd)) {
        MessageBoxW(NULL, L"无效的窗口句柄", L"错误", MB_ICONERROR);
        return rect;
    }
    if (!GetClientRect(hwnd, &rect)) {
        MessageBoxW(NULL, L"获取客户区失败", L"错误", MB_ICONERROR);
        return rect;
    }
    POINT ptLeftTop = { rect.left, rect.top };
    POINT ptRightBottom = { rect.right, rect.bottom };
    ClientToScreen(hwnd, &ptLeftTop);
    ClientToScreen(hwnd, &ptRightBottom);
    return RECT{
        ptLeftTop.x,
        ptLeftTop.y,
        ptRightBottom.x,
        ptRightBottom.y
    };
}

int tool::GetTaskbarHeight() {
    RECT taskbarRect;
    if (SystemParametersInfo(SPI_GETWORKAREA, 0, &taskbarRect, 0)) {
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        return screenHeight - (taskbarRect.bottom - taskbarRect.top);
    }
    return 0;
}

void tool::PreciseSync(std::vector<std::string>& first, const std::vector<std::string>& second) {
    for (size_t i = 0; i < second.size(); ++i) {
        if (second[i].find("netPack.npk") == std::string::npos) {
            if (i >= first.size()) {
                first.push_back(second[i]);
            }
            else {
                first[i] = second[i];
            }
        }
    }
}

std::wstring tool::GbkToWstring(const std::string& gbkStr) {
    if (gbkStr.empty()) return L"";
    int utf16Len = MultiByteToWideChar(
        CP_ACP,
        0,
        gbkStr.c_str(),
        -1,
        nullptr,
        0
    );
    if (utf16Len == 0) return L"";
    std::wstring utf16Str(utf16Len, L'\0');
    MultiByteToWideChar(
        CP_ACP, 0,
        gbkStr.c_str(), -1,
        &utf16Str[0], utf16Len
    );
    return utf16Str;
}

std::string tool::Utf8ToGbk(const std::string& utf8)
{
    // 如果已经是GBK编码,直接返回
    //if (IsGbkString(utf8)) return utf8;

    int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    std::wstring wstr(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wstr[0], len);

    len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string gbk(len, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &gbk[0], len, NULL, NULL);

    // 去掉末尾的\0
    if (!gbk.empty() && gbk.back() == '\0') gbk.pop_back();
    return gbk;
}

void tool::FlashAndActivateWindow(HWND hwnd,int Count,int TimeOut) {
    // 首先激活窗口
    SetForegroundWindow(hwnd);
    ShowWindow(hwnd, SW_RESTORE);  // 如果窗口最小化则恢复

    // 设置闪烁参数
    FLASHWINFO fwi;
    fwi.cbSize = sizeof(FLASHWINFO);
    fwi.hwnd = hwnd;
    fwi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG | FLASHW_TRAY;  // 闪烁标题栏和任务栏按钮，直到窗口获得焦点
    if (Count > 0) fwi.uCount = Count;  // 闪烁次数（或设为FLASHW_TIMER无限闪烁）
    else fwi.uCount = FLASHW_TIMER;
    fwi.dwTimeout = TimeOut;  // 使用默认闪烁频率

    FlashWindowEx(&fwi);
}

 std::string tool::GetCurrentTimeS() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X"); // 格式：YYYY-MM-DD HH:MM:SS
        return ss.str();
        }