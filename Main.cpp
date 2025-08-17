#include "Main.h"

PIMAGE backImage = newimage();
PIMAGE backImage_i = newimage();

HINSTANCE hInstanceTop = nullptr;
HWND mainWindow = nullptr;
HWND mainHwnd = nullptr;
POINT mousePos = {0};
POINT windowSize = {0};
POINT screen = {0};

std::string saveData_link[saveMax] = {};
std::string userset[saveConfigMax] = {};
std::string lastUpload[lastUploadMax] = {};
std::string lastDownload[lastDownloadMax];

std::vector<std::string> trustList;
std::vector<std::string> noList;

bool DarkMode = false;
int BKcolor = 0;


//===================================================================

bool debug = false;

//===================================================================

UINT WM_TASKBARCREATED = 0;

bool noChange = false;
HWND subWindow = NULL;
int subAlpha = 0;

HHOOK g_mouseHook = NULL;
bool max_EGE = false;
RECT lastPos = {0};
WNDPROC g_EGE_OldWndProc = nullptr;
bool resizing = false;
RECT resizeRect = {0};
bool a = false;
int GS = 0;
int temp_GS = 0;
bool rs = false;

// sendFileList结构体类型
std::vector<SendFileInfo> sendFileList;

int whle_temp = 0;
int whle = 0;
