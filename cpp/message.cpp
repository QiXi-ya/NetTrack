#include <vector>
#include <string>
#include "Main.h"
#include "message.h"



// 弹窗尺寸常量，可根据需要调整
#define FONT_SIZE 10
#define MAX_TEXT_WIDTH 500
#define MIN_TEXT_WIDTH 300
#define ICON_DEFAULT_SIZE 62
#define MARGIN 20
#define BUTTON_WIDTH 90
#define BUTTON_HEIGHT 36
#define BUTTON_SPACING 12
#define ROUND_RADIUS 12

// 按钮ID
#define ID_OK     1001
#define ID_CANCEL 1002
#define ID_YES    1003
#define ID_NO     1004

// 系统声音相关宏定义
#define SND_INFO MB_ICONINFORMATION
#define SND_WARNING MB_ICONWARNING
#define SND_ERROR MB_ICONERROR
#define SND_QUESTION MB_ICONQUESTION

std::vector<int> ofstr;

WinPrompt::Language WinPrompt::s_language = WinPrompt::LANG_C;

void WinPrompt::setLanguage(Language lang) {
    s_language = lang;
}

WinPrompt::Language WinPrompt::getLanguage() {
    return s_language;
}

WinPrompt::WinPrompt(HINSTANCE hInstance, HWND hParent)
    : m_hInstance(hInstance), m_hParent(hParent), m_hWnd(NULL),
    m_title(NULL), m_message(NULL), m_iconType(ICON_INFORMATION),
    m_buttonType(BTN_OK), m_mode(MODE_DEFAULT), m_gdiplusToken(0),
    m_isDarkMode(false), m_buttons(NULL), m_buttonCount(0),
    m_selectedResult(0), m_iconSize(ICON_DEFAULT_SIZE), m_margin(MARGIN) {
    initializeGDIPlus();
    detectDarkMode();
}

WinPrompt::~WinPrompt() {
    shutdownGDIPlus();
    if (m_buttons) {
        delete[] m_buttons;
        m_buttons = NULL;
    }
}

void WinPrompt::initializeGDIPlus() {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

void WinPrompt::shutdownGDIPlus() {
    if (m_gdiplusToken != 0) {
        GdiplusShutdown(m_gdiplusToken);
        m_gdiplusToken = 0;
    }
}

void WinPrompt::detectDarkMode() {
    // 检测系统是否使用深色模式
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER,
        _T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"),
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD value = 0;
        DWORD size = sizeof(DWORD);
        if (RegQueryValueEx(hKey, _T("AppsUseLightTheme"), NULL, NULL,
            (LPBYTE)&value, &size) == ERROR_SUCCESS) {
            m_isDarkMode = (value == 0);
        }
        RegCloseKey(hKey);
    }

    // 设置颜色方案
    if (m_isDarkMode) {
        m_bgColor = RGB(30, 30, 30);
        m_textColor = RGB(255, 255, 255);
        m_buttonNormal = RGB(60, 60, 60);
        m_buttonHover = RGB(80, 80, 80);
        m_buttonPressed = RGB(100, 100, 100);
        m_buttonBorder = RGB(90, 90, 90);
    }
    else {
        m_bgColor = RGB(255, 255, 255);
        m_textColor = RGB(0, 0, 0);
        m_buttonNormal = RGB(240, 240, 240);
        m_buttonHover = RGB(220, 220, 220);
        m_buttonPressed = RGB(200, 200, 200);
        m_buttonBorder = RGB(200, 200, 200);
    }
}

COLORREF WinPrompt::GetInverseColor(COLORREF color) {
    return RGB(255 - GetRValue(color), 255 - GetGValue(color), 255 - GetBValue(color));
}

void WinPrompt::calculateTextSize() {
    HDC hdc = GetDC(m_hWnd);
    HFONT hFont = CreateFont(
        -MulDiv(FONT_SIZE, GetDeviceCaps(hdc, LOGPIXELSY), 72),
        0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        VARIABLE_PITCH, _T("Segoe UI")
    );

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    RECT rect = { 0, 0, MAX_TEXT_WIDTH, 0 };
    DrawText(hdc, m_message, -1, &rect, DT_CALCRECT | DT_WORDBREAK | DT_LEFT);

    m_textSize.cx = rect.right;
    if (m_textSize.cx < MIN_TEXT_WIDTH) {
        m_textSize.cx = MIN_TEXT_WIDTH;
    }
    m_textSize.cy = rect.bottom;

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    ReleaseDC(m_hWnd, hdc);
}

void WinPrompt::createButtons() {
    // 确定按钮数量（保持不变）
    switch (m_buttonType) {
    case BTN_OK:
        m_buttonCount = 1;
        break;
    case BTN_OKCANCEL:
    case BTN_YESNO:
        m_buttonCount = 2;
        break;
    case BTN_YESNOCANCEL:
        m_buttonCount = 3;
        break;
    }

    // 分配按钮数组（保持不变）
    if (m_buttons) {
        delete[] m_buttons;
    }
    m_buttons = new Button[m_buttonCount];
    ZeroMemory(m_buttons, sizeof(Button) * m_buttonCount);

    // 获取客户区大小（保持不变）
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;

    // 计算按钮起始X坐标(靠右对齐)（保持不变）
    int totalWidth = (BUTTON_WIDTH * m_buttonCount) + (BUTTON_SPACING * (m_buttonCount - 1));
    int startX = clientWidth - totalWidth - m_margin;
    int yPos = clientHeight - BUTTON_HEIGHT - m_margin;

    // 多语言按钮文本定义
    struct ButtonTexts {
        const TCHAR* ok;
        const TCHAR* cancel;
        const TCHAR* yes;
        const TCHAR* no;
    };

    // 8种语言的按钮文本映射表
    const ButtonTexts langTexts[] = {
        {_T("确定"), _T("取消"), _T("是(Y)"), _T("否(N)")},       // 中文
        {_T("OK"), _T("Cancel"), _T("Yes(Y)"), _T("No(N)")},     // 英文
        {_T("確認"), _T("キャンセル"), _T("はい(Y)"), _T("いいえ(N)")}, // 日语
        {_T("확인"), _T("취소"), _T("예(Y)"), _T("아니오(N)")},    // 韩文
        {_T("OK"), _T("Annuler"), _T("Oui(O)"), _T("Non(N)")},    // 法语
        {_T("OK"), _T("Abbrechen"), _T("Ja(J)"), _T("Nein(N)")},  // 德语
        {_T("ОК"), _T("Отмена"), _T("Да(D)"), _T("Нет(H)")},      // 俄语
        {_T("Aceptar"), _T("Cancelar"), _T("Sí(S)"), _T("No(N)")} // 西班牙语
    };

    // 获取当前语言对应的文本
    const ButtonTexts& texts = langTexts[getLanguage()];
    int index = 0;

    // 创建按钮（根据当前语言设置文本）
    if (m_buttonType == BTN_OK || m_buttonType == BTN_OKCANCEL) {
        m_buttons[index].id = ID_OK;
        _tcscpy_s(m_buttons[index].text, texts.ok);
        m_buttons[index].rect = { startX, yPos, startX + BUTTON_WIDTH, yPos + BUTTON_HEIGHT };
        m_buttons[index].state = STATE_NORMAL;
        startX += BUTTON_WIDTH + BUTTON_SPACING;
        index++;
    }

    if (m_buttonType == BTN_YESNO || m_buttonType == BTN_YESNOCANCEL) {
        m_buttons[index].id = ID_YES;
        _tcscpy_s(m_buttons[index].text, texts.yes);
        m_buttons[index].rect = { startX, yPos, startX + BUTTON_WIDTH, yPos + BUTTON_HEIGHT };
        m_buttons[index].state = STATE_NORMAL;
        startX += BUTTON_WIDTH + BUTTON_SPACING;
        index++;
    }

    if (m_buttonType == BTN_YESNO || m_buttonType == BTN_YESNOCANCEL) {
        m_buttons[index].id = ID_NO;
        _tcscpy_s(m_buttons[index].text, texts.no);
        m_buttons[index].rect = { startX, yPos, startX + BUTTON_WIDTH, yPos + BUTTON_HEIGHT };
        m_buttons[index].state = STATE_NORMAL;
        startX += BUTTON_WIDTH + BUTTON_SPACING;
        index++;
    }

    if (m_buttonType == BTN_OKCANCEL || m_buttonType == BTN_YESNOCANCEL) {
        m_buttons[index].id = ID_CANCEL;
        _tcscpy_s(m_buttons[index].text, texts.cancel);
        m_buttons[index].rect = { startX, yPos, startX + BUTTON_WIDTH, yPos + BUTTON_HEIGHT };
        m_buttons[index].state = STATE_NORMAL;
    }
}

void WinPrompt::positionWindow() {
    // 计算窗口总大小
    int windowWidth = m_textSize.cx + m_margin * 2 + (m_iconType != -1 ? m_iconSize + m_margin : 0);
    int windowHeight = m_margin * 3 + m_textSize.cy + BUTTON_HEIGHT + 30; // 30是标题栏高度

    // 获取屏幕尺寸并居中窗口
    RECT screenRect;
    GetWindowRect(GetDesktopWindow(), &screenRect);
    int screenWidth = screenRect.right - screenRect.left;
    int screenHeight = screenRect.bottom - screenRect.top;

    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    SetWindowPos(m_hWnd, NULL, x, y, windowWidth, windowHeight, SWP_NOZORDER);
}

void WinPrompt::setTitleBarColor() {
    if (m_mode != MODE_DEFAULT) {
        COLORREF color;
        switch (m_mode) {
        case MODE_INFO:    color = RGB(50, 150, 255); break;  // 蓝色
        case MODE_WARNING: color = RGB(255, 180, 0); break;   // 黄色
        case MODE_ERROR:   color = RGB(255, 80, 80); break;   // 红色
        default:           return;
        }

        DwmSetWindowAttribute(m_hWnd, DWMWA_CAPTION_COLOR, &color, sizeof(color));
    }
}

void WinPrompt::FillRoundedRectangle(Graphics* graphics, Brush* brush, Rect rect, int radius) {
    if (radius <= 0) {
        graphics->FillRectangle(brush, rect);
        return;
    }

    GraphicsPath path;
    // 使用精确的REAL类型避免AddArc计算时出现的转换错误
    path.AddArc((REAL)rect.X, (REAL)rect.Y, (REAL)radius, (REAL)radius, 180.0f, 90.0f);
    path.AddArc((REAL)(rect.X + rect.Width - radius), (REAL)rect.Y, (REAL)radius, (REAL)radius, 270.0f, 90.0f);
    path.AddArc((REAL)(rect.X + rect.Width - radius), (REAL)(rect.Y + rect.Height - radius), (REAL)radius, (REAL)radius, 0.0f, 90.0f);
    path.AddArc((REAL)rect.X, (REAL)(rect.Y + rect.Height - radius), (REAL)radius, (REAL)radius, 90.0f, 90.0f);
    path.CloseAllFigures();
    graphics->FillPath(brush, &path);
}

void WinPrompt::DrawRoundedRectangle(Graphics* graphics, Pen* pen, Rect rect, int radius) {
    if (radius <= 0) {
        graphics->DrawRectangle(pen, rect);
        return;
    }

    GraphicsPath path;
    // 使用精确的REAL类型避免AddArc计算时出现的转换错误
    path.AddArc((REAL)rect.X, (REAL)rect.Y, (REAL)radius, (REAL)radius, 180.0f, 90.0f);
    path.AddArc((REAL)(rect.X + rect.Width - radius), (REAL)rect.Y, (REAL)radius, (REAL)radius, 270.0f, 90.0f);
    path.AddArc((REAL)(rect.X + rect.Width - radius), (REAL)(rect.Y + rect.Height - radius), (REAL)radius, (REAL)radius, 0.0f, 90.0f);
    path.AddArc((REAL)rect.X, (REAL)(rect.Y + rect.Height - radius), (REAL)radius, (REAL)radius, 90.0f, 90.0f);
    path.CloseAllFigures();
    graphics->DrawPath(pen, &path);
}

void WinPrompt::drawIcon(Graphics* graphics) {
    int x = m_margin;
    int y = m_margin;
    int iconSize = m_iconSize;

    // 用于绘制边框的画笔，根据图标类型设置颜色
    Pen* borderPen = nullptr;
    // 用于绘制符号的画刷，颜色为背景色反色
    SolidBrush* symbolBrush = nullptr;

    // 计算背景色反色，作为符号颜色
    COLORREF bgColorRef = m_bgColor;
    Color symbolColor(255 - GetRValue(bgColorRef), 255 - GetGValue(bgColorRef), 255 - GetBValue(bgColorRef));
    symbolBrush = new SolidBrush(symbolColor);

    switch (m_iconType) {
    case ICON_ERROR: {
        // 错误图标：红色圆形边框
        UINT backdropType = RGB(255, 0, 0);
        DwmSetWindowAttribute(m_hWnd, DWMWA_BORDER_COLOR, &backdropType, sizeof(backdropType));
        DwmSetWindowAttribute(m_hWnd, DWMWA_CAPTION_COLOR, &backdropType, sizeof(backdropType));
        
        borderPen = new Pen(Color(255, 0, 0), 5); // 红色边框，宽度 3
        Rect ellipseRect(x, y, iconSize, iconSize);
        graphics->DrawEllipse(borderPen, ellipseRect);

        // 绘制内部 X 符号
        Pen xPen(symbolBrush, 8);
        xPen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
        int offset = iconSize * 4 / 11;
        graphics->DrawLine(&xPen, x + offset, y + offset, x + iconSize - offset, y + iconSize - offset);
        graphics->DrawLine(&xPen, x + offset, y + iconSize - offset, x + iconSize - offset, y + offset);
        break;
    }
    case ICON_WARNING: {
        // 警告图标：黄色三角形边框
        UINT backdropType = RGB(255, 255, 0);
        DwmSetWindowAttribute(m_hWnd, DWMWA_BORDER_COLOR, &backdropType, sizeof(backdropType));
        DwmSetWindowAttribute(m_hWnd, DWMWA_CAPTION_COLOR, &backdropType, sizeof(backdropType));

        borderPen = new Pen(Color(255, 255, 0), 5); // 黄色边框，宽度 3
        PointF points[3] = {
            PointF(x + iconSize / 2, y),
            PointF(x, y + iconSize),
            PointF(x + iconSize, y + iconSize)
        };
        GraphicsPath path;
        path.AddPolygon(points, 3);
        graphics->DrawPath(borderPen, &path);

        // 绘制内部 ! 符号
        Pen exclamationPen(symbolBrush, 5);
        exclamationPen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
        int exclamationOffset = iconSize / 3;
        graphics->DrawLine(&exclamationPen, x + iconSize / 2, y + exclamationOffset + 6, x + iconSize / 2, y + iconSize - exclamationOffset);
        graphics->FillEllipse(symbolBrush, x + iconSize / 2 - 3, y + iconSize - exclamationOffset + 5, 6, 6);
        break;
    }
    case ICON_INFORMATION: {
        // 信息图标：蓝色圆形边框
        UINT backdropType = RGB(150, 150, 255);
        DwmSetWindowAttribute(m_hWnd, DWMWA_BORDER_COLOR, &backdropType, sizeof(backdropType));
        DwmSetWindowAttribute(m_hWnd, DWMWA_CAPTION_COLOR, &backdropType, sizeof(backdropType));

        borderPen = new Pen(Color(150, 150, 255), 5); // 蓝色边框，宽度 3
        Rect ellipseRect(x, y, iconSize, iconSize);
        graphics->DrawEllipse(borderPen, ellipseRect);

        // 绘制内部 i 符号
        Font font(L"Segoe UI", 30, FontStyleBold, UnitPixel);
        graphics->DrawString(L"i", 1, &font, PointF(x + iconSize / 2 - 8, y + iconSize / 2 - 17), symbolBrush);
        break;
    }
    case ICON_QUESTION: {
        // 疑问图标：蓝色圆形边框
        UINT backdropType = RGB(0, 255, 250);
        DwmSetWindowAttribute(m_hWnd, DWMWA_BORDER_COLOR, &backdropType, sizeof(backdropType));
        DwmSetWindowAttribute(m_hWnd, DWMWA_CAPTION_COLOR, &backdropType, sizeof(backdropType));

        borderPen = new Pen(Color(0, 255, 250), 5); // 蓝色边框，宽度 3
        Rect ellipseRect(x, y, iconSize, iconSize);
        graphics->DrawEllipse(borderPen, ellipseRect);

        // 绘制内部 ? 符号
        Font font(L"Segoe UI", 30, FontStyleBold, UnitPixel);
        graphics->DrawString(L"?", 1, &font, PointF(x + iconSize / 2 - 12, y + iconSize / 2 - 17), symbolBrush);
        break;
    }
    }

    // 释放资源
    if (borderPen) delete borderPen;
    if (symbolBrush) delete symbolBrush;
}

void WinPrompt::drawText(Graphics* graphics) {
    int x = (m_iconType != -1) ? (m_margin * 2 + m_iconSize) : m_margin;
    int y = m_margin;

    // 文本字体
    Font font(L"Segoe UI", FONT_SIZE, FontStyleRegular, UnitPoint);
    SolidBrush brush(Color(GetRValue(m_textColor), GetGValue(m_textColor), GetBValue(m_textColor)));

    // 设置文本格式自动换行
    StringFormat format;
    format.SetAlignment(StringAlignmentNear);
    format.SetLineAlignment(StringAlignmentNear);
    format.SetFormatFlags(StringFormatFlagsLineLimit | StringFormatFlagsNoClip);

    // 绘制文本
    RectF rect((REAL)x, (REAL)y, (REAL)m_textSize.cx, (REAL)m_textSize.cy);
    graphics->DrawString(m_message, -1, &font, rect, &format, &brush);
}

void WinPrompt::drawButtons(Graphics* graphics) {
    Font font(L"Segoe UI", FONT_SIZE, FontStyleRegular, UnitPoint);
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);

    for (int i = 0; i < m_buttonCount; i++) {
        // 根据按钮状态选择颜色
        Color fillColor;
        switch (m_buttons[i].state) {
        case STATE_HOVER:
            fillColor = Color(GetRValue(m_buttonHover), GetGValue(m_buttonHover), GetBValue(m_buttonHover));
            break;
        case STATE_PRESSED:
            fillColor = Color(GetRValue(m_buttonPressed), GetGValue(m_buttonPressed), GetBValue(m_buttonPressed));
            break;
        default:
            fillColor = Color(GetRValue(m_buttonNormal), GetGValue(m_buttonNormal), GetBValue(m_buttonNormal));
        }

        // 绘制按钮背景
        SolidBrush brush(fillColor);
        Rect rect(m_buttons[i].rect.left, m_buttons[i].rect.top,
            m_buttons[i].rect.right - m_buttons[i].rect.left,
            m_buttons[i].rect.bottom - m_buttons[i].rect.top);
        FillRoundedRectangle(graphics, &brush, rect, ROUND_RADIUS);

        // 绘制按钮边框
        Pen pen(Color(GetRValue(m_buttonBorder), GetGValue(m_buttonBorder), GetBValue(m_buttonBorder)));
        DrawRoundedRectangle(graphics, &pen, rect, ROUND_RADIUS);

        // 绘制按钮文本
        SolidBrush textBrush(Color(GetRValue(m_textColor), GetGValue(m_textColor), GetBValue(m_textColor)));
        graphics->DrawString(m_buttons[i].text, -1, &font,
            RectF((REAL)m_buttons[i].rect.left, (REAL)m_buttons[i].rect.top,
                (REAL)(m_buttons[i].rect.right - m_buttons[i].rect.left),
                (REAL)(m_buttons[i].rect.bottom - m_buttons[i].rect.top)),
            &format, &textBrush);
    }
}

void WinPrompt::drawWindow(Graphics* graphics) {
    // 绘制背景
    SolidBrush brush(Color(GetRValue(m_bgColor), GetGValue(m_bgColor), GetBValue(m_bgColor)));
    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    graphics->FillRectangle(&brush,
        (REAL)clientRect.left,
        (REAL)clientRect.top,
        (REAL)(clientRect.right - clientRect.left),
        (REAL)(clientRect.bottom - clientRect.top));

    // 绘制图标
    if (m_iconType != -1) {
        drawIcon(graphics);
    }

    // 绘制文本
    drawText(graphics);

    // 绘制按钮
    drawButtons(graphics);
}

int WinPrompt::hitTestButtons(int x, int y) {
    for (int i = 0; i < m_buttonCount; i++) {
        if (PtInRect(&m_buttons[i].rect, POINT(x, y))) {
            return i;
        }
    }
    return -1;
}

void WinPrompt::updateButtonState(int x, int y, bool isPressed) {
    int hoverIndex = hitTestButtons(x, y);
    bool stateChanged = false;

    for (int i = 0; i < m_buttonCount; i++) {
        ButtonState newState;
        if (i == hoverIndex) {
            newState = isPressed ? STATE_PRESSED : STATE_HOVER;
        }
        else {
            newState = STATE_NORMAL;
        }

        if (m_buttons[i].state != newState) {
            m_buttons[i].state = newState;
            stateChanged = true;
        }
    }

    // 只有状态改变时才重绘
    if (stateChanged) {
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

int WinPrompt::runMessageLoop() {
    MSG msg;

    // 使用PeekMessage替代GetMessage确保正确响应所有消息
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return m_selectedResult - 1000;
}

LRESULT CALLBACK WinPrompt::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WinPrompt* pThis = NULL;

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (WinPrompt*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->m_hWnd = hWnd;
    }
    else {
        pThis = (WinPrompt*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (pThis) {
        return pThis->handleMessage(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WinPrompt::handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        calculateTextSize();
        createButtons();
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 创建内存DC和位图
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBmp = CreateCompatibleBitmap(hdc, ps.rcPaint.right, ps.rcPaint.bottom);
        HGDIOBJ oldObj = SelectObject(memDC, memBmp);

        // 在内存DC上绘制
        Graphics graphics(memDC);
        graphics.SetSmoothingMode(SmoothingModeAntiAlias);
        drawWindow(&graphics);

        // 复制到屏幕DC
        BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, memDC, 0, 0, SRCCOPY);

        // 清理资源
        SelectObject(memDC, oldObj);
        DeleteObject(memBmp);
        DeleteDC(memDC);
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_MOUSEMOVE: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        updateButtonState(x, y, (wParam & MK_LBUTTON) != 0);
        return 0;
    }

    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        updateButtonState(x, y, true);
        SetCapture(hWnd);
        return 0;
    }

    case WM_LBUTTONUP: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        int buttonIndex = hitTestButtons(x, y);

        updateButtonState(x, y, false);
        ReleaseCapture();

        if (buttonIndex != -1) {
            // 记录选择的结果并关闭窗口
            m_selectedResult = m_buttons[buttonIndex].id;
            DestroyWindow(hWnd); // 关闭窗口会使消息循环退出
        }
        return 0;
    }

    case WM_LBUTTONDBLCLK: {
        return 0;
    }

    case WM_KEYDOWN: {
        // 处理键盘事件
        if (wParam == VK_ESCAPE) {
            // ESC模拟取消
            if (m_buttonType == BTN_OKCANCEL || m_buttonType == BTN_YESNOCANCEL) {
                SendMessage(hWnd, WM_COMMAND, ID_CANCEL, 0);
            }
        }
        else if (wParam == VK_RETURN) {
            // Enter模拟确定/是
            if (m_buttonType == BTN_OK || m_buttonType == BTN_OKCANCEL) {
                SendMessage(hWnd, WM_COMMAND, ID_OK, 0);
            }
            else if (m_buttonType == BTN_YESNO || m_buttonType == BTN_YESNOCANCEL) {
                SendMessage(hWnd, WM_COMMAND, ID_YES, 0);
            }
        }
        return 0;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

int WinPrompt::show(const TCHAR* title, const TCHAR* message,
    IconType icon, ButtonType buttons, WindowMode mode) {
    m_title = title;
    m_message = message;
    m_iconType = icon;
    m_buttonType = buttons;
    m_mode = mode;

    // 根据图标类型播放系统声音
    switch (m_iconType) {
    case ICON_INFORMATION:
        MessageBeep(SND_INFO);
        break;
    case ICON_WARNING:
        MessageBeep(SND_WARNING);
        break;
    case ICON_ERROR:
        MessageBeep(SND_ERROR);
        break;
    case ICON_QUESTION:
        MessageBeep(SND_QUESTION);
        break;
    }

    // 注册窗口类
    // 注册窗口类
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = m_hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    std::wstring number;

    int overNumber = 0;

    for(int k = 0;;k++)
    {
        bool isOK = false;
        if(!ofstr.empty())
        {
            for (int i = 0; i < ofstr.size(); i++)
            {
                if (ofstr[i] != k)
                {
                    isOK = true;
                    break;
                }
            }
        }
        else
        {
            isOK = true;
        }

        if(isOK)
        {
            number = std::to_wstring(k);
            ofstr.push_back(k);
            overNumber = k;
            break;
        }
    }

    std::wstring className = L"NetTrackInf_" + number;

    wc.lpszClassName = className.c_str();

    if (!RegisterClassEx(&wc)) {
        MessageBoxW(NULL, _T("error code: 45"), _T("error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // 创建窗口
    m_hWnd = CreateWindowExW(
        WS_EX_TOPMOST,
        className.c_str(),
        m_title,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        m_hParent,
        NULL,
        m_hInstance,
        this
    );

    if (m_hWnd == NULL) {
        MessageBoxW(NULL, _T("error code: 46"), _T("error"), MB_ICONEXCLAMATION | MB_OK);
        UnregisterClass(className.c_str(), m_hInstance);
        return 0;
    }

    // 计算文本大小并调整位置
    calculateTextSize();
    positionWindow();
    createButtons();
    setTitleBarColor();

    // 显示窗口
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    // 运行消息循环
    int result = runMessageLoop();

    for(int i=0;i< ofstr.size();i++)
    {
        if (overNumber == ofstr[i])
        {
            ofstr.erase(ofstr.begin() + i);
            break;
        }
    }

    // 清理
    UnregisterClass(className.c_str(), m_hInstance);
    return result;
}