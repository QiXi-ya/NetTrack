#include "Main.h"
#include "html.h"
#include "Tool.h"
#include <windows.h>
#include <string>
#include <thread>
#include <regex>
#include <filesystem>
#include <fstream>
#include <codecvt>
#include <locale>
#include "resource.h"
#define IDI_ICON1 101

using namespace std;

#define WM_HTML_LOADED (WM_USER + 1)// 动画和渲染状态

int lg = 0;

enum class HtmlViewerState { Loading, Fading, ShowHtml }; // 新增Fading状态
struct HtmlLine {
	wstring text;
	enum Type { H1, H3, P, HR, UNKNOWN };
	Type type;
	COLORREF color;
	int align; // 0=left, 1=center, 2=right
	bool bold;
	HtmlLine(const wstring& t, int ty, COLORREF c = RGB(40, 40, 40), int a = 0, bool b = false)
		: text(t), type(static_cast<Type>(ty)), color(c), align(a), bold(b) {
	}
}; 

class HtmlViewerImpl {
public:
	HtmlViewerImpl() = default; // 显式默认构造函数
    static HtmlViewerImpl& GetInstance() {
        static HtmlViewerImpl instance;
        return instance;
    }
    void SetLg(int lg) { m_lg = lg; }
	int LoadHtmlFromResource(HINSTANCE hInstance, const wchar_t* filePath, const wstring& title) {
		return ShowHtmlWindow(hInstance, filePath, title);
	}HtmlViewerState GetViewerState() const { return m_viewerState; }
	void SetViewerState(HtmlViewerState state) { m_viewerState = state; }
	bool IsHtmlLoaded() const { return m_htmlLoaded; }
	void SetHtmlLoaded(bool loaded) { m_htmlLoaded = loaded; }
	const vector<HtmlLine>& GetLines() const { return m_lines; }
private:
    int m_lg = 0;
    HtmlViewerState m_viewerState = HtmlViewerState::Loading;
    atomic<bool> m_htmlLoaded{false};
    vector<HtmlLine> m_lines;
    HWND m_hwnd = nullptr;
    int m_animationAlpha = 255;
    int m_scrollOffset = 0;
    int m_scrollTarget = 0;
    int m_contentHeight = 0;
    COLORREF m_autoTextColor = RGB(40, 40, 40);
    COLORREF m_autoBgColor = RGB(255, 255, 255);
    atomic<bool> m_scrolling{false};
    int m_loadingAngle = 0;
    int m_loadingSweep = 60;
    bool m_loadingGrow = false;
    int m_fadeAlpha = 0; // 渐变用

public:
    static void DrawHtmlContent(HtmlViewerImpl& impl, HDC memDC, int w, int h) {
        int y = 30 - impl.m_scrollOffset;
        int contentHeight = 0;
        for (const auto& line : impl.GetLines()) {
            int fontSize = 22;
            int weight = line.bold ? FW_BOLD : FW_NORMAL;
            switch (line.type) {
            case HtmlLine::H1: fontSize = 48; break;
            case HtmlLine::H3: fontSize = 32; break;
            case HtmlLine::P:  fontSize = 24; break;
            default: break;
            }
            const wchar_t* fontName = (impl.m_lg == 3) ? L"Ownglyph Soomini Regular" : L"Aa吧唧一口小可耐";
            HFONT hFont = CreateFontW(fontSize, 0, 0, 0, weight, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, fontName);
            auto oldFont = SelectObject(memDC, hFont);
            SetTextColor(memDC, line.color);
            SetBkMode(memDC, TRANSPARENT);
            if (line.type == HtmlLine::HR) {
                HPEN hrPen = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
                auto oldPen = SelectObject(memDC, hrPen);
                MoveToEx(memDC, 50, y, nullptr);
                LineTo(memDC, w - 50, y);
                SelectObject(memDC, oldPen);
                DeleteObject(hrPen);
                y += 20;
                contentHeight += 20;
            } else {
                RECT textRect;
                int margin = 50;
                textRect.left = margin;
                textRect.right = w - margin;
                textRect.top = y;
                textRect.bottom = y + 10000;
                UINT format = DT_WORDBREAK | DT_TOP;
                if (line.align == 1) format |= DT_CENTER;
                else if (line.align == 2) format |= DT_RIGHT;
                else format |= DT_LEFT;
                RECT calcRect = textRect;
                DrawTextW(memDC, line.text.c_str(), (int)line.text.size(), &calcRect, format | DT_CALCRECT);
                DrawTextW(memDC, line.text.c_str(), (int)line.text.size(), &textRect, format);
                int lineHeight = (calcRect.bottom - calcRect.top) + 12;
                y += lineHeight;
                contentHeight += lineHeight;
            }
            SelectObject(memDC, oldFont);
            DeleteObject(hFont);
        }
        impl.m_contentHeight = contentHeight;
    }
	
	~HtmlViewerImpl() = default;
	HtmlViewerImpl(const HtmlViewerImpl&) = delete;

	HtmlViewerImpl& operator=(const HtmlViewerImpl&) = delete;
	
	COLORREF ParseColor(const wstring& style) {
		wregex colorRe(LR"(color\s*:\s*rgb\((\d+),\s*(\d+),\s*(\d+)\))");
		wsmatch m;
		if (regex_search(style, m, colorRe)) {
			int r = stoi(m[1].str());
			int g = stoi(m[2].str());
			int b = stoi(m[3].str());
			return RGB(clamp(r, 0, 255), clamp(g, 0, 255), clamp(b, 0, 255));
		}
		// 没有专门设置颜色的字，深色模式下为白色，浅色为深灰
		return IsDarkModeEnabled() ? RGB(255, 255, 255) : RGB(40, 40, 40);
	}
	
	int ParseAlign(const wstring& style) {
		wregex alignRe(LR"(text-align\s*:\s*(\w+))");
		wsmatch m;
		if (regex_search(style, m, alignRe)) {
			wstring align = m[1].str();
			if (align == L"center") return 1;
			if (align == L"right") return 2;
		}
		return 0;
	}
	
	vector<HtmlLine> ParseSimpleHtml(const wstring& html) {
        vector<HtmlLine> lines;
        // 改进正则表达式，确保能匹配 p 标签，允许标签内有空格和其他属性
		wregex tagRe(LR"(<(h1|h3|p)(\s+[^>]*)?>(.*?)</\1\s*>)", regex::icase);
        wregex hrRe(LR"(<hr\s*/?>)", regex::icase);
        wregex brRe(LR"(<br\s*/?>)", regex::icase);
        wregex spaceRe(LR"(\s+)");
        wregex tagCleanRe(LR"(<[^>]*>)"); // 清除残留标签
        auto searchStart = html.cbegin(); // 修正 searchStart 声明
        wsmatch m;
        while (searchStart != html.cend()) {
            if (regex_search(searchStart, html.cend(), m, tagRe)) {
                wstring tag = m[1].str();
                transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
                wstring style = m[2].str();
                wstring text = m[3].str();
                // 处理换行和空格
                text = regex_replace(text, brRe, L"\n");
                text = regex_replace(text, tagCleanRe, L""); // 清除文本中的其他标签
                text = regex_replace(text, spaceRe, L" ");
                trim(text);
                if (text.empty() && tag != L"p") {
                    searchStart = m.suffix().first;
                    continue;
                }
                COLORREF color = ParseColor(style);
                int align = ParseAlign(style);
                bool bold = (tag == L"h1" || tag == L"h3");
                int type = HtmlLine::UNKNOWN;
                if (tag == L"h1") type = HtmlLine::H1;
                else if (tag == L"h3") type = HtmlLine::H3;
                else if (tag == L"p") type = HtmlLine::P;
                lines.emplace_back(text, type, color, align, bold);
                searchStart = m.suffix().first;
            }
            else if (regex_search(searchStart, html.cend(), m, hrRe)) {
                lines.emplace_back(L"", HtmlLine::HR);
                searchStart = m.suffix().first;
            }
            else {
                break;
            }
        }
        return lines;
    }
	
	static bool IsDarkModeEnabled() {
		DWORD value = 0;
		DWORD valueSize = sizeof(value);
		HKEY hKey;

		// 打开注册表项
		if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
			// 读取AppsUseLightTheme值
			if (RegGetValue(hKey, NULL, L"AppsUseLightTheme", RRF_RT_REG_DWORD, NULL, &value, &valueSize) == ERROR_SUCCESS) {
				RegCloseKey(hKey);
				return value == 0; // 0表示深色模式,1表示浅色模式
			}
			RegCloseKey(hKey);
		}

		return false; // 默认返回浅色模式
	}

	void ShowLoadingAnimation(HWND hwnd) {
		while (!m_htmlLoaded) {
			m_loadingAngle -= 8;
			if (m_loadingGrow) {
				if (m_loadingSweep < 270) m_loadingSweep += 2; else { m_loadingSweep = 270; m_loadingGrow = false; }
			} else {
				if (m_loadingSweep > 10) m_loadingSweep -= 2; else { m_loadingSweep = 10; m_loadingGrow = true; }
			}
			InvalidateRect(hwnd, nullptr, FALSE);
			Sleep(16);
		}
		m_viewerState = HtmlViewerState::Fading;
		m_fadeAlpha = 0;
		InvalidateRect(hwnd, nullptr, TRUE);
		thread([this, hwnd]{
			for (int a = 0; a <= 255; a += 16) {
				m_fadeAlpha = a;
				InvalidateRect(hwnd, nullptr, FALSE);
				Sleep(16);
			}
			m_viewerState = HtmlViewerState::ShowHtml;
			InvalidateRect(hwnd, nullptr, TRUE);
			PostMessage(hwnd, WM_HTML_LOADED, 0, 0);
		}).detach();
	}
	
	void DrawLoadingAnimation(HDC memDC, int w, int h) {
		RECT rc = {0, 0, w, h};
		bool darkMode = IsDarkModeEnabled();
		COLORREF bgColor = m_autoBgColor;
		COLORREF ringColor = RGB(155, 255, 250);
		COLORREF textColor = darkMode ? RGB(255, 255, 255) : RGB(40, 40, 40);
		HBRUSH hBrush = CreateSolidBrush(bgColor);
		FillRect(memDC, &rc, hBrush);
		DeleteObject(hBrush);
		int radius = min(w, h) / 4;
		int thickness = max(8, radius / 5);
		int cx = w / 2;
		int cy = h / 2 - radius / 2;
		double startAngle = (m_loadingAngle % 360) * 3.14159265 / 180.0;
		double endAngle = ((m_loadingAngle + m_loadingSweep) % 360) * 3.14159265 / 180.0;
		for (int i = 0; i < thickness; ++i) {
			int r = radius - i;
			int x1 = cx + int(r * cos(startAngle));
			int y1 = cy - int(r * sin(startAngle));
			int x2 = cx + int(r * cos(endAngle));
			int y2 = cy - int(r * sin(endAngle));
			HPEN hPen = CreatePen(PS_SOLID, 1, ringColor);
			HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			SelectObject(memDC, hPen);
			SelectObject(memDC, hNullBrush);
			Arc(memDC, cx - r, cy - r, cx + r, cy + r, x1, y1, x2, y2);
			DeleteObject(hPen);
		}
		// 画圆头
		int headR = thickness / 2;
		int r = radius - thickness + 1;
		int headX1 = cx + int(r * cos(startAngle)) * 10 / 9;
		int headY1 = cy - int(r * sin(startAngle)) * 10 / 9;
		int headX2 = cx + int(r * cos(endAngle)) * 10 / 9;
		int headY2 = cy - int(r * sin(endAngle)) * 10 / 9;
		HBRUSH headBrush = CreateSolidBrush(ringColor);
		SelectObject(memDC, headBrush);
		Ellipse(memDC, headX1 - headR, headY1 - headR, headX1 + headR, headY1 + headR);
		Ellipse(memDC, headX2 - headR, headY2 - headR, headX2 + headR, headY2 + headR);
		DeleteObject(headBrush);
		// 文字
		const wchar_t* loadingTexts[] = {
			L"正在加载...", L"Loading...", L"読み込み中...", L"로딩 중...", L"Chargement...", L"Wird geladen...", L"Загрузка...", L"Cargando..."
		};
		int langIdx = 0;
		try { langIdx = lg; } catch (...) { langIdx = 0; }
		if (langIdx < 0 || langIdx >= 8) langIdx = 0;
		const wchar_t* loadingText = loadingTexts[langIdx];
		int fontSize = max(24, radius / 3);
		HFONT hFont = CreateFontW(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
			(langIdx == 3) ? L"Ownglyph Soomini Regular" : L"Aa吧唧一口小可耐");
		SelectObject(memDC, hFont);
		SetBkMode(memDC, TRANSPARENT);
		SetTextColor(memDC, textColor);
		SIZE sz;
		GetTextExtentPoint32W(memDC, loadingText, lstrlenW(loadingText), &sz);
		TextOutW(memDC, w / 2 - sz.cx / 2, cy + radius + thickness + 20, loadingText, lstrlenW(loadingText));
		DeleteObject(hFont);
	}

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		auto& impl = GetInstance();
		// 鼠标样式
		static HCURSOR loadingCursor = LoadCursor(NULL, IDC_WAIT);
		static HCURSOR arrowCursor = LoadCursor(NULL, IDC_ARROW);
		switch (msg) {
		case WM_CREATE:
			impl.m_hwnd = hwnd;
			return 0;
		case WM_MOUSEMOVE:
			// 加载动画期间更改鼠标样式
			if (impl.GetViewerState() == HtmlViewerState::Loading) {
				SetCursor(loadingCursor);
			} else {
				SetCursor(arrowCursor);
			}
			// 可在此处处理鼠标移动信息（如显示坐标、交互等）
			return 0;
		case WM_KEYDOWN:
			// 加载动画期间翻译键盘信息
			if (impl.GetViewerState() == HtmlViewerState::Loading) {
				// 可根据需要处理键盘事件，如ESC关闭窗口
				if (wParam == VK_ESCAPE) {
					DestroyWindow(hwnd);
					return 0;
				}
			}
			return 0;
		case WM_MOUSEWHEEL: {
			if (impl.GetViewerState() != HtmlViewerState::Loading && IsWindow(impl.m_hwnd)) {
				int delta = GET_WHEEL_DELTA_WPARAM(wParam);
				int step = 80;
				impl.m_scrollTarget -= (delta / WHEEL_DELTA) * step;
				RECT rc; GetClientRect(hwnd, &rc);
				int maxScroll = max(0, impl.m_contentHeight - (rc.bottom - rc.top) + 30);
				if (impl.m_scrollTarget < 0) impl.m_scrollTarget = 0;
				if (impl.m_scrollTarget > maxScroll) impl.m_scrollTarget = maxScroll;
				if (!impl.m_scrolling) {
					impl.m_scrolling = true;
					thread([hwnd, &impl] {
						while (abs(impl.m_scrollOffset - impl.m_scrollTarget) > 1 && IsWindow(impl.m_hwnd)) {
							int diff = impl.m_scrollTarget - impl.m_scrollOffset;
							impl.m_scrollOffset += (int)round(diff * 0.2); // ease out
							InvalidateRect(hwnd, nullptr, FALSE);
							Sleep(16);
						}
						impl.m_scrollOffset = impl.m_scrollTarget;
						if(IsWindow(impl.m_hwnd)) InvalidateRect(hwnd, nullptr, FALSE);
						impl.m_scrolling = false;
					}).detach();
				}
			}
			return 0;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			RECT rc;
			GetClientRect(hwnd, &rc);
			int w = rc.right - rc.left;
			int h = rc.bottom - rc.top;
			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP memBmp = CreateCompatibleBitmap(hdc, w, h);
			HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, memBmp);
			HBRUSH bgBrush = CreateSolidBrush(impl.m_autoBgColor);
			FillRect(memDC, &rc, bgBrush);
			DeleteObject(bgBrush);
			if (impl.GetViewerState() == HtmlViewerState::ShowHtml) {
				HtmlViewerImpl::DrawHtmlContent(impl, memDC, w, h);
			} else if (impl.GetViewerState() == HtmlViewerState::Loading) {
				impl.DrawLoadingAnimation(memDC, w, h);
			} else if (impl.GetViewerState() == HtmlViewerState::Fading) {
				HDC animDC = CreateCompatibleDC(hdc);
				HBITMAP animBmp = CreateCompatibleBitmap(hdc, w, h);
				HBITMAP oldAnimBmp = (HBITMAP)SelectObject(animDC, animBmp);
				HBRUSH bgBrush2 = CreateSolidBrush(impl.m_autoBgColor);
				FillRect(animDC, &rc, bgBrush2);
				DeleteObject(bgBrush2);
				impl.DrawLoadingAnimation(animDC, w, h);
				HDC htmlDC = CreateCompatibleDC(hdc);
				HBITMAP htmlBmp = CreateCompatibleBitmap(hdc, w, h);
				HBITMAP oldHtmlBmp = (HBITMAP)SelectObject(htmlDC, htmlBmp);
				HBRUSH bgBrush3 = CreateSolidBrush(impl.m_autoBgColor);
				FillRect(htmlDC, &rc, bgBrush3);
				DeleteObject(bgBrush3);
				HtmlViewerImpl::DrawHtmlContent(impl, htmlDC, w, h);
				BitBlt(memDC, 0, 0, w, h, animDC, 0, 0, SRCCOPY);
				BLENDFUNCTION blend = { AC_SRC_OVER, 0, (BYTE)impl.m_fadeAlpha, 0 };
				AlphaBlend(memDC, 0, 0, w, h, htmlDC, 0, 0, w, h, blend);
				SelectObject(animDC, oldAnimBmp);
				DeleteObject(animBmp);
				DeleteDC(animDC);
				SelectObject(htmlDC, oldHtmlBmp);
				DeleteObject(htmlBmp);
				DeleteDC(htmlDC);
			}
			BitBlt(hdc, 0, 0, w, h, memDC, 0, 0, SRCCOPY);
			SelectObject(memDC, oldBmp);
			DeleteObject(memBmp);
			DeleteDC(memDC);
			EndPaint(hwnd, &ps);
			return 0;
		}
		case WM_SIZE:
			if(IsWindow(impl.m_hwnd)) InvalidateRect(hwnd, nullptr, TRUE);
			return 0;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			impl.m_hwnd = NULL;
			UnregisterClass(L"EnhancedHtmlWnd", GetModuleHandle(NULL));
			return 0;
		case WM_HTML_LOADED:
			if (IsWindow(impl.m_hwnd)) InvalidateRect(hwnd, nullptr, TRUE);
			return 0;
		}

		if (GetForegroundWindow() != hwnd && IsWindow(hwnd))
		{
			MessageBeep(MB_OK);
			tool::FlashAndActivateWindow(hwnd, 20, 250);
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	
	int ShowHtmlWindow(HINSTANCE hInstance, const wchar_t* filePath, const wstring& title) {
    // 读取并解析 HTML 文件
    wifstream fin(filePath);
    fin.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    wstring html((istreambuf_iterator<wchar_t>(fin)), istreambuf_iterator<wchar_t>());
    m_lines = ParseSimpleHtml(html);
    // 自动检测深浅色
    bool darkMode = IsDarkModeEnabled();
    m_autoBgColor = darkMode ? RGB(30, 30, 30) : RGB(255, 255, 255);
    m_autoTextColor = darkMode ? RGB(255, 255, 255) : RGB(40, 40, 40);
    // 获取屏幕尺寸，设置窗口为2/3屏幕
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    int winW = sw * 2 / 3;
    int winH = sh * 2 / 3;
    int winX = (sw - winW) / 2;
    int winY = (sh - winH) / 2;
    // 注册窗口类
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"EnhancedHtmlWnd";
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));  // 大图标
    RegisterClass(&wc);
    // 创建窗口
    m_hwnd = CreateWindowExW(0, wc.lpszClassName, title.c_str(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        winX, winY, winW, winH,
        nullptr, nullptr, hInstance, nullptr);
    if (!m_hwnd) return 0;
    // 重置动画状态并启动加载动画线程
    m_htmlLoaded = false;
    m_viewerState = HtmlViewerState::Loading;
    m_animationAlpha = 0; // 渐变初始为0
    thread loadingThread(&HtmlViewerImpl::ShowLoadingAnimation, this, m_hwnd);
    loadingThread.detach();
    // 模拟加载过程
    thread([this]{
        // 渐变切换到HTML
        for (int a = 255; a >= 0; a -= 16) {
            m_animationAlpha = a;
            InvalidateRect(m_hwnd, nullptr, FALSE);
            api_sleep(16);
        }
        m_htmlLoaded = true;
        for (int a = 0; a <= 255; a += 16) {
            m_animationAlpha = a;
            InvalidateRect(m_hwnd, nullptr, FALSE);
			api_sleep(16);
        }
    }).detach();
    // 消息循环
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    m_hwnd = nullptr;
    return (int)msg.wParam;
};// 辅助函数：修剪字符串前后空格

	static void trim(wstring& s) {
		s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
			return !isspace(ch);
			}));
		s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
			return !isspace(ch);
			}).base(), s.end());
	}// 辅助函数：数值范围限制
	template <typename T>
	static T clamp(T val, T min, T max) {
		return (val < min) ? min : (val > max) ? max : val;
	}
};

// 保留原有接口
void HtmlViewer::setLg(int lgA) {
	HtmlViewerImpl::GetInstance().SetLg(lgA);
}

int HtmlViewer::LoadHtmlFromResource(const wchar_t* filePath, const wstring& title) {
	return HtmlViewerImpl::GetInstance().LoadHtmlFromResource(
		GetModuleHandle(NULL), filePath, title);
}