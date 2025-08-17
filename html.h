#pragma once
#include "Main.h"


class HtmlViewer {
public:
    void setLg(int lgA);
    int LoadHtmlFromResource(const wchar_t* filePath, const std::wstring& title);
private:
    HWND hwnd_;
    std::wstring htmlContent_;
    std::wstring title_;
};