#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <filesystem>

class tool {
public:
    static bool IsWindows11OrGreater();
    static bool isValidChar(char c);
    static std::string wstring_to_utf8(const std::wstring& wstr);
    static void MoveNullToEnd(std::string arr[], int size);
    static std::wstring string_to_wstring(const std::string& str);
    static void RemoveAt_vector(std::vector<std::string>& vec, size_t pos);
    static std::wstring truncateWithEllipsis(const std::wstring& input, size_t maxLength);
    static void merge_vectors(std::vector<std::string>& dest, const std::vector<std::wstring>& src);
    static std::string wstring_to_string(const std::wstring& wstr);
    static bool RunProgramWithShellExecute(const std::wstring& programPath);
    static bool IsRectEqual(const RECT& rect1, const RECT& rect2);
    static RECT GetClientRectDetails(HWND hwnd);
    static int GetTaskbarHeight();
    static void PreciseSync(std::vector<std::string>& first, const std::vector<std::string>& second);
    static std::wstring GbkToWstring(const std::string& gbkStr);
    template <typename T>
    static int addAndShiftToFront(T arr[], int size, int capacity, const T& newElement);
    template <typename T>
    static bool removeFirstAndShift(T arr[]);
    static std::string Utf8ToGbk(const std::string& utf8);
    static void FlashAndActivateWindow(HWND hwnd, int Count, int TimeOut);
    static std::string GetCurrentTimeS();
};
