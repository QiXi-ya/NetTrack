#define  _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <filesystem>
#include <locale>
#include <codecvt>
#include <cstddef>


#include "Main.h"
#include "message.h"
#include "html.h"
#include "code.h"
#include "msg.h"
#include "WindowsEv.h"
#include "ease.h"
#include "GraphManager.h"
#include "trd.h"
#include "WPOC.h"
#include "FileCheck.h"
#include "FileControl.h"
#include "FirstStart.h"
#include "img.h"
#include "NetCode.h"
#include "Tool.h"
#include "AES.h"
#include "Net.h"
#include "portget.h"
#include <wininet.h>

#define IDI_ICON1 101

//#pragma comment(linker, "/subsystem:console")


#pragma region Data


int linkOS = -1;//正在连接的链接
bool loadASlink = false;
int loadAlink = 0;
int roundAlink = 0;
#pragma endregion

/// <summary>
/// 网络传输类
/// </summary>

//本地ip_string
string ip_local;
unsigned short port;

//8位字符串
string OMSR = "ERROR";

/// <summary>
/// 彩色console输出类
/// </summary>
class colorPrintf
{
public:
    // 启用虚拟终端处理
    void EnableVTMode() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    // 输出带有自定义RGB颜色的文字
    void PrintColorText(const string& text, int r, int g, int b, int IsReset) {
        // \x1b[38;2;<r>;<g>;<b>m 设置前景色
        cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m"
            << text;
        if (IsReset) cout << "\x1b[0m"; // 恢复默认颜色
    }
};






color_t mainColor = RGB(155, 255, 200); // 主颜色


// ========== 绘图函数 ==========

PIMAGE s_temp = newimage();

//是否待选框
bool isMessage = false;

//侧边栏alpha
int AHalpha[3] = { 0,0,0 };

//用户储存文件夹，默认为"下载"
wstring userSaveDir = L"NULL";





//地址栏
    //侧边栏厚度
const int SizeBar = 4;
//地址栏圆角大小
const int BarCircleSize = 6;
//地址栏厚度
const int BarSize = 200;

const int wordHigh = 70;
bool isBar = false;
int BarX = 0;
int barXSX = 0;
int userBarChoose = 0;

int lineLong = 10;
int lineLong_frame = 0;

int wordCS[6] = { 0,0,0,0,0,0 };
int wordSZT[6] = { -5,-5,-5,-5,-5,-5 };
int CBOS = 0;

//=======鼠标点击特效==========
int mouseOSN = 0;
POINT mouseOS = { 0,0 };





/// <summary>
/// 消息类
/// </summary>

int clock_math = 0; int cx_math = 0;

class ini
{
private:
    // 写入存档
    static void SaveIni() {
        vector<string> configData;
        for (int i = 0; i < saveConfigMax; i++) {
            configData.push_back(userset[i].empty() ? "0" : userset[i]);
        }
        SecureFileStorage::SaveSecureData(configData, L"config.ini");
    }

    // 保存连接记录
    static void SaveLinkData() {
        vector<string> linkData;
        for (int i = 0; i < saveMax; i++) {
            linkData.push_back(saveData_link[i].empty() ? "NULL" : saveData_link[i]);
        }
        SecureFileStorage::SaveSecureData(linkData, L"config.dll");
    }

    // 保存上传记录
    static void SaveUploadRecord() {
        vector<string> uploadData;
        for (int i = 0; i < lastUploadMax; i++) {
            uploadData.push_back(lastUpload[i].empty() ? "NULL" : lastUpload[i]);
        }
        SecureFileStorage::SaveSecureData(uploadData, L"Record_upload.dll");
    }

    // 保存下载记录
    static void SaveDownloadRecord() {
        vector<string> downloadData;
        for (int i = 0; i < lastDownloadMax; i++) {
            downloadData.push_back(lastDownload[i].empty() ? "NULL" : lastDownload[i]);
        }
        SecureFileStorage::SaveSecureData(downloadData, L"Record_download.dll");
    }

    static void saveTrustList()
    {
        if (trustList.empty())
        {
			trustList.push_back("NULL");
        }
        SecureFileStorage::SaveSecureData(trustList, L"ts.dll");
    }

public:
    static void SaveData()
    {
        SaveIni();
        SaveLinkData();
        SaveUploadRecord();
        SaveDownloadRecord();
        saveTrustList();
    }
};


int MacAdd = 0;
int MacAdd_frame = 60;
int totalMacAdd_frame = 60;
bool MacAddChange = false;

int linkMax = 0;

int MacAddFile = 27;
int MacAddFile_frame = 60;
int totalMacAddFile_frame = 60;
bool MacAddFileChange = true;
int MacAddFileRectAlpha = 0;

bool AddLink = false;

int AddLinkXChange = 0;
int AddLink_frame = 60;
int AddLink_totalFrame = 60;
bool isKEY = true;
bool isKEY_0x08 = true;
bool isKEY_0x0d = true;

int LKA = -1;
int linkX[saveMax] = {};
int linkX_frame[saveMax] = {};

char linkCodeA[9] = { '?','?', '?', '?', '?', '?', '?', '?','\0' };
char linkCode_tempA[9] = { '?','?', '?', '?', '?', '?', '?', '?','\0' };
int userLink_fontFrameA[8] = { 60,60,60,60,60,60,60,60 };

bool chooseFile = false;


RECT lastRect = { 0,0,0,0 };

int choose2Choose = 0;
int choose2ChooseX = 0;
int choose2Line = 40;

int choose2Choose_frame = 60;
int C_temp = 0;

bool isSendFileQuickSee = false;
int isSendFileQuickSee_frame = 60;
int isSendFileQuickSeeX = 0;



class msgBox
{

    static string GbkToUtf8(const string& gbkStr) {
        // 先将GBK转换为宽字符（Unicode）
        int wideCharLen = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, nullptr, 0);
        if (wideCharLen == 0) {
            return ""; // 转换失败
        }

        wchar_t* wideCharBuf = new wchar_t[wideCharLen];
        MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, wideCharBuf, wideCharLen);

        // 再将宽字符转换为UTF8
        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideCharBuf, -1, nullptr, 0, nullptr, nullptr);
        if (utf8Len == 0) {
            delete[] wideCharBuf;
            return ""; // 转换失败
        }

        char* utf8Buf = new char[utf8Len];
        WideCharToMultiByte(CP_UTF8, 0, wideCharBuf, -1, utf8Buf, utf8Len, nullptr, nullptr);

        string utf8Str(utf8Buf);

        // 释放内存
        delete[] wideCharBuf;
        delete[] utf8Buf;

        return utf8Str;
    }
    // 将string和const char*转换为TCHAR*的辅助函数
    static LPCTSTR toTCHAR(const string& str)
    {
        int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
        if (length == 0) return _T("");

        TCHAR* buffer = new TCHAR[length];
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, length);
        return buffer;
    }
public:
    static int msgbox(HINSTANCE hInstance, string title, string text,
        WinPrompt::IconType iconType, WinPrompt::ButtonType buttonType,
        WinPrompt::WindowMode mode = WinPrompt::MODE_DEFAULT)
    {
        int k = stoi(userset[0]);
        switch(k)
        {
        case 0:
            WinPrompt::setLanguage(WinPrompt::LANG_C); break;
        case 1:
            WinPrompt::setLanguage(WinPrompt::LANG_E); break;
        case 2:
            WinPrompt::setLanguage(WinPrompt::LANG_J); break;
        case 3:
            WinPrompt::setLanguage(WinPrompt::LANG_K); break;
        case 4:
            WinPrompt::setLanguage(WinPrompt::LANG_F); break;
        case 5:
            WinPrompt::setLanguage(WinPrompt::LANG_G); break;
        case 6:
            WinPrompt::setLanguage(WinPrompt::LANG_R); break;
        case 7:
            WinPrompt::setLanguage(WinPrompt::LANG_S); break;
        default:
            WinPrompt::setLanguage(WinPrompt::LANG_E); break;
        }

        title = GbkToUtf8(title);
        text = GbkToUtf8(text);
        // 转换字符串为宽字符
        LPCTSTR tTitle = toTCHAR(title);
        LPCTSTR tText = toTCHAR(text);

        // 创建提示窗口实例
        WinPrompt prompt(hInstance);

        // 显示提示窗口
        int result = prompt.show(
            tTitle,
            tText,
            iconType,
            buttonType,
            mode
        );

        // 释放转换后的字符串内存
        delete[] tTitle;
        delete[] tText;

        return result;
    }
};



class Debug
{
public:
    static void debugInf(string debugText, bool isDetach)
    {
        if (isDetach)
        {
            thread debug(msgBox::msgbox, hInstanceTop, "Debug", debugText, WinPrompt::ICON_WARNING, WinPrompt::BTN_OK, WinPrompt::MODE_WARNING);
            debug.detach();
        }
        else
        {
            msgBox::msgbox(hInstanceTop, "Debug", debugText, WinPrompt::ICON_WARNING, WinPrompt::BTN_OK, WinPrompt::MODE_WARNING);
        }
    }
};


bool am = true;

bool xm = false;
bool xml = false;

bool isD = false;

bool isFinish = false;
bool SHa = false;
bool isStart = false;
bool boom = false;

string linkJD = "N";
int linkMac = -1;
bool linkOk = false;
bool OH = false;

class addSend
{
public:
    static void pack(string a)
    {

        int color[3] = { 155,255,200 };
        int fc[3] = { -1,-1,-1 };
        string waitingForPackage[8] = {
            // 中文（简体）
            "等待包装文件夹",

            // 英文
            "Waiting to package folder",

            // 日语
            "フォルダのパッケージ待機中",

            // 韩语
            "폴더 패키징 대기 중",

            // 法语
            "En attente du dossier à empaqueter",

            // 德语
            "Warte auf Verpackung des Ordners",

            // 俄语
            "Ожидание упаковки папки",

            // 西班牙语
            "Esperando para empaquetar carpeta"
        };
        string kk = waitingForPackage[stoi(userset[0])] + " : " + tool::Utf8ToGbk(a);
        msg::Message(3, color, fc, 0, kk);

        bool k = false;
        while (1)
        {
            for (int i = 0; i < sendFileList.size(); i++)
            {
                if (sendFileList[i].filePath == "netPack.npk")
                {
                    k = true;
                    break;
                }
            }

            if (!k)
            {
                break;
            }
            else
            {
                k = false;
            }
            api_sleep(1000);
        }


        if (Net::packFolder(tool::Utf8ToGbk(a), "netPack.npk"))
        {
            isFinish = false;
            xml = true;
            xm = true;
            if (xm == true)
            {
                xml = xm = false;
                sendFileList.push_back({ "netPack.npk",tool::Utf8ToGbk(a) });

                int color[3] = { 150,255,150 };
                int fontColor[3] = { -1,-1,-1 };

                string mb[8] = {
                    // 中文（简体）
                    "加入传输队列 : ",

                    // 英文
                    "Add to transfer queue : ",

                    // 日语
                    "転送キューに追加 : ",

                    // 韩语
                    "전송 큐에 추가 : ",

                    // 法语
                    "Ajouter à la file de transfert : ",

                    // 德语
                    "Zur Übertragungswarteschlange hinzufügen : ",

                    // 俄语
                    "Добавить в очередь передачи : ",

                    // 西班牙语
                    "Añadir a la cola de transferencia : "
                };

                string mos = mb[stoi(userset[0])] + " : " + tool::Utf8ToGbk(a);

                msg::Message(1, color, fontColor, 360, mos);

                if (!isStart)isStart = true;
            }
            else
            {
                xml = xm = false;
                int color[3] = { 255,150,150 };
                int fontColor[3] = { -1,-1,-1 };

                string mb[8] = {
                    // 中文（简体）
                    "加入传输队列 : ",

                    // 英文
                    "Add to transfer queue : ",

                    // 日语
                    "転送キューに追加 : ",

                    // 韩语
                    "전송 큐에 추가 : ",

                    // 法语
                    "Ajouter à la file de transfert : ",

                    // 德语
                    "Zur Übertragungswarteschlange hinzufügen : ",

                    // 俄语
                    "Добавить в очередь передачи : ",

                    // 西班牙语
                    "Añadir a la cola de transferencia : "
                };

                string mos = mb[stoi(userset[0])] + tool::Utf8ToGbk(a);

                msg::Message(1, color, fontColor, 360, mos);

                if (!isStart)isStart = true;
            }
        }
        else
        {
            xml = true;
            xm = false;
        }
        msg::KillBox(kk);
    }

    static void addup(string a)
    {

        string checkingFiles[8] = {
            // 中文（简体）
            "正在检查文件 :",

            // 英文
            "Checking files :",

            // 日语
            "ファイルをチェック中 :",

            // 韩语
            "파일 확인 중 :",

            // 法语
            "Vérification des fichiers :",

            // 德语
            "Dateien werden überprüft :",

            // 俄语
            "Проверка файлов :",

            // 西班牙语
            "Verificando archivos :"
        };

        string msg = checkingFiles[stoi(userset[0])] + tool::Utf8ToGbk(a);
        int c1[3] = { 155,255,200 };
        int cf1[3] = { -1,-1,-1 };
        msg::Message(3, c1, cf1, 0, msg);

        sendFileList.push_back({ tool::Utf8ToGbk(a),tool::Utf8ToGbk(a) });
        if(debug) cout << endl << "添加文件 ->  " << tool::Utf8ToGbk(a);
        int color[3] = { 150,255,150 };
        int fontColor[3] = { -1,-1,-1 };

        string mb[8] = {
            // 中文（简体）
            "加入传输队列 : ",

            // 英文
            "Add to transfer queue : ",

            // 日语
            "転送キューに追加 : ",

            // 韩语
            "전송 큐에 추가 : ",

            // 法语
            "Ajouter à la file de transfert : ",

            // 德语
            "Zur Übertragungswarteschlange hinzufügen : ",

            // 俄语
            "Добавить в очередь передачи : ",

            // 西班牙语
            "Añadir a la cola de transferencia : "
        };

        string mos = mb[stoi(userset[0])] + tool::Utf8ToGbk(a);

        msg::Message(1, color, fontColor, 360, mos);

        msg::KillBox(msg);

        isFinish = false;

        if (!isStart)isStart = true;
    }
};

bool isEXIT_link = false;
void exitPX(int i)
{
    if (isEXIT_link) return;
    isEXIT_link = true;
    std::string wait[8] = {
    "等待结束连接",  // 中文 (简体)
    "Waiting to end connection",  // 英文
    "接続終了を待機中",  // 日语
    "연결 종료 대기 중",  // 韩语
    "En attente de la fin de la connexion",  // 法语
    "Warten auf Verbindungsende",  // 德语
    "Ожидание завершения соединения",  // 俄语
    "Esperando a terminar la conexión"  // 西班牙语
    };
    int c[3] = { 155,255,200 };
    int fc[3] = { -1,-1,-1 };
    msg::Message(3, c, fc, 0, wait[stoi(userset[0])]);
    Net::stopSendThread();
    sendFileList.clear();
    saveData_link[i] = "NULL";
    tool::MoveNullToEnd(saveData_link, saveMax);
    linkMac = -1; // 断开连接
    if (debug) cout << endl << "关闭连接";
    msg::KillBox(wait[stoi(userset[0])]);

    pair<string, unsigned short> ad = code::decode_ip_port(linkJD);
    Net::SendXorString(ad.first.c_str(), ad.second + 1, "linkStop");

    isEXIT_link = false;

    string deleteSuccessMsg[8] = {
        // 中文（简体）
        "删除链接成功",        // 标准版
        // "链接删除成功"      // 被动语态版（备选）

        // 英文
        "Link deleted successfully",  // 技术文档标准
        // "Successfully removed link" // 操作日志风格

        // 日语
        "リンクの削除に成功しました", // 完整敬体
        // "リンク削除済み"         // 状态简写（系统提示）

        // 韩语
        "링크 삭제 성공",          // 控制台风格
        // "링크가 삭제되었습니다"   // 用户提示风格

        // 法语
        "Lien supprimé avec succès", // 正式报告
        // "Suppression réussie"     // 简洁版

        // 德语
        "Link erfolgreich gelöscht", // 工业标准
        // "Erfolgreiche Löschung"   // 名词化形式

        // 俄语
        "Ссылка успешно удалена",    // 被动完成体
        // "Удаление выполнено"      // 中性报告

        // 西班牙语
        "Enlace eliminado correctamente", // 拉美标准
        // "Borrado exitoso"         // 简短确认
    };

    int color[3] = { 100,255,100 };
    int fontColor[3] = { -1,-1,-1 };

    msg::Message(1, color, fontColor, 240, deleteSuccessMsg[stoi(userset[0])]);
}

void exitPXD()
{
    if (isEXIT_link) return;
    isEXIT_link = true;
    std::string wait[8] = {
    "等待结束连接",  // 中文 (简体)
    "Waiting to end connection",  // 英文
    "接続終了を待機中",  // 日语
    "연결 종료 대기 중",  // 韩语
    "En attente de la fin de la connexion",  // 法语
    "Warten auf Verbindungsende",  // 德语
    "Ожидание завершения соединения",  // 俄语
    "Esperando a terminar la conexión"  // 西班牙语
    };
    int c[3] = { 155,255,200 };
    int fc[3] = { -1,-1,-1 };
    msg::Message(3, c, fc, 0, wait[stoi(userset[0])]);
   Net::stopSendThread();

    sendFileList.clear();

    pair<string, unsigned short> ad = code::decode_ip_port(linkJD);
    Net::SendXorString(ad.first.c_str(), ad.second + 1, "linkStop");

    linkMac = -1; // 断开连接
    if (debug) cout << endl << "关闭连接";
    msg::KillBox(wait[stoi(userset[0])]);
    isEXIT_link = false;

    std::string translations[8] = {
    "已断开连接",  // 中文 (简体)
    "Disconnected",  // 英文
    "接続が切断されました",  // 日语
    "연결이 끊겼습니다",  // 韩语
    "Déconnecté",  // 法语
    "Verbindung getrennt",  // 德语
    "Отключено",  // 俄语
    "Desconectado"  // 西班牙语
    };

    int color[3] = { 255,255,100 };
    int fontColor[3] = { -1,-1,-1 };

    msg::Message(1, color, fontColor, 240, translations[stoi(userset[0])]);
}

string lockM;

class tool_messageWork
{
public:
    static void link()
    {
        while (boom)
        {
            api_sleep(200);
        }
        boom = true;
        string code = Net::popEncryptedString();
        string lock = Net::popEncryptedString();

        if (debug) cout << endl << "请求连接设备";
        wstring translations[8] = {
            L"有设备请求连接",                     // 中文 (Chinese)
            L"A device is requesting connection",  // 英文 (English)
            L"デバイスが接続を要求しています",     // 日语 (Japanese)
            L"장치가 연결을 요청 중입니다",        // 韩文 (Korean)
            L"Un appareil demande une connexion",  // 法语 (French)
            L"Ein Gerät fordert eine Verbindung an", // 德语 (German)
            L"Устройство запрашивает подключение",  // 俄语 (Russian)
            L"Un dispositivo está solicitando conexión" // 西班牙语 (Spanish)
        };
        if (stoi(userset[7]) == 0) trd::ShowTrayMessage(mainHwnd, translations[stoi(userset[0])].c_str(), L"NetTrack", 0);

        string translationsB[8] = {
            "有设备请求连接",                     // 中文 (Chinese)
            "A device is requesting connection",  // 英文 (English)
            "デバイスが接続を要求しています",     // 日语 (Japanese)
            "장치가 연결을 요청 중입니다",        // 韩文 (Korean)
            "Un appareil demande une connexion",  // 法语 (French)
            "Ein Gerät fordert eine Verbindung an", // 德语 (German)
            "Устройство запрашивает подключение",  // 俄语 (Russian)
            "Un dispositivo está solicitando conexión" // 西班牙语 (Spanish)
        };
        string translationsC[8] = {
    "连接请求",          // 中文 (简体)
    "Connection request",  // English
    "接続要求",          // 日本語
    "연결 요청",         // 한국어
    "Demande de connexion",  // Français
    "Verbindungsanfrage",    // Deutsch
    "Запрос на подключение", // Русский
    "Solicitud de conexión"  // Español
        };

        string translationsD[8] = {
            // 中文 (简体)
            "有设备试图连接您的NetTrack，若您单击\"确定\"即为同意，否则NetTrack将拒绝连接",
            // English
            "A device is attempting to connect to your NetTrack. Click \"OK\" to accept, otherwise NetTrack will reject the connection.",
            // 日本語
            "デバイスがあなたのNetTrackへの接続を試みています。「確認」をクリックすると許可され、それ以外の場合NetTrackは接続を拒否します。",
            // 한국어
            "장치가 귀하의 NetTrack에 연결을 시도하고 있습니다. \"확인\"을 클릭하면 수락되고, 그렇지 않으면 NetTrack이 연결을 거부합니다.",
            // Français
            "Un appareil tente de se connecter à votre NetTrack. Cliquez sur \"OK\" pour accepter, sinon NetTrack refusera la connexion.",
            // Deutsch
            "Ein Gerät versucht, eine Verbindung zu Ihrem NetTrack herzustellen. Klicken Sie auf \"OK\", um zuzustimmen, andernfalls wird NetTrack die Verbindung ablehnen.",
            // Русский
            "Устройство пытается подключиться к вашему NetTrack. Нажмите \"ОК\" для согласия, в противном случае NetTrack отклонит подключение.",
            // Español
            "Un dispositivo está intentando conectarse a su NetTrack. Haga clic en \"Aceptar\" para permitirlo, de lo contrario NetTrack rechazará la conexión."
        };

        if (stoi(userset[7]) == 0)
        {
            if(lock == userset[2] || userset[2] == "0")
            {
                if (1 == msgBox::msgbox(hInstanceTop, translationsC[stoi(userset[0])], translationsD[stoi(userset[0])], WinPrompt::ICON_QUESTION, WinPrompt::BTN_OKCANCEL, WinPrompt::MODE_INFO))
                {
                    string translationsE[8] = {
                        // 中文 (简体)
                        "此链接为一个陌生链接，请再次确认是否与您连接，如果您信任此连接，请单击\"是\"，否则NetTrack将会拒绝连接。(为什么会看到此弹窗，因为此链接的设备从未连接过您的NetTrack，当您同意连接后，此设备将会加入到您的信任名单里。)",

                        // English
                        "This is an unfamiliar connection. Please confirm whether to connect. If you trust this connection, click \"Yes\", otherwise NetTrack will reject it. (Why this prompt? Because this device has never connected to your NetTrack before. After approval, it will be added to your trusted list.)",

                        // 日本語
                        "この接続は未確認のリンクです。接続を再度確認してください。この接続を信頼する場合は「はい」をクリックし、それ以外の場合NetTrackは接続を拒否します。(このポップアップが表示される理由：このデバイスは過去にNetTrackに接続したことがありません。承認後、信頼済みリストに追加されます。)",

                        // 한국어
                        "이 연결은 알 수 없는 링크입니다. 연결을 다시 확인하십시오. 이 연결을 신뢰하는 경우 \"예\"를 클릭하고, 그렇지 않으면 NetTrack이 연결을 거부합니다.(이 팝업이 표시되는 이유: 이 장치는 이전에 NetTrack에 연결된 적이 없습니다. 승인 후 신뢰할 수 있는 목록에 추가됩니다.)",

                        // Français
                        "Ce lien est une connexion inconnue. Veuillez confirmer si vous souhaitez vous connecter. Si vous faites confiance à cette connexion, cliquez sur \"Oui\", sinon NetTrack la rejettera. (Pourquoi cette fenêtre ? Parce que cet appareil ne s'est jamais connecté à votre NetTrack auparavant. Après approbation, il sera ajouté à votre liste de confiance.)",

                        // Deutsch
                        "Dies ist eine unbekannte Verbindung. Bitte bestätigen Sie, ob Sie eine Verbindung herstellen möchten. Wenn Sie dieser Verbindung vertrauen, klicken Sie auf \"Ja\", andernfalls wird NetTrack sie ablehnen. (Warum diese Meldung? Weil dieses Gerät noch nie mit Ihrem NetTrack verbunden war. Nach der Zustimmung wird es Ihrer Vertrauensliste hinzugefügt.)",

                        // Русский
                        "Это незнакомое соединение. Пожалуйста, подтвердите подключение. Если вы доверяете этому соединению, нажмите \"Да\", в противном случае NetTrack отклонит его. (Зачем это окно? Потому что это устройство ранее не подключалось к вашему NetTrack. После подтверждения оно будет добавлено в доверенный список.)",

                        // Español
                        "Esta es una conexión desconocida. Por favor, confirme si desea conectarse. Si confía en esta conexión, haga clic en \"Sí\", de lo contrario NetTrack la rechazará. (¿Por qué este mensaje? Porque este dispositivo nunca se ha conectado a su NetTrack antes. Después de aprobarlo, se agregará a su lista de confianza.)"
                    };
                    string translationsF[8] = {
            "NetTrack安全警告",          // 中文 (简体)
            "NetTrack Security Warning",  // English
            "NetTrackセキュリティ警告",    // 日本語
            "NetTrack 보안 경고",        // 한국어
            "Avertissement de sécurité NetTrack",  // Français
            "NetTrack-Sicherheitswarnung",         // Deutsch
            "Предупреждение безопасности NetTrack", // Русский
            "Advertencia de seguridad de NetTrack"  // Español
                    };

                    pair<string, unsigned short> a = code::decode_ip_port(code);

                    bool isTrust = false;
                    for (int i = 0; i < trustList.size(); i++)
                    {
                        if (trustList[i] == a.first)
                        {
                            isTrust = true;
                        }
                    }

                    if (!isTrust)
                    {
                        if (3 == msgBox::msgbox(hInstanceTop, translationsF[stoi(userset[0])], translationsE[stoi(userset[0])], WinPrompt::ICON_WARNING, WinPrompt::BTN_YESNO, WinPrompt::MODE_WARNING))
                        {
                            trustList.push_back(a.first);
                            cout << endl << a.first << "     " << a.second;
                            Net::SendXorString(a.first.c_str(), a.second + 1, "link-yes");

                            linkJD = code;
                        }
                        else
                        {
                            cout << endl << a.first << "     " << a.second;
                            Net::SendXorString(a.first.c_str(), a.second + 1, "link-no");
                        }
                    }
                    else
                    {
                        cout << endl << a.first << "     " << a.second;
                        Net::SendXorString(a.first.c_str(), a.second + 1, "link-yes");

                        linkJD = code;
                    }
                }
                else
                {
                    pair<string, unsigned short> a = code::decode_ip_port(code);
                    Net::SendXorString(a.first.c_str(), port + 1, "link-no");
                }
            }
            else
            {
                pair<string, unsigned short> a = code::decode_ip_port(code);
                Net::SendXorString(a.first.c_str(), a.second + 1, "link-lockerror");
                Net::SendXorString(a.first.c_str(), a.second + 1, "link-no");
            }
        }
        else if (stoi(userset[7]) == 1)
        {
            if (lock == userset[2] || userset[2] == "0")
            {
                pair<string, unsigned short> a = code::decode_ip_port(code);
                trustList.push_back(a.first);
                cout << endl << a.first << "     " << a.second;
                Net::SendXorString(a.first.c_str(), a.second + 1, "link-yes");
                linkJD = code;

                wstring connectedToNetTrackW[8] = {
    L"已与NetTrack连接",         // Chinese
    L"Connected to NetTrack",    // English
    L"NetTrackに接続しました",   // Japanese
    L"NetTrack에 연결되었습니다",// Korean
    L"Connecté à NetTrack",      // French
    L"Mit NetTrack verbunden",   // German
    L"Подключено к NetTrack",    // Russian
    L"Conectado a NetTrack"      // Spanish
                };
                wstring msg = tool::string_to_wstring(code) + connectedToNetTrackW[stoi(userset[0])];
				trd::ShowTrayMessage(mainHwnd, msg.c_str(), L"NetTrack", 0);
            }
            else
            {
                pair<string, unsigned short> a = code::decode_ip_port(code);
                Net::SendXorString(a.first.c_str(), a.second + 1, "link-lockerror");
                Net::SendXorString(a.first.c_str(), a.second + 1, "link-no");
            }
        }
        else
        {
            pair<string, unsigned short> a = code::decode_ip_port(code);
            trustList.push_back(a.first);
            cout << endl << a.first << "     " << a.second;
            Net::SendXorString(a.first.c_str(), a.second + 1, "link-yes");
            linkJD = code;

            wstring connectedToNetTrackW[8] = {
    L"已与NetTrack连接",         // Chinese
    L"Connected to NetTrack",    // English
    L"NetTrackに接続しました",   // Japanese
    L"NetTrack에 연결되었습니다",// Korean
    L"Connecté à NetTrack",      // French
    L"Mit NetTrack verbunden",   // German
    L"Подключено к NetTrack",    // Russian
    L"Conectado a NetTrack"      // Spanish
            };
            wstring msg = tool::string_to_wstring(code) + connectedToNetTrackW[stoi(userset[0])];
            trd::ShowTrayMessage(mainHwnd, msg.c_str(), L"NetTrack", 0);
        }
        boom = false;
    }
    static void textLink()
    {
        OH = true;
        int c1[3] = { 155,255,200 };
        int fc1[3] = { -1,-1,-1 };
        std::string textLinkM[8] = {
    "等待对方响应",          // 中文 (简体)
    "Waiting for peer response",  // English
    "相手の応答を待機中",    // 日本語
    "상대방 응답 대기 중",   // 한국어
    "En attente de réponse",  // Français (简写形式)
    "Warte auf Antwort",     // Deutsch
    "Ожидание ответа",       // Русский
    "Esperando respuesta"    // Español
        };
        msg::Message(3, c1, fc1, 0, textLinkM[stoi(userset[0])]);

        pair<string, unsigned short> a = code::decode_ip_port(saveData_link[linkMac]);
        Net::SendXorString(a.first.c_str(), port + 1, "linkCheck");
        Net::SendXorString(a.first.c_str(), port + 1, OMSR);
        int mos = 0;
        while (!linkOk)
        {
            api_sleep(100);
            mos += 1;
            if (mos > 50) break;
        }
        if (linkOk)
        {
            std::string linkOK[8] = {
            "连接状态良好",          // 中文 (简体)
            "Connection status: Good",  // English
           "接続状態：良好",        // 日本語
           "연결 상태: 양호",      // 한국어
            "État de connexion : Bon",  // Français
          "Verbindungsstatus: Gut",   // Deutsch
          "Состояние подключения: Хорошее",  // Русский
          "Estado de conexión: Bueno"         // Español
            };
            int c[3] = { 100,255,100 };
            int fc[3] = { -1,-1,-1 };
            msg::Message(1, c, fc, 240, linkOK[stoi(userset[0])]);
        }
        else
        {
            std::string linkNo[8] = {
                // 中文 (简体)
                "连接丢失或超时，对方未响应",

                // English
                "Connection lost or timed out, no response from the peer",

                // 日本語
                "接続が切断、またはタイムアウトしました。相手方から応答がありません",

                // 한국어
                "연결이 끊어졌거나 시간 초과되었습니다. 상대방의 응답이 없습니다",

                // Français
                "Connexion perdue ou expirée, aucune réponse de l'autre partie",

                // Deutsch
                "Verbindung verloren oder Zeitüberschreitung, keine Antwort vom Gegenüber",

                // Русский
                "Соединение потеряно или истекло время ожидания, нет ответа от удалённой стороны",

                // Español
                "Conexión perdida o tiempo de espera agotado, no hay respuesta del remitente"
            };
            int c[3] = { 255,100,100 };
            int fc[3] = { -1,-1,-1 };
            msg::Message(2, c, fc, 240, linkNo[stoi(userset[0])]);
        }
        linkOk = false;
        msg::KillBox(textLinkM[stoi(userset[0])]);
        OH = false;
    }
    static void linkPlease()
    {
        string waitingForResponse[8] = {
    "等待对方响应",          // Chinese
    "Waiting for response",  // English
    "相手の応答を待機中",    // Japanese
    "상대방의 응답을 기다리는 중",  // Korean
    "En attente de réponse",  // French
    "Warte auf Antwort",     // German
    "Ожидание ответа",       // Russian
    "Esperando respuesta"    // Spanish
        };
        int c[3] = { 155,255,200 };
        int fc[3] = { -1,-1,-1 };
        msg::Message(3, c, fc, 0, waitingForResponse[stoi(userset[0])]);
        pair<string, unsigned short> a = code::decode_ip_port(saveData_link[linkOS]);
        int backinf = Net::SendXorString(a.first.c_str(), a.second + 1, "link1");
        int backinf2 = Net::SendXorString(a.first.c_str(), a.second + 1, OMSR);
        int backinf3 = Net::SendXorString(a.first.c_str(), a.second + 1, lockM);
        if (0 != backinf || 0 != backinf2 || 0 != backinf3)
        {
            std::string errormsg[8] = {
                // 中文 (简体)
                "网络异常",

                // English
                "Network exception",

                // 日本語
                "ネットワーク異常",

                // 한국어
                "네트워크 이상",

                // Français
                "Exception réseau",

                // Deutsch
                "Netzwerkausnahme",

                // Русский
                "Сетевая аномалия",

                // Español
                "Excepción de red"
            };
            linkOS = -1;
            string preparingConnectionB[8] = {
                // 中文（简体）
                "正在准备连接",

                // 英文
                "Preparing connection",

                // 日语
                "接続準備中",

                // 韩语
                "연결 준비 중",

                // 法语
                "Préparation de la connexion",

                // 德语
                "Verbindung wird vorbereitet",

                // 俄语
                "Подготовка подключения",

                // 西班牙语
                "Preparando conexión"
            };
            msg::KillBox(preparingConnectionB[stoi(userset[0])]);
            int c[3] = { 255,100,100 };
            int fc[3] = { -1,-1,-1 };
            if (0 != backinf)msg::Message(1, c, fc, 240, errormsg[stoi(userset[0])] + " : " + to_string(backinf));
            else msg::Message(1, c, fc, 240, errormsg[stoi(userset[0])] + " : " + to_string(backinf2));
        }
        msg::KillBox(waitingForResponse[stoi(userset[0])]);
        lockM.clear();
    }
};

void messageWork()
{
    string mk = Net::popEncryptedString();
    if (mk != "NULL")
    {
        if(debug) cout << endl << "接收处理:" << mk;
        if (mk == "link1")
        {
            thread mos(tool_messageWork::link);
            mos.detach();
        }
        if (mk == "link-yes")
        {
            wstring connectionStatus[8] = {
               L"对方允许连接，已连接",
               L"Connection allowed, now connected",
              L"接続が許可され、接続しました",
                 L"상대방이 연결을 허용하여 연결되었습니다",
                 L"Connexion autorisée, maintenant établie",
              L"Verbindung zugelassen, jetzt verbunden",
             L"Подключение разрешено, соединение установлено",
             L"Conexión permitida, ahora conectado"
            };

            trd::ShowTrayMessage(mainHwnd, connectionStatus[stoi(userset[0])].c_str(), L"NetTrack", 0);

            pair<string, unsigned short> a = code::decode_ip_port(saveData_link[linkOS]);
            Net::stopSendThread();
            Net::startSendThread(a.first.c_str(), a.second);
            if(debug) cout << endl << "新的连接 ->  IP：" << a.first << "     端口：" << a.second;

            string preparingConnectionB[8] = {
                // 中文（简体）
                "正在准备连接",

                // 英文
                "Preparing connection",

                // 日语
                "接続準備中",

                // 韩语
                "연결 준비 중",

                // 法语
                "Préparation de la connexion",

                // 德语
                "Verbindung wird vorbereitet",

                // 俄语
                "Подготовка подключения",

                // 西班牙语
                "Preparando conexión"
            };
            msg::KillBox(preparingConnectionB[stoi(userset[0])]);

            string translationsF[8] = {
              "已与目标设备连接",          // 中文 (简体)  
               "Connected to the target device",  // 英语  
               "ターゲットデバイスに接続しました",  // 日语  
                "대상 장치에 연결되었습니다",  // 韩语  
                "Connecté à l'appareil cible",  // 法语  
                   "Mit dem Zielgerät verbunden",  // 德语  
               "Подключено к целевому устройству",  // 俄语  
              "Conectado al dispositivo objetivo"  // 西班牙语  
            };

            int c[3] = { 150,255,150 };
            int fc[3] = { -1,-1,-1 };
            msg::Message(1, c, fc, 240, translationsF[stoi(userset[0])]);

            linkMac = linkOS;
            linkOS = -1;
        }
        if (mk == "link-no")
        {
            wstring translations[8] = {
                L"对方拒绝连接",                // 中文 (简体)
                L"Connection refused",         // 英语
                L"相手が接続を拒否しました",    // 日语
                L"상대방이 연결을 거부했습니다", // 韩语
                L"Connexion refusée",          // 法语
                L"Verbindung abgelehnt",       // 德语
                L"Соединение отклонено",       // 俄语
                L"Conexión rechazada"          // 西班牙语
            };
            linkOS = -1;
            string preparingConnectionB[8] = {
                // 中文（简体）
                "正在准备连接",

                // 英文
                "Preparing connection",

                // 日语
                "接続準備中",

                // 韩语
                "연결 준비 중",

                // 法语
                "Préparation de la connexion",

                // 德语
                "Verbindung wird vorbereitet",

                // 俄语
                "Подготовка подключения",

                // 西班牙语
                "Preparando conexión"
            };
            msg::KillBox(preparingConnectionB[stoi(userset[0])]);

            int c[3] = { 255,100,100 };
            int fc[3] = { -1,-1,-1 };
            msg::Message(1, c, fc, 240, tool::Utf8ToGbk(tool::wstring_to_utf8(translations[stoi(userset[0])])));
            trd::ShowTrayMessage(mainHwnd, translations[stoi(userset[0])].c_str(), L"NetTrack", 2);
        }
        if (mk == "linkCheckYes")
        {
            linkOk = true;
        }
        if (mk == "linkCheck")
        {
            string mks = Net::popEncryptedString();
            pair<string, unsigned short> a = code::decode_ip_port(mks);
            Net::SendXorString(a.first.c_str(), port + 1, "linkCheckYes");
        }
        if (mk == "linkStop")
        {
            linkJD = "N";
            std::string connectionStopped[8] = {
    "对方停止连接",          // Chinese (Simplified)
    "Peer stopped connection",  // English
    "相手が接続を停止しました",  // Japanese
    "상대방이 연결을 중단했습니다",  // Korean
    "L'appareil distant a interrompu la connexion",  // French
    "Gegenpartei hat Verbindung getrennt",  // German
    "Удалённая сторона разорвала соединение",  // Russian
    "El dispositivo remoto detuvo la conexión"  // Spanish
            };
            int c[3] = { 150,255,200 };
            int fc[3] = { -1,-1,-1 };

            msg::Message(1, c, fc, 240, connectionStopped[stoi(userset[0])]);

            std::string waitForDisconnect[8] = {
    "等待对方停止连接",          // Chinese (Simplified)
    "Waiting for the peer to disconnect",  // English
    "相手の切断を待機中",        // Japanese
    "상대방의 연결 해제를 기다리는 중",  // Korean
    "En attente de la déconnexion de l'appareil distant",  // French
    "Warte auf Trennung durch die Gegenstelle",  // German
    "Ожидание отключения удалённой стороны",  // Russian
    "Esperando a que el dispositivo remoto se desconecte"  // Spanish
            };
           msg::KillBox(waitForDisconnect[stoi(userset[0])]);
        }
        if (mk == "linkStopPlease")
        {
            thread a(exitPXD);
            a.detach();
            string peerDisconnectRequest[8] = {
    "对方请求断开连接",          // Chinese (Simplified)
    "The peer requested to disconnect",  // English
    "相手が接続切断を要求しました",  // Japanese
    "상대방이 연결 해제를 요청했습니다",  // Korean
    "L'appareil distant a demandé la déconnexion",  // French
    "Die Gegenstelle hat die Trennung beantragt",  // German
    "Удалённая сторона запросила отключение",  // Russian
    "El dispositivo remoto solicitó la desconexión"  // Spanish
            };
            int c[3] = { 155,255,200 };
			int fc[3] = { -1,-1,-1 };
			msg::Message(1, c, fc, 240, peerDisconnectRequest[stoi(userset[0])]);
        }
        if (mk == "link-lockerror")
        {
            string wrongPasswordText[8] = {
      "密码错误",          // Chinese
      "Wrong password",   // English
      "パスワードが間違っています",  // Japanese
     "잘못된 비밀번호",   // Korean
     "Mot de passe incorrect",  // French
      "Falsches Passwort",  // German
      "Неверный пароль",   // Russian
       "Contraseña incorrecta"  // Spanish
            };
            int c[3] = { 255,100,100 };
            int fc[3] = { -1,-1,-1 };
            msg::Message(2, c, fc, 240, wrongPasswordText[stoi(userset[0])]);
            wstring wrongPasswordTextW[8] = {
    L"密码错误",         // Chinese
    L"Wrong password",  // English
    L"パスワードが間違っています",  // Japanese
    L"잘못된 비밀번호",  // Korean
    L"Mot de passe incorrect",  // French
    L"Falsches Passwort",  // German
    L"Неверный пароль",   // Russian
    L"Contraseña incorrecta"  // Spanish
            };
            linkOS = -1;
            string preparingConnectionB[8] = {
                // 中文（简体）
                "正在准备连接",

                // 英文
                "Preparing connection",

                // 日语
                "接続準備中",

                // 韩语
                "연결 준비 중",

                // 法语
                "Préparation de la connexion",

                // 德语
                "Verbindung wird vorbereitet",

                // 俄语
                "Подготовка подключения",

                // 西班牙语
                "Preparando conexión"
            };
            msg::KillBox(preparingConnectionB[stoi(userset[0])]);
            trd::ShowTrayMessage(mainHwnd, wrongPasswordTextW[stoi(userset[0])].c_str(), L"NetTrack", 2);
        }
    }
}



int MSA = 0;
bool isMSA = false;
int MSA_frame = 0;
int linkOSB = -1;
int MSAAlpha = 0;

string Speed;
int process = 0;
int process2 = 0;
int process_frame = 60;
string timeNeed;
bool kgms = true;

int start_P = 0;
int end_P = 0;

bool needlockX = false;
int  needlockXX = 0;
int needlockXXFrame = 60;


class needlock
{
private:
    static void math()
    {
        if(needlockX)
        {
            if (needlockXXFrame < 30)
            {
                int start = 1;
                int end = 100;
                double t = needlockXXFrame / (double)30;

                needlockXX = start + (end - start) * ease::easeOut(t, 3.5);
                needlockXXFrame++;
            }
        }
        else
        {
            if (needlockXXFrame < 30)
            {
                int start = 100;
                int end = 0;
                double t = needlockXXFrame / (double)30;

                needlockXX = start + (end - start) * ease::easeOut(t, 3.5);
                needlockXXFrame++;
            }
            }
    }
public:
    static void key()
    {
            if (kbhit() && lockM.length() < 6)
            {
                char m = getch();
                if (tool::isValidChar(m))
                {
                    lockM += m;
                }
            }

        if (keystate(0x08))
        {
            if (isKEY_0x08)
            {
                lockM.pop_back();
                isKEY_0x08 = false;
            }
        }
        else
        {
            isKEY_0x08 = true;
        }

        if (keystate(0x0d))
        {
            if (lockM.length() == 6 || lockM.empty())
            {
                thread a(tool_messageWork::linkPlease);
                a.detach();
                needlockX = false;
                needlockXXFrame = 0;
            }
        }

        if (keystate(0x01))
        {
            if (isKEY)
            {
                if (mousePos.x > windowSize.x - needlockXX - 120 && mousePos.x <  windowSize.x - needlockXX - 20 && mousePos.y > windowSize.y - needlockXX - 70 && mousePos.y < windowSize.y - needlockXX - 20)
                {
                    if (lockM.length() == 6 || lockM.empty())
                    {
                        thread a(tool_messageWork::linkPlease);
                        a.detach();
                        needlockX = false;
                        needlockXXFrame = 0;
                    }
                }
                if (mousePos.x > needlockXX + 20 && mousePos.x < needlockXX + 120 && mousePos.y > needlockXX + 20 && mousePos.y < needlockXX + 70)
                {
                    needlockX = false;
                    needlockXXFrame = 0;
                    lockM.clear();
                    linkOS = -1;
                    string preparingConnectionB[8] = {
                        // 中文（简体）
                        "正在准备连接",
                        // 英文
                        "Preparing connection",
                        // 日语
                        "接続準備中",
                        // 韩语
                        "연결 준비 중",
                        // 法语
                        "Préparation de la connexion",
                        // 德语
                        "Verbindung wird vorbereitet",
                        // 俄语
                        "Подготовка подключения",
                        // 西班牙语
                        "Preparando conexión"
                    };
                    msg::KillBox(preparingConnectionB[stoi(userset[0])]);
                }
            }
        }
        else
        {
            isKEY = true;
        }
    }
    
    static void draw()
    {
        math();
        
            if (DarkMode)
            {
                setfillcolor(EGERGBA(0, 0, 0, needlockXX));
            }
            else
            {
                setfillcolor(EGERGBA(255, 255, 255, needlockXX));
            }
            
        ege_fillrect(0, 0, windowSize.x, windowSize.y);

        if (needlockX)
        {
            setfillcolor(EGERGBA(155, 155, 155, 255 - needlockXX));
        }
        else
        {
            setfillcolor(EGERGBA(155, 155, 155, needlockXX * 255 / 100));
        }
        
        ege_fillrect(0 + needlockXX,0 + needlockXX, windowSize.x - needlockXX * 2, windowSize.y - needlockXX * 2);

        wstring passwordPromptMessages[8] = {
        L"输入密码验证（若无密码可留空）",  // Chinese (Simplified)
      L"Enter password for verification (leave blank if none)",  // English
      L"パスワードを入力（未設定の場合は空欄可）",  // Japanese
      L"비밀번호 입력（없으면 공백 가능）",  // Korean
      L"Entrez le mot de passe (laissez vide si aucun)",  // French
      L"Passwort eingeben (leer lassen falls keins)",  // German
     L"Введите пароль (оставьте пустым, если нет)",  // Russian
      L"Ingrese contraseña (dejar vacío si no hay)"  // Spanish
        };
        DrawManager::centerText();
        DrawManager::setFont(45);
        xyprintf(windowSize.x / 2, 150 + needlockXX, passwordPromptMessages[stoi(userset[0])].c_str());

        xyprintf(windowSize.x / 2, windowSize.y - 320 + needlockXX, lockM.c_str());
        

        setlinewidth(6);
        if (needlockX)
        {
            setcolor(EGERGBA(255 - BKcolor, 255 - BKcolor, 255 - BKcolor, 255 - needlockXX));
        }
        else
        {
            setcolor(EGERGBA(255 - BKcolor, 255 - BKcolor, 255 - BKcolor, needlockXX * 255 / 100));
        }
        ege_line(150, windowSize.y - 200, windowSize.x - 300, windowSize.y - 200);

        wstring confirmTextW[8] = {
       L"确定",     // Chinese
       L"OK",      // English
      L"確定",     // Japanese
        L"확인",     // Korean
       L"OK",      // French
       L"OK",      // German
       L"ОК",      // Russian
       L"Aceptar"  // Spanish
        };
        wstring returnTextW[8] = {
    L"返回",     // Chinese
    L"Back",    // English
    L"戻る",    // Japanese
    L"뒤로",    // Korean
    L"Retour",  // French
    L"Zurück",  // German
    L"Назад",   // Russian
    L"Volver"   // Spanish
        };
        DrawManager::setFillRectColor();
        DrawManager::fillroundrectwithrect(windowSize.x - needlockXX  - 120, windowSize.y - needlockXX  - 70, 100, 50,10);
        DrawManager::fillroundrectwithrect(needlockXX + 20, needlockXX + 20, 100, 50, 10);
        DrawManager::setFont(30);
        xyprintf(windowSize.x - needlockXX - 70, windowSize.y - needlockXX - 45, confirmTextW[stoi(userset[0])].c_str());
        xyprintf(needlockXX  + 70,  needlockXX + 45, returnTextW[stoi(userset[0])].c_str());
        DrawManager::startText();
    }
    static void mcstart()
    {
        needlockX = true;
        needlockXX = 1;
        needlockXXFrame = 0;
        lockM.clear();
    }
};

// 添加翻译函数
wstring GetTranslatedMessage(const wstring& filepath, int type) {
    if (type == 0)
    {
        switch (stoi(userset[0])) {
        case 0: // 中文
            return L"NetTrack接收新的文件，已保存在：\n" + filepath;
        case 1: // English
            return L"NetTrack received a new file, saved at: \n" + filepath;
        case 2: // 日本語
            return L"NetTrackが新しいファイルを受信し、保存しました：\n" + filepath;
        case 3: // 한국어
            return L"NetTrack에서 새 파일을 수신하여 저장했습니다：\n" + filepath;
        case 4: // Français
            return L"NetTrack a reçu un nouveau fichier, enregistré à：\n" + filepath;
        case 5: // Deutsch
            return L"NetTrack hat eine neue Datei empfangen, gespeichert unter：\n" + filepath;
        case 6: // Русский
            return L"NetTrack получил новый файл, сохранён в：\n" + filepath;
        case 7: // Español
            return L"NetTrack recibió un nuevo archivo, guardado en：\n" + filepath;
        default:
            return L"NetTrack received a new file, saved at: \n" + filepath;
        }
    }
    else
    {
        wstring folderSaved[8] = {
            // 中文（简体）
            L"文件夹已保存在：",

            // 英文
            L"Folder saved at: ",

            // 日语
            L"フォルダの保存先：",

            // 韩语
            L"폴더 저장 위치：",

            // 法语
            L"Dossier enregistré dans : ",

            // 德语
            L"Ordner gespeichert in: ",

            // 俄语
            L"Папка сохранена в: ",

            // 西班牙语
            L"Carpeta guardada en: "
        };
        return folderSaved[stoi(userset[0])] + L"\n" + filepath;
    }
}
void GetDownLoadFile()
{
    // 3. 获取保存路径
    wstring a = Net::popReceivedFileName();

    // 4. lastDownload安全移动
    if (a != L"NULL") {
        if (a.find(L"netPack.npk") == string::npos)
        {
            for (int i = lastDownloadMax - 1; i > 0; --i) {
                lastDownload[i] = lastDownload[i - 1];
            }
            lastDownload[0] = tool::GetCurrentTimeS() + "    " + tool::wstring_to_string(a);

            // 添加多语言通知
            wstring message = GetTranslatedMessage(a, 0);
            trd::ShowTrayMessage(mainHwnd, message.c_str(), L"NetTrack", 0);
        }
        else
        {
            // 添加多语言通知
            wstring message = GetTranslatedMessage(userSaveDir, 1);
            trd::ShowTrayMessage(mainHwnd, message.c_str(), L"NetTrack", 0);
        }
    }
}

void GetProcess()
{
    //cd 标识符 ： 0.5秒获取时间，速度，1秒获取进度
    bool cd = false;
    while (1)
    {
        api_sleep(500);
        if (linkMac == -1 || sendFileList.empty())
        {
            continue;
        }
        Speed = Net::GetSendSpeedString();
        timeNeed = Net::GetSendRemainTimeString();

        if(cd)
        {
            process = Net::GetTransferProgress();
            process_frame = 0;
            start_P = process2;
            end_P = process;
            cd = false;
        }
        else
        {
            cd = true;
        }

    }
}

int osm[6] = { -1,-1,-1,-1,-1,-1 };
void choose5OBI(int i)
{
    if (i == 0)
    {
        if (tool::RunProgramWithShellExecute(L"NetTrack.exe"))
        {
            exit(0);
        }
        else
        {
            string errorMsg[8] = {
                // 中文（简体）
                "无法启动NetTrack.exe，请检查文件路径",
                // 英文
                "Failed to start NetTrack.exe, please check the file path",
                // 日语
                "NetTrack.exeを起動できません。ファイルパスを確認してください",
                // 韩语
                "NetTrack.exe를 시작할 수 없습니다. 파일 경로를 확인하세요",
                // 法语
                "Impossible de démarrer NetTrack.exe, vérifiez le chemin du fichier",
                // 德语
                "NetTrack.exe konnte nicht gestartet werden, bitte überprüfen Sie den Dateipfad",
                // 俄语
                "Не удалось запустить NetTrack.exe, проверьте путь к файлу",
                // 西班牙语
                "No se pudo iniciar NetTrack.exe, verifique la ruta del archivo"
            };
            msg::KillBox(errorMsg[stoi(userset[0])]);
            int color[3] = { 255,100,100 };
            int fontColor[3] = { -1,-1,-1 };
            msg::Message(2, color, fontColor, 240, errorMsg[stoi(userset[0])]);
        }
    }
    if (i == 1)
    {
        if (linkMac == -1)
        {
            string noDeviceConnected[8] = {
                // 中文（简体）
                "您未连接到任何设备",

                // 英文
                "You are not connected to any device",

                // 日语
                "どのデバイスにも接続されていません",

                // 韩语
                "어떤 기기에도 연결되지 않았습니다",

                // 法语
                "Vous n'êtes connecté à aucun appareil",

                // 德语
                "Sie sind mit keinem Gerät verbunden",

                // 俄语
                "Вы не подключены ни к одному устройству",

                // 西班牙语
                "No estás conectado a ningún dispositivo"
            };
            msg::KillBox(noDeviceConnected[stoi(userset[0])]);
            int color[3] = { 255,100,100 };
            int fontColor[3] = { -1,-1,-1 };
            msg::Message(2, color, fontColor, 240, noDeviceConnected[stoi(userset[0])]);
        }
        else
        {
            if (!OH)
            {
                thread Mac(tool_messageWork::textLink);
                Mac.detach();
            }
            else
            {
                std::string linkWait[8] = {
               "等待之前的验证任务",  // 中文
               "Waiting for previous verification task",  // 英文
              "前の検証タスクを待機中",  // 日语
              "이전 검증 작업 대기 중",  // 韩文
             "En attente de la tâche de vérification précédente",  // 法语
             "Warten auf vorherige Verifizierungsaufgabe",  // 德语
              "Ожидание предыдущей задачи проверки",  // 俄语
                  "Esperando la tarea de verificación anterior"  // 西班牙语
                };
                msg::KillBox(linkWait[stoi(userset[0])]);
                int c[3] = { 255,255,100 };
                int fc[3] = { -1,-1,-1 };
                msg::Message(1, c, fc, 240, linkWait[stoi(userset[0])]);
            }
        }
    }
    if (i == 2)
    {
        if (osm[i] == -1)
        {
            string confirmMsg10s[8] = {
                // 中文（简体）
                "请确认，10秒后自动取消",

                // 英文
                "Confirm, auto-cancel in 10s",

                // 日语
                "確認してください（10秒で自動取消）",

                // 韩语
                "확인하세요 (10초 후 자동취소)",

                // 法语
                "Confirmer (annul. auto. 10s)",

                // 德语
                "Bestätigen (in 10s automat. Abbruch)",

                // 俄语
                "Подтвердите (отмена через 10 сек)",

                // 西班牙语
                "Confirme (se cancela en 10s)"
            };
            int color[3] = { 255,255,100 };
            int fontColor[3] = { -1,-1,-1 };
            msg::Message(3, color, fontColor, 0, confirmMsg10s[stoi(userset[0])]);
            osm[i] = 600;
            return;
        }
        if (osm[i] > 0)
        {
            string translationsB[8] = {
                // 中文 (简体)
                "此操作将会初始化NetTrack，您的配置文件和历史都会被清除，除非您知道自己正在做什么并且很清楚自己需要此操作，否则请取消",

                // English
                "This operation will initialize NetTrack. All your configuration files and history will be erased. Unless you know exactly what you're doing and fully understand the consequences, please cancel.",

                // 日本語
                "この操作はNetTrackを初期化します。設定ファイルと履歴はすべて削除されます。この操作の影響を完全に理解している場合を除き、キャンセルしてください。",

                // 한국어
                "이 작업은 NetTrack을 초기화합니다. 모든 설정 파일과 기록이 삭제됩니다. 작업 내용과 결과를 완전히 이해하지 못한 경우 취소하십시오.",

                // Français
                "Cette opération initialisera NetTrack. Tous vos fichiers de configuration et historiques seront effacés. À moins que vous ne sachiez exactement ce que vous faites et en compreniez pleinement les conséquences, veuillez annuler.",

                // Deutsch
                "Dieser Vorgang wird NetTrack zurücksetzen. Alle Konfigurationsdateien und Verläufe werden gelöscht. Brechen Sie den Vorgang ab, es sei denn, Sie wissen genau, was Sie tun, und verstehen die Konsequenzen vollständig.",

                // Русский
                "Эта операция инициализирует NetTrack. Все ваши конфигурационные файлы и история будут удалены. Если вы не уверены в своих действиях и не понимаете последствий, отмените операцию.",

                // Español
                "Esta operación inicializará NetTrack. Todos sus archivos de configuración e historial serán eliminados. A menos que sepa exactamente lo que está haciendo y comprenda completamente las consecuencias, cancele la operación."
            };
            string translationsA[8] = {
    "NetTrack终端警告",          // 中文 (简体)
    "NetTrack Terminal Warning",  // English
    "NetTrackターミナル警告",     // 日本語
    "NetTrack 터미널 경고",      // 한국어
    "Avertissement du terminal NetTrack",  // Français
    "NetTrack-Terminalwarnung",            // Deutsch
    "Терминальное предупреждение NetTrack", // Русский
    "Advertencia de terminal NetTrack"      // Español
            };
            if (1 == msgBox::msgbox(hInstanceTop, translationsA[stoi(userset[0])], translationsB[stoi(userset[0])], WinPrompt::ICON_WARNING, WinPrompt::BTN_OKCANCEL, WinPrompt::MODE_WARNING))
            {
                remove("config.ini");
                remove("config.dll");
                remove("Record_download.dll");
                remove("Record_upload.dll");
                remove("ts.dll");
                system("start NetTrack.exe");
                exit(0);
            }
            else
            {
                string confirmMsg10s[8] = {
                    // 中文（简体）
                    "请确认，10秒后自动取消",

                    // 英文
                    "Confirm, auto-cancel in 10s",

                    // 日语
                    "確認してください（10秒で自動取消）",

                    // 韩语
                    "확인하세요 (10초 후 자동취소)",

                    // 法语
                    "Confirmer (annul. auto. 10s)",

                    // 德语
                    "Bestätigen (in 10s automat. Abbruch)",

                    // 俄语
                    "Подтвердите (отмена через 10 сек)",

                    // 西班牙语
                    "Confirme (se cancela en 10s)"
                };
                msg::KillBox(confirmMsg10s[stoi(userset[0])]);
            }
        }
    }
    if (i == 3)
    {
        trustList.clear();
        string translations[8] = {
            // 中文 (简体)
            "已清除所有的信任列表，所有设备在下次连接您的NetTrack时都会安全警告",

            // English
            "All trusted lists have been cleared. All devices will receive security warnings when connecting to your NetTrack.",

            // 日本語
            "すべての信頼リストが消去されました。次回NetTrackに接続するすべてのデバイスにセキュリティ警告が表示されます。",

            // 한국어
            "모든 신뢰 목록이 지워졌습니다. 다음에 NetTrack에 연결하는 모든 장치에 보안 경고가 표시됩니다.",

            // Français
            "Toutes les listes de confiance ont été effacées. Tous les appareils recevront des avertissements de sécurité lors de la connexion à votre NetTrack.",

            // Deutsch
            "Alle Vertrauenslisten wurden gelöscht. Alle Geräte erhalten Sicherheitswarnungen bei der Verbindung mit Ihrem NetTrack.",

            // Русский
            "Все доверенные списки очищены. Все устройства будут получать предупреждения безопасности при подключении к вашему NetTrack.",

            // Español
            "Se han borrado todas las listas de confianza. Todos los dispositivos recibirán advertencias de seguridad al conectarse a su NetTrack."
        };
        thread a(msgBox::msgbox, hInstanceTop, "NetTrack", translations[stoi(userset[0])], WinPrompt::ICON_INFORMATION, WinPrompt::BTN_OK, WinPrompt::MODE_INFO);
        a.detach();
    }
    if (i == 4)
    {
        Net::exit();
        exit(0xFF);
    }
    if (i == 5)
    {
        ofstream del("del");
        del << "0.1bf";
        del.close();
        Net::exit();
        exit(0xFF);
    }
}

bool isGotoChoose4 = false;//是否进入设置详细选项
int isGotoChoose4Choose = -1;//进入设置详细选项的选择
int isGotoChoose4_frame = 60;//进入设置详细选项的帧数
PIMAGE isGotoChoose4Image = newimage();//进入设置详细选项的过渡图片
int isGotoChoose4XChange = 0;//进入设置详细选项的X轴变化(通用)

int whleHistory = 0;

bool isOpen = false;

PIMAGE screenShot = newimage();

bool Manager = false;

PIMAGE gogogo = newimage();

class html
{
public:
    static void ShowGuidelineHtml(wstring file, wstring title)
    {
        isOpen = true;
        HtmlViewer viewer;
        viewer.setLg(stoi(userset[0]));
        viewer.LoadHtmlFromResource(file.c_str(), title);
        isOpen = false;
        string name = "NetTrackUse" + userset[0] + ".html";
        remove(name.c_str());
    }

    static void showHtml(wstring file, wstring title = L"NetTrack")
    {
        if(!isOpen)
        {
            Manager = true;

            PIMAGE temp = newimage();
            getimage(temp, screenShot, 0, 0, windowSize.x, windowSize.y);
            img::BlurWithDownscale_Gaussian(temp, 10, 5);
            cleardevice();
            putimage(0, 0, temp);
            html::ShowGuidelineHtml(file, title);

            Manager = false;
            delimage(temp);

            UnregisterClass(L"EnhancedHtmlWnd", GetModuleHandle(NULL));
        }
    }
};

void choose6Choose(int MSK_TEMP)
{
    int MSK = MSK_TEMP + 1;
    if (MSK == 1)
    {
        int color[3] = { 150,150,150 };
        int FontColor[3] = { 0,0,0 };
        string b[3] = { "OK","NULL","NULL" };
        msg::MessageBar(color, FontColor, "NetTrack - v0.1_bf_Beta", b);
    }
    if (MSK == 2)
    {
        ifstream update("UpdateLog.log");
        if (!update.is_open())
        {
            string noChangelogMsg[8] = {
        "找不到或不存在更新日志",        // Chinese (Simplified)
        "No changelog found or doesn't exist",  // English
     "更新履歴が見つからないか存在しません",  // Japanese
      "변경 로그를 찾을 수 없거나 존재하지 않습니다",  // Korean
      "Aucun journal des modifications trouvé ou inexistant",  // French
      "Kein Änderungsprotokoll gefunden oder nicht vorhanden",  // German
      "Журнал изменений не найден или не существует",  // Russian
      "No se encontró el registro de cambios o no existe"  // Spanish
            };
            int c[3] = { 255,150,150 };
            int fc[3] = { -1,-1,-1 };
            msg::Message(2, c, fc, 240, noChangelogMsg[stoi(userset[0])]);
            update.close();
        }
        else
        {
            update.close();
            CopyFileW(L"UpdateLog.log", L"UpdateLog.html",0);
            system("start UpdateLog.html");
        }
    }
    if (MSK == 3)
    {
		if(!isOpen)
        {
            if (1 == FileControl::UnpackFilesFromOne("NetTrackHt.dll", ""))
            {
                for (int i = 0; i < 8; i++)
                {
                    if (i != stoi(userset[0]))
                    {
                        string name = "NetTrackUse" + to_string(i) + ".html";
                        remove(name.c_str());
                    }
                }
                std::wstring netTrackGuidelines[8] = {
                       L"NetTrack使用准则",          // Chinese (Simplified)
                     L"NetTrack Usage Guidelines",  // English
                      L"NetTrack利用規約",         // Japanese
                     L"NetTrack 사용 지침",       // Korean
                      L"Règles d'utilisation de NetTrack",  // French
                       L"NetTrack-Nutzungsrichtlinien",  // German
                       L"Правила использования NetTrack",  // Russian
                       L"Pautas de uso de NetTrack"  // Spanish
                };
                wstring name = L"NetTrackUse" + tool::string_to_wstring(userset[0]) + L".html";

                html::showHtml(name, netTrackGuidelines[stoi(userset[0])]);
            }
        }
        else
        {
            HWND MK = FindWindowA("EnhancedHtmlWnd", NULL);
            ShowWindow(MK, SW_SHOWNORMAL);
        }
        isOpen = true;
    }
    if (MSK == 4)
    {
        MSG msg = {};
        PIMAGE nek = newimage();
        getimage(nek, screenShot, 0, 0, windowSize.x, windowSize.y);
        img::BlurWithDownscale_Gaussian(nek, 7, 2);

        int frame = 0;
        int Y = 0;

        wstring supportOptionsW[8][2] = {
    {L"报告BUG", L"支持我们"},     // Chinese
    {L"Report Bug", L"Support Us"}, // English
    {L"バグ報告", L"サポート"},    // Japanese
    {L"버그 제보", L"후원하기"},   // Korean
    {L"Signaler un bug", L"Nous soutenir"}, // French
    {L"Fehler melden", L"Unterstützen Sie uns"}, // German
    {L"Сообщить об ошибке", L"Поддержите нас"}, // Russian
    {L"Reportar error", L"Apóyanos"} // Spanish
        };
        wstring githubStarRequestW[8] = {
    L"在GitHub上为NetTrack打星",      // Chinese
    L"Star NetTrack on GitHub",       // English
    L"GitHubでNetTrackをスターする",  // Japanese
    L"GitHub에서 NetTrack 스타하기",  // Korean
    L"Étoilez NetTrack sur GitHub",   // French
    L"NetTrack auf GitHub mit Stern bewerten", // German
    L"Поставьте звезду NetTrack на GitHub", // Russian
    L"Dale una estrella a NetTrack en GitHub" // Spanish
        };

        wstring msgA = supportOptionsW[stoi(userset[0])][0] + L" : " + L"Qixi_Ya@outlook.com";
        wstring msgB = githubStarRequestW[stoi(userset[0])] + L" : " + L"https://github.com/QiXi-ya/NetTrack   ★";
        wstring msgC = supportOptionsW[stoi(userset[0])][1] + L"  (p≧w≦q)";

        for (int i = 0; i < 16; i++)
        {
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            delay_fps(100);
            cleardevice();
            putimage(0, 0, screenShot);

            putimage_alphablend(NULL, nek, 0, 0, i * 16);

            if (i * 16 > 150)
            {
                if (DarkMode)setfillcolor(EGERGBA(0, 0, 0, i * 16 - 150));
                else setfillcolor(EGERGBA(255, 255, 255, i * 16 - 150));
                ege_fillrect(0, 0, windowSize.x, windowSize.y);
            }

            int start = 0;
            int end = 100;
            double t = frame / (double)15;
            Y = start + (end - start) * ease::easeOut(t, 3);

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(50, Y - 50, windowSize.x - 100, 600,15);

            DrawManager::centerText();
            DrawManager::setFont(40);
            xyprintf(windowSize.x / 2, Y, msgA.c_str());
            xyprintf(windowSize.x / 2, Y + 100, msgB.c_str());
            xyprintf(windowSize.x / 2, Y + 200, msgC.c_str());
            DrawManager::startText();

            frame++;
        }


        int pngAlpha = 10;

        while (!keystate(0x01))
        {
            delay_fps(60);
            cleardevice();
            putimage_alphablend(NULL, nek, 0, 0, 240);
            if (DarkMode)setfillcolor(EGERGBA(0, 0, 0,90));
            else setfillcolor(EGERGBA(255, 255, 255,90));
            ege_fillrect(0, 0, windowSize.x, windowSize.y);

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(50, Y - 50, windowSize.x - 100, 600, 15);

            mousePos = Window::GetMousePosX();

            DrawManager::centerText();
            DrawManager::setFont(40);
            xyprintf(windowSize.x / 2, Y, msgA.c_str());
            xyprintf(windowSize.x / 2, Y + 100, msgB.c_str());
            xyprintf(windowSize.x / 2, Y + 200, msgC.c_str());

            if (getwidth(gogogo) > 0)
            {
                putimage_alphablend(NULL, gogogo, windowSize.x / 2 - getwidth(gogogo) / 2, Y + 300, pngAlpha);
            }
            else
            {
                setcolor(EGERGB(255, 100, 100));
                xyprintf(windowSize.x / 2, Y + 200, L"ERROR");
            }
            DrawManager::startText();


            if (mousePos.y > Y + 270 && mousePos.y < Y + 330 + getheight(gogogo) && mousePos.x > windowSize.x / 2 - getwidth(gogogo) / 2 - 30 && mousePos.x < windowSize.x / 2 + getwidth(gogogo) / 2 + 30)
            {
                if (pngAlpha < 255)
                {
                    if (pngAlpha + 8 < 255)pngAlpha += 8;
                    else pngAlpha = 255;
                }
            }
            else
            {
                if (pngAlpha > 10)
                {
                    if (pngAlpha - 8 > 10) pngAlpha -= 8;
                    else pngAlpha = 10;
                }
            }

            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        }

        frame = 0;

        for (int i = 15; i >= 0; i--)
        {
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            delay_fps(60);
            cleardevice();
            putimage(0, 0, screenShot);

            putimage_alphablend(NULL, nek, 0, 0, i * 16);

            if (i * 16 > 150)
            {
                if (DarkMode)setfillcolor(EGERGBA(0, 0, 0, i * 16 - 150));
                else setfillcolor(EGERGBA(255, 255, 255, i * 16 - 150));
                ege_fillrect(0, 0, windowSize.x, windowSize.y);
            }

            int start = 100;
            int end = -200;
            double t = frame / (double)15;
            Y = start + (end - start) * ease::easeInBack(t, 3);

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(50, Y - 50, windowSize.x - 100, 600, 15);

            DrawManager::centerText();
            DrawManager::setFont(40);
            xyprintf(windowSize.x / 2, Y, msgA.c_str());
            xyprintf(windowSize.x / 2, Y + 100, msgB.c_str());
            xyprintf(windowSize.x / 2, Y + 200, msgC.c_str());
            DrawManager::startText();

            frame++;
        }
    }
    if (MSK == 5)
    {
        MSG msg = {};
        PIMAGE nek = newimage();
        getimage(nek, screenShot, 0, 0, windowSize.x, windowSize.y);
        img::BlurWithDownscale_Gaussian(nek, 7, 2);

        int frame = 0;
        int Y = 0;

        wstring emailTextW[8] = {
    L"邮箱",         // Chinese
    L"Email",       // English
    L"メールアドレス", // Japanese
    L"이메일",      // Korean
    L"Email",       // French
    L"E-Mail",      // German
    L"Электронная почта", // Russian
    L"Correo electrónico" // Spanish
        };

        wstring msgA = emailTextW[stoi(userset[0])] + L" : " + L"Qixi_Ya@outlook.com";
        wstring msgB = L"GitHub : https://github.com/QiXi-ya/NetTrack";
        wstring msgC = L"Bilibili : https://space.bilibili.com/3494360421238894?spm_id_from=333.337.0.0";
        wstring msgD = L"YouTube : https://www.youtube.com/@QiXi_Ya";

        for (int i = 0; i < 16; i++)
        {
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            delay_fps(100);
            cleardevice();
            putimage(0, 0, screenShot);

            putimage_alphablend(NULL, nek, 0, 0, i * 16);

            if (i * 16 > 150)
            {
                if (DarkMode)setfillcolor(EGERGBA(0, 0, 0, i * 16 - 150));
                else setfillcolor(EGERGBA(255, 255, 255, i * 16 - 150));
                ege_fillrect(0, 0, windowSize.x, windowSize.y);
            }

            int start = 0;
            int end = 100;
            double t = frame / (double)15;
            Y = start + (end - start) * ease::easeOut(t, 3);

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(50, Y - 50, windowSize.x - 100, 500, 15);

            DrawManager::centerText();
            DrawManager::setFont(40);
            xyprintf(windowSize.x / 2, Y, msgA.c_str());
            xyprintf(windowSize.x / 2, Y + 100, msgB.c_str());
            xyprintf(windowSize.x / 2, Y + 200, msgC.c_str());
            xyprintf(windowSize.x / 2, Y + 300, msgD.c_str());
            DrawManager::startText();

            frame++;
        }

        int temp = 0;

        while (1)
        {
            delay_fps(60);
            cleardevice();
            putimage_alphablend(NULL, nek, 0, 0, 240);
            if (DarkMode)setfillcolor(EGERGBA(0, 0, 0, 90));
            else setfillcolor(EGERGBA(255, 255, 255, 90));
            ege_fillrect(0, 0, windowSize.x, windowSize.y);

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(50, Y - 50, windowSize.x - 100, 500, 15);

            bool ohx = false;
            for (int i = 0; i < 3; i++)
            {
                if (mousePos.y > Y + 60 + i * 100 && mousePos.y < Y + 140 + i * 100)
                {
                    temp = i + 1;
                    ohx = true;
                    break;
                }
            }
            if (!ohx)
            {
                temp = 0;
            }

            DrawManager::centerText();
            DrawManager::setFont(40);
            xyprintf(windowSize.x / 2, Y, msgA.c_str());
            if(temp == 1)
            {
                xyprintf(windowSize.x / 2, Y + 200, msgC.c_str());
                xyprintf(windowSize.x / 2, Y + 300, msgD.c_str());
                setcolor(EGERGB(100, 150, 255));
                xyprintf(windowSize.x / 2, Y + 100, msgB.c_str());
            }
            else if (temp == 2)
            {
                xyprintf(windowSize.x / 2, Y + 300, msgD.c_str());
                xyprintf(windowSize.x / 2, Y + 100, msgB.c_str());
                setcolor(EGERGB(100, 150, 255));
                xyprintf(windowSize.x / 2, Y + 200, msgC.c_str());
            }
            else if (temp == 3)
            {
                xyprintf(windowSize.x / 2, Y + 100, msgB.c_str());
                xyprintf(windowSize.x / 2, Y + 200, msgC.c_str());
                setcolor(EGERGB(100, 150, 255));
                xyprintf(windowSize.x / 2, Y + 300, msgD.c_str());
            }
            else
            {
                xyprintf(windowSize.x / 2, Y + 100, msgB.c_str());
                xyprintf(windowSize.x / 2, Y + 200, msgC.c_str());
                xyprintf(windowSize.x / 2, Y + 300, msgD.c_str());
            }
            DrawManager::startText();

            mousePos = Window::GetMousePosX();

            if (keystate(0x01))
            {
                
                        if (temp == 1) ShellExecuteW(NULL, L"open", L"https://github.com/QiXi-ya/NetTrack", NULL, NULL, SW_SHOWNORMAL);
                        if (temp == 2) ShellExecuteW(NULL, L"open", L"https://space.bilibili.com/3494360421238894?spm_id_from=333.337.0.0", NULL, NULL, SW_SHOWNORMAL);
                        if (temp == 3) ShellExecuteW(NULL, L"open", L"https://www.youtube.com/@QiXi_Ya", NULL, NULL, SW_SHOWNORMAL);
                        break;
            }

            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        }

        frame = 0;

        for (int i = 15; i >= 0; i--)
        {
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            delay_fps(60);
            cleardevice();
            putimage(0, 0, screenShot);

            putimage_alphablend(NULL, nek, 0, 0, i * 16);

            if (i * 16 > 150)
            {
                if (DarkMode)setfillcolor(EGERGBA(0, 0, 0, i * 16 - 150));
                else setfillcolor(EGERGBA(255, 255, 255, i * 16 - 150));
                ege_fillrect(0, 0, windowSize.x, windowSize.y);
            }

            int start = 100;
            int end = -200;
            double t = frame / (double)15;
            Y = start + (end - start) * ease::easeInBack(t, 3);

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(50, Y - 50, windowSize.x - 100, 500, 15);

            DrawManager::centerText();
            DrawManager::setFont(40);
            xyprintf(windowSize.x / 2, Y, msgA.c_str());
            xyprintf(windowSize.x / 2, Y + 100, msgB.c_str());
            xyprintf(windowSize.x / 2, Y + 200, msgC.c_str());
            xyprintf(windowSize.x / 2, Y + 300, msgD.c_str());
            DrawManager::startText();

            frame++;
        }
    }
}


int lgChange = 0;
int lgChangeN = -1;
int fuckNMD = 0;
int fuckNMD_frame = 120;


int MFCl = 20;
int MFCl_frame = 60;
int MFClChange = 0;

int tempChange = -1;
int tempChangeN = -1;

bool MDcaonimadeshishenmeguidongxi = false;

vector<std::string> splitAsciiStringByLength(const std::string& input,
    size_t chunk_length) {
    if (chunk_length <= 0) {
        vector<std::string> result = { "ERROR" };
        return result;
    }

    std::vector<std::string> result;
    for (size_t i = 0; i < input.size(); i += chunk_length) {
        result.push_back(input.substr(i, chunk_length));
    }
    return result;
}

void replaceInStringArray(std::string arr[], size_t size,
    size_t strIdx, size_t charIdx, char newChar) {
    if (strIdx >= size) return;

    std::string& str = arr[strIdx];
    if (charIdx < str.size()) {
        str[charIdx] = newChar;
    }
}


void choose4ChooseKey()
{
    if(isGotoChoose4XChange <= 50 && !needlockX)
    {
        int Choose4Choose = isGotoChoose4Choose + 1;

        if (Choose4Choose == 2)
        {
            for (int i = 0; i < 3; i++)
            {
                if (keystate(0x01))
                {
                    if (isKEY)
                    {
                        if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20 && mousePos.y > 40 + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + (i + 1) * 120 + whle / 5 + 100)
                        {
                            if (stoi(userset[3]) != 0 && i == 2)
                            {
                                tempChangeN = 3;
                                tempChange = 0;
                                MFCl_frame = 0;
                                MFClChange = 0;
                            }
                            if (stoi(userset[3]) != 1 && i == 0)
                            {
                                tempChangeN = 3;
                                tempChange = 1;
                                MFCl_frame = 0;
                                MFClChange = 0;
                            }
                            if (stoi(userset[3]) != 2 && i == 1)
                            {
                                tempChangeN = 3;
                                tempChange = 2;
                                MFCl_frame = 0;
                                MFClChange = 0;
                            }
                        }
                    }
                }
                else
                {
                    isKEY = true;
                }
            }
        }
        if (Choose4Choose == 4)
        {
            if (keystate(0x01))
            {
                if (isKEY)
                {
                    int i = 1;
                    if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20 && mousePos.y > 40 + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + (i + 1) * 120 + whle / 5 + 100)
                    {
                        std::wstring selectSaveLocation[8] = {
                        L"选择默认保存新文件的位置",         // Chinese (Simplified)
                         L"Select default location for new files",  // English
                         L"新規ファイルの保存先を選択",        // Japanese
                         L"새 파일의 기본 저장 위치 선택",     // Korean
                         L"Sélectionner l'emplacement par défaut pour les nouveaux fichiers",  // French
                       L"Standardspeicherort für neue Dateien auswählen",  // German
                     L"Выбрать расположение для новых файлов",  // Russian
                      L"Seleccionar ubicación predeterminada para nuevos archivos"  // Spanish
                        };
                        string MKS = chooseFile::OpenFolderDialog(selectSaveLocation[stoi(userset[0])]);

                        if (!MKS.empty() && MKS != "ERROR")
                        {
                            userSaveDir = tool::string_to_wstring(MKS);
                            userset[4] = MKS;
                            string applyAfterReboot[8] = {
    "重启后应用该设置",       // Chinese (Simplified)
    "Apply this setting after reboot",  // English
    "再起動後にこの設定を適用",  // Japanese
    "재부팅 후 이 설정 적용",  // Korean
    "Appliquer ce paramètre après redémarrage",  // French
    "Diese Einstellung nach Neustart anwenden",  // German
    "Применить эту настройку после перезагрузки",  // Russian
    "Aplicar esta configuración después de reiniciar"  // Spanish
                            };

                            int c[3] = { 150,255,255 };
                            int fc[3] = { -1,-1 ,-1 };
                            msg::Message(1, c, fc, 240, applyAfterReboot[stoi(userset[0])]);
                        }
                        if (MKS == "ERROR")
                        {
                            int color[3] = { 255,100,100 };
                            int fontColor[3] = { -1,-1,-1 };

                            string errorCuteMessage[8] = {
        "出错啦 >_<",               // Chinese (Simplified)
        "Oopsie! >_<",             // English
        "エラーだよ >_<",           // Japanese
        "오류 발생! >_<",           // Korean
        "Oups ! >_<",              // French
        "Huch! >_<",               // German
        "Ой-ой! >_<",              // Russian
        "¡Ups! >_<"                // Spanish
                            };

                            string mos = errorCuteMessage[stoi(userset[0])];

                            msg::Message(2, color, fontColor, 420, mos);
                        }
                    }
                }
            }
            else
            {
                isKEY = true;
            }
        }
        if (Choose4Choose == 5)
        {
            for (int i = 0; i < 8; i++)
            {
                if (keystate(0x01))
                {
                    if (isKEY)
                    {
                        if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20 && mousePos.y > 40 + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + (i + 1) * 120 + whle / 5 + 100)
                        {
                            if(stoi(userset[0]) != i)
                            {
                                lgChange = 1;
                                lgChangeN = i;
                                fuckNMD_frame = 0;
                            }
                        }
                    }
                }
                else
                {
                    isKEY = true;
                }
            }
        }
        if (Choose4Choose == 7)
        {
            for (int i = 0; i < 3; i++)
            {
                if (keystate(0x01))
                {
                    if (isKEY)
                    {
                        if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20 && mousePos.y > 40 + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + (i + 1) * 120 + whle / 5 + 100)
                        {
                            if (stoi(userset[7]) != i)
                            {
                                tempChangeN = 7;
                                tempChange = i;
                                MFCl_frame = 0;
                                MFClChange = 0;
                            }
                        }
                    }
                }
                else
                {
                    isKEY = true;
                }
            }
        }
        if (Choose4Choose == 10)
        {
            for (int i = 0; i < 4; i++)
            {
                if (keystate(0x01))
                {
                    if (isKEY)
                    {
                        if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20 && mousePos.y > 40 + (i + 2) * 120 + whle / 5 && mousePos.y < 40 + (i + 2) * 120 + whle / 5 + 100)
                        {
                            if (stoi(userset[1]) != i)
                            {
                                tempChangeN = 1;
                                tempChange = i;
                                MFCl_frame = 0;
                                MFClChange = 0;
                            }
                        }
                    }
                }
                else
                {
                    isKEY = true;
                }
            }
        }
        if (Choose4Choose == 11)
        {
            for (int i = 0; i < 2; i++)
            {
                if (keystate(0x01))
                {
                    if (isKEY)
                    {
                        if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20 && mousePos.y > 40 + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + (i + 1) * 120 + whle / 5 + 100)
                        {
                            if (stoi(userset[9]) != i)
                            {
                                tempChangeN = 9;
                                tempChange = i;
                                MFCl_frame = 0;
                                MFClChange = 0;
                            }
                        }
                    }
                }
                else
                {
                    isKEY = true;
                }
            }
        }
    }
}

bool fuck_AH = false;
int fuck_AHFrame = 60;
int fuck_AHX = 0;

bool upLink = false;
int upLinkY = 100000;
int upLinkFrame = 60;

void DChoose2(int userBarChoose)
{
    const int userSetCircle = 15;

    if (userBarChoose != 4 && !MDcaonimadeshishenmeguidongxi)
    {
        isGotoChoose4 = false;
        isGotoChoose4Choose = -1;
        whle = whleHistory;
        isGotoChoose4_frame = 60;
        MDcaonimadeshishenmeguidongxi = true;
    }

    if(upLinkY < windowSize.y - 100)
    {
        if (keystate(0x01))
        {
            if (isKEY && !needlockX && linkJD != "N")
            {
                if (mousePos.x > windowSize.x - 120 && mousePos.x < windowSize.x - 20 && mousePos.y > windowSize.y - 120 && mousePos.y < windowSize.y - 20)
                {
                    if (fuck_AH)
                    {
                        fuck_AH = false;
                        fuck_AHFrame = 0;
                        pair<string, unsigned short> a = code::decode_ip_port(linkJD);
                        Net::SendXorString(a.first, a.second + 1, "linkStopPlease");
                        std::string waitForDisconnect[8] = {
    "等待对方停止连接",          // Chinese (Simplified)
    "Waiting for the peer to disconnect",  // English
    "相手の切断を待機中",        // Japanese
    "상대방의 연결 해제를 기다리는 중",  // Korean
    "En attente de la déconnexion de l'appareil distant",  // French
    "Warte auf Trennung durch die Gegenstelle",  // German
    "Ожидание отключения удалённой стороны",  // Russian
    "Esperando a que el dispositivo remoto se desconecte"  // Spanish
                        };
                        int c[3] = { 155,255,200 };
                        int fc[3] = { -1,-1,-1 };
                        msg::Message(3,c,fc,0,waitForDisconnect[stoi(userset[0])]);
                    }
                    else
                    {
                        fuck_AH = true;
                        fuck_AHFrame = 0;
                    }
                    isKEY = false;
                }
                else
                {
                    if (fuck_AH)
                    {
                        fuck_AH = false;
                        fuck_AHFrame = 0;
                        isKEY = false;
                    }
                }
            }
        }
        else
        {
            isKEY = true;
        }
    }
    else if(userBarChoose != 1 ||(userBarChoose == 1 && AddLinkXChange > windowSize.x / 4))
    {
            if (fuck_AH)
            {
                fuck_AH = false;
                fuck_AHFrame = 0;
            }
    }

    if (fuck_AHFrame < 60)
    {
        if (fuck_AH)
        {
            int startValue = 0;
            int endValue = 100;
            double t = fuck_AHFrame / (double)60;
            fuck_AHX = startValue + (endValue - startValue) * ease::easeOut(t, 10);
            fuck_AHFrame++;
        }
        else
        {
            int startValue = 100;
            int endValue = 0;
            double t = fuck_AHFrame / (double)60;
            fuck_AHX = startValue + (endValue - startValue) * ease::easeOutBack(t, 2);
            fuck_AHFrame++;
        }
    }

    if (userBarChoose == 1)
    {
        if (AddLink)
        {
            if (AddLink_frame < AddLink_totalFrame)
            {
                int startValue = 0;
                int endValue = windowSize.x;
                double t = AddLink_frame / (double)AddLink_totalFrame;
                AddLinkXChange = startValue + (endValue - startValue) * ease::easeInOut(t, 8);
                AddLink_frame++;
            }
            else
            {
                AddLinkXChange = windowSize.x;
            }
        }
        else
        {
            if (AddLink_frame < AddLink_totalFrame)
            {
                int startValue = windowSize.x;
                int endValue = 0;
                double t = AddLink_frame / (double)AddLink_totalFrame;
                AddLinkXChange = startValue + (endValue - startValue) * ease::easeOut(t, 6);
                AddLink_frame++;
            }
            else
            {
                AddLinkXChange = 0;
            }
        }

        if(mousePos.y < windowSize.y - 120 || AddLinkXChange > windowSize.x / 4)
        {
            if (AddLinkXChange > windowSize.x / 4)
            {
                if (keystate(0x1b) && !needlockX)
                {
                    AddLink = false;
                    AddLink_frame = 0;
                    for (int j = 0; j < 8; j++)
                    {
                        linkCodeA[j] = '?';
                        userLink_fontFrameA[j] = 0;
                    }
                }
                if (keystate(0x01))
                {
                    if (isKEY && !needlockX)
                    {
                        isKEY = false;
                        if (mousePos.x > 100 + windowSize.x - AddLinkXChange && mousePos.x < 100 + windowSize.x - AddLinkXChange + 150 && mousePos.y > 120 && mousePos.y < 180)
                        {
                            AddLink = false;
                            AddLink_frame = 0;
                            for (int j = 0; j < 8; j++)
                            {
                                linkCodeA[j] = '?';
                                userLink_fontFrameA[j] = 0;
                            }
                        }
                        if (mousePos.x > windowSize.x - 370 + windowSize.x - AddLinkXChange && mousePos.x < windowSize.x - 370 + windowSize.x - AddLinkXChange + 150 && mousePos.y > windowSize.y - 100 && mousePos.y < windowSize.y - 100 + 60)
                        {
                            AddLink = false;
                            AddLink_frame = 0;
                            for (int j = 0; j < 8; j++)
                            {
                                linkCodeA[j] = '?';
                                userLink_fontFrameA[j] = 0;
                            }
                        }
                        if (mousePos.x > windowSize.x - 200 + windowSize.x - AddLinkXChange && mousePos.x < windowSize.x - 200 + windowSize.x - AddLinkXChange + 150 && mousePos.y > windowSize.y - 100 && mousePos.y < windowSize.y - 100 + 60)
                        {
                            bool isRe = false;
                            string a = linkCodeA;
                            for (int i = 0; i < linkMax; i++)
                            {
                                if (saveData_link[i] == a)
                                {
                                    isRe = true;
                                    break;
                                }
                            }
                            if (isRe)
                            {
                                string linkExistsMsg[8] = {
                                    // 中文（简体）
                                    "已经存在这个链接了",

                                    // 英文
                                    "This link already exists",

                                    // 日语
                                    "このリンクは既に存在します",

                                    // 韩语
                                    "이 링크가 이미 존재합니다",

                                    // 法语
                                    "Ce lien existe déjà",

                                    // 德语
                                    "Dieser Link existiert bereits",

                                    // 俄语
                                    "Эта ссылка уже существует",

                                    // 西班牙语
                                    "Este enlace ya existe"
                                };
                                int color[3] = { 255,255,100 };
                                int fontColor[3] = { -1,-1,-1 };
                                msg::Message(1, color, fontColor, 300, linkExistsMsg[stoi(userset[0])]);
                            }
                            else
                            {
                                bool isFull = true;
                                for (int i = 0; i < 8; i++)
                                {
                                    if (linkCodeA[i] == '?')
                                    {
                                        isFull = false;
                                        break;
                                    }
                                }

                                if (isFull)
                                {
                                    bool OKS = false;
                                    for (int i = 0; i < saveMax; i++)
                                    {
                                        if (saveData_link[i] == "NULL")
                                        {
                                            OKS = true;
                                            saveData_link[i] = linkCodeA;
                                            linkOS = i;
                                            break;
                                        }
                                    }
                                    if (!OKS)
                                    {
                                        int color[3] = { 255,100,100 };
                                        int fontColor[3] = { 0,0,0 };
                                        string errorMsg[8] = {
                                            // 中文（简体）
                                          "没有多余的链接位置了",

                                          // 英文
                                        "No available connection slots left",

                                        // 日语
                                    "接続可能な空きがありません",

                                    // 韩语
                                     "연결 가능한 여유 슬롯이 없습니다",

                                     // 法语
                                     "Aucun emplacement de connexion disponible",

                                     // 德语
                                     "Keine freien Verbindungsplätze verfügbar",

                                     // 俄语
                                 "Нет свободных мест для подключения",

                                 // 西班牙语
                              "No hay espacios de conexión disponibles"
                                        };
                                        msg::KillBox(errorMsg[stoi(userset[0])]);
                                        msg::Message(2, color, fontColor, 600, errorMsg[stoi(userset[0])]);
                                    }
                                    else
                                    {
                                        int color[3] = { 100,255,100 };
                                        int fontColor[3] = { 0,0,0 };
                                        string connectMsg[8][2] = {
                                            // 中文（前句，后句）
                                            {"添加链接成功", "您可能需要输入被连接方的密码"},

                                            // 英文
                                            {"Connection added successfully", "You may need to enter the peer's password"},

                                            // 日语
                                            {"リンクの追加に成功しました", "接続先のパスワード入力が必要かもしれません"},

                                            // 韩语
                                            {"링크 추가 성공", "상대방의 비밀번호를 입력해야 할 수 있습니다"},

                                            // 法语
                                            {"Lien ajouté avec succès", "Vous devrez peut-être saisir le mot de passe du pair"},

                                            // 德语
                                            {"Verbindung erfolgreich hinzugefügt", "Sie müssen möglicherweise das Passwort der Gegenstelle eingeben"},

                                            // 俄语
                                            {"Связь успешно добавлена", "Вам может потребоваться ввести пароль удаленной стороны"},

                                            // 西班牙语
                                            {"Enlace añadido con éxito", "Es posible que necesite ingresar la contraseña del remitente"}
                                        };
                                        string msg = connectMsg[stoi(userset[0])][0] + " : " + connectMsg[stoi(userset[0])][1];
                                        msg::Message(1, color, fontColor, 600, msg);

                                        string preparingConnectionB[8] = {
                                            // 中文（简体）
                                            "正在准备连接",

                                            // 英文
                                            "Preparing connection",

                                            // 日语
                                            "接続準備中",

                                            // 韩语
                                            "연결 준비 중",

                                            // 法语
                                            "Préparation de la connexion",

                                            // 德语
                                            "Verbindung wird vorbereitet",

                                            // 俄语
                                            "Подготовка подключения",

                                            // 西班牙语
                                            "Preparando conexión"
                                        };
                                        msg::KillBox(preparingConnectionB[stoi(userset[0])]);
                                        int colorB[3] = { 155,255,200 };
                                        int fontColorB[3] = { -1,-1,-1 };
                                        msg::Message(3, colorB, fontColorB, 0, preparingConnectionB[stoi(userset[0])]);
                                        AddLink = false;
                                        AddLink_frame = 0;

                                        pair<string, unsigned short> a = code::decode_ip_port(saveData_link[linkOS]);
                                        if (a.first == "ERROR")
                                        {
                                            std::string errormsg[8] = {
                                                // 中文 (简体)
                                                "链接码异常，请确认此链接码无误",

                                                // English
                                                "Invalid link code, please verify the code is correct",

                                                // 日本語
                                                "リンクコード異常、正しいコードか確認してください",

                                                // 한국어
                                                "링크 코드 오류, 정확한 코드인지 확인하십시오",

                                                // Français
                                                "Code de lien anormal, veuillez vérifier que ce code est correct",

                                                // Deutsch
                                                "Ungültiger Link-Code, bitte überprüfen Sie die Richtigkeit des Codes",

                                                // Русский
                                                "Неверный код подключения, пожалуйста, проверьте правильность кода",

                                                // Español
                                                "Código de enlace anómalo, por favor confirme que el código es correcto"
                                            };
                                            int c[3] = { 255,100,100 };
                                            int fc[3] = { -1,-1,-1 };
                                            msg::Message(2, c, fc, 240, errormsg[stoi(userset[0])]);
                                        }
                                        else
                                        {
                                            if (linkMac != -1)
                                            {
                                                std::string Are[8] = {
                                    "当前已经连接了其他设备，是否断开并连接添加的设备",  // 中文 (简体)
                                     "Another device is currently connected. Disconnect and connect to the new device?",  // 英文
            "他のデバイスに接続中です。切断して新しいデバイスに接続しますか？",  // 日语
            "다른 기기에 연결되어 있습니다. 연결을 끊고 새 기기를 연결하시겠습니까?",  // 韩语
            "Un autre appareil est déjà connecté. Voulez-vous vous déconnecter et connecter le nouvel appareil ?",  // 法语
            "Ein anderes Gerät ist bereits verbunden. Trennen und mit dem neuen Gerät verbinden?",  // 德语
            "Уже подключено другое устройство. Отключить его и подключить новое устройство?",  // 俄语
            "Otro dispositivo está conectado actualmente. ¿Desconectar y conectar el nuevo dispositivo?"  // 西班牙语
                                                };
                                                if (3 == msgBox::msgbox(hInstanceTop, "NetTrack", Are[stoi(userset[0])], WinPrompt::ICON_QUESTION, WinPrompt::BTN_YESNO, WinPrompt::MODE_INFO))
                                                {
                                                    if (!sendFileList.empty())
                                                    {
                                                        std::string isOK[8] = {
                                                   "文件尚未传输完成，此操作将会删除未传输的文件列表，连接将会在当前文件传输结束后断开",  // 中文 (简体)  
                                                  "The file transfer is not yet complete. This operation will delete the list of untransferred files, and the connection will be disconnected after the current file transfer ends.",  // 英文  
                                                  "ファイルの転送がまだ完了していません。この操作により未転送のファイルリストが削除され、現在のファイル転送終了後に接続が切断されます。",  // 日语  
                                                     "파일 전송이 아직 완료되지 않았습니다. 이 작업은 미전송된 파일 목록을 삭제하며, 현재 파일 전송이 끝난 후 연결이 끊어집니다.",  // 韩语  
                                                      "Le transfert de fichiers n'est pas encore terminé. Cette opération supprimera la liste des fichiers non transférés et la connexion sera interrompue une fois le transfert en cours terminé.",  // 法语  
                                                      "Der Dateitransfer ist noch nicht abgeschlossen. Dieser Vorgang löscht die Liste der nicht übertragenen Dateien, und die Verbindung wird nach Abschluss des aktuellen Dateitransfers getrennt.",  // 德语  
                                                     "Передача файлов еще не завершена. Эта операция удалит список непереданных файлов, и соединение будет разорвано после завершения текущей передачи.",  // 俄语  
                                                    "La transferencia de archivos aún no se ha completado. Esta operación eliminará la lista de archivos no transferidos y la conexión se cerrará una vez que finalice la transferencia actual."  // 西班牙语  
                                                        };
                                                        if (1 == msgBox::msgbox(hInstanceTop, "NetTrack", isOK[stoi(userset[0])], WinPrompt::ICON_WARNING, WinPrompt::BTN_OKCANCEL, WinPrompt::MODE_WARNING))
                                                        {
                                                            string kks = sendFileList[0].displayPath;
                                                            string kks2 = sendFileList[0].filePath;
                                                            sendFileList.clear();
                                                            sendFileList.push_back({ kks2,kks });

                                                            thread exit(exitPX, linkMac);
                                                            exit.detach();
                                                        }
                                                    }
                                                    else
                                                    {
                                                        Net::stopSendThread();
                                                    }
                                                    needlock::mcstart();
                                                }
                                                else
                                                {
                                                    linkOS = -1;
                                                }
                                            }
                                            else
                                            {
                                                Net::stopSendThread();
                                                needlock::mcstart();
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    int color[3] = { 255,100,100 };
                                    int fontColor[3] = { 0,0,0 };
                                    string errorMsg[8] = {
                                        // 中文（简体）
                                        "您的连接码不完整",

                                        // 英文
                                        "Your connection code is incomplete",

                                        // 日语
                                        "接続コードが不完全です",

                                        // 韩语
                                        "연결 코드가 불완전합니다",

                                        // 法语
                                        "Votre code de connexion est incomplet",

                                        // 德语
                                        "Ihr Verbindungscode ist unvollständig",

                                        // 俄语
                                        "Ваш код подключения неполный",

                                        // 西班牙语
                                        "Tu código de conexión está incompleto"
                                    };
                                    msg::KillBox(errorMsg[stoi(userset[0])]);
                                    msg::Message(2, color, fontColor, 240, errorMsg[stoi(userset[0])]);
                                }
                            }
                        }
                    }
                }
                else
                {
                    isKEY = true;
                }
                if (keystate(0x0d))
                {
                    if (isKEY_0x0d && !needlockX)
                    {
                        isKEY_0x0d = false;


                        bool isRe = false;
                        string a = linkCodeA;
                        for (int i = 0; i < linkMax; i++)
                        {
                            if (saveData_link[i] == a)
                            {
                                isRe = true;
                                break;
                            }
                        }
                        if (isRe)
                        {
                            string linkExistsMsg[8] = {
                                // 中文（简体）
                                "已经存在这个链接了",

                                // 英文
                                "This link already exists",

                                // 日语
                                "このリンクは既に存在します",

                                // 韩语
                                "이 링크가 이미 존재합니다",

                                // 法语
                                "Ce lien existe déjà",

                                // 德语
                                "Dieser Link existiert bereits",

                                // 俄语
                                "Эта ссылка уже существует",

                                // 西班牙语
                                "Este enlace ya existe"
                            };
                            int color[3] = { 255,255,100 };
                            int fontColor[3] = { -1,-1,-1 };
                            msg::Message(1, color, fontColor, 300, linkExistsMsg[stoi(userset[0])]);
                        }
                        else
                        {


                            bool isFull = true;
                            for (int i = 0; i < 8; i++)
                            {
                                if (linkCodeA[i] == '?')
                                {
                                    isFull = false;
                                    break;
                                }
                            }

                            if (isFull)
                            {
                                bool OKS = false;
                                for (int i = 0; i < saveMax; i++)
                                {
                                    if (saveData_link[i] == "NULL")
                                    {
                                        OKS = true;
                                        saveData_link[i] = linkCodeA;
                                        linkOS = i;
                                        break;
                                    }
                                }
                                if (!OKS)
                                {
                                    int color[3] = { 255,100,100 };
                                    int fontColor[3] = { 0,0,0 };
                                    string errorMsg[8] = {
                                        // 中文（简体）
                                      "没有多余的链接位置了",

                                      // 英文
                                    "No available connection slots left",

                                    // 日语
                                "接続可能な空きがありません",

                                // 韩语
                                 "연결 가능한 여유 슬롯이 없습니다",

                                 // 法语
                                 "Aucun emplacement de connexion disponible",

                                 // 德语
                                 "Keine freien Verbindungsplätze verfügbar",

                                 // 俄语
                             "Нет свободных мест для подключения",

                             // 西班牙语
                          "No hay espacios de conexión disponibles"
                                    };
                                    msg::KillBox(errorMsg[stoi(userset[0])]);
                                    msg::Message(2, color, fontColor, 600, errorMsg[stoi(userset[0])]);
                                }
                                else
                                {
                                    AddLink = false;
                                    AddLink_frame = 0;

                                    int color[3] = { 100,255,100 };
                                    int fontColor[3] = { 0,0,0 };
                                    string connectMsg[8][2] = {
                                        // 中文（前句，后句）
                                        {"添加链接成功", "您可能需要输入被连接方的密码"},

                                        // 英文
                                        {"Connection added successfully", "You may need to enter the peer's password"},

                                        // 日语
                                        {"リンクの追加に成功しました", "接続先のパスワード入力が必要かもしれません"},

                                        // 韩语
                                        {"링크 추가 성공", "상대방의 비밀번호를 입력해야 할 수 있습니다"},

                                        // 法语
                                        {"Lien ajouté avec succès", "Vous devrez peut-être saisir le mot de passe du pair"},

                                        // 德语
                                        {"Verbindung erfolgreich hinzugefügt", "Sie müssen möglicherweise das Passwort der Gegenstelle eingeben"},

                                        // 俄语
                                        {"Связь успешно добавлена", "Вам может потребоваться ввести пароль удаленной стороны"},

                                        // 西班牙语
                                        {"Enlace añadido con éxito", "Es posible que necesite ingresar la contraseña del remitente"}
                                    };
                                    string msg = connectMsg[stoi(userset[0])][0] + " : " + connectMsg[stoi(userset[0])][1];
                                    msg::Message(1, color, fontColor, 600, msg);
                                    AddLink = false;
                                    AddLink_frame = 0;

                                    pair<string, unsigned short> a = code::decode_ip_port(saveData_link[linkOS]);
                                    if (a.first == "ERROR")
                                    {
                                        std::string errormsg[8] = {
                                            // 中文 (简体)
                                            "链接码异常，请确认此链接码无误",

                                            // English
                                            "Invalid link code, please verify the code is correct",

                                            // 日本語
                                            "リンクコード異常、正しいコードか確認してください",

                                            // 한국어
                                            "링크 코드 오류, 정확한 코드인지 확인하십시오",

                                            // Français
                                            "Code de lien anormal, veuillez vérifier que ce code est correct",

                                            // Deutsch
                                            "Ungültiger Link-Code, bitte überprüfen Sie die Richtigkeit des Codes",

                                            // Русский
                                            "Неверный код подключения, пожалуйста, проверьте правильность кода",

                                            // Español
                                            "Código de enlace anómalo, por favor confirme que el código es correcto"
                                        };
                                        int c[3] = { 255,100,100 };
                                        int fc[3] = { -1,-1,-1 };
                                        msg::Message(2, c, fc, 240, errormsg[stoi(userset[0])]);
                                    }
                                    else
                                    {



                                        if (linkMac != -1)
                                        {
                                            std::string Are[8] = {
                                "当前已经连接了其他设备，是否断开并连接添加的设备",  // 中文 (简体)
                                 "Another device is currently connected. Disconnect and connect to the new device?",  // 英文
        "他のデバイスに接続中です。切断して新しいデバイスに接続しますか？",  // 日语
        "다른 기기에 연결되어 있습니다. 연결을 끊고 새 기기를 연결하시겠습니까?",  // 韩语
        "Un autre appareil est déjà connecté. Voulez-vous vous déconnecter et connecter le nouvel appareil ?",  // 法语
        "Ein anderes Gerät ist bereits verbunden. Trennen und mit dem neuen Gerät verbinden?",  // 德语
        "Уже подключено другое устройство. Отключить его и подключить новое устройство?",  // 俄语
        "Otro dispositivo está conectado actualmente. ¿Desconectar y conectar el nuevo dispositivo?"  // 西班牙语
                                            };
                                            if (3 == msgBox::msgbox(hInstanceTop, "NetTrack", Are[stoi(userset[0])], WinPrompt::ICON_QUESTION, WinPrompt::BTN_YESNO, WinPrompt::MODE_INFO))
                                            {
                                                if (!sendFileList.empty())
                                                {
                                                    std::string isOK[8] = {
                                               "文件尚未传输完成，此操作将会删除未传输的文件列表，连接将会在当前文件传输结束后断开",  // 中文 (简体)  
                                              "The file transfer is not yet complete. This operation will delete the list of untransferred files, and the connection will be disconnected after the current file transfer ends.",  // 英文  
                                              "ファイルの転送がまだ完了していません。この操作により未転送のファイルリストが削除され、現在のファイル転送終了後に接続が切断されます。",  // 日语  
                                                 "파일 전송이 아직 완료되지 않았습니다. 이 작업은 미전송된 파일 목록을 삭제하며, 현재 파일 전송이 끝난 후 연결이 끊어집니다.",  // 韩语  
                                                  "Le transfert de fichiers n'est pas encore terminé. Cette opération supprimera la liste des fichiers non transférés et la connexion sera interrompue une fois le transfert en cours terminé.",  // 法语  
                                                  "Der Dateitransfer ist noch nicht abgeschlossen. Dieser Vorgang löscht die Liste der nicht übertragenen Dateien, und die Verbindung wird nach Abschluss des aktuellen Dateitransfers getrennt.",  // 德语  
                                                 "Передача файлов еще не завершена. Эта операция удалит список непереданных файлов, и соединение будет разорвано после завершения текущей передачи.",  // 俄语  
                                                "La transferencia de archivos aún no se ha completado. Esta operación eliminará la lista de archivos no transferidos y la conexión se cerrará una vez que finalice la transferencia actual."  // 西班牙语  
                                                    };
                                                    if (1 == msgBox::msgbox(hInstanceTop, "NetTrack", isOK[stoi(userset[0])], WinPrompt::ICON_WARNING, WinPrompt::BTN_OKCANCEL, WinPrompt::MODE_WARNING))
                                                    {
                                                        string kks = sendFileList[0].displayPath;
                                                        string kks2 = sendFileList[0].filePath;
                                                        sendFileList.clear();
                                                        sendFileList.push_back({ kks2,kks });

                                                        thread exit(exitPX, linkMac);
                                                        exit.detach();
                                                    }
                                                }
                                                else
                                                {
                                                    Net::stopSendThread();
                                                }

                                                needlock::mcstart();

                                            }
                                            else
                                            {
                                                linkOS = -1;
                                            }
                                        }
                                        else
                                        {
                                            Net::stopSendThread();
                                            needlock::mcstart();
                                        }
                                    }
                                }
                            }
                            else
                            {
                                int color[3] = { 255,100,100 };
                                int fontColor[3] = { 0,0,0 };
                                string errorMsg[8] = {
                                    // 中文（简体）
                                    "您的连接码不完整",

                                    // 英文
                                    "Your connection code is incomplete",

                                    // 日语
                                    "接続コードが不完全です",

                                    // 韩语
                                    "연결 코드가 불완전합니다",

                                    // 法语
                                    "Votre code de connexion est incomplet",

                                    // 德语
                                    "Ihr Verbindungscode ist unvollständig",

                                    // 俄语
                                    "Ваш код подключения неполный",

                                    // 西班牙语
                                    "Tu código de conexión está incompleto"
                                };
                                msg::KillBox(errorMsg[stoi(userset[0])]);
                                msg::Message(2, color, fontColor, 240, errorMsg[stoi(userset[0])]);
                            }
                        }
                    }
                }
                else
                {
                    isKEY_0x0d = true;
                }
                if (kbhit() && !needlockX)
                {
                    char s = getch();
                    if (tool::isValidChar(s))
                    {

                        for (int f = 0; f < 8; f++)
                        {
                            if (linkCodeA[f] == '?')
                            {
                                linkCodeA[f] = s;
                                linkCode_tempA[f] = s;
                                userLink_fontFrameA[f] = 0;
                                break;
                            }
                        }
                    }
                }

                if (keystate(0x08))
                {
                    if (isKEY_0x08 && !needlockX)
                    {
                        for (int j = 7; j >= 0; j--)
                        {
                            if (linkCodeA[j] != '?')
                            {
                                linkCodeA[j] = '?';
                                userLink_fontFrameA[j] = 0;
                                break;
                            }
                        }
                        isKEY_0x08 = false;
                    }
                }
                else
                {
                    isKEY_0x08 = true;
                }
            }
            else
            {
                if (mousePos.x > 100 && mousePos.x < windowSize.x - 20 && mousePos.y > 40 + CBOS + (linkMax + 1) * 120 + whle / 5 && mousePos.y < 140 + CBOS + (linkMax + 1) * 120 + whle / 5)
                {
                    if (keystate(0x01))
                    {
                        if (mousePos.y > 100)
                        {
                            if (isKEY && !AddLink && !needlockX)
                            {
                                if (!isBar && msg::noBarH())
                                {
                                    AddLink = true;
                                    AddLink_frame = 0;
                                    isKEY = false;
                                    for (int j = 0; j < 8; j++)
                                    {
                                        linkCodeA[j] = linkCode_tempA[j] = '?';
                                        userLink_fontFrameA[j] = 60;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        isKEY = true;
                    }

                    if (!MacAddChange && !isBar)
                    {
                        MacAddChange = true;
                        MacAdd_frame = 0;
                    }
                }
                else
                {
                    if (MacAddChange)
                    {
                        MacAddChange = false;
                        MacAdd_frame = 0;
                    }
                }
                if (keystate(0x01))
                {
                    if (isKEY && !needlockX)
                    {
                        for (int i = 0; i < linkMax; i++)
                        {
                            if (mousePos.x > 100 - AddLinkXChange + windowSize.x - 120 - 80 && mousePos.x < 100 - AddLinkXChange + windowSize.x - 120 - 5 && mousePos.y > 40 + CBOS + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + CBOS + (i + 1) * 120 + whle / 5 + 100 && mousePos.y > 100)
                            {
                                if (msg::noBarH() && isKEY)
                                {
                                    if (LKA != i)
                                    {
                                        linkX_frame[LKA] = 0;
                                        LKA = i;
                                        linkX_frame[LKA] = 0;
                                    }
                                    else
                                    {


                                        if (i == linkMac)
                                        {
                                            if (!sendFileList.empty())
                                            {
                                                std::string isOK[8] = {
                                           "文件尚未传输完成，此操作将会删除未传输的文件列表，连接将会在当前文件传输结束后断开",  // 中文 (简体)  
                                          "The file transfer is not yet complete. This operation will delete the list of untransferred files, and the connection will be disconnected after the current file transfer ends.",  // 英文  
                                          "ファイルの転送がまだ完了していません。この操作により未転送のファイルリストが削除され、現在のファイル転送終了後に接続が切断されます。",  // 日语  
                                             "파일 전송이 아직 완료되지 않았습니다. 이 작업은 미전송된 파일 목록을 삭제하며, 현재 파일 전송이 끝난 후 연결이 끊어집니다.",  // 韩语  
                                              "Le transfert de fichiers n'est pas encore terminé. Cette opération supprimera la liste des fichiers non transférés et la connexion sera interrompue une fois le transfert en cours terminé.",  // 法语  
                                              "Der Dateitransfer ist noch nicht abgeschlossen. Dieser Vorgang löscht die Liste der nicht übertragenen Dateien, und die Verbindung wird nach Abschluss des aktuellen Dateitransfers getrennt.",  // 德语  
                                             "Передача файлов еще не завершена. Эта операция удалит список непереданных файлов, и соединение будет разорвано после завершения текущей передачи.",  // 俄语  
                                            "La transferencia de archivos aún no se ha completado. Esta operación eliminará la lista de archivos no transferidos y la conexión se cerrará una vez que finalice la transferencia actual."  // 西班牙语  
                                                };
                                                if (1 == msgBox::msgbox(hInstanceTop, "NetTrack", isOK[stoi(userset[0])], WinPrompt::ICON_WARNING, WinPrompt::BTN_OKCANCEL, WinPrompt::MODE_WARNING))
                                                {
                                                    string kks = sendFileList[0].displayPath;
                                                    string kks2 = sendFileList[0].filePath;
                                                    sendFileList.clear();
                                                    sendFileList.push_back({ kks2,kks });

                                                    thread exit(exitPX, linkMac);
                                                    exit.detach();
                                                }
                                            }
                                            else
                                            {
                                                thread exit(exitPX, linkMac);
                                                exit.detach();
                                            }
                                        }
                                        else
                                        {
                                            saveData_link[i] = "NULL";
                                            tool::MoveNullToEnd(saveData_link, saveMax);
                                            string deleteSuccessMsg[8] = {
                                                // 中文（简体）
                                                "删除链接成功",        // 标准版
                                                // "链接删除成功"      // 被动语态版（备选）

                                                // 英文
                                                "Link deleted successfully",  // 技术文档标准
                                                // "Successfully removed link" // 操作日志风格

                                                // 日语
                                                "リンクの削除に成功しました", // 完整敬体
                                                // "リンク削除済み"         // 状态简写（系统提示）

                                                // 韩语
                                                "링크 삭제 성공",          // 控制台风格
                                                // "링크가 삭제되었습니다"   // 用户提示风格

                                                // 法语
                                                "Lien supprimé avec succès", // 正式报告
                                                // "Suppression réussie"     // 简洁版

                                                // 德语
                                                "Link erfolgreich gelöscht", // 工业标准
                                                // "Erfolgreiche Löschung"   // 名词化形式

                                                // 俄语
                                                "Ссылка успешно удалена",    // 被动完成体
                                                // "Удаление выполнено"      // 中性报告

                                                // 西班牙语
                                                "Enlace eliminado correctamente", // 拉美标准
                                                // "Borrado exitoso"         // 简短确认
                                            };
                                            int color[3] = { 100,255,100 };
                                            int fontColor[3] = { -1,-1,-1 };
                                            msg::Message(1, color, fontColor, 240, deleteSuccessMsg[stoi(userset[0])]);

                                            if (linkOS == i) linkOS = -1; // 更新链接索引
                                        }
                                        linkX_frame[LKA] = 30;
                                        LKA = -1;
                                    }
                                    isKEY = false;
                                }
                            }
                            else
                            {
                                if (i == LKA)
                                {
                                    if (mousePos.x < 100 - AddLinkXChange + windowSize.x - 120 - 80 - MacAdd)
                                    {
                                        if (msg::noBarH())
                                        {
                                            if (LKA != -1)
                                            {
                                                linkX_frame[LKA] = 0;
                                                LKA = -1;
                                            }
                                            isKEY = false;
                                        }
                                    }
                                }
                                else
                                {
                                    if (mousePos.x < 100 - AddLinkXChange + windowSize.x - 120 - 80 - 100)
                                    {
                                        if (msg::noBarH() && isKEY)
                                        {
                                            if (LKA != -1)
                                            {
                                                linkX_frame[LKA] = 0;
                                                LKA = -1;
                                            }
                                        }
                                        isKEY = false;
                                    }
                                    if (mousePos.x > 100 - AddLinkXChange + windowSize.x - 120 - 80 - 100 && mousePos.x < 100 - AddLinkXChange + windowSize.x - 120 - 80 && mousePos.y > 40 + CBOS + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + CBOS + (i + 1) * 120 + whle / 5 + 100 && mousePos.y > 100)
                                    {
                                        if (isKEY)
                                        {
                                            if (linkMac != i)
                                            {
                                                linkOS = i;
                                                string preparingConnection[8] = {
                                                    // 中文（简体）
                                                    "正在准备连接",

                                                    // 英文
                                                    "Preparing connection",

                                                    // 日语
                                                    "接続準備中",

                                                    // 韩语
                                                    "연결 준비 중",

                                                    // 法语
                                                    "Préparation de la connexion",

                                                    // 德语
                                                    "Verbindung wird vorbereitet",

                                                    // 俄语
                                                    "Подготовка подключения",

                                                    // 西班牙语
                                                    "Preparando conexión"
                                                };
                                                msg::KillBox(preparingConnection[stoi(userset[0])]);
                                                int color[3] = { 155,255,200 };
                                                int fontColor[3] = { -1,-1,-1 };
                                                msg::Message(3, color, fontColor, 0, preparingConnection[stoi(userset[0])]);

                                                Net::stopSendThread();
                                                pair<string, unsigned short> a = code::decode_ip_port(saveData_link[linkOS]);
                                                if (a.first == "ERROR")
                                                {
                                                    std::string errormsg[8] = {
                                                        // 中文 (简体)
                                                        "链接码异常，请确认此链接码无误",

                                                        // English
                                                        "Invalid link code, please verify the code is correct",

                                                        // 日本語
                                                        "リンクコード異常、正しいコードか確認してください",

                                                        // 한국어
                                                        "링크 코드 오류, 정확한 코드인지 확인하십시오",

                                                        // Français
                                                        "Code de lien anormal, veuillez vérifier que ce code est correct",

                                                        // Deutsch
                                                        "Ungültiger Link-Code, bitte überprüfen Sie die Richtigkeit des Codes",

                                                        // Русский
                                                        "Неверный код подключения, пожалуйста, проверьте правильность кода",

                                                        // Español
                                                        "Código de enlace anómalo, por favor confirme que el código es correcto"
                                                    };
                                                    int c[3] = { 255,100,100 };
                                                    int fc[3] = { -1,-1,-1 };
                                                    msg::Message(2, c, fc, 240, errormsg[stoi(userset[0])]);
                                                }
                                                else
                                                {
                                                    needlock::mcstart();
                                                }
                                            }
                                            else
                                            {
                                                if (!sendFileList.empty())
                                                {
                                                    std::string isOK[8] = {
                                               "文件尚未传输完成，此操作将会删除未传输的文件列表，连接将会在当前文件传输结束后断开",  // 中文 (简体)  
                                              "The file transfer is not yet complete. This operation will delete the list of untransferred files, and the connection will be disconnected after the current file transfer ends.",  // 英文  
                                              "ファイルの転送がまだ完了していません。この操作により未転送のファイルリストが削除され、現在のファイル転送終了後に接続が切断されます。",  // 日语  
                                                 "파일 전송이 아직 완료되지 않았습니다. 이 작업은 미전송된 파일 목록을 삭제하며, 현재 파일 전송이 끝난 후 연결이 끊어집니다.",  // 韩语  
                                                  "Le transfert de fichiers n'est pas encore terminé. Cette opération supprimera la liste des fichiers non transférés et la connexion sera interrompue une fois le transfert en cours terminé.",  // 法语  
                                                  "Der Dateitransfer ist noch nicht abgeschlossen. Dieser Vorgang löscht die Liste der nicht übertragenen Dateien, und die Verbindung wird nach Abschluss des aktuellen Dateitransfers getrennt.",  // 德语  
                                                 "Передача файлов еще не завершена. Эта операция удалит список непереданных файлов, и соединение будет разорвано после завершения текущей передачи.",  // 俄语  
                                                "La transferencia de archivos aún no se ha completado. Esta operación eliminará la lista de archivos no transferidos y la conexión se cerrará una vez que finalice la transferencia actual."  // 西班牙语  
                                                    };
                                                    if (1 == msgBox::msgbox(hInstanceTop, "NetTrack", isOK[stoi(userset[0])], WinPrompt::ICON_WARNING, WinPrompt::BTN_OKCANCEL, WinPrompt::MODE_WARNING))
                                                    {
                                                        string kks = sendFileList[0].displayPath;
                                                        string kks2 = sendFileList[0].filePath;
                                                        sendFileList.clear();
                                                        sendFileList.push_back({ kks2,kks });

                                                        thread exit(exitPXD);
                                                        exit.detach();
                                                    }
                                                }
                                                else
                                                {
                                                    thread exit(exitPXD);
                                                    exit.detach();
                                                }
                                            }
                                            isKEY = false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    isKEY = true;
                }
                if (MacAdd_frame < totalMacAdd_frame)
                {
                    if (MacAddChange)
                    {
                        int startValue = 0;
                        int endValue = 27;
                        double t = MacAdd_frame / (double)totalMacAdd_frame;
                        MacAdd = startValue + (endValue - startValue) * ease::easeOutBack(t, 0.2);
                        MacAdd_frame++;
                    }
                    else
                    {
                        int startValue = 27;
                        int endValue = 0;
                        double t = MacAdd_frame / (double)totalMacAdd_frame;
                        MacAdd = startValue + (endValue - startValue) * ease::easeInOut(t, 4);
                        MacAdd_frame++;
                    }
                }

                
            }
        }
        //===============================鼠标极值
        if(AddLinkXChange < windowSize.x / 4)
        {
            for (int i = 0; i < linkMax; i++)
            {
                if (linkX_frame[i] < 60)
                {
                    if (i == LKA)
                    {
                        int startValue = 0;
                        int endValue = 100;
                        double t = linkX_frame[i] / (double)60;
                        linkX[i] = startValue + (endValue - startValue) * ease::easeOut(t, 10);
                        linkX_frame[i]++;
                    }
                    else
                    {
                        int startValue = 100;
                        int endValue = 0;
                        double t = linkX_frame[i] / (double)60;
                        linkX[i] = startValue + (endValue - startValue) * ease::easeOutBack(t, 2);
                        linkX_frame[i]++;
                    }
                }
            }
            if (linkMax < 5)
            {
                const int min = 300 + 300 + (screen.y * 2 / 3 - windowSize.y) * 5;
                if (whle < -min)
                {
                    if (whle < -4000 - min) whle = -4000 - min;
                    else if (whle < -1500 - min) whle += 100;
                    else if (whle < -1000 - min) whle += 60;
                    else if (whle < -800 - min)whle += 50;
                    else if (whle < -600 - min) whle += 40;
                    else if (whle < -400 - min)whle += 30;
                    else if (whle < -200 - min)whle += 20;
                    else whle += 10;

                }
            }
            else
            {
                const int min = linkMax * 300 + 600 + (screen.y * 2 / 3 - windowSize.y) * 5;
                if (whle < -min)
                {
                    if (whle < -4000 - min) whle = -4000 - min;
                    else if (whle < -1500 - min) whle += 100;
                    else if (whle < -1000 - min) whle += 60;
                    else if (whle < -800 - min)whle += 50;
                    else if (whle < -600 - min) whle += 40;
                    else if (whle < -400 - min)whle += 30;
                    else if (whle < -200 - min)whle += 20;
                    else whle += 10;

                }
            }
        }
    }
    if (userBarChoose == 2)
    {
        /////////////////============================================上传文件控制区===========================================
        if (mousePos.x > 100 + (windowSize.x - 120) / 2 - 150 && mousePos.x < 100 + (windowSize.x - 120) / 2 + 150 && mousePos.y > 170 && mousePos.y < 370 && whle / 5 > -50)
        {
            if (keystate(0x01))
            {
                if (isKEY && !needlockX)
                {
                    int fontColor[3] = { 0,0,0 };
                    if (saveData_link[0] != "NULL")
                    {
                        int color[3] = { 155,255,200 };
                        string uploadTypeOptions[8][3] = {
                            // 中文
                            { "上传文件", "上传文件夹", "取消" },
                            // English
                            { "Upload File", "Upload Folder", "Cancel" },
                            // 日本語
                            { "ファイルをアップロード", "フォルダをアップロード", "キャンセル" },
                            // 한국어
                            { "파일 업로드", "폴더 업로드", "취소" },
                            // Français
                            { "Téléverser un fichier", "Téléverser un dossier", "Annuler" },
                            // Deutsch
                            { "Datei hochladen", "Ordner hochladen", "Abbrechen" },
                            // Русский
                            { "Загрузить файл", "Загрузить папку", "Отмена" },
                            // Español
                            { "Subir archivo", "Subir carpeta", "Cancelar" }
                        };
                        string uploadTypePrompt[8] = {
                            // 中文
                            "选择NetTrack需要上传的类型",
                            // English
                            "Select the type to upload in NetTrack",
                            // 日本語
                            "NetTrackでアップロードするタイプを選択",
                            // 한국어
                            "NetTrack에 업로드할 유형 선택",
                            // Français
                            "Sélectionnez le type à téléverser dans NetTrack",
                            // Deutsch
                            "Wählen Sie den zu uploadenden Typ in NetTrack",
                            // Русский
                            "Выберите тип загрузки в NetTrack",
                            // Español
                            "Seleccione el tipo de archivo a subir en NetTrack"
                        };
                        msg::MessageBar(color, fontColor, uploadTypePrompt[stoi(userset[0])], uploadTypeOptions[stoi(userset[0])]);
                        chooseFile = true;
                    }
                    else
                    {
                        wstring cutePrompt[8] = {
                            // 中文（完整句子组合）
                            {L"您需要先添加相关链接 0 在连接列表里添加链接(*_ *)" },

                            // English（分步组合）
                            {L"Please add the relevant link 0 to your connection list first 。_。" },

                            // 日本語（分步组合）
                            {L"まず関連リンク 0 を接続リストに追加してくださいね(//   >ω<   //)" },

                            // 한국어（分步组合）
                            {L"먼저 관련 링크 0 을 연결 목록에 추가해주세요 (๑•̀ㅂ•́)و✧" },

                            // Français（分步组合）
                            {L"Ajoutez d'abord le lien 0 dans la liste de connexion, s'il vous plaît (*△*  ~)" },

                            // Deutsch（分步组合）
                            {L"Bitte fügen Sie zuerst den Link 0 in die Verbindungsliste ein (* . *)" },

                            // Русский（分步组合）
                            {L"Сначала добавьте ссылку 0 в список подключений, пожалуйста (* . *)" },

                            // Español（分步组合）
                            {L"Primero añade el enlace 0 a la lista de conexiones, por favor (★ω★)/" }
                        };
                        int color[3] = { 255,100,100 };
                        string ok[8] = { "好的","OK","はい", "확인",   "D'accord", "OK",  "Хорошо",  "De acuerdo" };
                        string mk[3] = { ok[stoi(userset[0])] ,"NULL","NULL" };
                        msg::MessageBar(color, fontColor, tool::Utf8ToGbk(tool::wstring_to_utf8(cutePrompt[stoi(userset[0])])), mk);
                    }
                    isKEY = false;
                }
            }
            else
            {
                isKEY = true;
            }

            if (MacAddFileRectAlpha < 255)
            {
                MacAddFileRectAlpha += 5;
            }
            if (!MacAddFileChange)
            {
                MacAddFileChange = true;
                MacAddFile_frame = 0;
            }
        }
        else
        {
            if (MacAddFileRectAlpha > 0)
            {
                MacAddFileRectAlpha -= 5;
            }
            if (MacAddFileChange)
            {
                MacAddFileChange = false;
                MacAddFile_frame = 0;
            }
        }

        /////////////////============================================删除记录控制区===========================================
        if (keystate(0x01))
        {
            if (isKEY && !needlockX)
            {
                if (choose2Choose == 1)
                {
                    //////////=======================清除单列==============================================
                    for (int i = 0; i < lastUploadMax; i++)
                    {
                        if (lastUpload[i] == "NULL") break;
                        if (mousePos.x > 100 - choose2ChooseX + windowSize.x * 2 - 120 - 80 && mousePos.x < 100 - choose2ChooseX + windowSize.x * 2 - 120 - 5 && mousePos.y > 40 + CBOS + (i + 4) * 120 + whle / 5 && mousePos.y < 40 + CBOS + (i + 4) * 120 + whle / 5 + 100 && mousePos.y > 100)
                        {
                            if (msg::noBarH())
                            {
                                if (LKA != i)
                                {
                                    linkX_frame[LKA] = 0;
                                    LKA = i;
                                    linkX_frame[LKA] = 0;
                                    isKEY = false;
                                }
                                else
                                {
                                    lastUpload[LKA] = "NULL";
                                    if (debug) cout << endl << "user -> del Upload [" << LKA << "]";
                                    LKA = -1;
                                    tool::MoveNullToEnd(lastUpload, lastUploadMax);
                                    isKEY = false;
                                }

                            }
                        }
                        else if (mousePos.x < 100 - choose2ChooseX + windowSize.x * 2 - 120 - 80)
                        {
                            linkX_frame[LKA] = 0;
                            LKA = -1;
                            isKEY = false;
                        }
                    }
                    //////////=======================清空列表==============================================
                    if (mousePos.x > windowSize.x - 70 - 20 && mousePos.x < windowSize.x - 70 + 20)
                    {
                        if (140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 > 110)
                        {
                            if (mousePos.y > 140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 - 20 && mousePos.y < 140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 + 20)
                            {
                                if (LKA != -1)
                                {
                                    linkX_frame[LKA] = 59;
                                    LKA = -1;
                                }
                                for (int i = 0; i < lastUploadMax; i++)
                                {
                                    lastUpload[i] = "NULL";
                                }
                                int c[3] = { 150,255,150 };
                                int fc[3] = { -1,-1,-1 };

                                string translations[8] = {
                                "已清空全部相关记录",                     // 中文 (Chinese)
                                   "All related records have been cleared",  // 英文 (English)
                                   "すべての関連記録を消去しました",        // 日语 (Japanese)
                                      "모든 관련 기록이 삭제되었습니다",       // 韩文 (Korean)
                                  "Tous les enregistrements associés ont été effacés",  // 法语 (French)
                                  "Alle relevanten Aufzeichnungen wurden gelöscht",     // 德语 (German)
                                      "Все связанные записи были очищены",     // 俄语 (Russian)
                                     "Todos los registros relacionados han sido borrados"  // 西班牙语 (Spanish)
                                };
                                msg::KillBox(translations[stoi(userset[0])]);
                                msg::Message(1, c, fc, 180, translations[stoi(userset[0])]);
                                isKEY = false;
                            }
                        }
                        else
                        {
                            if (mousePos.y > 130 && mousePos.y < 170)
                            {
                                if (LKA != -1)
                                {
                                    linkX_frame[LKA] = 59;
                                    LKA = -1;
                                }
                                for (int i = 0; i < lastUploadMax; i++)
                                {
                                    lastUpload[i] = "NULL";
                                }
                                int c[3] = { 150,255,150 };
                                int fc[3] = { -1,-1,-1 };

                                string translations[8] = {
                                "已清空全部相关记录",                     // 中文 (Chinese)
                                   "All related records have been cleared",  // 英文 (English)
                                   "すべての関連記録を消去しました",        // 日语 (Japanese)
                                      "모든 관련 기록이 삭제되었습니다",       // 韩文 (Korean)
                                  "Tous les enregistrements associés ont été effacés",  // 法语 (French)
                                  "Alle relevanten Aufzeichnungen wurden gelöscht",     // 德语 (German)
                                      "Все связанные записи были очищены",     // 俄语 (Russian)
                                     "Todos los registros relacionados han sido borrados"  // 西班牙语 (Spanish)
                                };
                                msg::KillBox(translations[stoi(userset[0])]);
                                msg::Message(1, c, fc, 180, translations[stoi(userset[0])]);
                                isKEY = false;
                            }
                        }
                    }
                }
                else
                {
                    ///////////////=================================右划删除=======================================
                    for (int i = 0; i < sendFileList.size(); i++)
                    {
                        if (mousePos.x > 100 - choose2ChooseX + windowSize.x - 120 - 80 && mousePos.x < 100 - choose2ChooseX + windowSize.x - 120 - 5 && mousePos.y > 40 + CBOS + (i + 4) * 120 + whle / 5 && mousePos.y < 40 + CBOS + (i + 4) * 120 + whle / 5 + 100 && mousePos.y > 100)
                        {
                            if (msg::noBarH())
                            {
                                if (LKA != i)
                                {
                                    linkX_frame[LKA] = 0;
                                    LKA = i;
                                    linkX_frame[LKA] = 0;
                                    isKEY = false;
                                }
                                else
                                {
                                    if (LKA != 0)
                                    {
                                        linkX_frame[LKA] = 0;
                                        sendFileList.erase(sendFileList.begin() + i);
                                        LKA = -1;
                                        isKEY = false;
                                    }
                                }
                            }
                        }
                        else if (mousePos.x < 100 - choose2ChooseX + windowSize.x - 120 - 80)
                        {
                            linkX_frame[LKA] = 0;
                            LKA = -1;
                            isKEY = false;
                        }
                    }
                }
            }
        }
        else
        {
            isKEY = true;
        }


        /////////////////============================================横地址栏控制区===========================================
        if (keystate(0x01) && !needlockX)
        {
            if (140 + screen.y / 5 + 40 + whle / 5 > 110)
            {
                if (mousePos.y > 140 + screen.y / 5 + 40 + whle / 5 && mousePos.y < 140 + screen.y / 5 + 40 + 100 + whle / 5)
                {
                    if (mousePos.x > 170 && mousePos.x < 330 && choose2Choose != 0)
                    {
                        choose2Choose_frame = 0;
                        choose2Choose = 0;
                        linkX_frame[LKA] = 59;
                        LKA = -1;
                    }
                    if (mousePos.x > 170 + windowSize.x / 6 && mousePos.x < 330 + windowSize.x / 6 && choose2Choose != 1)
                    {
                        choose2Choose_frame = 0;
                        choose2Choose = 1;
                        linkX_frame[LKA] = 59;
                        LKA = -1;
                    }
                }
            }
            else
            {
                if (mousePos.y > 110 && mousePos.y < 180)
                {
                    if (mousePos.x > 170 && mousePos.x < 330 && choose2Choose != 0)
                    {
                        choose2Choose_frame = 0;
                        choose2Choose = 0;
                        linkX_frame[LKA] = 60;
                        LKA = -1;
                    }
                    if (mousePos.x > 170 + windowSize.x / 6 && mousePos.x < 330 + windowSize.x / 6 && choose2Choose != 1)
                    {
                        choose2Choose_frame = 0;
                        choose2Choose = 1;
                        linkX_frame[LKA] = 60;
                        LKA = -1;
                    }
                    C_temp = whle;
                }
            }
        }


        /////////////////============================================横地址栏右滑ease计算===========================================
        if (choose2Choose == 0)
        {
            for (int i = 0; i < sendFileList.size(); i++)
            {
                if (linkX_frame[i] < 60)
                {
                    if (i == LKA)
                    {
                        int startValue = 0;
                        int endValue = 100;
                        double t = linkX_frame[i] / (double)60;
                        linkX[i] = startValue + (endValue - startValue) * ease::easeOut(t, 10);
                        linkX_frame[i]++;
                    }
                    else
                    {
                        int startValue = 100;
                        int endValue = 0;
                        double t = linkX_frame[i] / (double)60;
                        linkX[i] = startValue + (endValue - startValue) * ease::easeOutBack(t, 2);
                        linkX_frame[i]++;
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < lastUploadMax; i++)
            {
                if (lastUpload[i] == "NULL")break;
                if (linkX_frame[i] < 60)
                {
                    if (i == LKA)
                    {
                        int startValue = 0;
                        int endValue = 100;
                        double t = linkX_frame[i] / (double)60;
                        linkX[i] = startValue + (endValue - startValue) * ease::easeOut(t, 10);
                        linkX_frame[i]++;
                    }
                    else
                    {
                        int startValue = 100;
                        int endValue = 0;
                        double t = linkX_frame[i] / (double)60;
                        linkX[i] = startValue + (endValue - startValue) * ease::easeOutBack(t, 2);
                        linkX_frame[i]++;
                    }
                }
            }
        }

        /////////////////============================================选择文件===========================================
        if (chooseFile == true)
        {
            if (msg::GetMessageBarChoose() == 0)
            {

                wstring selectFilePrompt[8] = {
                    // 中文
                       L"请选择需要上传的文件",
                       // English
                         L"Please select the file to upload",
                         // 日本語
                           L"アップロードするファイルを選択してください",
                           // 한국어
                           L"업로드할 파일을 선택해 주세요",
                           // Français
                             L"Veuillez sélectionner le fichier à téléverser",
                             // Deutsch
                              L"Bitte wählen Sie die hochzuladende Datei aus",
                              // Русский
                              L"Пожалуйста, выберите файл для загрузки",
                              // Español
                               L"Por favor seleccione el archivo a subir"
                };

                vector<pair<wstring, wstring>> filters[8] = {
                    // 中文
                    {
                        {L"所有文件", L"*.*"},
                        {L"文本文档", L"*.txt;*.doc;*.docx;*.rtf;*.odt;*.md;*.log;*.csv;*.xml;*.json"},
                        {L"图片文件", L"*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.tif;*.webp;*.svg;*.psd;*.raw"},
                        {L"应用程序", L"*.exe;*.msi;*.dmg;*.pkg;*.deb;*.rpm;*.app;*.apk;*.ipa"},
                        {L"脚本文件", L"*.py;*.js;*.php;*.sh;*.bat;*.ps1;*.vbs;*.lua;*.rb"},
                        {L"压缩文件", L"*.zip;*.rar;*.7z;*.tar;*.gz;*.bz2;*.xz;*.iso;*.dmg"},
                        {L"视频文件", L"*.mp4;*.avi;*.mov;*.wmv;*.flv;*.mkv;*.mpeg;*.mpg;*.webm;*.3gp"},
                        {L"音频文件", L"*.mp3;*.wav;*.flac;*.aac;*.ogg;*.wma;*.m4a;*.amr;*.mid"},
                        {L"字体文件", L"*.ttf;*.otf;*.woff;*.woff2;*.eot;*.fon"},
                        {L"网页文件", L"*.html;*.htm;*.css;*.js;*.mht;*.xhtml;*.asp;*.aspx;*.jsp"},
                    },
                    // 英文
                    {
                         {L"All Files", L"*.*"},
                        {L"Text Files", L"*.txt;*.doc;*.docx;*.rtf;*.odt;*.md;*.log;*.csv;*.xml;*.json"},
                        {L"Image Files", L"*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.tif;*.webp;*.svg;*.psd;*.raw"},
                        {L"Application Files", L"*.exe;*.msi;*.dmg;*.pkg;*.deb;*.rpm;*.app;*.apk;*.ipa"},
                        {L"Script Files", L"*.py;*.js;*.php;*.sh;*.bat;*.ps1;*.vbs;*.lua;*.rb"},
                        {L"Archive Files", L"*.zip;*.rar;*.7z;*.tar;*.gz;*.bz2;*.xz;*.iso;*.dmg"},
                        {L"Video Files", L"*.mp4;*.avi;*.mov;*.wmv;*.flv;*.mkv;*.mpeg;*.mpg;*.webm;*.3gp"},
                        {L"Audio Files", L"*.mp3;*.wav;*.flac;*.aac;*.ogg;*.wma;*.m4a;*.amr;*.mid"},
                        {L"Font Files", L"*.ttf;*.otf;*.woff;*.woff2;*.eot;*.fon"},
                        {L"Web Files", L"*.html;*.htm;*.css;*.js;*.mht;*.xhtml;*.asp;*.aspx;*.jsp"},
                    },
                    // 日语
                    {
                        {L"すべてのファイル", L"*.*"},
                        {L"テキストファイル", L"*.txt;*.doc;*.docx;*.rtf;*.odt;*.md;*.log;*.csv;*.xml;*.json"},
                        {L"画像ファイル", L"*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.tif;*.webp;*.svg;*.psd;*.raw"},
                        {L"アプリケーションファイル", L"*.exe;*.msi;*.dmg;*.pkg;*.deb;*.rpm;*.app;*.apk;*.ipa"},
                        {L"スクリプトファイル", L"*.py;*.js;*.php;*.sh;*.bat;*.ps1;*.vbs;*.lua;*.rb"},
                        {L"アーカイブファイル", L"*.zip;*.rar;*.7z;*.tar;*.gz;*.bz2;*.xz;*.iso;*.dmg"},
                        {L"動画ファイル", L"*.mp4;*.avi;*.mov;*.wmv;*.flv;*.mkv;*.mpeg;*.mpg;*.webm;*.3gp"},
                        {L"音声ファイル", L"*.mp3;*.wav;*.flac;*.aac;*.ogg;*.wma;*.m4a;*.amr;*.mid"},
                        {L"フォントファイル", L"*.ttf;*.otf;*.woff;*.woff2;*.eot;*.fon"},
                        {L"ウェブファイル", L"*.html;*.htm;*.css;*.js;*.mht;*.xhtml;*.asp;*.aspx;*.jsp"},
                    },
                    // 韩语
                    {
                        {L"모든 파일", L"*.*"},
                        {L"텍스트 파일", L"*.txt;*.doc;*.docx;*.rtf;*.odt;*.md;*.log;*.csv;*.xml;*.json"},
                        {L"이미지 파일", L"*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.tif;*.webp;*.svg;*.psd;*.raw"},
                        {L"응용 프로그램", L"*.exe;*.msi;*.dmg;*.pkg;*.deb;*.rpm;*.app;*.apk;*.ipa"},
                        {L"스크립트 파일", L"*.py;*.js;*.php;*.sh;*.bat;*.ps1;*.vbs;*.lua;*.rb"},
                        {L"압축 파일", L"*.zip;*.rar;*.7z;*.tar;*.gz;*.bz2;*.xz;*.iso;*.dmg"},
                        {L"동영상 파일", L"*.mp4;*.avi;*.mov;*.wmv;*.flv;*.mkv;*.mpeg;*.mpg;*.webm;*.3gp"},
                        {L"오디오 파일", L"*.mp3;*.wav;*.flac;*.aac;*.ogg;*.wma;*.m4a;*.amr;*.mid"},
                        {L"폰트 파일", L"*.ttf;*.otf;*.woff;*.woff2;*.eot;*.fon"},
                        {L"웹 파일", L"*.html;*.htm;*.css;*.js;*.mht;*.xhtml;*.asp;*.aspx;*.jsp"},
                    },
                    // 法语
                    {
                        {L"Tous les fichiers", L"*.*"},
                        {L"Fichiers texte", L"*.txt;*.doc;*.docx;*.rtf;*.odt;*.md;*.log;*.csv;*.xml;*.json"},
                        {L"Images", L"*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.tif;*.webp;*.svg;*.psd;*.raw"},
                        {L"Applications", L"*.exe;*.msi;*.dmg;*.pkg;*.deb;*.rpm;*.app;*.apk;*.ipa"},
                        {L"Scripts", L"*.py;*.js;*.php;*.sh;*.bat;*.ps1;*.vbs;*.lua;*.rb"},
                        {L"Archives", L"*.zip;*.rar;*.7z;*.tar;*.gz;*.bz2;*.xz;*.iso;*.dmg"},
                        {L"Vidéos", L"*.mp4;*.avi;*.mov;*.wmv;*.flv;*.mkv;*.mpeg;*.mpg;*.webm;*.3gp"},
                        {L"Audios", L"*.mp3;*.wav;*.flac;*.aac;*.ogg;*.wma;*.m4a;*.amr;*.mid"},
                        {L"Polices", L"*.ttf;*.otf;*.woff;*.woff2;*.eot;*.fon"},
                        {L"Fichiers web", L"*.html;*.htm;*.css;*.js;*.mht;*.xhtml;*.asp;*.aspx;*.jsp"},
                    },
                    // 德语
                    {
                        {L"Alle Dateien", L"*.*"},
                        {L"Textdateien", L"*.txt;*.doc;*.docx;*.rtf;*.odt;*.md;*.log;*.csv;*.xml;*.json"},
                        {L"Bilddateien", L"*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.tif;*.webp;*.svg;*.psd;*.raw"},
                        {L"Anwendungen", L"*.exe;*.msi;*.dmg;*.pkg;*.deb;*.rpm;*.app;*.apk;*.ipa"},
                        {L"Skriptdateien", L"*.py;*.js;*.php;*.sh;*.bat;*.ps1;*.vbs;*.lua;*.rb"},
                        {L"Archivdateien", L"*.zip;*.rar;*.7z;*.tar;*.gz;*.bz2;*.xz;*.iso;*.dmg"},
                        {L"Videodateien", L"*.mp4;*.avi;*.mov;*.wmv;*.flv;*.mkv;*.mpeg;*.mpg;*.webm;*.3gp"},
                        {L"Audiodateien", L"*.mp3;*.wav;*.flac;*.aac;*.ogg;*.wma;*.m4a;*.amr;*.mid"},
                        {L"Schriftarten", L"*.ttf;*.otf;*.woff;*.woff2;*.eot;*.fon"},
                        {L"Webdateien", L"*.html;*.htm;*.css;*.js;*.mht;*.xhtml;*.asp;*.aspx;*.jsp"},
                    },
                    // 俄语
                    {
                        {L"Все файлы", L"*.*"},
                        {L"Текстовые файлы", L"*.txt;*.doc;*.docx;*.rtf;*.odt;*.md;*.log;*.csv;*.xml;*.json"},
                        {L"Изображения", L"*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.tif;*.webp;*.svg;*.psd;*.raw"},
                        {L"Приложения", L"*.exe;*.msi;*.dmg;*.pkg;*.deb;*.rpm;*.app;*.apk;*.ipa"},
                        {L"Скрипты", L"*.py;*.js;*.php;*.sh;*.bat;*.ps1;*.vbs;*.lua;*.rb"},
                        {L"Архивы", L"*.zip;*.rar;*.7z;*.tar;*.gz;*.bz2;*.xz;*.iso;*.dmg"},
                        {L"Видео", L"*.mp4;*.avi;*.mov;*.wmv;*.flv;*.mkv;*.mpeg;*.mpg;*.webm;*.3gp"},
                        {L"Аудио", L"*.mp3;*.wav;*.flac;*.aac;*.ogg;*.wma;*.m4a;*.amr;*.mid"},
                        {L"Шрифты", L"*.ttf;*.otf;*.woff;*.woff2;*.eot;*.fon"},
                        {L"Веб-файлы", L"*.html;*.htm;*.css;*.js;*.mht;*.xhtml;*.asp;*.aspx;*.jsp"},
                    },
                    // 西班牙语
                    {
                        {L"Todos los archivos", L"*.*"},
                        {L"Archivos de texto", L"*.txt;*.doc;*.docx;*.rtf;*.odt;*.md;*.log;*.csv;*.xml;*.json"},
                        {L"Imágenes", L"*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.tif;*.webp;*.svg;*.psd;*.raw"},
                        {L"Aplicaciones", L"*.exe;*.msi;*.dmg;*.pkg;*.deb;*.rpm;*.app;*.apk;*.ipa"},
                        {L"Scripts", L"*.py;*.js;*.php;*.sh;*.bat;*.ps1;*.vbs;*.lua;*.rb"},
                        {L"Archivos comprimidos", L"*.zip;*.rar;*.7z;*.tar;*.gz;*.bz2;*.xz;*.iso;*.dmg"},
                        {L"Videos", L"*.mp4;*.avi;*.mov;*.wmv;*.flv;*.mkv;*.mpeg;*.mpg;*.webm;*.3gp"},
                        {L"Audios", L"*.mp3;*.wav;*.flac;*.aac;*.ogg;*.wma;*.m4a;*.amr;*.mid"},
                        {L"Fuentes", L"*.ttf;*.otf;*.woff;*.woff2;*.eot;*.fon"},
                        {L"Archivos web", L"*.html;*.htm;*.css;*.js;*.mht;*.xhtml;*.asp;*.aspx;*.jsp"},
                    }
                };
                string a = chooseFile::OpenFileDialog(selectFilePrompt[stoi(userset[0])], filters[stoi(userset[0])]);
                if (!a.empty() && a != "ERROR")
                {
                    thread packAK(addSend::addup, a);
                    packAK.detach();

                }
                if (a == "ERROR")
                {
                    int color[3] = { 255,100,100 };
                    int fontColor[3] = { -1,-1,-1 };

                    string errorMsg[8] = {
                        // 中文
                        "选择文件失败，请重试！",
                        // 英文
                        "Failed to select file, please try again!",
                        // 日语
                        "ファイルの選択に失敗しました。もう一度お試しください！",
                        // 韩语
                        "파일 선택에 실패했습니다. 다시 시도해 주세요!",
                        // 法语
                        "Échec de la sélection du fichier, veuillez réessayer !",
                        // 德语
                        "Dateiauswahl fehlgeschlagen, bitte versuchen Sie es erneut!",
                        // 俄语
                        "Не удалось выбрать файл, пожалуйста, попробуйте еще раз!",
                        // 西班牙语
                        "¡Error al seleccionar el archivo, por favor intente de nuevo!"
                    };

                    string mos = errorMsg[stoi(userset[0])];

                    msg::Message(2, color, fontColor, 420, mos);
                }
            }
            if (msg::GetMessageBarChoose() == 1)
            {

                wstring chooseF[8] = {
                L"选择文件夹",                // 中文  
                L"Select folder",            // 英文  
                L"フォルダを選択",            // 日语  
                 L"폴더 선택",                // 韩文  
                 L"Sélectionner dossier",    // 法语  
                  L"Ordner auswählen",        // 德语  
                  L"Выбрать папку",           // 俄语  
                  L"Seleccionar carpeta"      // 西班牙语  
                };
                string a = chooseFile::OpenFolderDialog(chooseF[stoi(userset[0])]);

                if (!a.empty() && a != "ERROR")
                {
                    thread packAS(addSend::pack, a);
                    packAS.detach();
                }
                if (a == "ERROR")
                {
                    int color[3] = { 255,100,100 };
                    int fontColor[3] = { -1,-1,-1 };

                    string errorMsg[8] = {
                        // 中文
                        "选择文件失败，请重试！",
                        // 英文
                        "Failed to select file, please try again!",
                        // 日语
                        "ファイルの選択に失敗しました。もう一度お試しください！",
                        // 韩语
                        "파일 선택에 실패했습니다. 다시 시도해 주세요!",
                        // 法语
                        "Échec de la sélection du fichier, veuillez réessayer !",
                        // 德语
                        "Dateiauswahl fehlgeschlagen, bitte versuchen Sie es erneut!",
                        // 俄语
                        "Не удалось выбрать файл, пожалуйста, попробуйте еще раз!",
                        // 西班牙语
                        "¡Error al seleccionar el archivo, por favor intente de nuevo!"
                    };

                    string mos = errorMsg[stoi(userset[0])];

                    msg::Message(2, color, fontColor, 420, mos);
                }
            }
            if (msg::GetMessageBarChoose() != -1) chooseFile = false;
        }

        /////////////////============================================选择文件ease计算===========================================
        if (MacAddFile_frame < totalMacAddFile_frame)
        {
            if (MacAddFileChange)
            {
                int startValue = 0;
                int endValue = 80;
                double t = MacAddFile_frame / (double)totalMacAddFile_frame;
                MacAddFile = startValue + (endValue - startValue) * ease::easeOutBack(t, 5);
                MacAddFile_frame++;
            }
            else
            {
                int startValue = 80;
                int endValue = 0;
                double t = MacAddFile_frame / (double)totalMacAddFile_frame;
                MacAddFile = startValue + (endValue - startValue) * ease::easeInOutBack(t, 8);
                MacAddFile_frame++;
            }
        }

        /////////////////============================================横地址栏滑动ease计算===========================================
        if (choose2Choose_frame < 60)
        {
            int startValue_X;
            int endValue_X;
            if (choose2Choose == 0)
            {
                startValue_X = windowSize.x;
                endValue_X = 0;
            }
            else
            {
                startValue_X = 0;
                endValue_X = windowSize.x;
            }
            int startLine = 0;
            int endLine = 40;
            int startA = C_temp;
            int endA = -(140 + CBOS + screen.y / 5 + 40 - 100) * 5;
            if (choose2Choose_frame < 20 && 140 + CBOS + screen.y / 5 + 40 + whle / 5 < 110)
            {
                double t2 = choose2Choose_frame / (double)20;
                whle = startA + (endA - startA) * ease::easeOut(t2, 7);
            }
            double t = choose2Choose_frame / (double)60;
            choose2ChooseX = startValue_X + (endValue_X - startValue_X) * ease::easeOut(t, 7);
            choose2Line = startLine + (endLine - startLine) * ease::easeOutBack(t, 10);

            choose2Choose_frame++;
        }

        /////////////////============================================鼠标极值===========================================
        int max = 0;
        for (int i = 0; i < lastUploadMax; i++)
        {
            if (lastUpload[i] == "NULL")
            {
                max = i + sendFileList.size();
                break;
            }
        }
        if (max < 3)
        {
            const int min = 300 + 300 + (screen.y * 2 / 3 - windowSize.y) * 5;
            if (whle < -min)
            {
                if (whle < -4000 - min) whle = -4000 - min;
                else if (whle < -1500 - min) whle += 100;
                else if (whle < -1000 - min) whle += 60;
                else if (whle < -800 - min)whle += 50;
                else if (whle < -600 - min) whle += 40;
                else if (whle < -400 - min)whle += 30;
                else if (whle < -200 - min)whle += 20;
                else whle += 10;

            }
        }
        else
        {
            const int min = 1200 + max * 300 + (screen.y * 2 / 3 - windowSize.y) * 5;
            if (whle < -min)
            {
                if (whle < -4000 - min) whle = -4000 - min;
                else if (whle < -1500 - min) whle += 100;
                else if (whle < -1000 - min) whle += 60;
                else if (whle < -800 - min)whle += 50;
                else if (whle < -600 - min) whle += 40;
                else if (whle < -400 - min)whle += 30;
                else if (whle < -200 - min)whle += 20;
                else whle += 10;
            }
        }
    }
    if (userBarChoose == 3)
    {
        int max = 0;
        for (int i = 0; i < lastDownloadMax; i++)
        {
            if (lastDownload[i] == "NULL")
            {
                max = i;
                break;
            }
        }
        if (max < 3)
        {
            const int min = 300 + 300 + (screen.y * 2 / 3 - windowSize.y) * 5;
            if (whle < -min)
            {
                if (whle < -4000 - min) whle = -4000 - min;
                else if (whle < -1500 - min) whle += 100;
                else if (whle < -1000 - min) whle += 60;
                else if (whle < -800 - min)whle += 50;
                else if (whle < -600 - min) whle += 40;
                else if (whle < -400 - min)whle += 30;
                else if (whle < -200 - min)whle += 20;
                else whle += 10;

            }
        }
        else
        {
            const int min = max * 300 - 300 + (screen.y * 2 / 3 - windowSize.y) * 5;
            if (whle < -min)
            {
                if (whle < -4000 - min) whle = -4000 - min;
                else if (whle < -1500 - min) whle += 100;
                else if (whle < -1000 - min) whle += 60;
                else if (whle < -800 - min)whle += 50;
                else if (whle < -600 - min) whle += 40;
                else if (whle < -400 - min)whle += 30;
                else if (whle < -200 - min)whle += 20;
                else whle += 10;

            }
        }

        for (int i = 0; i < lastDownloadMax; i++)
        {
            if (linkX_frame[i] < 60)
            {
                if (i == LKA)
                {
                    int startValue = 0;
                    int endValue = 100;
                    double t = linkX_frame[i] / (double)60;
                    linkX[i] = startValue + (endValue - startValue) * ease::easeOut(t, 10);
                    linkX_frame[i]++;
                }
                else
                {
                    int startValue = 100;
                    int endValue = 0;
                    double t = linkX_frame[i] / (double)60;
                    linkX[i] = startValue + (endValue - startValue) * ease::easeOutBack(t, 2);
                    linkX_frame[i]++;
                }
            }
        }

        if (keystate(0x01))
        {
            if (isKEY && !needlockX)
            {
                for (int i = 0; i < lastDownloadMax; i++)
                {
                    if (lastDownload[i] == "NULL") break;
                    if (mousePos.x > 100 + windowSize.x - 120 - 80 && mousePos.x < 100 + windowSize.x - 120 - 5 && mousePos.y > 40 + CBOS + (i + 1) * 120 + whle / 5 + 70 && mousePos.y < 40 + CBOS + (i + 1) * 120 + whle / 5 + 100 + 70 && mousePos.y > 100)
                    {
                        if (msg::noBarH())
                        {
                            if (LKA != i)
                            {
                                linkX_frame[LKA] = 0;
                                LKA = i;
                                linkX_frame[LKA] = 0;
                            }
                            else
                            {
                                lastDownload[LKA] = "NULL";
                                LKA = -1;
                                tool::MoveNullToEnd(lastDownload, lastDownloadMax);
                            }
                            isKEY = false;
                        }
                    }
                    else if (mousePos.x < 100 + windowSize.x - 120 - 80)
                    {
                        linkX_frame[LKA] = 0;
                        LKA = -1;
                        isKEY = false;
                    }
                }

                int i = 0;
                if (40 + CBOS + (i + 1) * 120 + whle / 5 > 110)
                {

                    if(mousePos.y > 40 + CBOS + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + CBOS + (i + 1) * 120 + whle / 5 + 60)
                    {
                        if (mousePos.x > windowSize.x - 65 - 15 - 10 && mousePos.x < windowSize.x - 35 - 15 + 10)
                        {

                            if (LKA != -1)
                            {
                                linkX_frame[LKA] = 59;
                                LKA = -1;
                            }
                            for (int i = 0; i < lastDownloadMax; i++)
                            {
                                lastDownload[i] = "NULL";
                            }
                            int c[3] = { 150,255,150 };
                            int fc[3] = { -1,-1,-1 };

                            string translations[8] = {
                            "已清空全部相关记录",                     // 中文 (Chinese)
                               "All related records have been cleared",  // 英文 (English)
                               "すべての関連記録を消去しました",        // 日语 (Japanese)
                                  "모든 관련 기록이 삭제되었습니다",       // 韩文 (Korean)
                              "Tous les enregistrements associés ont été effacés",  // 法语 (French)
                              "Alle relevanten Aufzeichnungen wurden gelöscht",     // 德语 (German)
                                  "Все связанные записи были очищены",     // 俄语 (Russian)
                                 "Todos los registros relacionados han sido borrados"  // 西班牙语 (Spanish)
                            };
                            msg::KillBox(translations[stoi(userset[0])]);
                            msg::Message(1, c, fc, 180, translations[stoi(userset[0])]);
                            isKEY = false;

                        }
                    }
                }
                else
                {

                    if (mousePos.y > 110 && mousePos.y < 170)
                    {
                        if (mousePos.x > windowSize.x - 65 - 15 - 10 && mousePos.x < windowSize.x - 35 - 15 + 10)
                        {

                            if (LKA != -1)
                            {
                                linkX_frame[LKA] = 59;
                                LKA = -1;
                            }
                            for (int i = 0; i < lastDownloadMax; i++)
                            {
                                lastDownload[i] = "NULL";
                            }
                            int c[3] = { 150,255,150 };
                            int fc[3] = { -1,-1,-1 };

                            string translations[8] = {
                            "已清空全部相关记录",                     // 中文 (Chinese)
                               "All related records have been cleared",  // 英文 (English)
                               "すべての関連記録を消去しました",        // 日语 (Japanese)
                                  "모든 관련 기록이 삭제되었습니다",       // 韩文 (Korean)
                              "Tous les enregistrements associés ont été effacés",  // 法语 (French)
                              "Alle relevanten Aufzeichnungen wurden gelöscht",     // 德语 (German)
                                  "Все связанные записи были очищены",     // 俄语 (Russian)
                                 "Todos los registros relacionados han sido borrados"  // 西班牙语 (Spanish)
                            };
                            msg::KillBox(translations[stoi(userset[0])]);
                            msg::Message(1, c, fc, 180, translations[stoi(userset[0])]);
                            isKEY = false;

                        }
                    }
                }

                
            }
        }
        else
        {
            isKEY = true;
        }
    }
    if (userBarChoose == 4)
    {
        MDcaonimadeshishenmeguidongxi = false;
        bool keystate0x01 = true;
        if (keystate(0x01))
        {
            //==============================设置主界面===========================================
            if (!isGotoChoose4 )
            {
                if (keystate0x01 && !needlockX)
                {
                    POINT mousePos = Window::GetMousePosX();
                    for (int i = 0; i < 16; i++)
                    {
                        if (mousePos.x > windowSize.x - 170 && mousePos.x < windowSize.x - 20 &&
                            mousePos.y > 40 + CBOS + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + CBOS + (i + 1) * 120 + whle / 5 + 100)
                        {
                            getimage(isGotoChoose4Image, 0, 0, windowSize.x - 20, windowSize.y);
                            keystate0x01 = false;
                            isGotoChoose4 = true;
                            isGotoChoose4Choose = i;
                            isGotoChoose4_frame = 0;
                            whleHistory = whle;
                            whle = 0;
                        }
                    }
                }
            }

            //==============================设置详细界面===========================================
            else
            {
                if (mousePos.x > 100 && mousePos.x < 250 &&
                    mousePos.y > 110 && mousePos.y < 150)
                {
                    getimage(isGotoChoose4Image, 100, 0, windowSize.x - 20, windowSize.y);
                    isGotoChoose4 = false;
                    isGotoChoose4Choose = -1;
                    whle = whleHistory;
                    whleHistory = 0;
                    isGotoChoose4_frame = 0;
                }

                choose4ChooseKey();
            }

        }
        else
        {
            keystate0x01 = true;
        }

        if(!isGotoChoose4)
        {
            int min = 4500 + 300 + (screen.y * 2 / 3 - windowSize.y) * 5;

            if (whle < -min)
            {
                if (whle < -4000 - min) whle = -4000 - min;
                else if (whle < -1500 - min) whle += 100;
                else if (whle < -1000 - min) whle += 60;
                else if (whle < -800 - min)whle += 50;
                else if (whle < -600 - min) whle += 40;
                else if (whle < -400 - min)whle += 30;
                else if (whle < -200 - min)whle += 20;
                else whle += 10;
            }
        }
        else
        {
            if(isGotoChoose4Choose == 1)
            {
                int min = 0;

                if (whle < -min)
                {
                    if (whle < -4000 - min) whle = -4000 - min;
                    else if (whle < -1500 - min) whle += 100;
                    else if (whle < -1000 - min) whle += 60;
                    else if (whle < -800 - min)whle += 50;
                    else if (whle < -600 - min) whle += 40;
                    else if (whle < -400 - min)whle += 30;
                    else if (whle < -200 - min)whle += 20;
                    else whle += 10;
                }
            }
            if (isGotoChoose4Choose == 4)
            {
                int min = 2500 + 300 + (screen.y * 2 / 3 - windowSize.y) * 5;

                if (whle < -min)
                {
                    if (whle < -4000 - min) whle = -4000 - min;
                    else if (whle < -1500 - min) whle += 100;
                    else if (whle < -1000 - min) whle += 60;
                    else if (whle < -800 - min)whle += 50;
                    else if (whle < -600 - min) whle += 40;
                    else if (whle < -400 - min)whle += 30;
                    else if (whle < -200 - min)whle += 20;
                    else whle += 10;
                }
            }
        }

        
    }
    if (userBarChoose == 5)
    {
        const int min = 1000 + 300 + (screen.y * 2 / 3 - windowSize.y) * 5;
        if (whle < -min)
        {
            if (whle < -4000 - min) whle = -4000 - min;
            else if (whle < -1500 - min) whle += 100;
            else if (whle < -1000 - min) whle += 60;
            else if (whle < -800 - min)whle += 50;
            else if (whle < -600 - min) whle += 40;
            else if (whle < -400 - min)whle += 30;
            else if (whle < -200 - min)whle += 20;
            else whle += 10;

        }

        if (keystate(0x01))
        {
            if (isKEY && !needlockX)
            {
                POINT mousePos = Window::GetMousePosX();
                for (int i = 0; i < 6; i++)
                {
                    if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 220 + 180)
                    {
                        if (mousePos.y > 65 + CBOS + (i + 1) * 120 + whle / 5 && mousePos.y < 65 + CBOS + (i + 1) * 120 + whle / 5 + 50)
                        {
                            choose5OBI(i);
                            isKEY = false;
                        }
                    }
                }

            }
        }
        else
        {
            isKEY = true;
        }
    }
    if (userBarChoose == 6)
    {

        if (keystate(0x01))
        {
            if (isKEY && !needlockX)
            {
                POINT mousePos = Window::GetMousePosX();
                for (int i = 0; i < 6; i++)
                {
                    if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 220 + 180)
                    {
                        if (mousePos.y > 65 + CBOS + (i + 1) * 120 + whle / 5 && mousePos.y < 65 + CBOS + (i + 1) * 120 + whle / 5 + 50)
                        {
                            choose6Choose(i);
                            isKEY = false;
                        }
                    }
                }

            }
        }
        else
        {
            isKEY = true;
        }

        const int min = 300 + 300 + (screen.y * 2 / 3 - windowSize.y) * 5;
        if (whle < -min)
        {
            if (whle < -4000 - min) whle = -4000 - min;
            else if (whle < -1500 - min) whle += 100;
            else if (whle < -1000 - min) whle += 60;
            else if (whle < -800 - min)whle += 50;
            else if (whle < -600 - min) whle += 40;
            else if (whle < -400 - min)whle += 30;
            else if (whle < -200 - min)whle += 20;
            else whle += 10;

        }
    }
}

//0  1  2
bool Choose4ChooseChange550[16] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
int Choose4ChooseChange550X[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
int Choose4ChooseChange550Xframe[16] = { 30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30 };

bool needChangeUserset5 = false;


#pragma region resetlock

int Xframe_moto = 0;
class moto
{
public:
    static string reset()
    {
        Xframe_moto = 0;
        MSG msg = {};
        wstring resetNetTrackPasswordWH[8] = {
    L"重置NetTrack的密码",      // Chinese
    L"Reset NetTrack password", // English
    L"NetTrackのパスワードをリセット", // Japanese
    L"NetTrack 비밀번호 재설정", // Korean
    L"Réinitialiser le mot de passe NetTrack", // French
    L"NetTrack-Passwort zurücksetzen", // German
    L"Сбросить пароль NetTrack", // Russian
    L"Restablecer contraseña de NetTrack" // Spanish
        };
        wstring createPassword[8] = {
    L"创建NetTrack的密码",  // Chinese (Simplified)
    L"Create password for NetTrack",  // English
    L"NetTrackのパスワードを作成",  // Japanese
    L"NetTrack 비밀번호 생성",  // Korean
    L"Créer un mot de passe pour NetTrack",  // French
    L"Passwort für NetTrack erstellen",  // German
    L"Создать пароль для NetTrack",  // Russian
    L"Crear contraseña para NetTrack"  // Spanish
        };

        wstring resetNetTrackPasswordW[8];
        if (userset[2] == "0")
        {
            for(int i=0;i<8;i++)
            {
                resetNetTrackPasswordW[i] = createPassword[i];
            }
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                resetNetTrackPasswordW[i] = resetNetTrackPasswordWH[i];
            }
        }

        for (int i = 0; i < 64; i++)
        {
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            delay_fps(100000000);
            cleardevice();
            putimage(0, 0, screenShot);
            if (i*4 > 50)
            {
                if (DarkMode)setfillcolor(EGERGBA(0, 0, 0, i*4 - 50));
                else setfillcolor(EGERGBA(255, 255, 255, i*4 - 50));
                ege_fillrect(0, 0, windowSize.x, windowSize.y);
            }
			PIMAGE screenShot2 = newimage();
			getimage(screenShot2, 0, 0, windowSize.x, windowSize.y);
            DrawManager::centerText();
            DrawManager::setFont(60);
            xyprintf(windowSize.x / 2, windowSize.y / 2, resetNetTrackPasswordW[stoi(userset[0])].c_str());
            DrawManager::startText();
            putimage_alphablend(NULL, screenShot2, 0, 0, 255 - i * 4);
            delimage(screenShot2);
        }

        for (Xframe_moto = 0; Xframe_moto < 60; Xframe_moto++)
        {
            double t = Xframe_moto / (double)60;
            int start = 0;
            int end = 255;
            delay_fps(60);
            cleardevice();
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            int v = start + (end - start) * ease::easeInOut(t, 4);
            putimage(0, 0, screenShot);

            if (DarkMode)setfillcolor(EGERGBA(0, 0, 0, 202));
            else setfillcolor(EGERGBA(255, 255, 255, 202));
            ege_fillrect(0, 0, windowSize.x, windowSize.y);

            DrawManager::setFont(60);
            DrawManager::centerText();
            xyprintf(windowSize.x / 2, windowSize.y / 2 - v, resetNetTrackPasswordW[stoi(userset[0])].c_str());
            DrawManager::startText();
            setlinewidth(6);
            setcolor(EGERGB(155, 155, 155));
            line(windowSize.x / 2 - v, windowSize.y / 2 + v, windowSize.x / 2 + v, windowSize.y / 2 + v);
        }
        
        int backAlpha = 0;
        int goAlpha = 0;

        string returnInf = "NULL";

        bool isback = false;

        while (1)
        {
            delay_fps(60);
            cleardevice();
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            putimage(0, 0, screenShot);

            if (DarkMode)setfillcolor(EGERGBA(0, 0, 0,202));
            else setfillcolor(EGERGBA(255, 255, 255, 202));
            ege_fillrect(0, 0, windowSize.x, windowSize.y);

            DrawManager::setFont(60);
            DrawManager::centerText();
            xyprintf(windowSize.x / 2, windowSize.y / 2 - 255, resetNetTrackPasswordW[stoi(userset[0])].c_str());
            setlinewidth(6);
            setcolor(EGERGB(155, 155, 155));
            line(windowSize.x / 2 - 255, windowSize.y / 2 + 255, windowSize.x / 2 + 255, windowSize.y / 2 + 255);
            
            if (returnInf != "NULL" && !returnInf.empty())
            {
                DrawManager::setFont(50);
                xyprintf(windowSize.x / 2, windowSize.y / 2 + 150, returnInf.c_str());
            }

            wstring buttonTextsW[8][2] = {
    {L"返回", L"确定"},     // Chinese
    {L"Back", L"OK"},      // English
    {L"戻る", L"確定"},    // Japanese
    {L"뒤로", L"확인"},    // Korean
    {L"Retour", L"OK"},    // French
    {L"Zurück", L"OK"},    // German
    {L"Назад", L"ОК"},     // Russian
    {L"Volver", L"Aceptar"} // Spanish
            };
            
            setcolor(EGERGBA(155, 155, 155, backAlpha));
            setfillcolor(EGERGBA(135, 135, 135, backAlpha));
            DrawManager::fillroundrectwithrect(20, 20, 200, 50, 10);
            DrawManager::setFont(25);
            xyprintf(120, 45, buttonTextsW[stoi(userset[0])][0].c_str());

            setcolor(EGERGBA(155, 155, 155, goAlpha));
            setfillcolor(EGERGBA(135, 135, 135, goAlpha));
            DrawManager::fillroundrectwithrect(windowSize.x - 220, windowSize.y - 70, 200, 50, 10);
            DrawManager::setFont(25);
            xyprintf(windowSize.x - 120, windowSize.y - 45, buttonTextsW[stoi(userset[0])][1].c_str());

            DrawManager::startText();

            mousePos = Window::GetMousePosX();

            if (mousePos.x > 20 && mousePos.x < 220 && mousePos.y > 20 && mousePos.y < 70)
            {
                if (backAlpha < 255)
                {
                    if (backAlpha + 8 < 255)backAlpha += 8;
                    else backAlpha = 255;
                }
            }
            else
            {
                if (backAlpha - 8 > 0)backAlpha -= 8;
                else backAlpha = 0;
            }

            if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20 && mousePos.y > windowSize.y - 70 && mousePos.y < windowSize.y - 20)
            {
                if (goAlpha < 255)
                {
                    if (goAlpha + 8 < 255)goAlpha += 8;
                    else goAlpha = 255;
                }
            }
            else
            {
                if (goAlpha - 8 > 0)goAlpha -= 8;
                else goAlpha = 0;
            }

            if (kbhit())
            {
                char m = getch();
                if(returnInf.length() < 6)
                {
                    if (returnInf == "NULL") returnInf.clear();
                    if (m >= '0' && m <= '9')
                    {
                        returnInf += m;
                    }
                }
            }

            if (keystate(0x08))
            {
                if (isKEY_0x08)
                {
                    returnInf.pop_back();
                    isKEY_0x08 = false;
                }
            }
            else
            {
                isKEY_0x08 = true;
            }

            if (keystate(0x01))
            {
                if (isKEY)
                {
                    if ((mousePos.x > 20 && mousePos.x < 220 && mousePos.y > 20 && mousePos.y < 70) || (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20 && mousePos.y > windowSize.y - 70 && mousePos.y < windowSize.y - 20))
                    {
                        if ((mousePos.x > 20 && mousePos.x < 220 && mousePos.y > 20 && mousePos.y < 70))
                        {
                            isback = true;
                        }
                        break;
                    }
                }
            }
            else isKEY = true;
        }

        PIMAGE screenShot3 = newimage();
        getimage(screenShot3, 0, 0, windowSize.x, windowSize.y);

        

        for (int i = 64; i > 0; i--)
        {
            while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            delay_fps(100000000);
            cleardevice();
            int alpha = i * 4;
            if (alpha > 255)alpha = 255;
            putimage(0, 0, screenShot);
            putimage_alphablend(NULL, screenShot3, 0, 0, alpha);
        }

        if (returnInf.empty() || returnInf.length() < 6 || isback) returnInf = "NULL";

        return returnInf;
    }
};
#pragma endregion


bool temp_AS = false;

void DrawisGotoChoose4()
{
    const int userSetCircle = 15;

	int Choose4Choose = isGotoChoose4Choose + 1;

    if (Choose4Choose == 1)
    {
        int mh = 300 - isGotoChoose4XChange;

        wstring connectionPrompt[8] = {
            // 中文
            L"此电脑的8位连接口令 : ",
            // 英文
            L"8-digit connection code of this PC : ",
            // 日文
            L"このPCの8桁接続パスコード : ",
            // 韩文
            L"이 PC의 8자리 연결 암호 : ",
            // 法文
            L"Code de connexion à 8 chiffres de cet ordinateur : ",
            // 德文
            L"8-stelliger Verbindungscode dieses PCs : ",
            // 俄文
            L"8-значный код подключения этого компьютера : ",
            // 西班牙文
            L"Código de conexión de 8 dígitos de esta PC : "
        };
        std::wstring deviceIPAddress[8] = {
    L"此设备的IP地址",       // Chinese (Simplified)
    L"This device's IP address",  // English
    L"このデバイスのIPアドレス",  // Japanese
    L"이 장치의 IP 주소",     // Korean
    L"Adresse IP de cet appareil",  // French
    L"IP-Adresse dieses Geräts",  // German
    L"IP-адрес этого устройства",  // Russian
    L"Dirección IP de este dispositivo"  // Spanish
        };
        wstring usx = connectionPrompt[stoi(userset[0])] + tool::string_to_wstring(OMSR);

        wstring s = deviceIPAddress[stoi(userset[0])] + L" : " + tool::string_to_wstring(ip_local);

        DrawManager::setFillRectColor();
        DrawManager::fillroundrectwithrect(400 - mh, 40 + (0 + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
        setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

        DrawManager::setFont(38);
        xyprintf(410 - mh, 65 + (0 + 1) * 120 + whle / 5, usx.c_str());

        DrawManager::setFillRectColor();
        DrawManager::fillroundrectwithrect(400 - mh, 40 + (1 + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
        setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

        DrawManager::setFont(38);
        xyprintf(410 - mh, 65 + (1 + 1) * 120 + whle / 5, s.c_str());
    }
    if (Choose4Choose == 2)
    {
        wstring themeModes[8][3] = {
            // Chinese (Simplified)
            {L"深色模式", L"浅色模式", L"跟随系统"},
            // English
            {L"Dark mode", L"Light mode", L"Follow system"},
            // Japanese
            {L"ダークモード", L"ライトモード", L"システムに従う"},
            // Korean
            {L"다크 모드", L"라이트 모드", L"시스템 설정 따름"},
            // French
            {L"Mode sombre",L"Mode clair", L"Suivre le système"},
            // German
            {L"Dunkler Modus", L"Heller Modus", L"System folgen"},
            // Russian
            {L"Тёмная тема", L"Светлая тема", L"Как в системе"},
            // Spanish
            {L"Modo oscuro", L"Modo claro", L"Seguir sistema"}
        };
        int mh = 300 - isGotoChoose4XChange;
        for (int i = 0; i < 3; i++)
        {
            

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(400 - mh, 40 + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            DrawManager::setFont(38);
            xyprintf(430 - mh, 65 + (i + 1) * 120 + whle / 5, themeModes[stoi(userset[0])][i].c_str());
            
            setlinewidth(5);
            setcolor(EGERGB(155, 155, 155));
			circle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, 13);
        }

        if (stoi(userset[3]) == 0)
        {
            int i = 2;
            setcolor(mainColor);
            setlinewidth(8);
            line(110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 - MFCl, 110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 + MFCl);

            setfillcolor(mainColor);
            setcolor(mainColor);
            setlinewidth(1);
            fillcircle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, MFCl / 2);
        }
        if (stoi(userset[3]) == 1)
        {
            int i = 0;
            setcolor(mainColor);
            setlinewidth(8);
            line(110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 - MFCl, 110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 + MFCl);

            setfillcolor(mainColor);
            setcolor(mainColor);
            setlinewidth(1);
            fillcircle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, MFCl / 2);
        }
        if (stoi(userset[3]) == 2)
        {
            int i = 1;
            setcolor(mainColor);
            setlinewidth(8);
            line(110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 - MFCl, 110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 + MFCl);

            setfillcolor(mainColor);
            setcolor(mainColor);
            setlinewidth(1);
            fillcircle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, MFCl / 2);
        }
    }
    if (Choose4Choose == 3)
    {
        wstring graphicSettings[8][2] = {
            // Chinese (Simplified)
            {L"模糊度", L"不透明度"},
            // English
            {L"Blur", L"Opacity"},
            // Japanese
            {L"ぼかし度", L"不透明度"},
            // Korean
            {L"흐림 정도", L"불투명도"},
            // French
            {L"Flou", L"Opacité"},
            // German
            {L"Unschärfe", L"Deckkraft"},
            // Russian
            {L"Размытие", L"Непрозрачность"},
            // Spanish
            {L"Desenfoque", L"Opacidad"}
        };

        for (int i = 0; i < 2; i++)
        {
            int mh = 300 - isGotoChoose4XChange;

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(400 - mh, 40 + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            DrawManager::setFont(38);
            xyprintf(410 - mh, 65 + (i + 1) * 120 + whle / 5, graphicSettings[stoi(userset[0])][i].c_str());

            xyprintf(400 - mh + windowSize.x - 120 - 100, 65 + (i + 1) * 120 + whle / 5,"%s %%", userset[5 + i]);

            setlinewidth(4);
            setcolor(EGERGB(155, 155, 155));
            if(i == 0 && Choose4ChooseChange550X[0] > 0)
            {
                line(400 - mh + windowSize.x - 120 - 100 + 20 - Choose4ChooseChange550X[0],
                    65 + (i + 1) * 120 + whle / 5 + 40,
                    400 - mh + windowSize.x - 120 - 100 + 20 + Choose4ChooseChange550X[0],
                    65 + (i + 1) * 120 + whle / 5 + 40
                );
            }
            if (i == 1 && Choose4ChooseChange550X[1] > 0)
            {
                line(400 - mh + windowSize.x - 120 - 100 + 20 - Choose4ChooseChange550X[1],
                    65 + (i + 1) * 120 + whle / 5 + 40,
                    400 - mh + windowSize.x - 120 - 100 + 20 + Choose4ChooseChange550X[1],
                    65 + (i + 1) * 120 + whle / 5 + 40
                );
            }

            if (keystate(0x01))
            {
                if (mousePos.x > 400 - mh + windowSize.x - 120 - 140 && mousePos.x < 400 - mh + windowSize.x - 120 - 60)
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (mousePos.y > 40 + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + (i + 1) * 120 + whle / 5 + 100)
                        {
                            for (int k = 0; k < 2; k++)
                            {
                                Choose4ChooseChange550[k] = false;
                                Choose4ChooseChange550Xframe[k] = 0;
                            }
                            Choose4ChooseChange550[i] = true;
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < 2; j++)
                    {
                        Choose4ChooseChange550[j] = false;
                        Choose4ChooseChange550Xframe[j] = 0;
                    }
                }
            }

            if (keystate(0x0d))
            {
                for (int j = 0; j < 2; j++)
                {
                    Choose4ChooseChange550[j] = false;
                    Choose4ChooseChange550Xframe[j] = 0;
                }
            }


                if (Choose4ChooseChange550Xframe[0] < 60)
                {
                    int start;
                    int end;
                    if (Choose4ChooseChange550[0])
                    {
                        start = 0;
                        end = 30;
                    }
                    else if(Choose4ChooseChange550X[0] > 0)
                    {
                        start = 30;
                        end = 0;
                    }
                    else
                    {
                        start = 0;
                        end = 0;
                    }
                    double t = Choose4ChooseChange550Xframe[0] / (double)60;

                    Choose4ChooseChange550X[0] = start + (end - start) * ease::easeOut(t, 4);

                    Choose4ChooseChange550Xframe[0]++;
                }
                if (Choose4ChooseChange550Xframe[1] < 60)
                {
                    int start;
                    int end;
                    if (Choose4ChooseChange550[1])
                    {
                        start = 0;
                        end = 30;
                    }
                    else if(Choose4ChooseChange550X[1] > 0)
                    {
                        start = 30;
                        end = 0;
                    }
                    else
                    {
                        start = 0;
                        end = 0;
                    }
                    double t = Choose4ChooseChange550Xframe[1] / (double)60;

                    Choose4ChooseChange550X[1] = start + (end - start) * ease::easeOut(t, 4);

                    Choose4ChooseChange550Xframe[1]++;
                }

            if (Choose4ChooseChange550[0] || Choose4ChooseChange550[1])
            {
                if (kbhit())
                {
                    char m = getch();
                    if (m >= '0' && m <= '9')
                    {
                        if (Choose4ChooseChange550[0])
                        {
                            int a = stoi(userset[5]);
                            string k(1, m);
                            if(a <= 100)
                            {
                                if (a <= 10)
                                {
                                    a *= 10;
                                    a += stoi(k);
                                }
                            }
                            userset[5] = to_string(a);
                            needChangeUserset5 = true;
                        }

                        if (Choose4ChooseChange550[1])
                        {
                            int a = stoi(userset[6]);
                            string k(1, m);
                            if (a <= 100)
                            {
                                if (a<=10)
                                {
                                    a *= 10;
                                    a += stoi(k);
                                }
                            }
                            userset[6] = to_string(a);
                        }
                    }
                }
                if (keystate(0x08))
                {
                    if(isKEY_0x08)
                    {
                        if (Choose4ChooseChange550[0])
                        {
                            int a = stoi(userset[5]);
                            if (a > 10) a /= 10;
                            else a = 0;
                            userset[5] = to_string(a);
                        }

                        if (Choose4ChooseChange550[1])
                        {
                            int a = stoi(userset[6]);
                            if (a > 10) a /= 10;
                            else a = 0;
                            userset[6] = to_string(a);
                        }
                        isKEY_0x08 = false;
                    }
                }
                else
                {
                    isKEY_0x08 = true;
                }
            }
        }
    }
    if (Choose4Choose == 4)
    {
        wstring downloadLocationStrings[8][2] = {
            // Chinese (Simplified)
            {L"设置的默认下载位置", L"重新选择默认的下载位置"},
            // English
            {L"Set default download location", L"Change default download location"},
            // Japanese
            {L"デフォルトのダウンロード場所を設定", L"デフォルトのダウンロード場所を再選択"},
            // Korean
            {L"기본 다운로드 위치 설정", L"기본 다운로드 위치 다시 선택"},
            // French
            {L"Définir l'emplacement de téléchargement par défaut", L"Modifier l'emplacement de téléchargement par défaut"},
            // German
            {L"Standard-Downloadort festlegen",L"Standard-Downloadort ändern"},
            // Russian
            {L"Установить папку загрузок по умолчанию", L"Изменить папку загрузок по умолчанию"},
            // Spanish
            {L"Establecer ubicación de descarga predeterminada", L"Cambiar ubicación de descarga predeterminada"}
        };

        int mh = 300 - isGotoChoose4XChange;

        for (int i = 0; i < 2; i++)
            {
                DrawManager::setFillRectColor();
                DrawManager::fillroundrectwithrect(400 - mh, 40 + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);

                DrawManager::setFont(38);
                wstring msg;
                if(i == 0)
                {
                    if (userset[4] == "0") msg = downloadLocationStrings[stoi(userset[0])][i] + Net::GetDownloadFolderPath();
                    else msg = downloadLocationStrings[stoi(userset[0])][i] + userSaveDir;
                }
                else
                {
                    msg = downloadLocationStrings[stoi(userset[0])][i];
                }
                xyprintf(430 - mh, 65 + (i + 1) * 120 + whle / 5, msg.c_str());
            }

        setlinewidth(9);
        setcolor(EGERGB(155, 155, 155));
        line(-200 + mh + windowSize.x - 120 - 60, 40 + (1 + 1) * 120 + whle / 5 + 35, -200 + mh + windowSize.x - 120 - 40, 40 + (1 + 1) * 120 + whle / 5 + 50);
        line(-200 + mh + windowSize.x - 120 - 60, 40 + (1+ 1) * 120 + whle / 5 + 65, -200 + mh + windowSize.x - 120 - 40, 40 + (1 + 1) * 120 + whle / 5 + 50);
    }
    if (Choose4Choose == 5)
    {
        std::wstring languageSelection[8] = {
    L"选择语言",        // Chinese (Simplified)
    L"Select language", // English
    L"言語選択",        // Japanese
    L"언어 선택",       // Korean
    L"Choisir langue",  // French
    L"Sprache wählen",  // German
    L"Выбор языка",     // Russian
    L"Elegir idioma"    // Spanish
        };
        for (int i = 0; i < 8; i++)
        {
            int mh = 300 - isGotoChoose4XChange;

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(400 - mh, 40 + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            if (lgChange == 1)
            {
                if (fuckNMD_frame < 120)
                {
                    double start = 0;
                    double end = 38;

                    double t = fuckNMD_frame / (double)120;

                    int startB = 20;
                    int endB = 0;
                    MFCl = startB + (endB - startB) * ease::easeIn(t, 3);

                    fuckNMD  = start + (end - start) * ease::easeIn(t, 3);
                    fuckNMD_frame++;
                }
                else
                {
                    fuckNMD_frame = 0;
                    lgChange = 2;
                    if (lgChangeN > -1 && lgChangeN < 8) userset[0] = to_string(lgChangeN);
                    else userset[0] = "0";
                    lgChangeN = -1;
                }
            }
            if (lgChange == 2)
            {
                if (fuckNMD_frame < 120)
                {
                    double start = 38;
                    double end = 0;

                    double t = fuckNMD_frame / (double)120;

                    int startB = 0;
                    int endB = 20;
                    MFCl = startB + (endB - startB) * ease::easeOutBack(t, 3);

                    fuckNMD = start + (end - start) * ease::easeOutBack(t, 2);
                    fuckNMD_frame++;
                }
                else
                {
                    MFCl = 20;
                    lgChange = 0;
                }
            }

                if (i == 3)
                {
                    setfont(38, 0, "Ownglyph Soomini Regular");
                }
                else
                {
                    setfont(38, 0, "Aa吧唧一口小可耐");
                }
            xyprintf(430 - mh, 65 + (i + 1) * 120 + whle / 5, languageSelection[i].c_str());

            setlinewidth(5);
            setcolor(EGERGB(155, 155, 155));
            circle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, 13);

            if (stoi(userset[0]) == i)
            {
                setcolor(mainColor);
                setlinewidth(8);
                line(110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 - MFCl, 110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 + MFCl);

                setfillcolor(mainColor);
                setcolor(mainColor);
                setlinewidth(1);
                fillcircle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, MFCl / 2);
            }
        }
    }
    if (Choose4Choose == 6)
    {
        int mh = 300 - isGotoChoose4XChange;
        std::wstring clearListTranslations[8] = {
        L"清除名单",          // Chinese (Simplified)
          L"Clear list",       // English
            L"リストをクリア",    // Japanese
         L"목록 지우기",      // Korean
          L"Effacer la liste", // French
          L"Liste löschen",    // German
          L"Очистить список",  // Russian
          L"Limpiar lista"     // Spanish
        };
        DrawManager::setFillRectColor();
        DrawManager::fillroundrectwithrect(400 - mh, 40 + (0 + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
        DrawManager::setFont(38);
        xyprintf(430 - mh, 65 + (0 + 1) * 120 + whle / 5, clearListTranslations[stoi(userset[0])].c_str());

        setlinewidth(9);
        setcolor(EGERGB(155, 155, 155));
        line(-200 + mh + windowSize.x - 120 - 60, 40 + (0 + 1) * 120 + whle / 5 + 35, -200 + mh + windowSize.x - 120 - 40, 40 + (0 + 1) * 120 + whle / 5 + 50);
        line(-200 + mh + windowSize.x - 120 - 60, 40 + (0 + 1) * 120 + whle / 5 + 65, -200 + mh + windowSize.x - 120 - 40, 40 + (0 + 1) * 120 + whle / 5 + 50);

        if (keystate(0x01))
        {
            if(isKEY)
            {
                if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20)
                {
                    if(mousePos.y > 40 + (0 + 1) * 120 + whle / 5 && mousePos.y < 40 + (0 + 1) * 120 + whle / 5 + 100)
                    {
                        noList.clear();
                        std::string blacklistClearedMessages[8] = {
                          "已经清除了您的NetTrack黑名单",  // Chinese (Simplified)
                          "Your NetTrack blacklist has been cleared",  // English
                         "NetTrackのブラックリストをクリアしました",  // Japanese
                         "NetTrack 블랙리스트가 초기화되었습니다",  // Korean
                         "Votre liste noire NetTrack a été effacée",  // French
                            "Ihre NetTrack-Blacklist wurde geleert",  // German
                         "Ваш черный список NetTrack был очищен",  // Russian
                         "Se ha limpiado tu lista negra de NetTrack"  // Spanish
                        };
                        thread a(msgBox::msgbox,hInstanceTop,"NetTrack",blacklistClearedMessages[stoi(userset[0])],WinPrompt::ICON_INFORMATION,WinPrompt::BTN_OK,WinPrompt::MODE_INFO);
                        a.detach();
						isKEY = false;
					}
                }
            }
        }
        else
        {
            isKEY = true;
        }
    }
    if (Choose4Choose == 7)
    {
        std::string connectionOptions[8][3] = {
            // Chinese (Simplified)
            {
                "允许通过密码连接并提示链接",
                "允许直接通过密码连接NetTrack",
                "不通过密码即可直接连接NetTrack"
            },
            // English
            {
                "Allow password-protected connections with prompts",
                "Allow direct password connections to NetTrack",
                "Connect to NetTrack directly without password"
            },
            // Japanese
            {
                "パスワード接続を許可し接続を通知",
                "パスワードでNetTrackに直接接続を許可",
                "パスワードなしでNetTrackに直接接続"
            },
            // Korean
            {
                "비밀번호 연결 허용 및 연결 알림",
                "비밀번호로 NetTrack에 직접 연결 허용",
                "비밀번호 없이 NetTrack에 직접 연결"
            },
            // French
            {
                "Autoriser les connexions par mot de passe avec notification",
                "Autoriser les connexions directes par mot de passe à NetTrack",
                "Se connecter directement à NetTrack sans mot de passe"
            },
            // German
            {
                "Passwortgeschützte Verbindungen mit Hinweis erlauben",
                "Direkte Passwortverbindungen zu NetTrack erlauben",
                "Direkte Verbindung zu NetTrack ohne Passwort"
            },
            // Russian
            {
                "Разрешить подключение по паролю с уведомлением",
                "Разрешить прямое подключение к NetTrack по паролю",
                "Подключаться к NetTrack напрямую без пароля"
            },
            // Spanish
            {
                "Permitir conexiones con contraseña y notificación",
                "Permitir conexiones directas con contraseña a NetTrack",
                "Conectarse directamente a NetTrack sin contraseña"
            }
        };
        for (int i = 0; i < 3; i++)
        {
            int mh = 300 - isGotoChoose4XChange;

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(400 - mh, 40 + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            DrawManager::setFont(38);
            xyprintf(430 - mh, 65 + (i + 1) * 120 + whle / 5, connectionOptions[stoi(userset[0])][i].c_str());

            setlinewidth(5);
            setcolor(EGERGB(155, 155, 155));
            circle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, 13);

            if (stoi(userset[7]) == i)
            {
                if(i != 2)
                {
                    setfillcolor(mainColor);
                    setcolor(mainColor);
                }
                else
                {
                    setfillcolor(EGERGB(255,255,100));
                    setcolor(EGERGB(255, 255, 100));
                }
                setlinewidth(1);
                fillcircle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, MFCl / 2);

                if (i != 2)
                {
                    setcolor(mainColor);
                }
                else
                {
                    setcolor(EGERGB(255, 255, 100));
                }
                setlinewidth(8);
                line(110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 - MFCl, 110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 + MFCl);
            }
        }
    }
    if (Choose4Choose == 8)
    {
        std::string resetPassword[8] = {
    "重新设置当前密码",  // Chinese (Simplified)
    "Reset current password",  // English
    "現在のパスワードを再設定",  // Japanese
    "현재 비밀번호 재설정",  // Korean
    "Réinitialiser le mot de passe actuel",  // French
    "Aktuelles Passwort zurücksetzen",  // German
    "Сбросить текущий пароль",  // Russian
    "Restablecer contraseña actual"  // Spanish
        };
        std::string createPassword[8] = {
    "创建NetTrack的密码",  // Chinese (Simplified)
    "Create password for NetTrack",  // English
    "NetTrackのパスワードを作成",  // Japanese
    "NetTrack 비밀번호 생성",  // Korean
    "Créer un mot de passe pour NetTrack",  // French
    "Passwort für NetTrack erstellen",  // German
    "Создать пароль для NetTrack",  // Russian
    "Crear contraseña para NetTrack"  // Spanish
        };


            int mh = 300 - isGotoChoose4XChange;

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(400 - mh, 40 + (0 + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            DrawManager::setFont(38);
            if(userset[2] == "0") xyprintf(410 - mh, 65 + (0 + 1) * 120 + whle / 5, createPassword[stoi(userset[0])].c_str());
            else xyprintf(410 - mh, 65 + (0 + 1) * 120 + whle / 5, resetPassword[stoi(userset[0])].c_str());

            setlinewidth(9);
            setcolor(EGERGB(155, 155, 155));
            line(-200 + mh + windowSize.x - 120 - 60, 40 + (0 + 1) * 120 + whle / 5 + 35, -200 + mh + windowSize.x - 120 - 40, 40 + (0 + 1) * 120 + whle / 5 + 50);
            line(-200 + mh + windowSize.x - 120 - 60, 40 + (0 + 1) * 120 + whle / 5 + 65, -200 + mh + windowSize.x - 120 - 40, 40 + (0 + 1) * 120 + whle / 5 + 50);

            if (keystate(0x01))
            {
                if (isKEY)
                {
                    if (mousePos.x > windowSize.x - 220 && mousePos.x < windowSize.x - 20)
                    {
                        if (mousePos.y > 40 + (0 + 1) * 120 + whle / 5 && mousePos.y < 40 + (0 + 1) * 120 + whle / 5 + 100)
                        {
                            string back = moto::reset();
                            if (back != "NULL")
                            {
                                userset[2] = back;
                                string passwordUpdatedMsg[8] = {
                           "您的密码在刚刚被NetTrack终端更新为：",  // Chinese
                           "Your password has just been updated by NetTrack terminal to:",  // English
                           "NetTrack端末によりパスワードが更新されました:",  // Japanese
                          "NetTrack 단말기에서 비밀번호를 다음으로 업데이트했습니다:",  // Korean
                              "Votre mot de passe vient d'être mis à jour par le terminal NetTrack sur :",  // French
                             "Ihr Passwort wurde soeben vom NetTrack-Terminal aktualisiert auf:",  // German
                               "Ваш пароль был только что обновлен терминалом NetTrack на:",  // Russian
                                  "Su contraseña acaba de ser actualizada por el terminal NetTrack a:"  // Spanish
                                };

                                int c[3] = { 115,43,245 };
                                int fc[3] = { -1,-1,-1 };

                                msg::Message(1, c,fc, 240, passwordUpdatedMsg[stoi(userset[0])] + back);
                            }
                            isKEY = false;
                        }
                    }
                }
            }
            else
            {
                isKEY = true;
            }
    }
    if (Choose4Choose == 9)
    {
        std::string speedLimitDescription[8] = {
            // Chinese (Simplified)
            "设置您的传输速度上限,当传输时,NetTrack为了达到最大的速度上限时可能会占用大量的系统资源导致卡顿,如果您希望降低占用或需要高性能环境时，可设置此选项",

            // English
            "Set your maximum transfer speed. During transfers, NetTrack may consume significant system resources to reach the maximum speed limit, which could cause lag. You can configure this option if you want to reduce resource usage or need a high-performance environment.",

            // Japanese
            "転送速度の上限を設定します。転送時、NetTrackは最大速度上限に達するために大量のシステムリソースを占有する可能性があり、ラグが発生する場合があります。リソース使用量を減らしたい場合や高性能環境が必要な場合は、このオプションを設定できます。",

            // Korean
            "전송 속도 상한을 설정하십시오. 전송 시 NetTrack은 최대 속도 상한에 도달하기 위해 상당한 시스템 리소스를 점유할 수 있어 지연이 발생할 수 있습니다. 리소스 사용량을 줄이거나 고성능 환경이 필요한 경우 이 옵션을 설정할 수 있습니다.",

            // French
            "Définissez votre limite de vitesse de transfert. Pendant les transferts, NetTrack peut consommer des ressources système importantes pour atteindre la vitesse maximale, ce qui peut provoquer des ralentissements. Vous pouvez configurer cette option si vous souhaitez réduire l'utilisation des ressources ou avez besoin d'un environnement haute performance.",

            // German
            "Legen Sie Ihre maximale Übertragungsgeschwindigkeit fest. Während der Übertragung kann NetTrack erhebliche Systemressourcen beanspruchen, um die maximale Geschwindigkeit zu erreichen, was zu Verzögerungen führen kann. Sie können diese Option konfigurieren, wenn Sie die Ressourcennutzung reduzieren möchten oder eine Hochleistungsumgebung benötigen.",

            // Russian
            "Установите максимальную скорость передачи. Во время передачи NetTrack может использовать значительные системные ресурсы для достижения максимальной скорости, что может вызвать задержки. Вы можете настроить этот параметр, если хотите уменьшить использование ресурсов или вам нужна высокопроизводительная среда.",

            // Spanish
            "Establezca su límite máximo de velocidad de transferencia. Durante las transferencias, NetTrack puede consumir recursos significativos del sistema para alcanzar el límite máximo de velocidad, lo que podría causar retrasos. Puede configurar esta opción si desea reducir el uso de recursos o necesita un entorno de alto rendimiento."
        };
        std::string setSpeedLimit[8] = {
    "设置速度上限",          // Chinese (Simplified)
    "Set speed limit",      // English
    "速度制限を設定",        // Japanese
    "속도 제한 설정",       // Korean
    "Définir limite de vitesse",  // French
    "Geschwindigkeitsbegrenzung einstellen",  // German
    "Установить ограничение скорости",  // Russian
    "Establecer límite de velocidad"  // Spanish
        };

        int mh = 300 - isGotoChoose4XChange;

        for (int i = 0; i < 1; i++)
        {
            

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(400 - mh, 40 + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            DrawManager::setFont(38);
            xyprintf(410 - mh, 65 + (i + 1) * 120 + whle / 5, setSpeedLimit[stoi(userset[0])].c_str());

            xyprintf(400 - mh + windowSize.x - 120 - 160, 65 + (i + 1) * 120 + whle / 5, "%s MB/s", userset[8]);

            setcolor(EGERGB(155, 155, 155));
            setlinewidth(6);
            line(400 - mh + windowSize.x - 120 - 100 - 20 - Choose4ChooseChange550X[2],
                65 + (i + 1) * 120 + whle / 5 + 40,
                400 - mh + windowSize.x - 120 - 100 - 20 + Choose4ChooseChange550X[2],
                65 + (i + 1) * 120 + whle / 5 + 40
            );
        }

        if (keystate(0x01))
        {
            if(isKEY)
            {
                if (mousePos.x > 400 - mh + windowSize.x - 120 - 140 - 40 && mousePos.x < 400 - mh + windowSize.x - 120 - 60 - 40)
                {
                    for (int i = 0; i < 1; i++)
                    {
                        if (mousePos.y > 40 + (i + 1) * 120 + whle / 5 && mousePos.y < 40 + (i + 1) * 120 + whle / 5 + 100)
                        {
                            temp_AS = true;
                            Choose4ChooseChange550Xframe[2] = 0;
                            Choose4ChooseChange550[2] = true;
                        }
                    }
                }
                else
                {
                    Choose4ChooseChange550[2] = false;
                    Choose4ChooseChange550Xframe[2] = 0;
                    if(temp_AS)
                    {
                        string speedLimitUpdated[8] = {
                         "已更新传输速度上限为：",          // Chinese
                          "Transfer speed limit updated to:",  // English
                           "転送速度制限を以下に更新しました:",  // Japanese
                           "전송 속도 제한이 다음으로 업데이트됨:",  // Korean
                              "Limite de vitesse de transfert mise à jour à :",  // French
                              "Übertragungsgeschwindigkeitsbegrenzung aktualisiert auf:",  // German
                              "Лимит скорости передачи обновлен до:",  // Russian
                              "Límite de velocidad de transferencia actualizado a:"  // Spanish
                        };

                        Net::setSendSpeedLimitMB(stoi(userset[8]));

                        int c[3] = { 150,255,150 };
                        int fc[3] = { -1,-1,-1 };
                        if (stoi(userset[8]) != 0)msg::Message(1, c, fc, 240, speedLimitUpdated[stoi(userset[0])] + userset[8] + " MB/s");
                        else
                        {
                            string noSpeedLimit[8] = {
            "不限制速度",          // Chinese
            "No speed limit",     // English
            "速度制限なし",       // Japanese
            "속도 제한 없음",     // Korean
            "Pas de limite de vitesse",  // French
            "Keine Geschwindigkeitsbegrenzung",  // German
            "Без ограничения скорости",  // Russian
            "Sin límite de velocidad"  // Spanish
                            };
                            msg::Message(1, c, fc, 240, noSpeedLimit[stoi(userset[0])]);
                        }
                    }

                    temp_AS = false;
                }
                isKEY = false;
            }
        }
        else
        {
            isKEY = true;
        }

        if (keystate(0x0d))
        {
            if(isKEY_0x0d)
            {
                Choose4ChooseChange550[2] = false;
                Choose4ChooseChange550Xframe[2] = 0;

                if(temp_AS)
                {
                    string speedLimitUpdated[8] = {
                       "已更新传输速度上限为：",          // Chinese
                      "Transfer speed limit updated to:",  // English
                          "転送速度制限を以下に更新しました:",  // Japanese
                          "전송 속도 제한이 다음으로 업데이트됨:",  // Korean
                             "Limite de vitesse de transfert mise à jour à :",  // French
                              "Übertragungsgeschwindigkeitsbegrenzung aktualisiert auf:",  // German
                              "Лимит скорости передачи обновлен до:",  // Russian
                              "Límite de velocidad de transferencia actualizado a:"  // Spanish
                    };

                    Net::setSendSpeedLimitMB(stoi(userset[8]));

                    int c[3] = { 150,255,150 };
                    int fc[3] = { -1,-1,-1 };
                    if (stoi(userset[8]) != 0)msg::Message(1, c, fc, 240, speedLimitUpdated[stoi(userset[0])] + userset[8] + " MB/s");
                    else
                    {
                        string noSpeedLimit[8] = {
        "不限制速度",          // Chinese
        "No speed limit",     // English
        "速度制限なし",       // Japanese
        "속도 제한 없음",     // Korean
        "Pas de limite de vitesse",  // French
        "Keine Geschwindigkeitsbegrenzung",  // German
        "Без ограничения скорости",  // Russian
        "Sin límite de velocidad"  // Spanish
                        };
                        msg::Message(1, c, fc, 240, noSpeedLimit[stoi(userset[0])]);
                    }
                }

                temp_AS = false;
                isKEY_0x0d = false;
            }
        }
        else
        {
            isKEY_0x0d = true;
        }


        if (Choose4ChooseChange550Xframe[2] < 60)
        {
            int start;
            int end;
            if (Choose4ChooseChange550[2])
            {
                start = 0;
                end = 30;
            }
            else if (Choose4ChooseChange550X[2] > 0)
            {
                start = 30;
                end = 0;
            }
            else
            {
                start = 0;
                end = 0;
            }
            double t = Choose4ChooseChange550Xframe[2] / (double)60;

            Choose4ChooseChange550X[2] = start + (end - start) * ease::easeOut(t, 4);

            Choose4ChooseChange550Xframe[2]++;
        }

        if (Choose4ChooseChange550[2])
        {
            if (kbhit())
            {
                char m = getch();
                if (m >= '0' && m <= '9')
                {
                    if (Choose4ChooseChange550[2])
                    {
                        int a = stoi(userset[8]);
                        string k(1, m);
                        if (a <= 100)
                        {
                            if (a <= 1000)
                            {
                                a *= 10;
                                a += stoi(k);
                            }
                        }
                        userset[8] = to_string(a);
                        needChangeUserset5 = true;
                    }

                }
            }
            if (keystate(0x08))
            {
                if (isKEY_0x08)
                {
                    if (Choose4ChooseChange550[2])
                    {
                        int a = stoi(userset[8]);
                        if (a > 10) a /= 10;
                        else a = 0;
                        userset[8] = to_string(a);
                    }
                    isKEY_0x08 = false;
                }
            }
            else
            {
                isKEY_0x08 = true;
            }
        }
    }
    if (Choose4Choose == 10)
    {
        std::string encryptionModes[8][2] = {
            // Chinese (Simplified)
            {"加密模式", "无加密"},
            // English
            {"Encryption mode", "No encryption"},
            // Japanese
            {"暗号化モード", "暗号化なし"},
            // Korean
            {"암호화 모드", "암호화 없음"},
            // French
            {"Mode chiffrement", "Pas de chiffrement"},
            // German
            {"Verschlüsselungsmodus", "Keine Verschlüsselung"},
            // Russian
            {"Режим шифрования", "Без шифрования"},
            // Spanish
            {"Modo cifrado", "Sin cifrado"}
        };
        string encryption[3] =
        {
            "XOR",
            "LCD（NetTrack）",
            "AES"
        };

        for (int i = 0; i < 2; i++)
        {
            int mh = 300 - isGotoChoose4XChange;

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(400 - mh, 40 + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            DrawManager::setFont(38);
            xyprintf(430 - mh, 65 + (i + 1) * 120 + whle / 5, encryptionModes[stoi(userset[0])][i].c_str());

            if (i == 1)
            {
                setlinewidth(5);
                setcolor(EGERGB(155, 155, 155));
                circle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, 13);

                if (stoi(userset[1]) == 0)
                {
                    setcolor(EGERGB(255,255,100));
                    setlinewidth(8);
                    line(110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 - MFCl, 110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 + MFCl);

                    setfillcolor(EGERGB(255, 255, 100));
                    setcolor(EGERGB(255, 255, 100));
                    setlinewidth(1);
                    fillcircle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, MFCl / 2);
                }
            }
        }

        for (int i = 0; i < 3; i++)
        {
            int mh = 300 - isGotoChoose4XChange;

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(400 - mh, 40 + (i + 3) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            DrawManager::setFont(38);
            xyprintf(430 - mh, 65 + (i + 3) * 120 + whle / 5, encryption[i].c_str());

            setlinewidth(5);
            setcolor(EGERGB(155, 155, 155));
            circle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 3) * 120 + whle / 5 + 50, 13);

            if (stoi(userset[1]) == 1 + i)
            {
                setcolor(mainColor);
                setlinewidth(8);
                line(110 + 300 - mh, 40 + (i + 3) * 120 + whle / 5 + 50 - MFCl, 110 + 300 - mh, 40 + (i + 3) * 120 + whle / 5 + 50 + MFCl);

                setfillcolor(mainColor);
                setcolor(mainColor);
                setlinewidth(1);
                fillcircle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 3) * 120 + whle / 5 + 50, MFCl / 2);
            }
        }
    }
    if (Choose4Choose == 11)
    {
        wstring toggleStatesW[8][2] = {
    {L"开启", L"禁用"},     // Chinese
    {L"Enable", L"Disable"}, // English
    {L"有効", L"無効"},     // Japanese
    {L"활성화", L"비활성화"}, // Korean
    {L"Activer", L"Désactiver"}, // French
    {L"Aktivieren", L"Deaktivieren"}, // German
    {L"Включить", L"Отключить"}, // Russian
    {L"Habilitar", L"Deshabilitar"} // Spanish
        };
        int mh = 300 - isGotoChoose4XChange;
        for (int i = 0; i < 2; i++)
        {
            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(400 - mh, 40 + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            DrawManager::setFont(38);
            xyprintf(430 - mh, 65 + (i + 1) * 120 + whle / 5, toggleStatesW[stoi(userset[0])][i].c_str());

            setlinewidth(5);
            setcolor(EGERGB(155, 155, 155));
            circle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, 13);

            if (stoi(userset[9]) == i)
            {
                if(stoi(userset[9]) == 0) setcolor(mainColor);
                else setcolor(EGERGB(255,255,100));
                setlinewidth(8);
                line(110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 - MFCl, 110 + 300 - mh, 40 + (i + 1) * 120 + whle / 5 + 50 + MFCl);

                if (stoi(userset[9]) == 0)
                {
                    setfillcolor(mainColor);
                    setcolor(mainColor);
                }
                else
                {
                    setfillcolor(EGERGB(255, 255, 100));
                    setcolor(EGERGB(255, 255, 100));
                }
                setlinewidth(1);
                fillcircle(400 + windowSize.x - 120 - mh - 50, 40 + (i + 1) * 120 + whle / 5 + 50, MFCl / 2);
            }
        }


    }
    

    wstring setLg[8][16] = {
        // 中文
        {
            L"设备属性", L"颜色", L"NetTrack的背景", L"默认下载位置", L"语言", L"名单设置",
            L"设备连接的方式", L"本机密码重新设置", L"传输速度限制", L"传输加密选项", L"随机端口更换",
            L"", L"", L"", L"", L""
        },
        // English
        {
            L"Device Properties", L"Color", L"NetTrack Background", L"Default Download Location", L"Language", L"List Settings",
            L"Device Connection Method", L"Local Password Reset", L"Transfer Speed Limit", L"Transfer Encryption Option", L"Random Port Switching",
            L"", L"", L"", L"", L""
        },
        // 日本語
        {
            L"デバイスプロパティ", L"色", L"NetTrack背景", L"デフォルトのダウンロード場所", L"言語", L"リスト設定",
            L"デバイス接続方法", L"ローカルパスワード再設定", L"転送速度制限", L"転送暗号化オプション", L"ランダムポート切替",
            L"", L"", L"", L"", L""
        },
        // 한국어
        {
            L"장치 속성", L"색상", L"NetTrack 배경", L"기본 다운로드 위치", L"언어", L"목록 설정",
            L"장치 연결 방식", L"로컬 암호 재설정", L"전송 속도 제한", L"전송 암호화 옵션", L"랜덤 포트 변경",
            L"", L"", L"", L"", L""
        },
        // Français
        {
            L"Propriétés de l'appareil", L"Couleur", L"Fond NetTrack", L"Emplacement de téléchargement par défaut", L"Langue", L"Paramètres de liste",
            L"Méthode de connexion de l'appareil", L"Réinitialisation du mot de passe local", L"Limite de vitesse de transfert", L"Option de chiffrement de transfert", L"Changement aléatoire de port",
            L"", L"", L"", L"", L""
        },
        // Deutsch
        {
            L"Geräteeigenschaften", L"Farbe", L"NetTrack-Hintergrund", L"Standard-Downloadort", L"Sprache", L"Listeneinstellungen",
            L"Geräteverbindungsmethode", L"Lokale Passwortzurücksetzung", L"Übertragungsgeschwindigkeitsbegrenzung", L"Übertragungsverschlüsselungsoption", L"Zufälliger Portwechsel",
            L"", L"", L"", L"", L""
        },
        // Русский
        {
            L"Свойства устройства", L"Цвет", L"Фон NetTrack", L"Место загрузки по умолчанию", L"Язык", L"Настройки списка",
            L"Способ подключения устройства", L"Сброс локального пароля", L"Ограничение скорости передачи", L"Опция шифрования передачи", L"Случайная смена порта",
            L"", L"", L"", L"", L""
        },
        // Español
        {
            L"Propiedades del dispositivo", L"Color", L"Fondo de NetTrack", L"Ubicación de descarga predeterminada", L"Idioma", L"Configuración de lista",
            L"Método de conexión del dispositivo", L"Restablecimiento de contraseña local", L"Límite de velocidad de transferencia", L"Opción de cifrado de transferencia", L"Cambio aleatorio de puerto",
            L"", L"", L"", L"", L""
        }
    };
    DrawManager::setFillRectColor();
    DrawManager::fillroundrectwithrect(100, 110, 80 + setLg[stoi(userset[0])][Choose4Choose - 1].length() * 14, 40, userSetCircle / 2);

    

    DrawManager::setFont(25 - fuckNMD / 2);
    DrawManager::centerText();

    wstring bh = L"<    " + setLg[stoi(userset[0])][Choose4Choose - 1];
    xyprintf(110 + bh.length() * 7,130, bh.c_str());
    DrawManager::startText();

    if (isGotoChoose4_frame < 60)
    {

        double start = 300;
        double end = 0;

        double t = isGotoChoose4_frame / (double)60;
        isGotoChoose4XChange = start + (end - start) * ease::easeOut(t, 5);

        int mh = 300 - isGotoChoose4XChange;
        putimage_alphablend(NULL, isGotoChoose4Image, 0 - mh, 0, 255 - mh * 255 / 300);


        isGotoChoose4_frame++;
    }

    if (MFClChange == 0)
    {
        if (MFCl_frame < 30)
        {
            double t = MFCl_frame / (double)30;

            MFCl = 20 + (0 - 20) * ease::easeInBack(t, 3);

            MFCl_frame++;
        }
        else
        {
            MFClChange = 1;
            MFCl_frame = 0;

            if(tempChangeN != -1 && tempChange != -1) userset[tempChangeN] = to_string(tempChange);

            tempChangeN = tempChange = -1;
        }
    }
    if (MFClChange == 1)
    {
        if (MFCl_frame < 30)
        {
            double t = MFCl_frame / (double)30;

            MFCl = 0 + (20 - 0) * ease::easeOutBack(t, 3);

            MFCl_frame++;
        }
    }
}

// 检测字符串编码类型
UINT DetectEncoding(const std::string & str) {
    // 尝试用常见编码检测
    const UINT testEncodings[] = {
        CP_UTF8,
        CP_ACP,  // 系统默认ANSI编码(通常是GBK)
        936,     // GBK代码页
        54936,   // GB18030
        1200,    // UTF-16 LE
        1201,    // UTF-16 BE
        65001    // UTF-8
    };

    for (UINT encoding : testEncodings) {
        int result = MultiByteToWideChar(
            encoding,
            MB_ERR_INVALID_CHARS,
            str.c_str(),
            static_cast<int>(str.size()),
            nullptr,
            0
        );

        if (result > 0) {
            return encoding;
        }
    }

    return 0; // 未知编码
}

// 将字符串转换为GBK编码
std::string ConvertToGBK(const std::string& str, UINT srcEncoding) {
    // 首先转换为宽字符
    int wideLen = MultiByteToWideChar(
        srcEncoding,
        0,
        str.c_str(),
        static_cast<int>(str.size()),
        nullptr,
        0
    );

    if (wideLen == 0) {
        throw std::runtime_error("Failed to convert to wide char");
    }

    std::vector<wchar_t> wideStr(wideLen);
    MultiByteToWideChar(
        srcEncoding,
        0,
        str.c_str(),
        static_cast<int>(str.size()),
        wideStr.data(),
        wideLen
    );

    // 然后从宽字符转换为GBK
    int gbkLen = WideCharToMultiByte(
        936, // GBK代码页
        0,
        wideStr.data(),
        wideLen,
        nullptr,
        0,
        nullptr,
        nullptr
    );

    if (gbkLen == 0) {
        throw std::runtime_error("Failed to convert to GBK");
    }

    std::vector<char> gbkStr(gbkLen);
    WideCharToMultiByte(
        936,
        0,
        wideStr.data(),
        wideLen,
        gbkStr.data(),
        gbkLen,
        nullptr,
        nullptr
    );

    return std::string(gbkStr.data(), gbkStr.size());
}

// 主函数：自动检测并转换为GBK
std::string AutoConvertToGBK(const std::string& str) {
    // 检测当前编码
    UINT detectedEncoding = DetectEncoding(str);

    // 如果已经是GBK或系统默认ANSI编码(通常是GBK)，直接返回
    if (detectedEncoding == 936 || detectedEncoding == CP_ACP) {
        return str;
    }

    // 如果不是GBK，进行转换
    if (detectedEncoding != 0) {
        try {
            return ConvertToGBK(str, detectedEncoding);
        }
        catch (const std::exception& e) {
            // 转换失败，返回原始字符串
            return str;
        }
    }

    // 无法识别的编码，尝试强制转换
    try {
        return ConvertToGBK(str, CP_UTF8); // 假设是UTF-8
    }
    catch (...) {
        return str; // 转换失败，返回原始字符串
    }
}

void DrawChoose(int userBarChoose)
{
    const int userSetCircle = 15;

    POINT windowSize = Window::GetWindowSize();
    for (int i = 0; i < 6; i++)
    {
        if (osm[i] > 0)osm[i] -= 1;
    }
    if (osm[2] == 0)
    {
        string confirmMsg10s[8] = {
            // 中文（简体）
            "请确认，10秒后自动取消",

            // 英文
            "Confirm, auto-cancel in 10s",

            // 日语
            "確認してください（10秒で自動取消）",

            // 韩语
            "확인하세요 (10초 후 자동취소)",

            // 法语
            "Confirmer (annul. auto. 10s)",

            // 德语
            "Bestätigen (in 10s automat. Abbruch)",

            // 俄语
            "Подтвердите (отмена через 10 сек)",

            // 西班牙语
            "Confirme (se cancela en 10s)"
        };
        msg::KillBox(confirmMsg10s[stoi(userset[0])]);
        osm[2] = -1;
    }

    GetDownLoadFile();
    if (linkJD != "N" || linkMac > -1)
    {
        if (MSA_frame < 90)
        {

            double t = MSA_frame / (double)90;

            double startValue = 5.0;
            double endValue;
            endValue = windowSize.x - 120 + 245;

            if (isMSA)
            {
                MSA = startValue + (endValue - startValue) * ease::easeInOut(t, 2);
            }
            else
            {
                MSA = startValue + (endValue - startValue) * ease::easeInOut(t, 2);
            }
            MSA_frame++;
        }
        else
        {
            if (isMSA) isMSA = false;
            else isMSA = true;
            MSA_frame = 0;
        }
    }

    if (userBarChoose == 1)
    {
        if (AddLinkXChange > windowSize.x / 4)
        {
            wstring connectPrompt[8] = {
                // 中文（颜文字+语气词）
                L"输入Ta的8位连接码就可以连上啦～(  •̀ᴗ-)✧",
                // English（爱心+缩写）
                L"Use their 8-digit code to connect （゜▽＾*））",
                // 日本語（颜文字+可爱语气）
                L"相手の8桁接続コードで繋がれますよ～(っ´ω`)/",
                // 한국어（缩略语+波浪号）
                L"상대방의 8자리 연결 코드로 연결돼요~ （゜▽＾*））",
                // Français（爱心+亲切动词）
                L"Utilisez son code à 8 chiffres pour vous connecter （゜▽＾*））",
                // Deutsch（小写+花朵符号）
                L"mit ihrem 8-stelligen code verbinden （゜▽＾*））",
                // Русский（表情符号）
                L"Используйте 8-значный код для подключения （゜▽＾*））",
                // Español（笑脸+亲切表达）
                L"Usa su código de 8 dígitos para conectar （゜▽＾*））"
            };
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            DrawManager::setFont(50);

            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));
            xyprintf(windowSize.x + windowSize.x / 2 - AddLinkXChange, windowSize.y / 4, connectPrompt[stoi(userset[0])].c_str());

            for (int i = 0; i < 8; i++)
            {
                setcolor(EGERGB(150, 150, 150));
                setfont(50 * 9 / 5, 0, "Aa吧唧一口小可耐");
                xyprintf(windowSize.x / 2 + windowSize.x - AddLinkXChange + windowSize.x / 29 + windowSize.x * 2 / 30 * (i - 4), windowSize.y - windowSize.y / 4 - 50, "__");
            }



            int w = windowSize.x; int h = windowSize.y;


            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            int v = 30;
            for (int i = 0; i < 8; i++)
            {
                if (linkCodeA[i] == '?')
                {
                    int totalFrames1 = 20;
                    double startValue1 = 55.0;
                    double endValue1 = 0.0;
                    int v22 = 0;
                    if (userLink_fontFrameA[i] < totalFrames1)
                    {
                        double t = userLink_fontFrameA[i] / (double)totalFrames1;
                        v22 = startValue1 + (endValue1 - startValue1) * ease::easeInBack(t, 5);

                        setfont(v22, 0, "Aa吧唧一口小可耐");

                        if (linkCode_tempA[i] != '?') xyprintf(w / 2 + windowSize.x - AddLinkXChange + w / 29 + w * 2 / 30 * (i - 4), h / 2 + h / 7, "%c", linkCode_tempA[i]);

                        userLink_fontFrameA[i]++;
                    }
                    else
                    {
                        linkCodeA[i] = '?';
                    }
                }
                else
                {
                    int totalFrames1 = 20;
                    double startValue1 = 0.0;
                    double endValue1 = 55.0;
                    int v22 = 0;

                    if (userLink_fontFrameA[i] < totalFrames1)
                    {
                        double t = userLink_fontFrameA[i] / (double)totalFrames1;
                        v22 = startValue1 + (endValue1 - startValue1) * ease::easeOutBack(t, 5);

                        setfont(v22, 0, "Aa吧唧一口小可耐");

                        xyprintf(w / 2 + windowSize.x - AddLinkXChange + w / 29 + w * 2 / 30 * (i - 4), h / 2 + h / 7, "%c", linkCodeA[i]);
                        userLink_fontFrameA[i]++;
                    }
                    else
                    {
                        v22 = endValue1;

                        setfont(v22, 0, "Aa吧唧一口小可耐");
                        xyprintf(w / 2 + windowSize.x - AddLinkXChange + w / 29 + w * 2 / 30 * (i - 4), h / 2 + h / 7, "%c", linkCodeA[i]);
                    }
                }
            }


            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(100 + windowSize.x - AddLinkXChange, 120, 150, 60, 5);

            DrawManager::fillroundrectwithrect(windowSize.x - 370 + windowSize.x - AddLinkXChange, windowSize.y - 100, 150, 60, 5);
            DrawManager::fillroundrectwithrect(windowSize.x - 200 + windowSize.x - AddLinkXChange, windowSize.y - 100, 150, 60, 5);
            DrawManager::setFont(27);
            string ok_cancel[8][2] = {
                // 中文（简体）
                {"好的", "取消"},

                // 英文
                {"OK", "Cancel"},

                // 日语
                {"はい", "キャンセル"},  // 或 "OK"/"取消"

                // 韩语
                {"확인", "취소"},       // "确认"/"取消"

                // 法语
                {"OK", "Annuler"},

                // 德语
                {"OK", "Abbrechen"},

                // 俄语
                {"Хорошо", "Отмена"},  // 或 "OK"/"Отменить"

                // 西班牙语
                {"Aceptar", "Cancelar"}  // 注意西语常用 "Aceptar" 替代 "OK"
            };
            xyprintf(windowSize.x - 370 + windowSize.x - AddLinkXChange + 75, windowSize.y - 100 + 30, ok_cancel[stoi(userset[0])][1].c_str());
            xyprintf(windowSize.x - 200 + windowSize.x - AddLinkXChange + 75, windowSize.y - 100 + 30, ok_cancel[stoi(userset[0])][0].c_str());

            DrawManager::setFont(30);
            if (DarkMode)setcolor(EGERGB(255, 255, 255));
            else setcolor(EGERGB(20, 20, 20));
            wstring backButton[8] = {
                // 中文
                L"<  返回",
                // English
                L"<  Back",
                // 日本語
                L"<  戻る",
                // 한국어
                L"<  뒤로",
                // Français
                L"<  Retour",
                // Deutsch
                L"<  Zurück",
                // Русский
                L"<  Назад",
                // Español
                L"<  Atrás"
            };
            xyprintf(100 + windowSize.x - AddLinkXChange + 75, 150, backButton[stoi(userset[0])].c_str());
            settextjustify(LEFT_TEXT, TOP_TEXT);
        }
        if (AddLinkXChange < windowSize.x / 4 * 3)
        {
            for (int i = 0; i < saveMax; i++)
            {
                if (saveData_link[i] == "NULL")
                {
                    DrawManager::setFillRectColor();
                    DrawManager::fillroundrectwithrect(100 - AddLinkXChange, 40 + CBOS + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
                    setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));
                    DrawManager::setFont(40);

                    wstring addConnection[8] = {
                        // 中文
                        L"添加新的连接",
                        // English
                        L"Add New Connection",
                        // 日本語
                        L"新しい接続を追加",
                        // 한국어
                        L"새 연결 추가",
                        // Français
                        L"Ajouter une nouvelle connexion",
                        // Deutsch
                        L"Neue Verbindung hinzufügen",
                        // Русский
                        L"Добавить новое подключение",
                        // Español
                        L"Añadir nueva conexión"
                    };

                    xyprintf(110 - AddLinkXChange, 65 + CBOS + (i + 1) * 120 + whle / 5, addConnection[stoi(userset[0])].c_str());

                    setcolor(mainColor);
                    setlinewidth(6);
                    ege_line(
                        windowSize.x - AddLinkXChange - 60 - screen.x / 85 + MacAdd * 2,
                        CBOS + (i + 1) * 120 + whle / 5 + 90,
                        windowSize.x - AddLinkXChange - 60 + screen.x / 85 - MacAdd * 2,
                        CBOS + (i + 1) * 120 + whle / 5 + 90
                    );
                    ege_line(
                        windowSize.x - AddLinkXChange - 60,
                        CBOS + (i + 1) * 120 + whle / 5 + 90 - screen.y / 45 + MacAdd * 2,
                        windowSize.x - AddLinkXChange - 60,
                        CBOS + (i + 1) * 120 + whle / 5 + 90 + screen.y / 45 - MacAdd * 2
                    );
                    linkMax = i;
                    break;
                }

                wstring deleteText[8] = {
                    // 中文
                    L"删除",
                    // English
                    L"Delete",
                    // 日本語
                    L"削除",
                    // 한국어
                    L"삭제",
                    // Français
                    L"Supprimer",
                    // Deutsch
                    L"Löschen",
                    // Русский
                    L"Удалить",
                    // Español
                    L"Eliminar"
                };

                if (linkX[i] * 255 / 150 >= 0)
                    setfillcolor(EGERGBA(205, 100, 100, linkX[i] * 255 / 100));
                else
                    setfillcolor(EGERGBA(205, 100, 100, 0));

                ege_fillroundrect(100 - linkX[i] - AddLinkXChange, 40 + CBOS + (i + 1) * 120 + whle / 5, windowSize.x - 120 + linkX[i], 100, userSetCircle);

                if (i == LKA)
                {
                    setcolor(EGERGB(255, 255, 255));
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    if (userset[0] != "3")
                    {
                        setfont(25, 0, "Aa吧唧一口小可耐");
                    }
                    else
                    {
                        setfont(25, 0, "Ownglyph Soomini Regular");
                    }
                    xyprintf(100 - AddLinkXChange + windowSize.x - 170, 40 + CBOS + (i + 1) * 120 + whle / 5 + 50, deleteText[stoi(userset[0])].c_str());
                    settextjustify(LEFT_TEXT, TOP_TEXT);

                }

                DrawManager::setFillRectColor();
                DrawManager::fillroundrectwithrect(100 - linkX[i] - AddLinkXChange, 40 + CBOS + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);

                if (i == linkOS)
                {
                    if (loadASlink == true)
                    {
                        if (loadAlink < 270)
                        {
                            loadAlink += 2;
                        }
                        else
                        {
                            loadAlink = 270;
                            loadASlink = false;
                        }
                    }
                    else
                    {
                        if (loadAlink > 10)
                        {
                            loadAlink -= 2;
                        }
                        else
                        {
                            loadAlink = 10;
                            loadASlink = true;
                        }
                    }

                    roundAlink -= 8;

                    if (roundAlink < -720) roundAlink = 0;



                    setlinecap(LINECAP_ROUND, LINECAP_ROUND);
                    setlinewidth(8);
                    int w = windowSize.x; int h = windowSize.y;

                    setcolor(mainColor);
                    arc(windowSize.x - 150 - linkX[i] - AddLinkXChange, 40 + CBOS + (i + 1) * 120 + whle / 5 + 50, roundAlink, roundAlink + loadAlink, 20);


                }

                if (linkOSB != linkOS)
                {
                    if (MSAAlpha > 0)
                    {
                        if (MSAAlpha - 15 > 0)MSAAlpha -= 15;
                        else
                        {
                            MSAAlpha = 0;
                            linkOSB = linkOS;
                        }
                    }
                    else
                    {
                        MSAAlpha = 0;
                        linkOSB = linkOS;
                    }
                }
                else
                {
                    if (MSAAlpha < 255)
                    {
                        if (MSAAlpha + 10 < 255)MSAAlpha += 10;
                        else
                        {
                            MSAAlpha = 255;
                            linkOSB = linkOS;
                        }
                    }
                    else
                    {
                        MSAAlpha = 255;
                        linkOSB = linkOS;
                    }
                }

                if (linkOSB == i)
                {
                    if (MSA_frame < 90)
                    {

                        double t = MSA_frame / (double)90;

                        double startValue = 5.0;
                        double endValue;
                        endValue = windowSize.x - 120 + 245;

                        if (isMSA)
                        {
                            MSA = startValue + (endValue - startValue) * ease::easeIn(t, 3);
                        }
                        else
                        {
                            MSA = startValue + (endValue - startValue) * ease::easeOut(t, 3);
                        }
                        MSA_frame++;
                    }
                    else
                    {
                        if (isMSA) isMSA = false;
                        else isMSA = true;
                        MSA_frame = 0;
                    }

                    int end = 100 - linkX[i] - AddLinkXChange + MSA;
                    int start = end - 250;


                    if (start < 100 - linkX[i] - AddLinkXChange + 5) start = 100 - linkX[i] - AddLinkXChange + 5;

                    if (end > windowSize.x - 20 - linkX[i] - 5) end = windowSize.x - 20 - linkX[i] - 5;
                    if (start > windowSize.x - 20 - linkX[i] - 5) start = windowSize.x - 20 - linkX[i] - 5;

                    setcolor(EGERGBA(155, 255, 200, MSAAlpha));
                    setlinewidth(6);
                    ege_line(start, 40 + CBOS + (i + 1) * 120 + whle / 5 - 6 + 100, end, 40 + CBOS + (i + 1) * 120 + whle / 5 - 6 + 100);
                }
                if (linkMac == i)
                {
                    int end = 100 - linkX[i] - AddLinkXChange + MSA;
                    int start = end - 250;


                    if (start < 100 - linkX[i] - AddLinkXChange + 5) start = 100 - linkX[i] - AddLinkXChange + 5;

                    if (end > windowSize.x - 20 - linkX[i] - 5) end = windowSize.x - 20 - linkX[i] - 5;
                    if (start > windowSize.x - 20 - linkX[i] - 5) start = windowSize.x - 20 - linkX[i] - 5;

                    setcolor(EGERGBA(150, 255, 200, MSAAlpha));
                    setlinewidth(6);
                    ege_line(start, 40 + CBOS + (i + 1) * 120 + whle / 5 - 6 + 100, end, 40 + CBOS + (i + 1) * 120 + whle / 5 - 6 + 100);
                }


                DrawManager::setFont(30);
                if (userset[0] == "0") xyprintf(110 - linkX[i] - AddLinkXChange, 72 + CBOS + (i + 1) * 120 + whle / 5, "%s 的链接", tool::Utf8ToGbk(saveData_link[i]));
                if (userset[0] == "1") xyprintf(110 - linkX[i] - AddLinkXChange, 72 + CBOS + (i + 1) * 120 + whle / 5, "Link of %s", tool::Utf8ToGbk(saveData_link[i]));
                if (userset[0] == "2") xyprintf(110 - linkX[i] - AddLinkXChange, 72 + CBOS + (i + 1) * 120 + whle / 5, "%s のリンク", tool::Utf8ToGbk(saveData_link[i]));
                if (userset[0] == "3")
                {
                    wstring k = tool::string_to_wstring(tool::Utf8ToGbk(saveData_link[i])) + L" 의 링크";
                    xyprintf(110 - linkX[i] - AddLinkXChange, 72 + CBOS + (i + 1) * 120 + whle / 5, k.c_str());
                }
                if (userset[0] == "4") xyprintf(110 - linkX[i] - AddLinkXChange, 72 + CBOS + (i + 1) * 120 + whle / 5, "Lien de %s", tool::Utf8ToGbk(saveData_link[i]));
                if (userset[0] == "5") xyprintf(110 - linkX[i] - AddLinkXChange, 72 + CBOS + (i + 1) * 120 + whle / 5, "Link von %s", tool::Utf8ToGbk(saveData_link[i]));
                if (userset[0] == "6") xyprintf(110 - linkX[i] - AddLinkXChange, 72 + CBOS + (i + 1) * 120 + whle / 5, "Ссылка %s", tool::Utf8ToGbk(saveData_link[i]));
                if (userset[0] == "7") xyprintf(110 - linkX[i] - AddLinkXChange, 72 + CBOS + (i + 1) * 120 + whle / 5, "Enlace de %s", tool::Utf8ToGbk(saveData_link[i]));

                setcolor(EGERGB(155, 155, 155));
                setlinewidth(3);

                line(100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 60, 40 + CBOS + (i + 1) * 120 + whle / 5 + 40, 100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 25, 40 + CBOS + (i + 1) * 120 + whle / 5 + 40);
                line(100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 60, 40 + CBOS + (i + 1) * 120 + whle / 5 + 50, 100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 25, 40 + CBOS + (i + 1) * 120 + whle / 5 + 50);
                line(100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 60, 40 + CBOS + (i + 1) * 120 + whle / 5 + 60, 100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 25, 40 + CBOS + (i + 1) * 120 + whle / 5 + 60);
            }
        }
    }
    if (userBarChoose == 2)
    {
        PIMAGE g = newimage();
        getimage(g, 80, 140 + CBOS, windowSize.x - 80, screen.y / 5 + 40);

        DrawManager::setFillRectColor();
        DrawManager::fillroundrectwithrect(100, 160 + CBOS, windowSize.x - 120, screen.y / 5, screen.x / 100);



        setcolor(EGERGBA(155, 255, 200, MacAddFileRectAlpha));
        setlinestyle(CENTER_LINE, NULL, 3);
        ege_roundrect(100, 160 + CBOS, windowSize.x - 120, screen.y / 5, screen.x / 100);
        setlinestyle(SOLID_LINE, NULL, 1);

        DrawManager::setFont(30 + MacAddFile / 8);

        settextjustify(CENTER_TEXT, CENTER_TEXT);
        wstring fileUploadPrompt[8] = {
            // 中文
            L"单击选择文件或拖拽文件发送",
            // English
            L"Click to select files or drag and drop to send",
            // 日本語
            L"クリックでファイルを選択、またはドラッグ＆ドロップで送信",
            // 한국어
            L"클릭하여 파일 선택 또는 드래그 앤 드롭으로 전송",
            // Français
            L"Cliquez pour sélectionner un fichier ou glissez-déposez pour envoyer",
            // Deutsch
            L"Klicken zur Dateiauswahl oder Drag & Drop zum Senden",
            // Русский
            L"Нажмите для выбора файла или перетащите для отправки",
            // Español
            L"Haz clic para seleccionar archivos o arrastra y suelta para enviar"
        };
        xyprintf(100 + (windowSize.x - 120) / 2, 300 + CBOS, fileUploadPrompt[stoi(userset[0])].c_str());

        DrawManager::setFont(90 + MacAddFile);
		setcolor(EGERGB(155, 155, 155));
        xyprintf(100 + (windowSize.x - 120) / 2, 220 + CBOS, "+");


        if (whle / 5 > 0)
        {
            putimage_alphablend(NULL, g, 80, 140 + CBOS, 0);
        }
        else
        {
            int k = -whle / 5;
            if (k > 255) k = 255;
            putimage_alphablend(NULL, g, 80, 140 + CBOS, k);
        }
        delimage(g);

        settextjustify(LEFT_TEXT, TOP_TEXT);
        wstring deleteText[8] = {
            // 中文
            L"删除",
            // English
            L"Delete",
            // 日本語
            L"削除",
            // 한국어
            L"삭제",
            // Français
            L"Supprimer",
            // Deutsch
            L"Löschen",
            // Русский
            L"Удалить",
            // Español
            L"Eliminar"
        };
        if (choose2Choose == 1)
        {
            for (int i = 0; i < lastUploadMax; i++)
            {
                if (lastUpload[0] == "NULL")
                {
                    DrawManager::setFillRectColor();
                    DrawManager::fillroundrectwithrect(100 + windowSize.x - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);

                    DrawManager::setFont(40);
                    settextjustify(CENTER_TEXT, CENTER_TEXT);

                    wstring recentRecordsPrompt[8] = {
                        // 中文（保留波浪号表达亲切感）
                        L"最近的记录都会在这里显示~  ^_ -",
                        // English
                        L"Recent records will be displayed here~  ^_ -",
                        // 日本語（使用波浪号保持语气）
                        L"最近の記録はここに表示されます～  ^_ -",
                        // 한국어（波浪号表达友好语气）
                        L"최근 기록은 여기에 표시됩니다~  ^_ -",
                        // Français
                        L"Les enregistrements récents s'afficheront ici~  ^_ -",
                        // Deutsch
                        L"Letzte Aufzeichnungen werden hier angezeigt~  ^_ -",
                        // Русский
                        L"Последние записи будут отображаться здесь~  ^_ -",
                        // Español
                        L"Los registros recientes se mostrarán aquí~  ^_ -"
                    };

                    xyprintf(100 + (windowSize.x - 120) / 2 + windowSize.x - choose2ChooseX, 82 + CBOS + (i + 4) * 120 + whle / 5, recentRecordsPrompt[stoi(userset[0])].c_str());
                    settextjustify(LEFT_TEXT, TOP_TEXT);
                    break;
                }
                if (lastUpload[i] == "NULL") break;

                if (i == LKA)
                {
                    if (linkX[i] * 255 / 150 >= 0)
                        setfillcolor(EGERGBA(205, 100, 100, linkX[i] * 255 / 100));
                    else
                        setfillcolor(EGERGBA(205, 100, 100, 0));

                    DrawManager::fillroundrectwithrect(100 - linkX[i] - AddLinkXChange, 40 + CBOS + (i + 4) * 120 + whle / 5, windowSize.x - 120 + linkX[i], 100, userSetCircle);

                    setcolor(EGERGB(255, 255, 255));
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    if (userset[0] != "3")
                    {
                        setfont(25, 0, "Aa吧唧一口小可耐");
                    }
                    else
                    {
                        setfont(25, 0, "Ownglyph Soomini Regular");
                    }
                    xyprintf(100 - AddLinkXChange + windowSize.x - 170, 40 + CBOS + (i + 4) * 120 + whle / 5 + 50, deleteText[stoi(userset[0])].c_str());
                    settextjustify(LEFT_TEXT, TOP_TEXT);
                }

                DrawManager::setFillRectColor();
                DrawManager::fillroundrectwithrect(100 - linkX[i] + windowSize.x - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);

                DrawManager::setFont(30);

                string m = lastUpload[i];
                xyprintf(110 - linkX[i] + windowSize.x - choose2ChooseX, 72 + CBOS + (i + 4) * 120 + whle / 5, m.c_str());

                setcolor(EGERGB(155, 155, 155));
                setlinewidth(3);

                line(100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 60 + windowSize.x - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5 + 40, 100 - linkX[i] + windowSize.x + windowSize.x - choose2ChooseX - 120 - 25, 40 + CBOS + (i + 4) * 120 + whle / 5 + 40);
                line(100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 60 + windowSize.x - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5 + 50, 100 - linkX[i] + windowSize.x + windowSize.x - choose2ChooseX - 120 - 25, 40 + CBOS + (i + 4) * 120 + whle / 5 + 50);
                line(100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 60 + windowSize.x - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5 + 60, 100 - linkX[i] + windowSize.x + windowSize.x - choose2ChooseX - 120 - 25, 40 + CBOS + (i + 4) * 120 + whle / 5 + 60);
            }
        }
        else
        {
            for (int i = 0; i < sendFileList.size(); i++)
            {
                if (i > 10)
                {
                    DrawManager::setFillRectColor();
                    DrawManager::fillroundrectwithrect(100 - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
                    setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

                    DrawManager::setFont(40);

                    settextjustify(CENTER_TEXT, CENTER_TEXT);

                    string tolong = " . . .  + " + to_string(sendFileList.size() - 10);

                    xyprintf(100 + (windowSize.x - 120) / 2 - choose2ChooseX, 82 + CBOS + (i + 4) * 120 + whle / 5, tolong.c_str());
                    settextjustify(LEFT_TEXT, TOP_TEXT);
                    break;
                }
                if (i == LKA)
                {
                    if (i != 0)
                    {
                        if (linkX[i] * 255 / 150 >= 0)
                            setfillcolor(EGERGBA(205, 100, 100, linkX[i] * 255 / 100));
                        else
                            setfillcolor(EGERGBA(205, 100, 100, 0));
                    }
                    else
                    {
                        if (linkX[i] * 255 / 150 >= 0)
                            setfillcolor(EGERGBA(150, 100, 100, linkX[i] * 255 / 100));
                        else
                            setfillcolor(EGERGBA(150, 100, 100, 0));
                    }

                    DrawManager::fillroundrectwithrect(100 - linkX[i] - AddLinkXChange, 40 + CBOS + (i + 4) * 120 + whle / 5, windowSize.x - 120 + linkX[i], 100, userSetCircle);

                    if (i != 0)
                    {
                        setcolor(EGERGB(255, 255, 255));
                    }
                    else
                    {
                        setcolor(EGERGB(150, 150, 150));
                    }
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    if (userset[0] != "3")
                    {
                        setfont(25, 0, "Aa吧唧一口小可耐");
                    }
                    else
                    {
                        setfont(25, 0, "Ownglyph Soomini Regular");
                    }
                    xyprintf(100 - AddLinkXChange + windowSize.x - 170, 40 + CBOS + (i + 4) * 120 + whle / 5 + 50, deleteText[stoi(userset[0])].c_str());
                    settextjustify(LEFT_TEXT, TOP_TEXT);
                }

                DrawManager::setFillRectColor();
                DrawManager::fillroundrectwithrect(100 - linkX[i] - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
                DrawManager::setFont(30);

                string transferRecordsTitle[8][2] = {
                    // 中文
                    {"等待传输 : ", "正在传输 : "},

                    // 英文
                    {"Waiting to transfer : ", "Transferring : "},

                    // 日语
                    {"転送待ち : ", "転送中 : "},

                    // 韩语
                    {"전송 대기 : ", "전송 중 : "},

                    // 法语
                    {"En attente de transfert : ", "Transfert en cours : "},

                    // 德语
                    {"Warten auf Übertragung : ", "Wird übertragen : "},

                    // 俄语
                    {"Ожидание передачи : ", "Идет передача : "},

                    // 西班牙语
                    {"Esperando transferencia : ", "Transfiriendo : "}
                };
                string m;
                if (i == 0)
                {
                    m = transferRecordsTitle[stoi(userset[0])][1] + sendFileList[i].displayPath;
                }
                else
                {
                    m = transferRecordsTitle[stoi(userset[0])][0] + sendFileList[i].displayPath;
                }
                xyprintf(110 - linkX[i] - choose2ChooseX, 72 + CBOS + (i + 4) * 120 + whle / 5, m.c_str());

                setcolor(EGERGB(155, 155, 155));
                setlinewidth(3);

                line(100 - linkX[i] + windowSize.x - 120 - 60 - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5 + 40, 100 - linkX[i] + windowSize.x - choose2ChooseX - 120 - 25, 40 + CBOS + (i + 4) * 120 + whle / 5 + 40);
                line(100 - linkX[i] + windowSize.x - 120 - 60 - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5 + 50, 100 - linkX[i] + windowSize.x - choose2ChooseX - 120 - 25, 40 + CBOS + (i + 4) * 120 + whle / 5 + 50);
                line(100 - linkX[i] + windowSize.x - 120 - 60 - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5 + 60, 100 - linkX[i] + windowSize.x - choose2ChooseX - 120 - 25, 40 + CBOS + (i + 4) * 120 + whle / 5 + 60);

                if (i == 0)
                {
                    if (loadASlink == true)
                    {
                        if (loadAlink < 270)
                        {
                            loadAlink += 2;
                        }
                        else
                        {
                            loadAlink = 270;
                            loadASlink = false;
                        }
                    }
                    else
                    {
                        if (loadAlink > 10)
                        {
                            loadAlink -= 2;
                        }
                        else
                        {
                            loadAlink = 10;
                            loadASlink = true;
                        }
                    }

                    roundAlink -= 10;

                    if (roundAlink < -720) roundAlink = 0;



                    setlinecap(LINECAP_ROUND, LINECAP_ROUND);
                    setlinewidth(8);
                    int w = windowSize.x; int h = windowSize.y;

                    setcolor(EGERGB(155, 255, 200));
                    arc(windowSize.x - 150 - linkX[i] - choose2ChooseX, 40 + CBOS + (i + 4) * 120 + whle / 5 + 50, roundAlink, roundAlink + loadAlink, 20);

                    if (MSAAlpha < 255)
                    {
                        if (MSAAlpha + 5 < 255)MSAAlpha += 5;
                        else MSAAlpha = 255;
                    }
                    else
                    {
                        MSAAlpha = 255;
                    }
                    if (MSA_frame < 90)
                    {

                        double t = MSA_frame / (double)90;

                        double startValue = 5.0;
                        double endValue;
                        endValue = windowSize.x - 120 + 245;

                        if (isMSA)
                        {
                            MSA = startValue + (endValue - startValue) * ease::easeIn(t, 3);
                        }
                        else
                        {
                            MSA = startValue + (endValue - startValue) * ease::easeOut(t, 3);
                        }
                        MSA_frame++;
                    }
                    else
                    {
                        if (isMSA) isMSA = false;
                        else isMSA = true;
                        MSA_frame = 0;
                    }

                    int end = 100 - linkX[i] - AddLinkXChange + MSA;
                    int start = end - 250;


                    if (start < 100 - linkX[i] - AddLinkXChange + 5) start = 100 - linkX[i] - AddLinkXChange + 5;

                    if (end > windowSize.x - 20 - linkX[i] - 5) end = windowSize.x - 20 - linkX[i] - 5;
                    if (start > windowSize.x - 20 - linkX[i] - 5) start = windowSize.x - 20 - linkX[i] - 5;

                    setcolor(EGERGBA(150, 255, 200, MSAAlpha));
                    setlinewidth(4);
                    ege_line(start, 40 + CBOS + (i + 4) * 120 + whle / 5 - 2 + 100, end, 40 + CBOS + (i + 4) * 120 + whle / 5 - 2 + 100);




                    double process3 = 0;
                    if (process_frame < 50)
                    {
                        double startValue = start_P;
                        double endValue = end_P;
                        double t = process_frame / (double)50;

                        process2 = process3 = startValue + (endValue - startValue) * ease::easeOut(t, 4);

                        process_frame++;
                    }
                    else
                    {
                        process3 = end_P;
                    }

                    setlinewidth(3);
                    setcolor(EGERGB(155, 155, 155));
                    ege_line(100 - linkX[i] - choose2ChooseX + 20, 40 + CBOS + (i + 4) * 120 + whle / 5 + 80, 100 - linkX[i] - choose2ChooseX + 20 + windowSize.x - 450, 40 + CBOS + (i + 4) * 120 + whle / 5 + 80);

                    setlinewidth(6);
                    setcolor(EGERGB(155, 255, 200));
                    ege_line(100 - linkX[i] - choose2ChooseX + 20, 40 + CBOS + (i + 4) * 120 + whle / 5 + 80, 100 - linkX[i] - choose2ChooseX + 20 + (windowSize.x - 450) * process3 / 100, 40 + CBOS + (i + 4) * 120 + whle / 5 + 80);

                    if (DarkMode)
                    {
                        setcolor(EGERGB(255, 255, 255));
                    }
                    else
                    {
                        setcolor(EGERGB(20, 20, 20));
                    }
                    setfont(25, 0, "Aa吧唧一口小可耐");
                    xyprintf(100 - linkX[i] - choose2ChooseX + 20 + windowSize.x - 440, 40 + CBOS + (i + 4) * 120 + whle / 5 + 70, "%d %%  %s/s %s", process,Speed,timeNeed);
                }
                else
                {
                    static int MSB = 0;
                    if (i == 1) MSB += 15;

                    if (MSB > windowSize.x - 20 - linkX[i] - 5 + 250) MSB = 0;

                    int end = 100 - linkX[i] - AddLinkXChange + MSB;
                    int start = end - 250;

                    if (start < 100 - linkX[i] - AddLinkXChange + 5) start = 100 - linkX[i] - AddLinkXChange + 5;

                    if (end > windowSize.x - 20 - linkX[i] - 5) end = windowSize.x - 20 - linkX[i] - 5;
                    if (start > windowSize.x - 20 - linkX[i] - 5) start = windowSize.x - 20 - linkX[i] - 5;

                    setcolor(EGERGBA(150, 255, 200, MSAAlpha));
                    setlinewidth(6);
                    ege_line(start, 40 + CBOS + (i + 4) * 120 + whle / 5 - 6 + 100, end, 40 + CBOS + (i + 4) * 120 + whle / 5 - 6 + 100);
                }

            }
        }

        int process5 = Net::GetTransferProgress();
        if (process5 == 100)
        {
            if (kgms)
            {
                // 6. 进度及下标安全处理
                process_frame = 60;
                start_P = end_P = 0;
                process = 0;
                if (LKA > 0) LKA -= 1;
                else LKA = 0;

                kgms = false;
            }
        }
        else
        {
            kgms = true;
        }

        settextjustify(CENTER_TEXT, CENTER_TEXT);
        DrawManager::setFillRectColor();
        if (140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 > 110)
        {
            DrawManager::fillroundrectwithrect(100, 140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5, windowSize.x - 120, 80, screen.x / 100);

            DrawManager::setFont(30);

            wstring transferStatus[8][2] = {
                // 中文（简体）          [0]正在传输       [1]完成传输
                {L"正在传输",            L"传输完成"},

                // 英文
                {L"Transferring",       L"Transfer complete"},

                // 日语
                {L"転送中",             L"転送完了"},

                // 韩语
                {L"전송 중",            L"전송 완료"},

                // 法语
                {L"Transfert en cours", L"Transfert terminé"},

                // 德语
                {L"Übertragung läuft",  L"Übertragung abgeschlossen"},

                // 俄语
                {L"Передача данных",    L"Передача завершена"},

                // 西班牙语
                {L"Transfiriendo",      L"Transferencia completada"}
            };
            xyprintf(250, 140 + CBOS + screen.y / 5 + 40 + 50 + whle / 5, transferStatus[stoi(userset[0])][0].c_str());
            xyprintf(250 + windowSize.x / 6, 140 + CBOS + screen.y / 5 + 40 + 50 + whle / 5, transferStatus[stoi(userset[0])][1].c_str());

            setcolor(mainColor);
            setlinewidth(5);
            line(choose2ChooseX / 6 + 250 - choose2Line, 140 + CBOS + screen.y / 5 + 40 + 70 + whle / 5, choose2ChooseX / 6 + 250 + choose2Line, 140 + CBOS + screen.y / 5 + 40 + 70 + whle / 5);

            if (choose2ChooseX > 0)
            {
                setcolor(EGERGB(255, 150, 150));
                if (choose2ChooseX < 1000)
                {
                    line(
                        windowSize.x - 70 - choose2ChooseX / 50,
                        140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 - choose2ChooseX / 50,
                        windowSize.x - 70 + choose2ChooseX / 50,
                        140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 + choose2ChooseX / 50);
                    line(
                        windowSize.x - 70 - choose2ChooseX / 50,
                        140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 + choose2ChooseX / 50,
                        windowSize.x - 70 + choose2ChooseX / 50,
                        140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 - choose2ChooseX / 50);
                }
                else
                {
                    line(
                        windowSize.x - 70 - 20,
                        140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 - 20,
                        windowSize.x - 70 + 20,
                        140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 + 20);

                    line(
                        windowSize.x - 70 - 20,
                        140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 + 20,
                        windowSize.x - 70 + 20,
                        140 + CBOS + screen.y / 5 + 40 + 10 + whle / 5 + 40 - 20);
                }
            }
        }
        else
        {
            DrawManager::fillroundrectwithrect(100, 110, windowSize.x - 120, 80, screen.x / 100);

            DrawManager::setFont(30);

            wstring transferStatus[8][2] = {
                // 中文（简体）          [0]正在传输       [1]完成传输
                {L"正在传输",            L"传输完成"},

                // 英文
                {L"Transferring",       L"Transfer complete"},

                // 日语
                {L"転送中",             L"転送完了"},

                // 韩语
                {L"전송 중",            L"전송 완료"},

                // 法语
                {L"Transfert en cours", L"Transfert terminé"},

                // 德语
                {L"Übertragung läuft",  L"Übertragung abgeschlossen"},

                // 俄语
                {L"Передача данных",    L"Передача завершена"},

                // 西班牙语
                {L"Transfiriendo",      L"Transferencia completada"}
            };
            xyprintf(250, 150, transferStatus[stoi(userset[0])][0].c_str());
            xyprintf(250 + windowSize.x / 6, 150, transferStatus[stoi(userset[0])][1].c_str());

            setcolor(EGERGB(155, 255, 200));
            setlinewidth(5);
            line(choose2ChooseX / 6 + 250 - choose2Line, 170, choose2ChooseX / 6 + 250 + choose2Line, 170);

            if (choose2ChooseX > 0)
            {
                setcolor(EGERGB(255, 150, 150));
                if (choose2ChooseX < 1000)
                {
                    line(
                        windowSize.x - 70 - choose2ChooseX / 50,
                        110 + 40 - choose2ChooseX / 50,
                        windowSize.x - 70 + choose2ChooseX / 50,
                        110 + 40 + choose2ChooseX / 50);
                    line(
                        windowSize.x - 70 - choose2ChooseX / 50,
                        110 + 40 + choose2ChooseX / 50,
                        windowSize.x - 70 + choose2ChooseX / 50,
                        110 + 40 - choose2ChooseX / 50);
                }
                else
                {
                    line(
                        windowSize.x - 70 - 20,
                        110 + 40 - 20,
                        windowSize.x - 70 + 20,
                        110 + 40 + 20);

                    line(
                        windowSize.x - 70 - 20,
                        110 + 40 + 20,
                        windowSize.x - 70 + 20,
                        110 + 40 - 20);
                }
            }
        }



        settextjustify(LEFT_TEXT, TOP_TEXT);
    }
    if (userBarChoose == 3)
    {
        for (int i = 0; i < lastDownloadMax; i++)
        {

            if (lastDownload[0] == "NULL")
            {
                DrawManager::setFillRectColor();
                DrawManager::fillroundrectwithrect(100, 40 + CBOS + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
                DrawManager::setFont(40);
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                wstring noTransferRecords[8] = {
                    // 中文
                    L"还没有人给您传过文件呢~ >_<",
                    // English
                    L"No one has sent you any files yet ✨",
                    // 日本語
                    L"まだ誰もファイルを送っていませんよ～(´ω '`)",
                    // 한국어
                    L"아직 아무도 파일을 안 보냈어요~",
                    // Français
                    L"Personne ne vous a encore envoyé de fichiers ❤",
                    // Deutsch
                    L"noch hat dir niemand dateien geschickt 🌸",
                    // Русский
                    L"Вам пока никто не отправлял файлов 🌼",
                    // Español
                    L"Nadie te ha enviado archivos todavía  (。﹏。)"
                };
                xyprintf(100 + (windowSize.x - 120) / 2, 85 + CBOS + (i + 1) * 120 + whle / 5, noTransferRecords[stoi(userset[0])].c_str());
                settextjustify(LEFT_TEXT, TOP_TEXT);
                break;
            }
            else
            {
                if (lastDownload[i] == "NULL") break;
            }

            if (i == LKA)
            {
                wstring deleteText[8] = {
                    // 中文
                    L"删除",
                    // English
                    L"Delete",
                    // 日本語
                    L"削除",
                    // 한국어
                    L"삭제",
                    // Français
                    L"Supprimer",
                    // Deutsch
                    L"Löschen",
                    // Русский
                    L"Удалить",
                    // Español
                    L"Eliminar"
                };
                if (linkX[i] * 255 / 150 >= 0) setfillcolor(EGERGBA(205, 100, 100, linkX[i] * 255 / 100));
                else setfillcolor(EGERGBA(205, 100, 100, 0));
                ege_fillroundrect(100 - linkX[i] - AddLinkXChange, 40 + CBOS + (i + 1) * 120 + whle / 5 + 70, windowSize.x - 120 + linkX[i], 100, userSetCircle);
                setcolor(EGERGB(255, 255, 255));
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                if (userset[0] != "3")
                {
                    setfont(25, 0, "Aa吧唧一口小可耐");
                }
                else
                {
                    setfont(25, 0, "Ownglyph Soomini Regular");
                }
                xyprintf(100 - AddLinkXChange + windowSize.x - 170, 40 + CBOS + (i + 1) * 120 + whle / 5 + 50 + 70, deleteText[stoi(userset[0])].c_str());
                settextjustify(LEFT_TEXT, TOP_TEXT);
            }

            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(100 - linkX[i], 40 + CBOS + (i + 1) * 120 + whle / 5 + 70, windowSize.x - 120, 100, userSetCircle);
            DrawManager::setFont(30);

            string m = lastUpload[i];
            xyprintf(110 - linkX[i], 142 + CBOS + (i + 1) * 120 + whle / 5, m.c_str());

            setcolor(EGERGB(155, 155, 155));
            setlinewidth(3);
            line(100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 60, 40 + CBOS + (i + 1) * 120 + whle / 5 + 40 + 70, 100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 25, 40 + CBOS + (i + 1) * 120 + whle / 5 + 40 + 70);
            line(100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 60, 40 + CBOS + (i + 1) * 120 + whle / 5 + 50 + 70, 100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 25, 40 + CBOS + (i + 1) * 120 + whle / 5 + 50 + 70);
            line(100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 60, 40 + CBOS + (i + 1) * 120 + whle / 5 + 60 + 70, 100 - linkX[i] - AddLinkXChange + windowSize.x - 120 - 25, 40 + CBOS + (i + 1) * 120 + whle / 5 + 60 + 70);
        }

        if(lastDownload[0] != "NULL")
        {
            string translations[8] = {
           "接收文件历史记录:",                     // 中文 (Chinese)
           "Received file history:",               // 英文 (English)
           "受信ファイルの履歴:",                 // 日语 (Japanese)
           "수신 파일 기록:",                     // 韩文 (Korean)
           "Historique des fichiers reçus:",      // 法语 (French)
           "Empfangene Dateihistorie:",           // 德语 (German)
           "История полученных файлов:",          // 俄语 (Russian)
           "Historial de archivos recibidos:"     // 西班牙语 (Spanish)
            };
            int i = 0;
            DrawManager::setFillRectColor();
            if (40 + CBOS + (i + 1) * 120 + whle / 5 > 110)
            {
                DrawManager::fillroundrectwithrect(100, 40 + CBOS + (i + 1) * 120 + whle / 5, windowSize.x - 120, 60, userSetCircle);

                setlinewidth(5);
                setcolor(EGERGBA(255, 150, 150, 255 - CBOS * 255 / 155));
                ege_line(windowSize.x - 65 - 15, 70 + CBOS + (i + 1) * 120 + whle / 5 - 15, windowSize.x - 35 - 15, 70 + CBOS + (i + 1) * 120 + whle / 5 + 15);
                ege_line(windowSize.x - 35 - 15, 70 + CBOS + (i + 1) * 120 + whle / 5 - 15, windowSize.x - 65 - 15, 70 + CBOS + (i + 1) * 120 + whle / 5 + 15);
            }
            else
            {
                DrawManager::fillroundrectwithrect(100, 110, windowSize.x - 120, 60, userSetCircle);

                setlinewidth(5);
                setcolor(EGERGBA(255, 150, 150, 255 - CBOS * 255 / 155));
                ege_line (  windowSize.x - 70 - 15 + 5, 90 + 30 + 5, windowSize.x - 30 - 15 - 5,130 + 30 - 5);
                ege_line(windowSize.x - 30 - 15 - 5, 90 + 30 + 5, windowSize.x - 70 - 15 + 5, 130 + 30 - 5);
            }
            DrawManager::setFont(30);
            if (40 + CBOS + (i + 1) * 120 + whle / 5 > 110) xyprintf(110, 55 + CBOS + (i + 1) * 120 + whle / 5, translations[stoi(userset[0])].c_str());
            else xyprintf(110, 125, translations[stoi(userset[0])].c_str());

            
        }

    }
    if (userBarChoose == 4)
    {
        wstring setLg[8][16] = {
            // 中文
            {
                L"设备属性", L"颜色", L"NetTrack的背景", L"默认下载位置", L"语言", L"名单设置",
                L"设备连接的方式", L"本机密码重新设置", L"传输速度限制", L"传输加密选项", L"随机端口更换",
                L"", L"", L"", L"", L""
            },
            // English
            {
                L"Device Properties", L"Color", L"NetTrack Background", L"Default Download Location", L"Language", L"List Settings",
                L"Device Connection Method", L"Local Password Reset", L"Transfer Speed Limit", L"Transfer Encryption Option", L"Random Port Switching",
                L"", L"", L"", L"", L""
            },
            // 日本語
            {
                L"デバイスプロパティ", L"色", L"NetTrack背景", L"デフォルトのダウンロード場所", L"言語", L"リスト設定",
                L"デバイス接続方法", L"ローカルパスワード再設定", L"転送速度制限", L"転送暗号化オプション", L"ランダムポート切替",
                L"", L"", L"", L"", L""
            },
            // 한국어
            {
                L"장치 속성", L"색상", L"NetTrack 배경", L"기본 다운로드 위치", L"언어", L"목록 설정",
                L"장치 연결 방식", L"로컬 암호 재설정", L"전송 속도 제한", L"전송 암호화 옵션", L"랜덤 포트 변경",
                L"", L"", L"", L"", L""
            },
            // Français
            {
                L"Propriétés de l'appareil", L"Couleur", L"Fond NetTrack", L"Emplacement de téléchargement par défaut", L"Langue", L"Paramètres de liste",
                L"Méthode de connexion de l'appareil", L"Réinitialisation du mot de passe local", L"Limite de vitesse de transfert", L"Option de chiffrement de transfert", L"Changement aléatoire de port",
                L"", L"", L"", L"", L""
            },
            // Deutsch
            {
                L"Geräteeigenschaften", L"Farbe", L"NetTrack-Hintergrund", L"Standard-Downloadort", L"Sprache", L"Listeneinstellungen",
                L"Geräteverbindungsmethode", L"Lokale Passwortzurücksetzung", L"Übertragungsgeschwindigkeitsbegrenzung", L"Übertragungsverschlüsselungsoption", L"Zufälliger Portwechsel",
                L"", L"", L"", L"", L""
            },
            // Русский
            {
                L"Свойства устройства", L"Цвет", L"Фон NetTrack", L"Место загрузки по умолчанию", L"Язык", L"Настройки списка",
                L"Способ подключения устройства", L"Сброс локального пароля", L"Ограничение скорости передачи", L"Опция шифрования передачи", L"Случайная смена порта",
                L"", L"", L"", L"", L""
            },
            // Español
            {
                L"Propiedades del dispositivo", L"Color", L"Fondo de NetTrack", L"Ubicación de descarga predeterminada", L"Idioma", L"Configuración de lista",
                L"Método de conexión del dispositivo", L"Restablecimiento de contraseña local", L"Límite de velocidad de transferencia", L"Opción de cifrado de transferencia", L"Cambio aleatorio de puerto",
                L"", L"", L"", L"", L""
            }
        };

        if(!isGotoChoose4)
        {
            if (isGotoChoose4_frame < 60)
            {
                double start = 300;
                double end = 0;

                double t = isGotoChoose4_frame / (double)60;
                isGotoChoose4XChange = start + (end - start) * ease::easeOut(t, 5);

                isGotoChoose4_frame++;

                int mh = 300 - isGotoChoose4XChange;

                for (int i = 0; i < 11; i++)
                {
                    DrawManager::setFillRectColor();
                    DrawManager::fillroundrectwithrect(-200 + mh, 40 + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
                    setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

                    DrawManager::setFont(40);
                    xyprintf(-190 + mh, 65 + (i + 1) * 120 + whle / 5, setLg[stoi(userset[0])][i].c_str());

                    setlinewidth(9);
					setcolor(EGERGB(155,155,155));
                    line(-200 + mh + windowSize.x - 120 - 60, 40 + (i + 1) * 120 + whle / 5 + 35, -200 + mh + windowSize.x - 120 - 40, 40 + (i + 1) * 120 + whle / 5 + 50);
                    line(-200 + mh + windowSize.x - 120 - 60, 40 + (i + 1) * 120 + whle / 5 + 65, -200 + mh + windowSize.x - 120 - 40, 40 + (i + 1) * 120 + whle / 5 + 50);
                }

                putimage_alphablend(NULL, isGotoChoose4Image, mh, 0, 255 - mh * 255 / 300);
            }
            else
            {
                for (int i = 0; i < 11; i++)
                {
                    DrawManager::setFillRectColor();
                    DrawManager::fillroundrectwithrect(100, 40 + CBOS + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
                    setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

                    DrawManager::setFont(40);
                    xyprintf(110, 65 + CBOS + (i + 1) * 120 + whle / 5, setLg[stoi(userset[0])][i].c_str());


                    setlinewidth(9);
                    setcolor(EGERGB(155,155,155));
                    line(100 + windowSize.x - 120 - 60, 40 + (i + 1) * 120 + whle / 5 + 35 + CBOS, 100 + windowSize.x - 120 - 40, 40 + (i + 1) * 120 + whle / 5 + 50 + CBOS);
                    line(100 + windowSize.x - 120 - 60, 40 + (i + 1) * 120 + whle / 5 + 65 + CBOS,100 + windowSize.x - 120 - 40, 40 + (i + 1) * 120 + whle / 5 + 50 + CBOS);
                }
            }
        }
        else
        {
            DrawisGotoChoose4();
        }
    }
    if (userBarChoose == 5)
    {
        wstring netTrackActions[8][6] = {
            // 中文
            {
                L"重新启动NetTrack",
                L"立即检查连接状态",
                L"初始化并还原配置",
                L"清除所有信任列表",  // 修改项
                L"立即退出NetTrack并终止后台任务",
                L"立即退出并卸载NetTrack"
            },
            // English
            {
                L"Restart NetTrack",
                L"Check Connection Status Now",
                L"Initialize and Restore Configuration",
                L"Clear All Trusted Lists",  // Modified
                L"Exit NetTrack and Terminate Background Tasks",
                L"Exit and Uninstall NetTrack"
            },
            // 日本語
            {
                L"NetTrackを再起動",
                L"接続状態を即時チェック",
                L"初期化して設定を復元",
                L"すべての信頼リストを消去",  // 修正
                L"NetTrackを終了しバックグラウンドタスクを停止",
                L"終了してNetTrackをアンインストール"
            },
            // 한국어
            {
                L"NetTrack 재시작",
                L"연결 상태 즉시 확인",
                L"초기화 및 설정 복원",
                L"모든 신뢰 목록 지우기",  // 수정
                L"NetTrack 종료 및 백그라운드 작업 중지",
                L"종료 후 NetTrack 제거"
            },
            // Français
            {
                L"Redémarrer NetTrack",
                L"Vérifier immédiatement l'état de la connexion",
                L"Initialiser et restaurer la configuration",
                L"Effacer toutes les listes de confiance",  // Modifié
                L"Quitter NetTrack et terminer les tâches en arrière-plan",
                L"Quitter et désinstaller NetTrack"
            },
            // Deutsch
            {
                L"NetTrack neu starten",
                L"Verbindungsstatus sofort prüfen",
                L"Initialisieren und Konfiguration wiederherstellen",
                L"Alle Vertrauenslisten löschen",  // Geändert
                L"NetTrack beenden und Hintergrundaufgaben beenden",
                L"Beenden und NetTrack deinstallieren"
            },
            // Русский
            {
                L"Перезапустить NetTrack",
                L"Немедленно проверить состояние подключения",
                L"Инициализировать и восстановить конфигурацию",
                L"Очистить все доверенные списки",  // Изменено
                L"Выйти из NetTrack и завершить фоновые задачи",
                L"Выйти и удалить NetTrack"
            },
            // Español
            {
                L"Reiniciar NetTrack",
                L"Verificar estado de conexión inmediatamente",
                L"Inicializar y restaurar configuración",
                L"Borrar todas las listas de confianza",  // Modificado
                L"Salir de NetTrack y terminar tareas en segundo plano",
                L"Salir y desinstalar NetTrack"
            }
        };
        wstring executeNow[8] = {
            // 中文（简体）
            L"立即执行",

            // 英文
           L"Execute Now",  // 或 "Run Immediately"

           // 日语
           L"すぐに実行",   // 或 "即時実行"

           // 韩语
           L"즉시 실행",

           // 法语
           L"Exécuter maintenant",

           // 德语
           L"Sofort ausführen",

           // 俄语
           L"Выполнить сейчас",

           // 西班牙语
           L"Ejecutar ahora"
        };
        for (int i = 0; i < 6; i++)
        {
            if (osm[2] == 0)
            {
                string confirmMsg10s[8] = {
                    // 中文（简体）
                    "请确认，10秒后自动取消",

                    // 英文
                    "Confirm, auto-cancel in 10s",

                    // 日语
                    "確認してください（10秒で自動取消）",

                    // 韩语
                    "확인하세요 (10초 후 자동취소)",

                    // 法语
                    "Confirmer (annul. auto. 10s)",

                    // 德语
                    "Bestätigen (in 10s automat. Abbruch)",

                    // 俄语
                    "Подтвердите (отмена через 10 сек)",

                    // 西班牙语
                    "Confirme (se cancela en 10s)"
                };
                msg::KillBox(confirmMsg10s[stoi(userset[0])]);
                osm[2] = -1;
                return;
            }

            DrawManager::setFillRectColor();

            DrawManager::fillroundrectwithrect(100, 40 + CBOS + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);

            if(DarkMode) setfillcolor(EGERGB(BKcolor + 50, BKcolor + 50, BKcolor + 50));
            else setfillcolor(EGERGB(BKcolor - 50, BKcolor - 50, BKcolor - 50));
            DrawManager::fillroundrectwithrect(windowSize.x - 220, 65 + CBOS + (i + 1) * 120 + whle / 5, 180, 50, userSetCircle);
            settextjustify(CENTER_TEXT, CENTER_TEXT);

            DrawManager::setFont(28);

            xyprintf(windowSize.x - 220 + 90, 65 + CBOS + (i + 1) * 120 + whle / 5 + 25, executeNow[stoi(userset[0])].c_str());
            settextjustify(LEFT_TEXT, TOP_TEXT);
            setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

            DrawManager::setFont(40);
            xyprintf(110, 65 + CBOS + (i + 1) * 120 + whle / 5, netTrackActions[stoi(userset[0])][i].c_str());
        }
    }
    if (userBarChoose == 6)
    {
        wstring tr[8][5] = {
            // 中文
            { L"版本信息：您当前是个人版，免费使用 v0.1",
              L"更新日志：看看新版本都更新了什么",
              L"使用准则：查看NetTrack的使用规范与免责声明",
              L"报告BUG与支持",
              L"检查并获取最新的更新" },
              // 英文
              { L"Version Info: You are currently using Personal Edition (Free) v0.1",
                L"Changelog: See what's new in this version",
                L"Usage Guidelines: View NetTrack usage policies and disclaimer",
                L"Report BUGs & Support",
                L"Check for updates" },
                // 日文
                { L"バージョン情報：現在個人版（無料）v0.15をご利用中です",
                  L"更新履歴：新バージョンの変更点を確認",
                  L"利用規約：NetTrackの使用ポリシーと免責事項",
                  L"バグ報告とサポート",
                  L"更新を確認" },
                  // 韩文
                  { L"버전 정보: 현재 개인용 무료 버전 v0.1 사용 중",
                    L"변경 로그: 새 버전의 업데이트 내용 확인",
                    L"사용 지침: NetTrack 사용 규정 및 면책 조항",
                    L"버그 보고 및 지원",
                    L"업데이트 확인" },
                    // 法文
                    { L"Version : Vous utilisez actuellement l'édition personnelle gratuite v0.1",
                      L"Journal des modifications : Voir les nouveautés",
                      L"Règles d'utilisation : Politiques d'utilisation et avertissement de NetTrack",
                      L"Signaler des bugs & Support",
                      L"Vérifier les mises à jour" },
                      // 德文
                      { L"Version: Sie nutzen die kostenlose Personal Edition v0.1",
                        L"Änderungsprotokoll: Neuerungen anzeigen",
                        L"Nutzungsrichtlinien: NetTrack-Bestimmungen und Haftungsausschluss",
                        L"Fehler melden & Support",
                        L"Nach Updates suchen" },
                        // 俄文
                        { L"Версия: Вы используете бесплатную персональную версию v0.1",
                          L"История изменений: что нового в этой версии",
                          L"Правила использования: политика и отказ от ответственности NetTrack",
                          L"Сообщить об ошибке и поддержка",
                          L"Проверить обновления" },
                          // 西班牙语
                          { L"Versión: Actualmente usa la edición personal gratuita v0.1",
                            L"Registro de cambios: Novedades de esta versión",
                            L"Normas de uso: Políticas y descargo de responsabilidad de NetTrack",
                            L"Reportar errores y soporte",
                            L"Buscar actualizaciones" }
        };
        for (int i = 0; i < 5; i++)
        {
            DrawManager::setFillRectColor();
            DrawManager::fillroundrectwithrect(100, 40 + CBOS + (i + 1) * 120 + whle / 5, windowSize.x - 120, 100, userSetCircle);
            DrawManager::setFont(40);

            xyprintf(110, 65 + CBOS + (i + 1) * 120 + whle / 5, tr[stoi(userset[0])][i].c_str());

            setlinewidth(9);
            setcolor(EGERGB(155,155,155));
            line(100 + windowSize.x - 120 - 60, 40 + (i + 1) * 120 + whle / 5 + 35 + CBOS, 100 + windowSize.x - 120 - 40, 40 + (i + 1) * 120 + whle / 5 + 50 + CBOS);
            line(100 + windowSize.x - 120 - 60, 40 + (i + 1) * 120 + whle / 5 + 65 + CBOS, 100 + windowSize.x - 120 - 40, 40 + (i + 1) * 120 + whle / 5 + 50 + CBOS);
        }
    }

    if ((linkJD != "N" && userBarChoose == 1 && AddLinkXChange < windowSize.x / 4 * 3) || (mousePos.y > windowSize.y - 60 && userBarChoose != 6))
    {
        if (!upLink)
        {
            upLinkFrame = 0;
            upLink = true;
        }

        if (linkJD == "N")
        {
            if (upLinkFrame < 30)
            {
                double t = upLinkFrame / (double)30;
                int start = windowSize.y + 100;
                int end = windowSize.y - 116;
                upLinkY = start + (end - start) * ease::easeOut(t, 3);
                upLinkFrame++;
            }
        }
        else
        {
            if (upLinkFrame < 30)
            {
                double t = upLinkFrame / (double)30;
                int start = windowSize.y - 23;
                int end = windowSize.y - 116;
                upLinkY = start + (end - start) * ease::easeOut(t, 3);
                upLinkFrame++;
            }
        }

    }
    else
    {
        if (upLink)
        {
            upLinkFrame = 0;
            upLink = false;
        }
        if (linkJD != "N")
        {
            if (upLinkFrame < 30)
            {
                double t = upLinkFrame / (double)30;
                int start = windowSize.y - 116;
                int end = windowSize.y - 23;
                upLinkY = start + (end - start) * ease::easeOut(t, 3);
                upLinkFrame++;
            }
        }
        else
        {
            if (upLinkFrame < 30)
            {
                double t = upLinkFrame / (double)30;
                int start = windowSize.y - 116;
                int end = windowSize.y + 100;
                upLinkY = start + (end - start) * ease::easeInBack(t, 2);
                upLinkFrame++;
            }
        }
    }

    if (fuck_AHX * 255 / 150 >= 0)
        setfillcolor(EGERGBA(205, 100, 100, fuck_AHX * 255 / 100));
    else
        setfillcolor(EGERGBA(205, 100, 100, 0));

    ege_fillroundrect(100 - fuck_AHX, upLinkY, windowSize.x - 120 + fuck_AHX, 100, userSetCircle);

    if (fuck_AH)
    {
        std::wstring disconnectMessages[8] = {
    L"断开连接",               // Chinese (Simplified)
    L"Disconnect",            // English
    L"接続を切断",            // Japanese
    L"연결 해제",             // Korean
    L"Déconnecter",           // French
    L"Trennen",               // German
    L"Отключить",             // Russian
    L"Desconectar"            // Spanish
        };
        setcolor(EGERGB(255, 255, 255));
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        if (userset[0] != "3")
        {
            setfont(25, 0, "Aa吧唧一口小可耐");
        }
        else
        {
            setfont(25, 0, "Ownglyph Soomini Regular");
        }
        xyprintf(100 + windowSize.x - 170, upLinkY + 50, disconnectMessages[stoi(userset[0])].c_str());
        settextjustify(LEFT_TEXT, TOP_TEXT);

    }

    DrawManager::setFillRectColor();
    DrawManager::fillroundrectwithrect(100 - fuck_AHX, upLinkY, windowSize.x - 120, 100, userSetCircle);
    setfont(30, 0, "Aa吧唧一口小可耐");
    setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));
    string keepConnectedMessage[8] = {
    " 保持与NetTrack连接",      // Chinese (Simplified)
    " keep connected to NetTrack",  // English
    " はNetTrackとの接続を維持します",  // Japanese
    ", NetTrack 연결 유지",     // Korean
    " rester connecté à NetTrack",  // French
    " mit NetTrack verbunden bleiben",  // German
    " поддерживать соединение с NetTrack",  // Russian
    " manténgase conectado a NetTrack"  // Spanish
    };
    string msg;
    if(linkJD != "N")  msg = tool::Utf8ToGbk(linkJD) + keepConnectedMessage[stoi(userset[0])];
    else
    {
        string noConnectionMessages[8] = {
    "当前无连接",                     // Chinese (Simplified)
    "No active connection",           // English
    "接続がありません",               // Japanese
    "현재 연결 없음",                  // Korean
    "Aucune connexion active",        // French
    "Keine aktive Verbindung",        // German
    "Нет активного подключения",      // Russian
    "Sin conexión activa"             // Spanish
        };
		msg = noConnectionMessages[stoi(userset[0])];
    }
    xyprintf(110 - fuck_AHX, upLinkY + 32,msg.c_str());

    

    setcolor(EGERGB(155, 155, 155));
    setlinewidth(3);
    line(100 + windowSize.x - 120 - 60 - fuck_AHX, upLinkY + 40, 100  + windowSize.x - 120 - 25 - fuck_AHX, upLinkY + 40);
    line(100 + windowSize.x - 120 - 60 - fuck_AHX, upLinkY + 50, 100  + windowSize.x - 120 - 25 - fuck_AHX, upLinkY + 50);
    line(100 + windowSize.x - 120 - 60 - fuck_AHX, upLinkY + 60, 100  + windowSize.x - 120 - 25 - fuck_AHX, upLinkY + 60);

    if(linkJD != "N")
    {
        int end = 100 + MSA - fuck_AHX;
        int start = end - 250 - fuck_AHX;


        if (start < 100 + 5 - fuck_AHX) start = 100 + 5;

        if (end > windowSize.x - 20 - 5 - fuck_AHX) end = windowSize.x - 20 - 5;
        if (start > windowSize.x - 20 - 5 - fuck_AHX) start = windowSize.x - 20 - 5;

        setcolor(EGERGBA(150, 255, 200, MSAAlpha));
        setlinewidth(6);
        ege_line(start, windowSize.y - 20, end, windowSize.y - 20);
    }

    DChoose2(userBarChoose);
}

//地址栏图标
PIMAGE bar_img[6];
int bar_imgX[6] = { 0,0,0,0,0,0 };
int bar_imgX_frame = 50;
int barimgTEMP = 0;

int clock_port = 0;

int samal = 20;

int CBDS_frame = 0;



void DrawBack()
{
    RECT windowPos = {};
    if (!mainWindow || !IsWindow(mainWindow)) {
        mainWindow = getHWnd();
    }
    windowPos = tool::GetClientRectDetails(mainWindow);


    if (!tool::IsRectEqual(lastRect, windowPos) || (needChangeUserset5&&!Choose4ChooseChange550[0]))
    {
        if (0 == getimage(backImage_i, backImage, windowPos.left, windowPos.top + tool::GetTaskbarHeight() * 3 / 2, windowPos.right - windowPos.left, windowPos.bottom - windowPos.top))
        {
            memcpy(&lastRect, &windowPos, sizeof(RECT));
            if (stoi(userset[5]) > 0)
            {
                int down;
                if (stoi(userset[5]) > 5) down = stoi(userset[5]) / 5;
                else down = 1;
                img::BlurWithDownscale_Gaussian(backImage_i, stoi(userset[5]), stoi(userset[5]), down);
            }
        }

        if (needChangeUserset5)needChangeUserset5 = false;
    }

    if (getwidth(backImage_i) > 0)
    {
        if (DarkMode)
        {
            putimage_alphablend(NULL, backImage_i, 0, 0, stoi(userset[6]) * 255 / 100);
        }
        else
        {
            putimage_alphablend(NULL, backImage_i, 0, 0, stoi(userset[6]) * 255 / 100);
        }
    }
}

int clock_CheckWindowPos;
POINT lastPos_CheckWindowPos;
int CheckWindowPos_frame = 60;
POINT check_pos;
bool isMoveRight = false;

void CheckWindowPos()
{
    POINT wp = Window::GetWindowPos();

    if(!isMoveRight)
    {
        if (wp.x != lastPos_CheckWindowPos.x || wp.y != lastPos_CheckWindowPos.y)
        {
            clock_CheckWindowPos = 0;
            lastPos_CheckWindowPos.y = wp.y;
            lastPos_CheckWindowPos.x = wp.x;
            CheckWindowPos_frame = 60;
        }
        else
        {
            clock_CheckWindowPos += 1;
        }
    }

    if (clock_CheckWindowPos > 60)
    {
        if(wp.x < 0 || wp.y < 0 || wp.x > screen.x - windowSize.x/2 || wp.y > screen.y - windowSize.y / 4)
        {
            if (!isMoveRight)
            {
                CheckWindowPos_frame = 0;
                check_pos = wp;
            }
            isMoveRight = true;
        }
    }

    if(isMoveRight)
    {
        if (CheckWindowPos_frame < 30)
        {
            double t = CheckWindowPos_frame / (double)30;
            int startX = check_pos.x;
            
            int endX, endY;
            if(startX < 0) endX = 50;
			else endX = screen.x - windowSize.x - 50;
            int startY = check_pos.y;
            
            if(startY < 0) endY = 50;
			else endY = screen.y - windowSize.y - 50;

            int vX, vY;
            if (startX < 0 || startX > screen.x - windowSize.x / 2) vX = startX + (endX - startX) * ease::easeOut(t, 5);
            else vX = startX;
            if (startY < 0 || startY  > screen.y - windowSize.y / 4) vY = startY + (endY - startY) * ease::easeOut(t, 5);
            else vY = startY;

            movewindow(vX, vY);
            CheckWindowPos_frame++;
        }
        else
        {
            isMoveRight = false;
        }
    }
}

void Draw()
{
    RECT ms = { 0 };
    DwmGetWindowAttribute(mainWindow, DWMWA_EXTENDED_FRAME_BOUNDS, &ms, sizeof(ms));
    SetWindowPos(subWindow, HWND_TOP, ms.left - 5, ms.top - 5, ms.right - ms.left + 25, ms.bottom - ms.top + 25, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
    if (GS > 0)
    {
        POINT startPos = Window::GetWindowPos();
        POINT startSize = Window::GetWindowSize();
        int targetX = resizeRect.left;
        int targetY = resizeRect.top;
        int targetW = resizeRect.right - resizeRect.left;
        int targetH = resizeRect.bottom - resizeRect.top;

        const int totalFrames = easeTM; // 帧数更大，动画更慢
        double t = 1.0 - (GS / static_cast<double>(totalFrames));

        if (targetW < startSize.x && targetH < startSize.y) t = ease::easeOut(t, 2.5);
        else
        {
            t = ease::easeInOutBack(t, 1.2);
        }

        int newX = static_cast<int>(startPos.x + (targetX - startPos.x) * t);
        int newY = static_cast<int>(startPos.y + (targetY - startPos.y) * t);
        int newW = static_cast<int>(startSize.x + (targetW - startSize.x) * t);
        int newH = static_cast<int>(startSize.y + (targetH - startSize.y) * t);

        // 增加收敛阈值，防止抽搐
        if (abs(newW - targetW) < 5 && abs(newH - targetH) < 5 &&
            abs(newX - targetX) < 5 && abs(newY - targetY) < 5) {
            movewindow(targetX, targetY, false);
            resizewindow(targetW, targetH);
            GS = 0;
            noChange = false;
        }
        else {
            movewindow(newX, newY, false);
            resizewindow(newW, newH);
            GS--;
        }
    }
    else
    {
        if (rs == true)
        {
            noChange = false;
            if (rs == true)
            {
                SetForegroundWindow(mainWindow);
                ShowWindow(mainWindow, SW_SHOW);
                subAlpha = 0;
                rs = false;
            }
        }
        cleardevice();
    }


    CheckWindowPos();

    if (debug)
    {
        setfont(20, 0, "微软雅黑");
        int wordColor = 255 - BKcolor;
        setcolor(EGERGB(wordColor, wordColor, wordColor));
        xyprintf(300, 150, "Debug -> mousePosX>%d  --  mousePosY>%d", mousePos.x, mousePos.y);
        xyprintf(300, 250, "Debug -> 窗口宽>%d  --  窗口高>%d", windowSize.x, windowSize.y);
    }
    {

        DrawBack();

        DrawChoose(userBarChoose + 1);

        getimage(s_temp, 0, 0, windowSize.x, 100);
        if (whle < -200) img::BlurWithDownscale(s_temp, 2, 2); // 2倍降采样

        int BarColor = 0;
        if (DarkMode) BarColor = BKcolor + 30;
        else BarColor = BKcolor - 30;
        setcolor(EGERGBA(BarColor, BarColor, BarColor, 150));
        setfillcolor(EGERGBA(BarColor, BarColor, BarColor, 150));
        putimage(0, 0, s_temp);
        ege_fillrect(0, 0, windowSize.x, 100);

        setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

        for (int i = 0; i < 6; i++)
        {
            if (userBarChoose == i)
            {
                if (userset[0] != "3")
                {
                    setfont(40 - fuckNMD, 0, "Aa吧唧一口小可耐");
                }
                else
                {
                    setfont(40 - fuckNMD, 0, "Ownglyph Soomini Regular");
                }

                wstring connectionPrompt[8] = {
                    // 中文
                    L"此电脑的8位连接口令 : ",
                    // 英文
                    L"8-digit connection code of this PC : ",
                    // 日文
                    L"このPCの8桁接続パスコード : ",
                    // 韩文
                    L"이 PC의 8자리 연결 암호 : ",
                    // 法文
                    L"Code de connexion à 8 chiffres de cet ordinateur : ",
                    // 德文
                    L"8-stelliger Verbindungscode dieses PCs : ",
                    // 俄文
                    L"8-значный код подключения этого компьютера : ",
                    // 西班牙文
                    L"Código de conexión de 8 dígitos de esta PC : "
                };
                wstring usx = connectionPrompt[stoi(userset[0])] + tool::string_to_wstring(OMSR);
                xyprintf(100, 30, usx.c_str());
                break;
            }
        }
        setbkcolor(EGERGB(BKcolor, BKcolor, BKcolor));
        for (int i = 0; i < 3; i++)
        {
            setcolor(EGERGBA(150, 255, 200, AHalpha[i]));
            setfillcolor(EGERGBA(150, 255, 200, AHalpha[i]));

            //左
            if (i == 0)
            {
                ege_fillrect(0, 0, SizeBar, windowSize.y);
            }
            //右
            if (i == 1)
            {
                ege_fillrect(windowSize.x - SizeBar, 0, windowSize.x, windowSize.y);
            }
            //下
            if (i == 2)
            {
                ege_fillrect(0, windowSize.y - SizeBar, windowSize.x, windowSize.y);
            }
        }
    }

    {
        int BarColor = 0;
        if (DarkMode) BarColor = BKcolor + 30;
        else BarColor = BKcolor - 30;
        setcolor(EGERGBA(BarColor, BarColor, BarColor, 150));
        setfillcolor(EGERGBA(BarColor, BarColor, BarColor, 150));
        DrawManager::fillroundrectwithrect(20, 20, BarX + 60, windowSize.y - 40, BarCircleSize);

        if (DarkMode) BarColor = BKcolor + 50;
        else BarColor = BKcolor - 50;

        setcolor(EGERGBA(BarColor, BarColor, BarColor, 200));
        setfillcolor(EGERGBA(BarColor, BarColor, BarColor, 200));
        setlinewidth(2);

        DrawManager::fillroundrectwithrect(30, 60 + wordHigh * (userBarChoose + 1) - 20, 240 - BarSize + BarX, 40, BarCircleSize);
        setcolor(mainColor);
        setlinewidth(7);
        setlinecap(LINECAP_ROUND);
        line(30, 60 + wordHigh * (userBarChoose + 1) - lineLong, 30, 60 + wordHigh * (userBarChoose + 1) + lineLong);


        if (isBar)
        {
            settextjustify(CENTER_TEXT, CENTER_TEXT);

            DrawManager::setFont(30);

            xyprintf(150, 60, "NetTrack");

            wstring md[8][6] = {
                // 中文 (原始文本)
                { L"连接列表", L"发送文件", L"接收文件", L"偏好设置", L"高级设置", L"关于更新" },

                // English
                { L"Connection List", L"Send File", L"Receive File", L"Preferences", L"Advanced Settings", L"About Updates" },

                // 日本語
                { L"接続一覧", L"ファイルを送信", L"ファイルを受信", L"環境設定", L"詳細設定", L"更新について" },

                // 한국어
                { L"연결 목록", L"파일 보내기", L"파일 받기", L"환경 설정", L"고급 설정", L"업데이트 정보" },

                // Français
                { L"Liste de connexions", L"Envoyer un fichier", L"Recevoir un fichier", L"Préférences", L"Paramètres avancés", L"À propos des mises à jour" },

                // Deutsch
                { L"Verbindungsliste", L"Datei senden", L"Datei empfangen", L"Einstellungen", L"Erweiterte Einstellungen", L"Über Updates" },

                // Русский
                { L"Список подключений", L"Отправить файл", L"Получить файл", L"Настройки", L"Дополнительные настройки", L"Об обновлениях" },

                // Español
                { L"Lista de conexiones", L"Enviar archivo", L"Recibir archivo", L"Preferencias", L"Ajustes avanzados", L"Acerca de actualizaciones" }
            };

            for (int i = 0; i < 6; i++)
            {
                DrawManager::setFont(26 - wordSZT[i] - fuckNMD / 2);
                xyprintf(150 + wordCS[i], 60 + wordHigh * (i + 1), md[stoi(userset[0])][i].c_str());
            }
        }
        setlinewidth(4);
        setcolor(mainColor);
        line(35, 50, 65, 50 + BarX / 16);
        line(35, 70, 65, 70 - BarX / 16);
        line(40 + BarX / 7, 60, 60, 60);
        settextjustify(LEFT_TEXT, LEFT_TEXT);

    }


    if (bar_imgX_frame < 120)
    {
        for (int i = 0; i < 6; i++)
        {
            if (i == userBarChoose)
            {
                double startValue = 0.0;
                double endValue = 5.0;

                double t = bar_imgX_frame / (double)120;

                bar_imgX[i] = startValue + (endValue - startValue) * ease::easeOutBack(t, 20);
            }
            else
            {
                if (barimgTEMP == i)
                {
                    double startValue = 5.0;
                    double endValue = 0.0;

                    double t = bar_imgX_frame / (double)120;

                    bar_imgX[i] = startValue + (endValue - startValue) * ease::easeOut(t, 5);
                }
            }
            bar_imgX_frame++;
        }
    }
    else
    {
        barimgTEMP = userBarChoose;
    }

    for (int i = 0; i < 6; i++)
    {

        if (i < 3) putimage_withalpha(NULL, bar_img[i], bar_imgX[i] + 36, 36 + wordHigh * (i + 1), 0, 0, 0, 0);
        else putimage_withalpha(NULL, bar_img[i], bar_imgX[i] + 36, 38 + wordHigh * (i + 1), 0, 0, 0, 0);
    }




    msg::DrawMessageBar();
    msg::msgBoxManager();

    //快速访问
    if (isSendFileQuickSeeX > 0)
    {
        if (DarkMode)setfillcolor(EGERGBA(0, 0, 0, 150));
        else setfillcolor(EGERGBA(250, 250, 250, 150));
        ege_fillrect(windowSize.x - isSendFileQuickSeeX, 0, windowSize.x, windowSize.y);

        if (sendFileList.empty())
        {
            if (DarkMode)setcolor(EGERGB(255, 255, 255));
            else setcolor(EGERGB(20, 20, 20));

            if (stoi(userset[0]) != 3)
            {
                setfont(40, 0, "Aa吧唧一口小可耐");
            }
            else
            {
                setfont(40, 0, "Ownglyph Soomini Regular");
            }

            wstring noFiles[8] = {
                // 中文（简体）
                L"传输列表中没有文件",

                // 英文
                L"No files in transfer list",

                // 日语
                L"転送リストにファイルがありません",

                // 韩语
                L"전송 목록에 파일이 없습니다",

                // 法语
                L"Aucun fichier dans la liste de transfert",

                // 德语
                L"Keine Dateien in der Übertragungsliste",

                // 俄语
                L"Нет файлов в списке передачи",

                // 西班牙语
                L"No hay archivos en la lista de transferencia"
            };

            xyprintf(windowSize.x - isSendFileQuickSeeX / 4 * 3, windowSize.y / 2, noFiles[stoi(userset[0])].c_str());
        }
        else
        {
            static int orMS = 0;
            const int orMSLong = 100;
            orMS += 10;
            if (orMS > 660 + orMSLong) orMS = 0;

            if (DarkMode)setfillcolor(EGERGBA(50, 50, 50, 150));
            else setfillcolor(EGERGBA(200, 200, 200, 150));

            DrawManager::fillroundrectwithrect(windowSize.x - isSendFileQuickSeeX + 20, 20, windowSize.x - 20, 60, 10);

            if (DarkMode)setcolor(EGERGB(255, 255, 255));
            else setcolor(EGERGB(20, 20, 20));

            if (stoi(userset[0]) != 3)
            {
                setfont(25, 0, "Aa吧唧一口小可耐");
            }
            else
            {
                setfont(25, 0, "Ownglyph Soomini Regular");
            }

            wstring transferList[8] = {
                // 中文（简体）
                L"传输列表",

                // 英文
                L"Transfer list",

                // 日语
                L"転送リスト",

                // 韩语
                L"전송 목록",

                // 法语
                L"Liste de transfert",

                // 德语
                L"Übertragungsliste",

                // 俄语
                L"Список передачи",

                // 西班牙语
                L"Lista de transferencia"
            };

            xyprintf(windowSize.x - isSendFileQuickSeeX / 4 * 3, 40, transferList[stoi(userset[0])].c_str());

            for (int i = 0; i < sendFileList.size(); i++)
            {
                if (DarkMode)setfillcolor(EGERGBA(50, 50, 50, 150));
                else setfillcolor(EGERGBA(200, 200, 200, 150));

                DrawManager::fillroundrectwithrect(windowSize.x - isSendFileQuickSeeX + 20, 90 + i * 95, 660, 80, 10);

                wstring mk1 = tool::GbkToWstring(sendFileList[i].displayPath);

                if (DarkMode)setcolor(EGERGB(255, 255, 255));
                else setcolor(EGERGB(20, 20, 20));
                xyprintf(windowSize.x - isSendFileQuickSeeX + 40, 110 + i * 95 + 10, mk1.c_str(), process);

                setcolor(EGERGB(155, 255, 200));
                setlinewidth(3);

                int startX = windowSize.x - isSendFileQuickSeeX + 25 + orMS;
                int endX = windowSize.x - isSendFileQuickSeeX + 25 + orMS - orMSLong;

                if (startX > windowSize.x - isSendFileQuickSeeX + 655)
                {
                    startX = windowSize.x - isSendFileQuickSeeX + 655;
                }
                if (endX < windowSize.x - isSendFileQuickSeeX + 25)
                {
                    endX = windowSize.x - isSendFileQuickSeeX + 25;
                }

                if (i != 0) line(startX, 90 + i * 95 + 77, endX, 90 + i * 95 + 77);
                else line(windowSize.x - isSendFileQuickSeeX + 25, 90 + i * 95 + 77, windowSize.x - isSendFileQuickSeeX + 25 + 655 * process2 / 100, 90 + i * 95 + 77);
            }
        }
    }

    msg::DrawMessageBox();

    if (mouseOSN > 0)
    {
        setlinewidth(mouseOSN / 6);
        setcolor(EGERGBA(150, 255, 200, mouseOSN * 5));
        ege_ellipse(mouseOS.x - (40 - mouseOSN), mouseOS.y - (40 - mouseOSN), (40 - mouseOSN) * 2, (40 - mouseOSN) * 2);
    }

    if (needlockXX > 0)
    {
        needlock::draw();
    }

    getimage(screenShot, 0, 0, windowSize.x, windowSize.y);
}

void Math()
{

    if (isStart)
    {
        if (isFinish)
        {
            if (SHa)
            {
                wstring allTransfersComplete[8] = {
                    // 中文（简体）
                    L"所有文件都传输完毕啦!",

                    // 英文
                    L"All transfers completed successfully!",

                    // 日语
                    L"全ての転送が完了しました！",

                    // 韩语
                    L"모든 전송 완료됐어요!",

                    // 法语
                    L"Tous les transferts sont terminés !",

                    // 德语
                    L"Alle Übertragungen abgeschlossen!",

                    // 俄语
                    L"Все передачи завершены!",

                    // 西班牙语
                    L"¡Todas las transferencias completadas!"
                };
                wprintf(L"\n -> debug -> text : %s", allTransfersComplete[stoi(userset[0])].c_str());
                trd::ShowTrayMessage(mainHwnd, allTransfersComplete[stoi(userset[0])].c_str(), L"NetTrack", 0);
                SHa = false;
            }
        }
        else
        {
            SHa = true;
        }
    }

    if (sendFileList.empty())
    {
        if (SHa == true)
        {
            isFinish = true;
        }
    }

    if (clock_port < 60)clock_port += 1;
    else
    {
        clock_port = 0;
        messageWork();
    }
    //常量
    //===========深色浅色==========
    const int DarkColor = 20;
    const int LightColor = 240;
    const int goToDL_Speed = 5;
    //地址栏
    //宽
    const int barWD = 34;

    windowSize = Window::GetWindowSize();
    clock_math += 1;
    if (clock_math >= 180)clock_math = 0;
    if (clock_math == 179)
    {
        DarkMode = Window::IsDarkModeEnabled();
        if (DarkMode)mainColor = EGERGB(155, 255, 200);
		else mainColor = EGERGB(55, 155, 100);
    }
    mousePos = Window::GetMousePos();
    //------------------侧边栏
    {
        if (mousePos.x < 12)
        {
            if (AHalpha[0] < 255)
            {
                if (255 - AHalpha[0] > 3)
                    AHalpha[0] += 3;
                else AHalpha[0] = 255;
            }
            else AHalpha[0] = 255;
        }
        else
        {
            if (AHalpha[0] > 0)
            {
                if (AHalpha[0] > 3)AHalpha[0] -= 3;
                else AHalpha[0] = 0;
            }
            else AHalpha[0] = 0;
        }
        if (mousePos.x > windowSize.x - 12)
        {
            if (AHalpha[1] < 255)
            {
                if (255 - AHalpha[1] > 3)
                    AHalpha[1] += 3;
                else AHalpha[1] = 255;
            }
            else AHalpha[1] = 255;
        }
        else
        {
            if (AHalpha[1] > 0)
            {
                if (AHalpha[1] > 3)
                    AHalpha[1] -= 3;
                else AHalpha[1] = 0;
            }
            else AHalpha[1] = 0;
        }
        if (mousePos.y > windowSize.y - 12)
        {
            if (AHalpha[2] < 255)
            {
                if (255 - AHalpha[2] > 3)
                    AHalpha[2] += 3;
                else AHalpha[2] = 255;
            }
            else AHalpha[2] = 255;
        }
        else
        {
            if (AHalpha[2] > 0)
            {
                if (AHalpha[2] > 3)
                    AHalpha[2] -= 3;
                else AHalpha[2] = 0;
            }
            else AHalpha[2] = 0;
        }
    }
    // 附属窗口 
    {
        if (subAlpha < 0) subAlpha = 0;
        if (mainWindow == GetForegroundWindow() && ((mousePos.x > -20 && mousePos.x < 20) || (mousePos.x < windowSize.x + 20) && (mousePos.x > windowSize.x - 20) || (mousePos.y > windowSize.y - 20 && mousePos.y < windowSize.y + 20)) && mousePos.y > 0)
        {
            subAlpha = 50;
        }
        else
        {
            if (!noChange)
            {
                subAlpha = 0;
            }
        }
    }

    mousePos = Window::GetMousePosX();

    //快速查看窗口
    {
        if (isSendFileQuickSee_frame < 60)
        {
            if (isSendFileQuickSee)
            {
                double startValue = 0.0;
                double endValue = 700;

                double t = isSendFileQuickSee_frame / (double)60;

                isSendFileQuickSeeX = startValue + (endValue - startValue) * ease::easeOutBack(t, 0.5);
            }
            else
            {
                double startValue = 700;
                double endValue = 0.0;

                double t = isSendFileQuickSee_frame / (double)60;

                isSendFileQuickSeeX = startValue + (endValue - startValue) * ease::easeOut(t, 8);
            }
            isSendFileQuickSee_frame++;
        }
    }

    //鼠标滚轮
    {
        if (whle_temp != 0)
        {
            whle += whle_temp;
            if (whle_temp > 0)whle_temp -= 2;
            else whle_temp += 2;
        }
    }
    //鼠标滚轮边界
    {


        if (whle > 0)
        {
            if (whle > 4000) whle = 4000;
            else if (whle > 1500) whle -= 100;
            else if (whle > 1000) whle -= 60;
            else if (whle > 800)whle -= 50;
            else if (whle > 600) whle -= 40;
            else if (whle > 400)whle -= 30;
            else if (whle > 200)whle -= 20;
            else whle -= 10;

        }
    }
    //------------------深色浅色
    {
        if (DarkMode)
        {
            if (BKcolor > DarkColor)
            {
                if (BKcolor - DarkColor > goToDL_Speed)
                {
                    BKcolor -= goToDL_Speed;
                }
                else
                {
                    BKcolor = DarkColor;
                }
            }
            else
            {
                BKcolor = DarkColor;
            }
        }
        else
        {
            if (BKcolor < LightColor)
            {
                if (LightColor - BKcolor > goToDL_Speed)
                {
                    BKcolor += goToDL_Speed;
                }
                else
                {
                    BKcolor = LightColor;
                }
            }
            else
            {
                BKcolor = LightColor;
            }
        }
    }

    //------------------地址栏
    {
        if (lineLong_frame < 20)
        {
            double startValue = 0.0;
            double endValue = 10;

            double t = lineLong_frame / (double)20;
            lineLong = startValue + (endValue - startValue) * ease::easeOutBack(t, 3.5);
            lineLong_frame++;
        }
        else
        {
            lineLong = 10;
        }

        const int totalFrame_CBDS = 40;
        if (CBDS_frame < totalFrame_CBDS)
        {
            double startValue_1 = 150.0;
            double endValue_1 = 0.0;

            double t = CBDS_frame / (double)totalFrame_CBDS;

            CBOS = startValue_1 + (endValue_1 - startValue_1) * ease::easeOut(t, 5);
            CBDS_frame++;
        }

        for (int i = 0; i < 6; i++)
        {
            if (wordSZT[i] > -5) wordSZT[i] -= 1;
            if (userBarChoose == i)
            {
                if (wordCS[i] < 10)
                {
                    wordCS[i] += 2;
                }
            }
            else
            {
                if (wordCS[i] > 0)
                {
                    wordCS[i] -= 2;
                }
            }
        }

        {
            if (mousePos.x < 100 && isBar == false)
            {
                barXSX = 45;
                isBar = true;
            }
            if (mousePos.x > 200 && isBar == true)
            {
                barXSX = 45;
                isBar = false;
            }

            if (barXSX > 0)
            {
                if (!isBar)
                {
                    BarX -= barXSX;
                }
                else
                {
                    BarX += barXSX;
                }
                barXSX -= 7;
            }

            if (BarX < 10)BarX = 10;
        }
    }

    //------------------鼠标点击特效
    {
        if (mouseOSN > 0)
        {
            mouseOSN -= 1;
        }
        else
        {
            mouseOSN = 0;
        }
    }
}

int keep = 0;
POINT kgs1 = { 0,0 };
POINT kgs2 = { 0,0 };
void Key()
{
    bool mouseLeftDown = true;
    bool KEY = false;
    while (1)
    {
        api_sleep(10);
        if (msg::noBarH() && !needlockX)
        {
            if (keystate(0x10) && keystate(0x56) && keystate(0x11))
            {
                int color[3] = { 150,150,150 };
                int FontColor[3] = { 0,0,0 };
                string b[3] = { "OK","NULL","NULL" };
                msg::MessageBar(color, FontColor, "NetTrack - v0.1_bf_Beta", b);
            }

            if (keystate(0x01))
            {
                mouseOSN = 40;
                mouseOS = Window::GetMousePosX();

                if (isSendFileQuickSee && KEY)
                {
                    isSendFileQuickSee = false;
                    isSendFileQuickSee_frame = 0;
                }

                if(keystate(0x11))
                {
                    if (keep == 0)
                    {
                        kgs1 = Window::GetMousePosX();
                    }
                    

                    if (keep < 20)keep += 1;
                    else
                    {
                        kgs2 = Window::GetMousePosX();
                        if (kgs1.x - kgs2.x > 100)
                        {
                            if (!isSendFileQuickSee)
                            {
                                isSendFileQuickSee = true;
                                isSendFileQuickSee_frame = 0;
                            }
                            KEY = false;
                            kgs2 = kgs1 = { 0,0 };
                        }

                    }
                }
                else
                {
                    keep = 0;
                }
            }
            else
            {
                KEY = true;
                keep = 0;
            }


            if (isMessage == false && !resizing)
            {
                mouse_msg mouseMsg;
                if (mousemsg())
                {
                    mouseMsg = getmouse();
                    if (mouseMsg.is_wheel())   whle_temp = mouseMsg.wheel / 2;
                    if (mouseMsg.is_left())
                    {
                        if (mouseLeftDown == true)
                        {

                            if (isBar)
                            {
                                if (mousePos.x < 200 && mousePos.x > 30)
                                {
                                    for (int i = 0; i < 6; i++)
                                        if (mousePos.y > 60 + wordHigh * (i + 1) - wordHigh / 2 && mousePos.y < 60 + wordHigh * (i + 1) + wordHigh / 2 && userBarChoose != i)
                                        {
                                            if (bar_imgX_frame < 120)
                                            {
                                                barimgTEMP = userBarChoose;
                                            }
                                            userBarChoose = i;
                                            lineLong = 0;
                                            lineLong_frame = 0;
                                            wordSZT[i] = 1;
                                            CBDS_frame = 0;
                                            CBOS = 150;

                                            whle = whle_temp = 0;
                                            whleHistory = 0;
                                            bar_imgX_frame = 0;

                                            linkX_frame[LKA] = 59;
                                            LKA = -1;
                                        }
                                }
                            }
                            mouseLeftDown = false;
                        }
                        else
                        {
                            mouseLeftDown = true;
                        }
                    }
                    flushmouse();
                }
            }
        }
        if (needlockX)
        {
            needlock::key();
        }
    }
}

//字体加载
bool InstallFont(const string& fontPath) {
    // 将路径转换为宽字符
    wstring wFontPath(fontPath.begin(), fontPath.end());
    // 安装字体
    int result = AddFontResourceExW(wFontPath.c_str(), FR_PRIVATE, NULL);
    if (result == 0) {
        // 安装失败
        return false;
    }
    return true;
}

HWND CreateSubWindow(HWND parent) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc_class::SubWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"NetTrackSub";
    wc.hbrBackground = NULL;
    RegisterClass(&wc);


    RECT mainRc;
    GetWindowRect(parent, &mainRc);
    int x = mainRc.left;
    int y = mainRc.bottom;
    int w = mainRc.right - mainRc.left, h = mainRc.bottom - mainRc.top;

    subWindow = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, wc.lpszClassName, L"NetTrackSub",
        WS_POPUP | WS_THICKFRAME | WS_VISIBLE,
        x, y, w, h, parent, NULL, wc.hInstance, NULL);


    ShowWindow(subWindow, SW_SHOW);
    SetForegroundWindow(mainWindow);
    return subWindow;
}

bool noFirstStart = true;

int ReadIni() {
    ifstream isFile("config.ini");
    if (!isFile.is_open()) {
        if (-1 == First::firstStart()) exit(0xF1);
        else noFirstStart = false;
        return 0;
    }
    isFile.close();

    colorPrintf color;

    try {
        // 读取config.ini
        vector<string> configData = 
        SecureFileStorage::LoadSecureData(L"config.ini");
        if (!configData.empty()) {
            for (int i = 0; i < min((int)configData.size(), saveConfigMax); i++) {
                userset[i] = (configData[i].empty() || configData[i] == "NULL") ? "0" : configData[i];
            }
        }

        if (userset[4] != "0") userSaveDir = tool::string_to_wstring(userset[4]);
        else userSaveDir = Net::GetDownloadFolderPath();
        if (userset[5].empty()) userset[5] = "30";
        if (userset[6].empty()) userset[5] = "70";
        if (userset[7].empty()) userset[5] = "0";
        if (userset[8].empty()) userset[8] = "20";
        if(userset[9].empty()) userset[9] = "0";

        // 读取config.dll
        vector<string> linkData = 
        SecureFileStorage::LoadSecureData(L"config.dll");
        if (!linkData.empty()) {
            for (int i = 0; i < min((int)linkData.size(), saveMax); i++) {
                saveData_link[i] = (linkData[i].empty() || linkData[i] == "NULL") ? "NULL" : linkData[i];
            }
        }

        // 读取Record_upload.dll
        vector<string>  uploadData = 
        SecureFileStorage::LoadSecureData(L"Record_upload.dll");
        if (!uploadData.empty()) {
            for (int i = 0; i < min((int)uploadData.size(), lastUploadMax); i++) {
                lastUpload[i] = (uploadData[i].empty() || uploadData[i] == "NULL") ? "NULL" : uploadData[i];
            }
        }

        // 读取Record_download.dll
        vector<string>  downloadData = 
        SecureFileStorage::LoadSecureData(L"Record_download.dll");
        if (!downloadData.empty()) {
            for (int i = 0; i < min((int)downloadData.size(), lastDownloadMax); i++) {
                lastDownload[i] = (downloadData[i].empty() || downloadData[i] == "NULL") ? "NULL" : downloadData[i];
            }
        }

        trustList = SecureFileStorage::LoadSecureData(L"ts.dll");
        if (!trustList.empty())
        {
            if(trustList[0] == "NULL" || trustList[0].empty())
            {
                trustList.clear();
			}
        }

        if (debug)
        {
            for (int i = 0; i < saveConfigMax; i++)
            {
                if(debug) cout << endl << "userset    [" + to_string(i) + "]    " + userset[i];
            }
            for (int i = 0; i < saveMax; i++)
            {
                if(debug) cout << endl << "saveData_link    [" + to_string(i) + "]    " + saveData_link[i];
            }
            for (int i = 0; i < lastUploadMax; i++)
            {
                if(debug) cout << endl << "lastUpload    [" + to_string(i) + "]    " + lastUpload[i];
            }
            for (int i = 0; i < lastDownloadMax; i++)
            {
                if(debug) cout << endl << "lastDownload    [" + to_string(i) + "]    " + lastDownload[i];
            }
        }
        return 1;
    }
    catch (...) {
        color.PrintColorText("debug -> 配置文件读取失败\n", 255, 0, 0, 0);
        return -1;
    }


};

int main(HINSTANCE hInstance, HINSTANCE, int) {

    SetProcessDPIAware();

    if(debug)
    {
        AllocConsole();  // 分配控制台
        freopen("CONOUT$", "w", stdout);  // 重定向 stdout
        freopen("CONOUT$", "w", stderr);  // 重定向 stderr
    }

    for (int i = 0; i < saveMax; i++)
    {
        saveData_link[i] = "NULL";
        linkX[i] = 0;
        linkX_frame[i] = 60;
    }
    for (int i = 0; i < lastUploadMax; i++)
    {
        lastUpload[i] = "NULL";
    }
    for (int i = 0; i < lastDownloadMax; i++)
    {
        lastDownload[i] = "NULL";
    }

    

    

    colorPrintf color;
    color.EnableVTMode();
    WSADATA wsaDataX;
    if (WSAStartup(MAKEWORD(2, 2), &wsaDataX) != 0) {
        cerr << "WSAStartup failed\n";
        MessageBoxA(NULL, "Failed to load \nError code：WSASTARTUPERROR", "NetTrackERROR", MB_OK + MB_SYSTEMMODAL + 16);
        exit(-1);
    }
    //初始化
    {
        if (!isFile("FontManager.dll"))
        {
            MessageBoxA(NULL, "code:FONTMANAGERLOST", "NetTrackERROR", MB_OK + MB_SYSTEMMODAL + 16);
            exit(-1);
        }
        if (!isFile("FontManager2.dll"))
        {
            MessageBoxA(NULL, "code:FONTMANAGER2LOST", "NetTrackERROR", MB_OK + MB_SYSTEMMODAL + 16);
            exit(-1);
        }
        InstallFont("FontManager.dll");
        InstallFont("FontManager2.dll");

        if (!isFile("NetTrackHt.dll"))
        {
            MessageBoxA(NULL, "code:NETTRACKHTLOST", "NetTrackERROR", MB_OK + MB_SYSTEMMODAL + 16);
            exit(-1);
        }

        if (!isFile("NetTrackImgManager.dll"))
        {
            MessageBoxA(NULL, "code:NETTRACKIMGMANAGERLOST", "NetTrackERROR", MB_OK + MB_SYSTEMMODAL + 16);
            exit(-1);
        }

        // 注册窗口类
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = WindowProc_class::WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"NetTrack";
        RegisterClass(&wc);

        mainHwnd = CreateWindow(wc.lpszClassName, L"NetTrack", 0, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

        // 获取 WM_TASKBARCREATED 消息ID
        WM_TASKBARCREATED = RegisterWindowMessageW(L"TaskbarCreated");

        // 初始化托盘图标数据
        trd::InitTrayIcon(mainHwnd, L"NetTrack");

        hInstanceTop = hInstance;

        RECT desktopRect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopRect, 0);
        screen.x = desktopRect.right - desktopRect.left;
        screen.y = desktopRect.bottom - desktopRect.top;

        initgraph(screen.x / 4 * 3, screen.y / 4 * 3, 0x0 | 0x10 | 0x40 | 0x40);

        setcaption("NetTrack");
        mainWindow = getHWnd();

        movewindow(screen.x / 2 - screen.x / 4 * 3 / 2, screen.y + 50);


        // 获取壁纸路径
        wchar_t wallpaperPath[MAX_PATH] = { 0 };
        SystemParametersInfoW(SPI_GETDESKWALLPAPER, MAX_PATH, wallpaperPath, 0);

        // 检查路径是否有效
        if (wcslen(wallpaperPath) > 0 && _waccess(wallpaperPath, 0) == 0) {
            // 加载壁纸到PIMAGE
            backImage = newimage();
            if (backImage) {
                // EGE 支持 BMP/JPG/PNG
                if (getimage(backImage, wallpaperPath, 0, 0) != 0) {
                    delimage(backImage);
                    backImage = nullptr;
                }
            }
        }
        else
        {
            MessageBoxA(NULL, "哎呀，出现了一个尴尬的小错误", "Error", MB_OK + MB_SYSTEMMODAL + 16);
        }

        g_EGE_OldWndProc = (WNDPROC)SetWindowLongPtr(mainWindow, GWLP_WNDPROC, (LONG_PTR)WindowProc_class::EGE_WindowProc);
        LONG style = GetWindowLong(mainWindow, GWL_STYLE);
        style |= WS_MINIMIZEBOX;
        SetWindowLong(mainWindow, GWL_STYLE, style);
        subWindow = CreateSubWindow(mainWindow);
        WindowProc_class::InstallMouseHook();

    }

    ifstream cs("NetTrack_imgPack.dll");
    if (cs.is_open())
    {
        cs.close();

        FileControl::UnpackFilesFromOne("NetTrack_imgPack.dll", "");
        for (int i = 0; i < 6; i++)
        {
            bar_img[i] = newimage();
        }
        string imgName[6] = { "connect.png","download.png","inf.png","set.png","set2.png","upload.png" };

        getimage(bar_img[0], imgName[0].c_str(), 0, 0);
        getimage(bar_img[1], imgName[5].c_str(), 0, 0);
        getimage(bar_img[2], imgName[1].c_str(), 0, 0);
        getimage(bar_img[3], imgName[3].c_str(), 0, 0);
        getimage(bar_img[4], imgName[4].c_str(), 0, 0);
        getimage(bar_img[5], imgName[2].c_str(), 0, 0);

        for (int i = 0; i < 6; i++)
        {
            remove(imgName[i].c_str());
        }

        for (int i = 0; i < 6; i++)
        {
            img::ZoomImage_Alpha(bar_img[i], 45, 45);
        }
    }
    else
    {
        MessageBoxA(NULL, "ERROR Code: NETTRACKIMGPACKDLLLOST", "Error", MB_OK + MB_SYSTEMMODAL + 16);
        exit(-1);
    }

    thread Graph_key(Key);
    Graph_key.detach();

    
    ReadIni();

    FileControl::UnpackFilesFromOne("NetTrackImgManager.dll", "");
    getimage(gogogo, "1.jpg", 0, 0);
    img::zoomImage(gogogo, 200, 200);
    remove("1.jpg"); remove("2.png");

    
    ip_local = Net::get_local_ip();
    if (stoi(userset[9]) == 0) port = GenerateDynamicPort();
    else 
    {
        for (int i = 0; i < 100; i++)
        {
            if (IsPortAvailable(7750 - i * 10) && IsPortAvailable(7751 - i * 10))
            {
                port = 7750 - i * 10;
                break;
            }
        }
    }

    

    OMSR = code::encode_ip_port(ip_local, port);
    if (debug) if(debug) cout << "本机IP: " << ip_local << ", 端口: " << port << ", 8位口令: " << OMSR << endl;

    
    ifstream Debug("debug.dll");
    if (Debug.is_open() || debug)
    {
        string s;
        Debug >> s;
        if (s == "NETTRACKDEBUG_SET" || debug)
        {
            debug = true;
            init_console();
        }
        Debug.close();
    }
    else
    {
        close_console();
    }

    if (debug) if(debug) cout << endl << "初始化";

    setbkmode(TRANSPARENT);

    
    isSendFileQuickSee = false;
    img::ScaleWallpaperToScreen();

    Net::setSendSpeedLimitMB(stoi(userset[8]));

    if (!Net::CheckInternetConnection())
    {
        int color[3] = { 255,100,100 };
        int fontColor[3] = { 0,0,0 };

        string networkErrorMsg[8][2] = {
            // 中文（简体）
            "您似乎未连接到网络","请检查网络连接状态",

            // 英文
            "You appear to be offline"," Please check your network connection",

            // 日语
            "ネットワークに接続されていないようです","接続状態を確認してください",

            // 韩语
            "인터넷에 연결되지 않은 것 같습니다","네트워크 상태를 확인하세요",

            // 法语
            "Vous semblez hors ligne","Veuillez vérifier votre connexion réseau",

            // 德语
            "Sie scheinen offline zu sein","Bitte überprüfen Sie Ihre Netzwerkverbindung",

            // 俄语
            "Кажется, вы не в сети"," Проверьте подключение к сети",

            // 西班牙语
            "Parece que no estás conectado","Verifica tu conexión de red"
        };

        string s = networkErrorMsg[stoi(userset[0])][0] + " 0 " + networkErrorMsg[stoi(userset[0])][1];

        string confirmMsg[8] = {
            // 中文（简体）
            "确定",        // 标准按钮文本
            // "确认"      // 备选（更正式）

            // 英文
            "OK",         // 通用对话框标准
            // "Confirm"   // 需二次确认场景

            // 日语
            "はい",        // 通用确认（Ha-i）
            // "確認"      // 系统级操作（Kakunin）

            // 韩语
            "확인",        // 标准确认（Hwag-in）
            // "예"        // 肯定回答（Ye，类似"Yes"）

            // 法语
            "OK",         // 通用（保留英文）
            // "Valider"   // 表单提交场景

            // 德语
            "OK",         // 通用
            // "Bestätigen" // 重要操作确认

            // 俄语
            "ОК",         // 通用（西里尔字母）
            // "Подтвердить" // 完整动词

            // 西班牙语
            "Aceptar",    // 标准接受
            // "OK"        // 拉丁美洲常用
        };

        string b[3] = {
            confirmMsg[stoi(userset[0])], // 确认按钮
            "NULL",                        // 取消按钮
            "NULL"                     // 重新连接按钮
        };
        msg::MessageBar(color, fontColor, s, b);
    }

    // 消息循环
    MSG msg;

    LONG style = GetWindowLong(mainWindow, GWL_STYLE);
    style |= WS_MAXIMIZEBOX;
    SetWindowLong(mainWindow, GWL_STYLE, style);

    ifstream lc("lcin.dll");
    if (lc.is_open())
    {
        first::firststartA = true;
        lc.close();
        string consentOptions[8][3] = {
            // 中文
            { "同意", "拒绝", "退出" },
            // English
            { "Accept", "Decline", "Exit" },
            // 日本語
            { "同意する", "拒否する", "終了する" },
            // 한국어
            {"동의", "거절", "종료" },
            // Français
            { "Accepter", "Refuser", "Quitter" },
            // Deutsch
            { "Zustimmen", "Ablehnen", "Beenden" },
            // Русский
            { "Принять", "Отклонить", "Выйти" },
            // Español
            { "Aceptar", "Rechazar", "Salir" }
        };
        string eulaPrompt[8] = {
            // 中文
            "请阅读并同意NetTrack的使用准则",
            // English
            "Please read and agree to NetTrack's Terms of Use",
            // 日本語
            "NetTrackの利用規約を読み同意してください",
            // 한국어
            "NetTrack 사용 약관을 읽고 동의해 주세요",
            // Français
            "Veuillez lire et accepter les conditions d'utilisation de NetTrack",
            // Deutsch
            "Bitte lesen und akzeptieren Sie die Nutzungsbedingungen von NetTrack",
            // Русский
            "Пожалуйста, прочитайте и примите условия использования NetTrack",
            // Español
            "Por favor lea y acepte los términos de uso de NetTrack"
        };
        int color[3] = { 150,150,150 };
        int fontColor[3] = { 0,0,0 };
        msg::MessageBar(color, fontColor, eulaPrompt[stoi(userset[0])], consentOptions[stoi(userset[0])]);

        if(1 == FileControl::UnpackFilesFromOne("NetTrackHt.dll",""))
        {
            for (int i = 0; i < 8; i++)
            {
                if (i != stoi(userset[0]))
                {
                    string name = "NetTrackUse" + to_string(i) + ".html";
                    remove(name.c_str());
                }
            }
            std::wstring netTrackGuidelines[8] = {
                   L"NetTrack使用准则",          // Chinese (Simplified)
                 L"NetTrack Usage Guidelines",  // English
                  L"NetTrack利用規約",         // Japanese
                 L"NetTrack 사용 지침",       // Korean
                  L"Règles d'utilisation de NetTrack",  // French
                   L"NetTrack-Nutzungsrichtlinien",  // German
                   L"Правила использования NetTrack",  // Russian
                   L"Pautas de uso de NetTrack"  // Spanish
            };
            wstring name = L"NetTrackUse" + tool::string_to_wstring(userset[0]) + L".html";
            
            html::showHtml(name, netTrackGuidelines[stoi(userset[0])]);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    Net::startRecvThread(port, tool::tool::Utf8ToGbk(tool::wstring_to_string(userSaveDir)).c_str());

    Net::startRecvStringThread(port + 1);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    thread am(GetProcess);
    am.detach();

    wstring netTrackBackground[8] = {
        // 中文（简体）
        L"NetTrack 正在后台运行",

        // 英文
        L"NetTrack is now running in background",

        // 日语
        L"NetTrack がバックグラウンドで動作中です",

        // 韩语
        L"NetTrack 이(가) 백그라운드에서 실행 중",

        // 法语
        L"NetTrack s'exécute maintenant en arrière-plan",

        // 德语
        L"NetTrack läuft jetzt im Hintergrund",

        // 俄语
        L"NetTrack теперь работает в фоновом режиме",

        // 西班牙语
        L"NetTrack se está ejecutando en segundo plano"
    };

    trd::ShowTrayMessage(mainHwnd, netTrackBackground[stoi(userset[0])].c_str(), L"NetTrack", 0);

    if(userset[2] == "0")
    {
        string translationsF[8] = {
        "NetTrack安全警告",          // 中文 (简体)
        "NetTrack Security Warning",  // English
        "NetTrackセキュリティ警告",    // 日本語
        "NetTrack 보안 경고",        // 한국어
        "Avertissement de sécurité NetTrack",  // Français
        "NetTrack-Sicherheitswarnung",         // Deutsch
        "Предупреждение безопасности NetTrack", // Русский
        "Advertencia de seguridad de NetTrack"  // Español
        };
        string translations[8] = {
            // 中文 (简体)
            "请尽快设置您的密码，点击\"偏好设置\"中的\"本机密码重新设置\"进行设置，否则其他设备可以直接连接您的NetTrack",

            // English
            "Please set your password as soon as possible by clicking \"Local Password Reset\" in \"Preferences\", otherwise other devices can directly connect to your NetTrack.",

            // 日本語
            "できるだけ早くパスワードを設定してください。「環境設定」の「ローカルパスワード再設定」をクリックして設定します。設定しない場合、他のデバイスが直接あなたのNetTrackに接続できるようになります。",

            // 한국어
            "빠른 시일 내에 비밀번호를 설정하십시오. \"환경 설정\"에서 \"로컬 암호 재설정\"을 클릭하여 설정하지 않으면 다른 장치가 직접 귀하의 NetTrack에 연결할 수 있습니다.",

            // Français
            "Veuillez définir votre mot de passe dès que possible en cliquant sur \"Réinitialisation du mot de passe local\" dans \"Préférences\", sinon d'autres appareils pourront se connecter directement à votre NetTrack.",

            // Deutsch
            "Bitte legen Sie Ihr Passwort so bald wie möglich fest, indem Sie auf \"Lokale Passwortzurücksetzung\" in \"Einstellungen\" klicken, andernfalls können andere Geräte direkt eine Verbindung zu Ihrem NetTrack herstellen.",

            // Русский
            "Пожалуйста, как можно скорее установите пароль, нажав \"Сброс локального пароля\" в \"Настройки\", иначе другие устройства смогут напрямую подключаться к вашему NetTrack.",

            // Español
            "Establezca su contraseña lo antes posible haciendo clic en \"Restablecimiento de contraseña local\" en \"Preferencias\", de lo contrario otros dispositivos podrán conectarse directamente a su NetTrack."
        };
        thread a(msgBox::msgbox,hInstance,translationsF[stoi(userset[0])],translations[stoi(userset[0])],WinPrompt::ICON_WARNING,WinPrompt::BTN_OK,WinPrompt::MODE_WARNING);
        a.detach();
    }

    DarkMode = Window::IsDarkModeEnabled();
    if (DarkMode)mainColor = EGERGB(155, 255, 200);
    else mainColor = EGERGB(55, 155, 100);
    int BKCOLOR = 0;
    if (DarkMode)
    {
        setbkcolor(EGERGB(20, 20, 20));
        BKCOLOR = 20;
        BKcolor = 20;
    }
    else
    {
        setbkcolor(EGERGB(240, 240, 240));
        BKCOLOR = 240;
        BKcolor = 240;
    }

    showwindow();

    if(noFirstStart)
    {
        int frameS = 0;
        for (frameS; frameS < 60; frameS++)
        {
            api_sleep(10);

            double t = frameS / (double)60;

            int start = screen.y + 50;
            int end = 50;
            double v = start + (end - start) * ease::easeOut(t, 5);

            movewindow(screen.x / 2 - screen.x / 4 * 3 / 2, v);
        }
    }

    

    int rd = 255;
    while (is_run()) {
        if (first::firststartA)
        {
            if (msg::GetMessageBarChoose() == 1 || msg::GetMessageBarChoose() == 2)
            {
                exit(-1);
            }

            if (msg::GetMessageBarChoose() == 0)
            {
                remove("lcin.dll");
                first::firststartA = false;
            }
        }
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        if (msg.message == WM_TASKBARCREATED) {
            WindowProc_class::WindowProc(mainHwnd, msg.message, msg.wParam, msg.lParam);
        }
        delay_fps(60);
        cleardevice();
        if(!Manager)
        {
            Draw();
            if (rd > 0)
            {
                rd -= 15;
                setcolor(EGERGBA(BKCOLOR, BKCOLOR, BKCOLOR, rd));
                setfillcolor(EGERGBA(BKCOLOR, BKCOLOR, BKCOLOR, rd));
            }
            Math();
            if (rd > 0) ege_fillrect(0, 0, windowSize.x, windowSize.y);
        }
    }


    ini::SaveData();

    POINT ms = Window::GetWindowPos();
    POINT rsb = Window::GetWindowSize();

    PIMAGE rt = newimage();
    getimage(rt, 0, 0, rsb.x, rsb.y);
    img::BlurWithDownscale(rt, 2, 2); // 2倍降采样


    int rs = 20;
    int mg = 0;
    subAlpha = 0;
    for (int i = 0; i < 50; i++)
    {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        api_sleep(10);
        cleardevice();
        putimage(0, 0, rt);
        movewindow(ms.x, ms.y - mg);
        mg += rs;
        rs -= 2;
    }
    ShowWindow(mainWindow, SW_MINIMIZE);

    LONG style3 = GetWindowLong(mainWindow, GWL_EXSTYLE);
    style3 |= WS_EX_TOOLWINDOW;
    SetWindowLong(mainWindow, GWL_EXSTYLE, style);

    delimage(rt);
    trd::RemoveTrayIcon(mainHwnd);
    WindowProc_class::UninstallMouseHook();
    delimage(gogogo);
    Net::exit();
    return 0;

}