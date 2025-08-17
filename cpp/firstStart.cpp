#include <graphics.h>
#include <iostream>
#include <filesystem>
#include <string>
#include "Main.h"
#include "ease.h"
#include "GraphManager.h"
#include "WindowsEv.h"
#include "img.h"
#include "AES.h"
#include "FileControl.h"
#include "Tool.h"
#include "FirstStart.h"

using namespace std;
namespace fs = filesystem;

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


#pragma region firstStart
int userLg = 0;
int userFileLock = 1;
int userLock[6] = { -1,-1,-1,-1,-1,-1 };
int userLock_temp[6] = { -1,-1,-1,-1,-1,-1 };

int cgos = -1;
int cgos_frame = 60;

int frameA = 120;
int frameB = 120;
int frameC = 0;
int frameD = 0;
int userLock_fontFrame[8] = { 60,60,60,60,60,60,60,60 };

char linkCode[8] = { '?','?', '?', '?', '?', '?', '?', '?' };
char linkCode_temp[8] = { '?','?', '?', '?', '?', '?', '?', '?' };
int userLink_fontFrame[8] = { 60,60,60,60,60,60,60,60 };

bool loadAS = false;
int loadA = 0;
int roundA = 0;

wstring zh2[2] = { L"确定", L"选择语言" };
wstring en2[2] = { L"OK", L"Select language" };
wstring jp2[2] = { L"確認", L"言語選択" };
wstring kr2[2] = { L"확인", L"언어 선택" };
wstring fr2[2] = { L"OK", L"Choisir langue" };
wstring de2[2] = { L"OK", L"Sprache wählen" };
wstring ru2[2] = { L"ОК", L"Выбор языка" };
wstring es2[2] = { L"Aceptar", L"Elegir idioma" };

PIMAGE logoImage[5];

struct ROUND
{
    POINT round = { 0,0 };
    int size = 0;
    int maxSize = 0;
    bool KO = false;
    bool other = false;
};

const int max_struct = 10;
ROUND round_struct[max_struct];
int animFrame = 0;

    bool first::start1(int w, int h)
    {
        bool isFinish = false;


        
        const int animTotalFrames = 90; // 动画帧数（1.5秒）
        const double startScale = 1.0;
        const double endScale = 0.5;

        // 位置动画参数
        int logoW = getwidth(logoImage[0]);
        int logoH = getheight(logoImage[0]);
        int startX = w / 2 - int(logoW * startScale) / 2;
        int endX = w / 5 - int(logoW * endScale) / 2;
        int startY = h / 2 - int(logoH * startScale) / 2; // 居中
        int endY = h / 2 - int(logoH * endScale) / 2; // 居中

        double scale = endScale;
        int drawX = endX, drawY = endY;
        if (animFrame < animTotalFrames) {
            double t = animFrame / (double)(animTotalFrames);
            double easeT = ease::easeInOut(t, 5);
            scale = startScale + (endScale - startScale) * easeT;
            drawX = startX + int((endX - startX) * easeT);
            drawY = startY + int((endY - startY) * easeT);
            animFrame++;
        }
        else {
            scale = endScale;
            drawX = endX;
            drawY = endY;
            isFinish = true;
        }

        int drawW = static_cast<int>(logoW * scale);
        int drawH = static_cast<int>(logoH * scale);




        // 临时缩放，不破坏原图
        PIMAGE tempLogo = newimage(drawW, drawH);
        putimage(tempLogo, 0, 0, drawW, drawH, logoImage[0], 0, 0, logoW, logoH);
        putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
        delimage(tempLogo);

        return isFinish;
    }

    void first::OBI(int type, int w, int h)
    {
        if (type == 1)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0.6;
            const double endScale02 = 0.4;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 2 - int(logoW * startScale02) / 2;
            int endX = w / 5 - int(logoW * endScale02) / 2;
            int startY = h / 2 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeInOut(t, 5);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame2++;
            }
            else {
                scale = endScale02;
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[1], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 2)
        {

            static int animFrame23 = 0;
            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0.25;
            const double endScale02 = 0.15;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 5 + w / 20 - int(logoW * startScale02) / 2;
            int endX = w / 5 + w / 20 - int(logoW * endScale02) / 2;
            int startY = h / 3 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - h / 8 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame23 < animTotalFrames2) {
                double t = animFrame23 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeInOutBack(t, 12);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame23++;

                int drawW = static_cast<int>(logoW * scale);
                int drawH = static_cast<int>(logoH * scale);

                // 临时缩放，不破坏原图
                PIMAGE tempLogo = newimage(drawW, drawH);
                putimage(tempLogo, 0, 0, drawW, drawH, logoImage[2], 0, 0, logoW, logoH);
                putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
                delimage(tempLogo);
            }
        }
        if (type == 3)
        {


            wstring ysFile[8] = { L"文件加密",L"File Encryption",L"ファイルの暗号化",L"파일 암호화",L"Français",L"Deutsch",L"Русский",L"Español" };
            wstring ysYS[8] = { L"确定", L"OK", L"確認", L"확인", L"Confirmer", L"Bestätigen", L"Подтвердить", L"Confirmar" };

            int totalFrames = 90;
            double startValue = 0.0;
            double endValue = w / 27;
            static int frame = 0;
            int fsz = 0;
            if (frame < totalFrames)
            {
                double t = frame / (double)totalFrames;
                int v = startValue + (endValue - startValue) * ease::easeOutBack(t, 3);
                fsz = v;
                frame++;
            }
            else
            {
                fsz = w / 27;
            }

            if (DarkMode) setcolor(WHITE);
            else setcolor(BLACK);
            if (userLg != 3)
            {
                setfont(fsz, 0, "Aa吧唧一口小可耐");
            }
            else
            {
                setfont(fsz, 0, "Ownglyph Soomini Regular");
            }
            xyprintf(w * 3 / 4, h / 7 + whle / 5, ysFile[userLg].c_str());
        }
        if (type == 4)
        {


            wstring ys[8] = { L"无加密",L"Unencrypted",L"暗号化な",L"암호화 없음",L"Non chiffré",L"Keine Verschlüsselung",L"Нет шифрования",L"Sin Cifrar" };
            wstring PK[3] = { L"XOR",L"LCD(NetTrack)",L"AES" };
            for (int i = 0; i < 4; i++)
            {
                if (DarkMode)
                {
                    setcolor(EGERGBA(90, 90, 90, 200));
                    setfillcolor(EGERGBA(70, 70, 70, 150));
                }
                else
                {
                    setcolor(EGERGBA(220, 220, 220, 200));
                    setfillcolor(EGERGBA(240, 240, 240, 150));
                }


                int totalFrames = 600;
                double startValue = 0.0;
                double endValue = 30.0;
                static int frame2 = 0;
                double v = 0;
                if (frame2 < totalFrames)
                {
                    double t = frame2 / (double)totalFrames;
                    v = startValue + (endValue - startValue) * ease::easeOutBack(t, 6);
                }
                else
                {
                    v = endValue;
                }

                setlinewidth(2);
                DrawManager::fillroundrectwithrect(w / 2 + 45 - v / 2, h / 7 * (i + 2) + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 40 + v, screen.x / 90);
                ege_roundrect(w / 2 + 45 - v / 2, h / 7 * (i + 2) + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 40 + v, screen.x / 90);
                if (i > 0)
                {
                    if (DarkMode) setcolor(WHITE);
                    else setcolor(BLACK);
                    setfont(v * 3 / 2, 0, "Aa吧唧一口小可耐");
                    xyprintf(w * 3 / 4, h / 7 * (i + 2) + whle / 5, PK[i - 1].c_str());
                }
                else
                {
                    setlinewidth(2);
                    DrawManager::fillroundrectwithrect(w / 2 + 45 - v / 2, h / 7 * (0 + 1) + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 40 + v, screen.x / 90);
                    ege_roundrect(w / 2 + 45 - v / 2, h / 7 * (0 + 1) + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 40 + v, screen.x / 90);

                    if (DarkMode) setcolor(WHITE);
                    else setcolor(BLACK);
                    setfont(v * 3 / 2, 0, "Aa吧唧一口小可耐");
                    xyprintf(w * 3 / 4, h / 7 * (i + 2) + whle / 5, ys[userLg].c_str());
                }
                frame2++;
            }
        }
        if (type == 5)
        {

            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0.15;
            const double endScale02 = 0.15;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 5 + w / 20 - int(logoW * startScale02) / 2;
            int endX = w / 5 + w / 20 - int(logoW * endScale02) / 2;
            int startY = h / 2 - h / 8 - int(logoH * startScale02) / 2; // 居中
            int endY = h * 3 / 2 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (frameA < animTotalFrames2) {
                double t = frameA / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeIn(t, 6);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                frameA++;

                int drawW = static_cast<int>(logoW * scale);
                int drawH = static_cast<int>(logoH * scale);

                // 临时缩放，不破坏原图
                PIMAGE tempLogo = newimage(drawW, drawH);
                putimage(tempLogo, 0, 0, drawW, drawH, logoImage[2], 0, 0, logoW, logoH);

                putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
                delimage(tempLogo);
            }
        }
        if (type == 6)
        {

            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0;
            const double endScale02 = 0.15;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 5 + w / 20 - int(logoW * startScale02) / 2;
            int endX = w / 5 + w / 20 - int(logoW * endScale02) / 2;
            int startY = h - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - h / 8 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (frameA < animTotalFrames2) {
                double t = frameA / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeOutBack(t, 3);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                frameA++;
            }
            else
            {
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[2], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 7)
        {
            for (int i = 0; i < 8; i++)
            {
                if (userFileLock == i)
                {

                    int totalFrames = 90;
                    double startValue = 0.0;
                    double endValue = 30;
                    double v = 0;


                    if (frameC < totalFrames)
                    {
                        double t = frameC / (double)totalFrames;
                        v = startValue + (endValue - startValue) * ease::easeOutBack(t, 4);
                        frameC++;
                    }
                    else
                    {
                        v = endValue;
                    }
                    setcolor(EGERGB(155, 255, 200));
                    ege_roundrect(w / 2 + 45 - v / 2, h / 7 * (i + 1) + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 40 + v, screen.x / 90);
                }
            }
        }
        if (type == 8)
        {


            int totalFrames = 90;
            double startValue = 0.0;
            double endValue = 150;
            double v = 0;


            if (frameD < totalFrames)
            {
                double t = frameD / (double)totalFrames;
                v = startValue + (endValue - startValue) * ease::easeOutBack(t, 4);
                frameD++;
            }
            else
            {
                v = endValue;
            }

            if (DarkMode)
            {
                setcolor(EGERGBA(20, 20, 20, 150));
                setfillcolor(EGERGBA(20, 20, 20, 150));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, 150));
                setfillcolor(EGERGBA(240, 240, 240, 150));
            }
            DrawManager::fillroundrectwithrect(w - 470 + v / 2, h - 150 + v * 2 / 7, v * 2, v * 4 / 7, w / 100);

            if (!DarkMode)
            {
                setcolor(EGERGB(20, 20, 20));
            }
            else
            {
                setcolor(EGERGB(240, 240, 240));
            }

            if (userLg != 3)setfont(v / 3, 0, "Aa吧唧一口小可耐");
            else setfont(v / 3, 0, "Ownglyph Soomini Regular");
            if (userLg == 0) xyprintf(w - 370 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"确定");
            else if (userLg == 1) xyprintf(w - 370 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"OK");
            else if (userLg == 2) xyprintf(w - 370 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"確認");
            else if (userLg == 3) xyprintf(w - 370 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"확인");
            else if (userLg == 4) xyprintf(w - 370 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Confirmer");
            else if (userLg == 5) xyprintf(w - 370 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Bestätigen");
            else if (userLg == 6) xyprintf(w - 370 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Подтвердить");
            else if (userLg == 7) xyprintf(w - 370 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Confirmar");
        }

        if (type == 9)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0.4;
            const double endScale02 = 0.25;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 2 - int(logoW * startScale02) / 2;
            int endX = w / 5 - w / 16 - int(logoW * endScale02) / 2;
            int startY = h / 2 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - h / 6 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeInOut(t, 5);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame2++;
            }
            else {
                scale = endScale02;
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[2], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 10)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0;
            const double endScale02 = 0.43;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 5 + w / 10 - int(logoW * startScale02) / 2;
            int endX = w / 5 + w / 25 - int(logoW * endScale02) / 2;
            int startY = h / 2 - h / 10 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - h / 15 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeOutBack(t, 6);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame2++;
            }
            else {
                scale = endScale02;
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[4], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 11)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0;
            const double endScale02 = 0.43;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 5 - w / 10 - int(logoW * startScale02) / 2;
            int endX = w / 5 - w / 25 - int(logoW * endScale02) / 2;
            int startY = h / 2 + h / 10 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 + h / 15 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeOutBack(t, 6);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame2++;
            }
            else {
                scale = endScale02;
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[3], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 12)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0.25;
            const double endScale02 = 0.15;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 5 - w / 16 - int(logoW * startScale02) / 2;
            int endX = w / 5 - w / 16 - int(logoW * endScale02) / 2;
            int startY = h / 2 - h / 6 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - h / 10 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeInOutBack(t, 5);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame2++;
            }
            else {
                scale = endScale02;
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[2], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 13)
        {

            wstring ys[8] = {
                L"设置您的专有密码",
                L"Set your proprietary password",
                L"あなたの専有パスワードを設定します",
                L"당신의 전용 비밀번호를 설정하세요",
                L"Définissez votre mot de passe propriétaire",
                L"Legen Sie Ihr proprietäres Passwort fest",
                L"Установите свой собственный пароль",
                L"Establece tu contraseña de propiedad"
            };
            //wstring PK[7] = { L"NONE",L"XOR - min",L"SPX<NetTrack> - min",L"FNX - standard",L"FNXS<NetTrack> - standard",L"QRMES<NetTrack> - max",L"CTMHES<NetTrack - danger> - max" };
            if (DarkMode)
            {
                setcolor(EGERGBA(90, 90, 90, 200));
                setfillcolor(EGERGBA(70, 70, 70, 150));
            }
            else
            {
                setcolor(EGERGBA(220, 220, 220, 200));
                setfillcolor(EGERGBA(240, 240, 240, 150));
            }


            int totalFrames = 120;
            double startValue = 0.0;
            double endValue = 30.0;
            static int frame2 = 0;
            double v = 0;
            if (frame2 < totalFrames)
            {
                double t = frame2 / (double)totalFrames;
                v = startValue + (endValue - startValue) * ease::easeOutBack(t, 6);
                frame2++;
            }
            else
            {
                v = endValue;
            }

            setlinewidth(2);
            DrawManager::fillroundrectwithrect(w / 2 + 45 - v / 2, h / 7 * 1 + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 40 + v, screen.x / 90);
            ege_roundrect(w / 2 + 45 - v / 2, h / 7 * 1 + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 40 + v, screen.x / 90);

            DrawManager::fillroundrectwithrect(w / 2 + 45 - v / 2, h / 7 * 2 + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 40 + v, screen.x / 90);
            ege_roundrect(w / 2 + 45 - v / 2, h / 7 * 2 + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 40 + v, screen.x / 90);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            setfont(v * 3, 0, "Aa吧唧一口小可耐");

            xyprintf(w / 2 + 45 - v / 2 + (w / 2 - 90 + v) / 2, h / 7 * 2 + whle / 5 - h / 20 + 15 - v / 2 + (h / 9 - 40 + v) / 2, "__  __  __  __  __  __");

            if (DarkMode)setcolor(EGERGB(255, 255, 255));
            else setcolor(EGERGB(20, 20, 20));
            for (int i = 0; i < 6; i++)
            {
                if (userLock[i] <= -1)
                {
                    int totalFrames1 = 60;
                    double startValue1 = 50.0;
                    double endValue1 = 0.0;
                    int v22 = 0;

                    if (userLock_fontFrame[i] < totalFrames1)
                    {
                        double t = userLock_fontFrame[i] / (double)totalFrames1;
                        v22 = startValue1 + (endValue1 - startValue1) * ease::easeInBack(t, 5);

                        setfont(v22, 0, "Aa吧唧一口小可耐");
                        string a = to_string(userLock_temp[i]);
                        xyprintf(w / 2 + w / 31 * 2 - v / 2 + (w / 2 - 90 + v) / 2 + w / 31 * 2 * (i - 3), h / 7 * 2 + whle / 5 - h / 20 + 15 - v / 2 + (h / 9 - 40 + v) / 2 - h / 60, a.c_str());

                        userLock_fontFrame[i]++;
                    }
                    else
                    {
                        userLock_temp[i] = -1;
                    }
                }
                else
                {
                    int totalFrames1 = 60;
                    double startValue1 = 0.0;
                    double endValue1 = 50.0;
                    int v22 = 0;

                    if (userLock_fontFrame[i] < totalFrames1)
                    {
                        double t = userLock_fontFrame[i] / (double)totalFrames1;
                        v22 = startValue1 + (endValue1 - startValue1) * ease::easeOutBack(t, 5);

                        setfont(v22, 0, "Aa吧唧一口小可耐");
                        string a = to_string(userLock_temp[i]);
                        xyprintf(w / 2 + w / 31 * 2 - v / 2 + (w / 2 - 90 + v) / 2 + w / 31 * 2 * (i - 3), h / 7 * 2 + whle / 5 - h / 20 + 15 - v / 2 + (h / 9 - 40 + v) / 2 - h / 60, a.c_str());
                        userLock_fontFrame[i]++;
                    }
                    else
                    {
                        v22 = endValue1;

                        setfont(v22, 0, "Aa吧唧一口小可耐");
                        string a = to_string(userLock_temp[i]);
                        xyprintf(w / 2 + w / 31 * 2 - v / 2 + (w / 2 - 90 + v) / 2 + w / 31 * 2 * (i - 3), h / 7 * 2 + whle / 5 - h / 20 + 15 - v / 2 + (h / 9 - 40 + v) / 2 - h / 60, a.c_str());
                    }
                }
            }

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (DarkMode)
                    {
                        setcolor(EGERGBA(90, 90, 90, 200));
                        setfillcolor(EGERGBA(70, 70, 70, 150));
                    }
                    else
                    {
                        setcolor(EGERGBA(220, 220, 220, 200));
                        setfillcolor(EGERGBA(240, 240, 240, 150));
                    }



                    DrawManager::fillroundrectwithrect(w / 2 + w / 10 + 10 + j * 120, h / 2 - h / 8 + 10 + i * 120, 70 + v, 70 + v, w / 100);
                    ege_roundrect(w / 2 + w / 10 + 10 + j * 120, h / 2 - h / 8 + 10 + i * 120, 70 + v, 70 + v, w / 100);

                    setfont(v * 3 / 2, 0, "Aa吧唧一口小可耐");
                    int s = 1 + j + i * 3;
                    string m = to_string(s);
                    if (DarkMode)setcolor(EGERGB(240, 240, 240));
                    else setcolor(EGERGB(20, 20, 20));
                    xyprintf(w / 2 + w / 10 + 10 + j * 120 + 50, h / 2 - h / 8 + 10 + i * 120 + 50, "%s", m);
                }
            }

            if (DarkMode)
            {
                setcolor(EGERGBA(90, 90, 90, 200));
                setfillcolor(EGERGBA(70, 70, 70, 150));
            }
            else
            {
                setcolor(EGERGBA(220, 220, 220, 200));
                setfillcolor(EGERGBA(240, 240, 240, 150));
            }
            DrawManager::fillroundrectwithrect(w / 2 + w / 10 + 10 + 3 * 120, h / 2 - h / 8 + 10 + 0 * 120, 70 + v, 190 + v, w / 100);
            ege_roundrect(w / 2 + w / 10 + 10 + 3 * 120, h / 2 - h / 8 + 10 + 0 * 120, 70 + v, 190 + v, w / 100);
            DrawManager::fillroundrectwithrect(w / 2 + w / 10 + 10 + 3 * 120, h / 2 - h / 8 + 10 + 2 * 120, 70 + v, 70 + v, w / 100);
            ege_roundrect(w / 2 + w / 10 + 10 + 3 * 120, h / 2 - h / 8 + 10 + 2 * 120, 70 + v, 70 + v, w / 100);

            setfont(v * 3 / 2, 0, "Aa吧唧一口小可耐");
            if (DarkMode)setcolor(EGERGB(240, 240, 240));
            else setcolor(EGERGB(20, 20, 20));
            xyprintf(w / 2 + w / 10 + 10 + 3 * 120 + 50, h / 2 - h / 8 + 10 + 2 * 120 + 50, "0");
            setfont(v * 3, 0, "Aa吧唧一口小可耐");
            if (DarkMode)setcolor(EGERGB(240, 140, 140));
            else setcolor(EGERGB(120, 20, 20));
            xyprintf(w / 2 + w / 10 + 10 + 3 * 120 + 50, h / 2 - h / 8 + 10 + 0 * 120 + 110, "X");

            if (DarkMode)setcolor(EGERGB(240, 240, 240));
            else setcolor(EGERGB(20, 20, 20));
            if (userLg != 3)
            {
                setfont(v * 3 / 2, 0, "Aa吧唧一口小可耐");
            }
            else
            {
                setfont(v * 3 / 2, 0, "Ownglyph Soomini Regular");
            }
            xyprintf(w * 3 / 4, h / 7 * 1 + whle / 5, ys[userLg].c_str());
            frame2++;
        }
        if (type == 14)
        {


            int totalFrames = 90;
            double startValue = 0.0;
            double endValue = 150;
            double v = 0;
            static int frame001 = 0;

            if (frame001 < totalFrames)
            {
                double t = frame001 / (double)totalFrames;
                v = startValue + (endValue - startValue) * ease::easeOutBack(t, 4);
                frame001++;
            }
            else
            {
                v = endValue;
            }

            if (DarkMode)
            {
                setcolor(EGERGBA(20, 20, 20, 150));
                setfillcolor(EGERGBA(20, 20, 20, 150));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, 150));
                setfillcolor(EGERGBA(240, 240, 240, 150));
            }
            DrawManager::fillroundrectwithrect(w - 420 + v / 2, h - 150 + v * 2 / 7, v * 2, v * 4 / 7, w / 100);
            DrawManager::fillroundrectwithrect(w - 770 + v / 2, h - 150 + v * 2 / 7, v * 2, v * 4 / 7, w / 100);
            if (!DarkMode)
            {
                setcolor(EGERGB(20, 20, 20));
            }
            else
            {
                setcolor(EGERGB(240, 240, 240));
            }

            if (userLg != 3)setfont(v / 3, 0, "Aa吧唧一口小可耐");
            else setfont(v / 3, 0, "Ownglyph Soomini Regular");

            settextjustify(CENTER_TEXT, CENTER_TEXT);
            if (userLg == 0)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"确定");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"跳过");
            }
            else if (userLg == 1)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"OK");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Skip");
            }
            else if (userLg == 2)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"確認");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"飛び越える");
            }
            else if (userLg == 3)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"확인");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"건너뛰어요");
            }
            else if (userLg == 4)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Confirmer");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Ignorez-le");
            }
            else if (userLg == 5)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Bestätigen");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Überspringen");
            }
            else if (userLg == 6)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Подтвердить");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Пропустить это");
            }
            else if (userLg == 7)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Confirmar");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Paso");
            }
        }
        if (type == 15)
        {

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (cgos == 1 + j + i * 3)
                    {
                        int totalFrame = 35;
                        double startValue = 0.0;
                        double endValue = 255;
                        int v = 0;
                        if (cgos_frame < totalFrame)
                        {
                            double t = cgos_frame / (double)totalFrame;
                            v = startValue + (endValue - startValue) * ease::easeOutBack(t, 6);

                            setcolor(EGERGBA(155, 255, 200, v));
                            ege_roundrect(w / 2 + w / 10 + 10 + j * 120 + 15 - v * 15 / 255, h / 2 - h / 8 + 10 + i * 120 + 15 - v * 15 / 255, 70 + v * 30 / 255, 70 + v * 30 / 255, w / 100);

                            cgos_frame++;
                        }
                    }
                }
            }

            if (cgos == 0)
            {
                int totalFrame = 35;
                double startValue = 0.0;
                double endValue = 255;
                int v = 0;
                if (cgos_frame < totalFrame)
                {
                    double t = cgos_frame / (double)totalFrame;
                    v = startValue + (endValue - startValue) * ease::easeOutBack(t, 6);

                    setcolor(EGERGBA(155, 255, 200, v));
                    ege_roundrect(w / 2 + w / 10 + 10 + 3 * 120 + 15 - v * 15 / 255, h / 2 - h / 8 + 10 + 2 * 120 + 15 - v * 15 / 255, 70 + v * 30 / 255, 70 + v * 30 / 255, w / 100);

                    cgos_frame++;
                }
            }

            if (cgos == 100)
            {
                int totalFrame = 35;
                double startValue = 0.0;
                double endValue = 255;
                int v = 0;
                if (cgos_frame < totalFrame)
                {
                    double t = cgos_frame / (double)totalFrame;
                    v = startValue + (endValue - startValue) * ease::easeOutBack(t, 6);

                    setcolor(EGERGBA(255, 150, 150, v));
                    ege_roundrect(w / 2 + w / 10 + 10 + 3 * 120 + 15 - v * 15 / 255, h / 2 - h / 8 + 10 + 0 * 120 + 15 - v * 15 / 255, 70 + v * 30 / 255, 190 + v * 30 / 255, w / 100);

                    cgos_frame++;
                }
            }
        }

        if (type == 16)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 60; // 动画帧数（1.5秒）
            const double startScale02 = 0.15;
            const double endScale02 = 0;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 5 - w / 16 - int(logoW * startScale02) / 2;
            int endX = w / 5 - w / 16 - int(logoW * endScale02) / 2;
            int startY = h / 2 - h / 10 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - h / 6 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeInBack(t, 1.5);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);

                int drawW = static_cast<int>(logoW * scale);
                int drawH = static_cast<int>(logoH * scale);

                // 临时缩放，不破坏原图
                PIMAGE tempLogo = newimage(drawW, drawH);
                putimage(tempLogo, 0, 0, drawW, drawH, logoImage[2], 0, 0, logoW, logoH);
                putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
                delimage(tempLogo);

                animFrame2++;
            }
        }
        if (type == 17)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0.43;
            const double endScale02 = 0.6;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 5 + w / 25 - int(logoW * startScale02) / 2;
            int endX = w / 2 + w / 20 - int(logoW * endScale02) / 2;
            int startY = h / 2 - h / 15 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - h / 5 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeInOut(t, 7);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame2++;
            }
            else {
                scale = endScale02;
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[4], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 18)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
            const double startScale02 = 0.43;
            const double endScale02 = 0.6;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 5 - w / 25 - int(logoW * startScale02) / 2;
            int endX = w / 2 - w / 20 - int(logoW * endScale02) / 2;
            int startY = h / 2 + h / 15 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - h / 10 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeInOut(t, 7);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame2++;
            }
            else {
                scale = endScale02;
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[3], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 19)
        {
            wstring a[8] =
            {
                L"让我们来建立连接",
                L"Let's build a connection",
                L"接続を作りましょう",
                L"우리 연결을 구축해요",
                L"Établissons un lien",
                L"Lassen Sie uns eine Verbindung aufbauen",
                L"Давайте установим связь",
                L"Construyamos una conexión"
            };



            int totalFrames = 60;
            double startValue = 0.0;
            double endValue = 50;
            double v = 0;
            static int frame001 = 0;

            if (frame001 < totalFrames)
            {
                double t = frame001 / (double)totalFrames;
                v = startValue + (endValue - startValue) * ease::easeOutBack(t, 4);
                frame001++;
            }
            else
            {
                v = endValue;
            }

            if (userLg != 3) setfont(v * 6 / 5, 0, "Aa吧唧一口小可耐");
            else setfont(v * 6 / 5, 0, "Ownglyph Soomini Regular");

            if (DarkMode)setcolor(WHITE);
            else setcolor(EGERGB(20, 20, 20));
            xyprintf(w / 2, h - h / 5 - v * 2, a[userLg].c_str());

            setcolor(EGERGB(150, 150, 150));
            setfont(v * 9 / 5, 0, "Aa吧唧一口小可耐");
            xyprintf(w / 2, h - h / 6 - v, "__  __  __  __  __  __  __  __");

            v = v * 3;

            if (DarkMode)
            {
                setcolor(EGERGBA(20, 20, 20, 150));
                setfillcolor(EGERGBA(20, 20, 20, 150));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, 150));
                setfillcolor(EGERGBA(240, 240, 240, 150));
            }
            DrawManager::fillroundrectwithrect(w - 420 + v / 2, h - 150 + v * 2 / 7, v * 2, v * 4 / 7, w / 100);
            DrawManager::fillroundrectwithrect(w - 770 + v / 2, h - 150 + v * 2 / 7, v * 2, v * 4 / 7, w / 100);
            if (!DarkMode)
            {
                setcolor(EGERGB(20, 20, 20));
            }
            else
            {
                setcolor(EGERGB(240, 240, 240));
            }

            if (userLg != 3)setfont(v / 3, 0, "Aa吧唧一口小可耐");
            else setfont(v / 3, 0, "Ownglyph Soomini Regular");

            settextjustify(CENTER_TEXT, CENTER_TEXT);
            if (userLg == 0)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"确定");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"跳过");
            }
            else if (userLg == 1)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"OK");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Skip");
            }
            else if (userLg == 2)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"確認");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"飛び越える");
            }
            else if (userLg == 3)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"확인");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"건너뛰어요");
            }
            else if (userLg == 4)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Confirmer");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Ignorez-le");
            }
            else if (userLg == 5)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Bestätigen");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Überspringen");
            }
            else if (userLg == 6)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Подтвердить");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Пропустить это");
            }
            else if (userLg == 7)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Confirmar");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Paso");
            }
        }
        if (type == 20)
        {


            if (DarkMode)setcolor(EGERGB(255, 255, 255));
            else setcolor(EGERGB(20, 20, 20));
            int v = 30;
            for (int i = 0; i < 8; i++)
            {
                if (linkCode[i] == '?')
                {
                    int totalFrames1 = 60;
                    double startValue1 = 55.0;
                    double endValue1 = 0.0;
                    int v22 = 0;
                    if (userLink_fontFrame[i] < totalFrames1)
                    {
                        double t = userLink_fontFrame[i] / (double)totalFrames1;
                        v22 = startValue1 + (endValue1 - startValue1) * ease::easeInBack(t, 5);

                        setfont(v22, 0, "Aa吧唧一口小可耐");

                        xyprintf(w / 2 + w / 29 + w * 2 / 30 * (i - 4), h / 2 + h / 4, "%c", linkCode_temp[i]);

                        userLink_fontFrame[i]++;
                    }
                    else
                    {
                        linkCode[i] = '?';
                    }
                }
                else
                {
                    int totalFrames1 = 60;
                    double startValue1 = 0.0;
                    double endValue1 = 55.0;
                    int v22 = 0;

                    if (userLink_fontFrame[i] < totalFrames1)
                    {
                        double t = userLink_fontFrame[i] / (double)totalFrames1;
                        v22 = startValue1 + (endValue1 - startValue1) * ease::easeOutBack(t, 5);

                        setfont(v22, 0, "Aa吧唧一口小可耐");

                        xyprintf(w / 2 + w / 29 + w * 2 / 30 * (i - 4), h / 2 + h / 4, "%c", linkCode[i]);
                        userLink_fontFrame[i]++;
                    }
                    else
                    {
                        v22 = endValue1;

                        setfont(v22, 0, "Aa吧唧一口小可耐");
                        string a = to_string(userLock_temp[i]);
                        xyprintf(w / 2 + w / 29 + w * 2 / 30 * (i - 4), h / 2 + h / 4, "%c", linkCode[i]);
                    }
                }
            }
        }

        if (type == 21)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 60; // 动画帧数（1.5秒）
            const double startScale02 = 0.6;
            const double endScale02 = 0;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 2 + w / 20 - int(logoW * startScale02) / 2;
            int endX = w / 2 - int(logoW * endScale02) / 2;
            int startY = h / 2 - h / 5 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeInBack(t, 4);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame2++;
            }
            else {
                scale = endScale02;
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[4], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 22)
        {

            static int animFrame2 = 0;
            const int animTotalFrames2 = 60; // 动画帧数（1.5秒）
            const double startScale02 = 0.6;
            const double endScale02 = 0;

            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int startX = w / 2 - w / 20 - int(logoW * startScale02) / 2;
            int endX = w / 2 - int(logoW * endScale02) / 2;
            int startY = h / 2 - h / 10 - int(logoH * startScale02) / 2; // 居中
            int endY = h / 2 - int(logoH * endScale02) / 2; // 居中

            double scale = endScale02;
            int drawX = endX, drawY = endY;
            if (animFrame2 < animTotalFrames2) {
                double t = animFrame2 / (double)(animTotalFrames2 - 1);
                double easeT = ease::easeInBack(t, 4);
                scale = startScale02 + (endScale02 - startScale02) * easeT;
                drawX = startX + int((endX - startX) * easeT);
                drawY = startY + int((endY - startY) * easeT);
                animFrame2++;
            }
            else {
                scale = endScale02;
                drawX = endX;
                drawY = endY;
            }

            int drawW = static_cast<int>(logoW * scale);
            int drawH = static_cast<int>(logoH * scale);

            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[3], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
            delimage(tempLogo);
        }
        if (type == 23)
        {
            wstring a[8] =
            {
                L"让我们来建立连接",
                L"Let's build a connection",
                L"接続を作りましょう",
                L"우리 연결을 구축해요",
                L"Établissons un lien",
                L"Lassen Sie uns eine Verbindung aufbauen",
                L"Давайте установим связь",
                L"Construyamos una conexión"
            };



            int totalFrames = 60;
            double startValue = 50.0;
            double endValue = 0;
            double v = 0;
            static int frame001 = 0;

            if (frame001 < totalFrames)
            {
                double t = frame001 / (double)totalFrames;
                v = startValue + (endValue - startValue) * ease::easeInBack(t, 4);
                frame001++;
            }
            else
            {
                v = endValue;
            }

            if (userLg != 3) setfont(v * 6 / 5, 0, "Aa吧唧一口小可耐");
            else setfont(v * 6 / 5, 0, "Ownglyph Soomini Regular");

            if (DarkMode)setcolor(WHITE);
            else setcolor(EGERGB(20, 20, 20));
            xyprintf(w / 2, h - h / 5 - v * 2, a[userLg].c_str());

            setcolor(EGERGB(150, 150, 150));
            setfont(v * 9 / 5, 0, "Aa吧唧一口小可耐");
            xyprintf(w / 2, h - h / 6 - v, "__  __  __  __  __  __  __  __");

            v = v * 3;

            if (DarkMode)
            {
                setcolor(EGERGBA(20, 20, 20, 150));
                setfillcolor(EGERGBA(20, 20, 20, 150));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, 150));
                setfillcolor(EGERGBA(240, 240, 240, 150));
            }
            DrawManager::fillroundrectwithrect(w - 420 + v / 2, h - 150 + v * 2 / 7, v * 2, v * 4 / 7, w / 100);
            DrawManager::fillroundrectwithrect(w - 770 + v / 2, h - 150 + v * 2 / 7, v * 2, v * 4 / 7, w / 100);
            if (!DarkMode)
            {
                setcolor(EGERGB(20, 20, 20));
            }
            else
            {
                setcolor(EGERGB(240, 240, 240));
            }

            if (userLg != 3)setfont(v / 3, 0, "Aa吧唧一口小可耐");
            else setfont(v / 3, 0, "Ownglyph Soomini Regular");

            settextjustify(CENTER_TEXT, CENTER_TEXT);
            if (userLg == 0)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"确定");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"跳过");
            }
            else if (userLg == 1)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"OK");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Skip");
            }
            else if (userLg == 2)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"確認");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"飛び越える");
            }
            else if (userLg == 3)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"확인");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"건너뛰어요");
            }
            else if (userLg == 4)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Confirmer");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Ignorez-le");
            }
            else if (userLg == 5)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Bestätigen");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Überspringen");
            }
            else if (userLg == 6)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Подтвердить");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Пропустить это");
            }
            else if (userLg == 7)
            {
                xyprintf(w - 320 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Confirmar");
                xyprintf(w - 670 + v / 2 + v / 4, h - 150 + v * 2 / 7 + v * 2 / 7, L"Paso");
            }
        }
        if (type == 24)
        {
            wstring a[8] =
            {
            L"请稍等，马上就好啦！",
            L"Please wait, it'll be ready soon!",
            L"少々お待ちください、すぐに完了します！",
            L"잠시만 기다려주세요, 금방 준비될 거예요!",
            L"Veuillez patienter, ce sera bientôt prêt !",
            L"Bitte warten, es wird gleich fertig sein!",
            L"Пожалуйста, подождите, скоро будет готово!",
            L"¡Por favor espere, estará listo enseguida!"
            };



            int totalFrames = 60;
            double startValue = 0.0;
            double endValue = 50;
            double v = 0;
            static int frame001 = 0;

            if (frame001 < totalFrames)
            {
                double t = frame001 / (double)totalFrames;
                v = startValue + (endValue - startValue) * ease::easeOutBack(t, 4);
                frame001++;
            }
            else
            {
                v = endValue;
            }

            if (userLg != 3) setfont(v * 6 / 5, 0, "Aa吧唧一口小可耐");
            else setfont(v * 7 / 5, 0, "Ownglyph Soomini Regular");

            if (DarkMode)setcolor(WHITE);
            else setcolor(EGERGB(20, 20, 20));
            xyprintf(w / 2, h / 2 + v * 4, a[userLg].c_str());
        }
        if (type == 25)
        {


            int totalFrames = 60;
            double startValue = 0.0;
            double endValue = 75;
            double v = 0;
            static int frame001 = 0;

            if (frame001 < totalFrames)
            {
                double t = frame001 / (double)totalFrames;
                v = startValue + (endValue - startValue) * ease::easeOutBack(t, 4);
                frame001++;
            }
            else
            {
                v = endValue;
            }

            if (loadAS == true)
            {
                if (loadA < 270)
                {
                    loadA += 2;
                }
                else
                {
                    loadA = 270;
                    loadAS = false;
                }
            }
            else
            {
                if (loadA > 10)
                {
                    loadA -= 2;
                }
                else
                {
                    loadA = 10;
                    loadAS = true;
                }
            }

            roundA += 8;

            if (roundA > 720) roundA = 0;


            setlinecap(LINECAP_ROUND, LINECAP_ROUND);
            setlinewidth(w / 35);

            setcolor(EGERGB(10, 10, 10));
            arc(w / 2, h / 2 - h / 8 + h / 48, roundA, roundA + loadA, v * 2);

            setcolor(EGERGB(155, 255, 200));
            arc(w / 2, h / 2 - h / 8, roundA, roundA + loadA, v * 2);
        }
    }

    bool first::firststartA;

    void first::backOut()
    {
        PIMAGE img = newimage();

        POINT sz = Window::GetWindowSize();

        bool success = false;
        bool dkmode = Window::IsDarkModeEnabled();
        if (0 == getimage(img, 0, 0, sz.x, sz.y))
        {
            success = true;
        }

        MSG msg = {};

        for (int i = 0; i < 256; i++)
        {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            delay_fps(10000);
            cleardevice();

            if (success)
            {
                putimage(0, 0, img);
            }
            if (dkmode) setfillcolor(EGERGBA(0, 0, 0, i));
            else setfillcolor(EGERGBA(240, 240, 240, i));

            ege_fillrect(0, 0, sz.x, sz.y);
        }

        exit(0);
    }

    void first::drawTriangle(POINT pos, int size, COLORREF color, float angle) {
        // 计算三角形三个顶点的坐标
        POINT pts[3];

        // 等边三角形的三个顶点（初始方向：顶点朝上）
        for (int i = 0; i < 3; ++i) {
            float theta = angle + i * 2 * PI / 3;  // 每120度一个顶点
            pts[i].x = pos.x + size * cos(theta);
            pts[i].y = pos.y + size * sin(theta);
        }

        int OS[6] = { pts[0].x, pts[0].y, pts[1].x, pts[1].y, pts[2].x, pts[2].y };
        // 设置填充颜色
        setfillcolor(color);
        // 绘制填充三角形
        fillpoly(3, OS);
    }

    PIMAGE OHS = newimage();

    void first::roundF()
    {
        for (int i = 0; i < max_struct; i++)
        {
            if (round_struct[i].maxSize == 0)
            {
                randomize();

                round_struct[i].maxSize = random(200);
                round_struct[i].round.x = random(Window::GetWindowSize().x);
                round_struct[i].round.y = random(Window::GetWindowSize().y);
                round_struct[i].other = random(2);
            }
            else
            {
                if (!round_struct[i].KO)
                {
                    if (round_struct[i].size < round_struct[i].maxSize)
                    {
                        round_struct[i].size += 1;
                    }
                    else
                    {
                        round_struct[i].KO = true;
                    }
                }
                else
                {
                    if (round_struct[i].size > 0)
                    {
                        round_struct[i].size -= 1;
                    }
                    else
                    {
                        round_struct[i].KO = false;

                        round_struct[i].maxSize = 0;
                    }
                }
            }

            setfillcolor(EGERGB(155, 255, 200));
            if (!round_struct[i].other)solidcircle(round_struct[i].round.x, round_struct[i].round.y, round_struct[i].size);
            else drawTriangle(round_struct[i].round, round_struct[i].size, EGERGB(155, 255, 250), 5);
        }

        
        getimage(OHS, 0, 0, Window::GetWindowSize().x, Window::GetWindowSize().y);


        int samal = 20;

        if (samal > 0)
        {
            img::BlurWithDownscale_Gaussian(OHS, samal, samal, 3);
        }

        putimage(0, 0, OHS);
    }


int First::firstStart()
{
    setbkmode(TRANSPARENT);

    if (-1 == FileControl::UnpackFilesFromOne("ofstartimage.ntk", ""))
    {
        MessageBoxA(NULL, "code：M_OFSTARTIMAGENTK", "NetTrackError", MB_OK + MB_SYSTEMMODAL + 16);
        return -1;
    }
    {
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
    }
    int imageX, imageY;
    imageX = getwidth(backImage);
    imageY = getheight(backImage);


    POINT ws = Window::GetWindowSize();
    if (imageY * ws.x / imageX < ws.y)
    {
        img::ZoomImage_Alpha(backImage, imageX * ws.y / imageY, ws.y);
    }
    else
    {
        img::ZoomImage_Alpha(backImage, ws.x, imageY * ws.x / imageX);
    }


    img::AcrylicGaussianBlur(backImage, 15, 15);


    string imageName[5] = { "world.png", "file.png", "lock.png", "conectA.png", "conectB.png" };
    for (int i = 0; i < 5; i++)
    {
        logoImage[i] = newimage();
        getimage(logoImage[i], imageName[i].c_str(), 0, 0);
        remove(imageName[i].c_str());
    }
    int LG = 0;
    POINT RG = Window::GetWindowSize();
    wstring zh[3] = { L"你好", L"欢迎", L"单击开始" };
    wstring en[3] = { L"Hello", L"Welcome", L"Click to Start" };
    wstring jp[3] = { L"こんにちは", L"ようこそ", L"開始をクリック" };
    wstring kr[3] = { L"안녕하세요", L"환영합니다", L"시작을 클릭하세요" };
    wstring fr[3] = { L"Bonjour", L"Bienvenue", L"Cliquez pour démarrer" };
    wstring de[3] = { L"Hallo", L"Willkommen", L"Klicken zum Starten" };
    wstring ru[3] = { L"Привет", L"Добро пожаловать", L"Нажмите, чтобы начать" };
    wstring es[3] = { L"Hola", L"Bienvenido", L"Haga clic para comenzar" };

    int w = RG.x;
    int h = RG.y;

    settextjustify(CENTER_TEXT, CENTER_TEXT);

    wstring* langs[] = { zh, en, jp, kr, fr, de, ru, es };
    int langCount = sizeof(langs) / sizeof(langs[0]);
    MSG msg;
    bool ISEXIT = false;
    setbkcolor(EGERGB(20, 20, 20));

    showwindow();

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



    while (is_run())
    {
        randomize();
        int cx = random(2);
        wstring* curLang = langs[LG % langCount];
        setfont(w / 20, 0, "Aa吧唧一口小可耐");

        setbkcolor(EGERGB(20, 20, 20));
        // 渐变淡入
        for (int j = 0; j < 50; j++)
        {
            delay_fps(300);
            cleardevice();

            first::roundF();

            setcolor(WHITE);
            if (LG == 3) setfont(w / 20, 0, "Ownglyph Soomini Regular");

            xyprintf(w / 2, h / 2 - 80, L"%s", curLang[cx].c_str());
            xyprintf(w / 2, h / 2 + 10, L"%s", curLang[2].c_str());


            putimage_alphablend(NULL,OHS,0,0, 255 - j * 255 / 50);


            if (LG == 3) setfont(w / 20, 0, "Aa吧唧一口小可耐");

            // 处理消息队列
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            if (keystate(0x01)) ISEXIT = true;
        }
        // 停留一段时间
        int stay = 90;
        for (int t = 0; t < stay; t++)
        {
            delay_fps(120);
            cleardevice();

            first::roundF();

            setcolor(WHITE);
            if (LG == 3) setfont(w / 20, 0, "Ownglyph Soomini Regular");
            //putimage_alphablend(NULL, backImage, 0, 0, 150);
            xyprintf(w / 2, h / 2 - 80, L"%s", curLang[cx].c_str());
            xyprintf(w / 2, h / 2 + 10, L"%s", curLang[2].c_str());
            if (LG == 3) setfont(w / 20, 0, "Aa吧唧一口小可耐");

            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            if (keystate(0x01) || ISEXIT || !is_run())
            {
                ISEXIT = true;
                break;
            }
        }
        // 渐变淡出
        for (int j = 0; j < 50; j++)
        {
            delay_fps(300);
            cleardevice();

            first::roundF();

            setcolor(WHITE);
            if (LG == 3) setfont(w / 20, 0, "Ownglyph Soomini Regular");
            //putimage_alphablend(NULL, backImage, 0, 0, 150);
            xyprintf(w / 2, h / 2 - 80, L"%s", curLang[cx].c_str());
            xyprintf(w / 2, h / 2 + 10, L"%s", curLang[2].c_str());
            if (LG == 3) setfont(w / 20, 0, "Aa吧唧一口小可耐");
            
            putimage_alphablend(NULL, OHS, 0, 0, j * 255 / 50);

            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            if (keystate(0x01)) ISEXIT = true;
        }
        if (ISEXIT) break;
        LG++;
        if (LG >= langCount) LG = 0;
    }

    if (!is_run())
    {
        first::backOut();
    }

    wstring ys[8] = { L"中文",L"English",L"日本語",L"한국어",L"Chiffrement des fichiers",L"Dateiverschlüsselung",L"Шифрование файлов",L"Cifrado de archivos" };

    mouse_msg mouseMsg;
    PIMAGE OG = newimage();


    if (DarkMode)setbkcolor(EGERGB(40, 40, 40));
    else setbkcolor(EGERGB(240, 240, 240));

    int BK1 = 255;
    int logo_world_size = 10;
    int chooseLG = -1;

    bool lgs = false;

    bool fontRectChange = false;

    static int frame2 = 0;
    int frame3 = 0;
    int frame4 = 80;

    int FontFrame[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int FontTotalFrame = 100;

    int clockStart = 0;

    bool EXIT2 = 0;
    bool temp_C = false;
    while (is_run())
    {
        delay_fps(60);
        cleardevice();
        if (clockStart < 100) clockStart += 1;
        //KEY
        {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (keystate(0x01))
            {
                POINT mousePos = Window::GetMousePosX();
                POINT WindowSize = Window::GetWindowSize();
                //MessageBoxA(NULL, "code:0x01", "Debug", MB_OK + MB_SYSTEMMODAL + 48);
                for (int i = 0; i < 8; i++)
                {
                    if (mousePos.x > w / 2 + 30 && mousePos.x < w - 30 && mousePos.y < WindowSize.y - 110 && mousePos.y > 50)
                    {
                        if (mousePos.y > h / 7 * (i + 1) + whle / 5 - h / 20 && mousePos.y < h / 7 * (i + 1) + whle / 5 - h / 20 + h / 9)
                        {
                            if (chooseLG != i)
                            {
                                chooseLG = i;
                                fontRectChange = true;
                                frame2 = 0;
                                if (temp_C) frame4 = 0;
                                else temp_C = true;
                            }
                        }
                    }
                }

                if (chooseLG > -1)
                {
                    if (mousePos.x > w - 470 + 150 / 2 && mousePos.x < w - 470 + 150 / 2 + 300 && mousePos.y > h - 150 + 150 * 2 / 7 && mousePos.y < h - 150 + 150 * 2 / 7 + 150 * 4 / 7)
                    {
                        userLg = chooseLG;
                        EXIT2 = true;
                    }
                }
            }



            if (mousemsg())
            {
                mouseMsg = getmouse();
                if (mouseMsg.is_wheel())   whle_temp = mouseMsg.wheel / 2;
            }
        }

        //MATH
        {
            if (whle_temp != 0)
            {
                whle += whle_temp;
                if (whle_temp > 0)whle_temp -= 2;
                else whle_temp += 2;
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
                const int min = 1500;
                if (whle < -min)
                {
                    if (whle < -4500 - min) whle = -4500;
                    else if (whle < -1500 - min) whle += 100;
                    else if (whle < -1000 - min) whle += 60;
                    else if (whle < -800 - min)whle += 50;
                    else if (whle < -600 - min) whle += 40;
                    else if (whle < -400 - min)whle += 30;
                    else if (whle < -200 - min)whle += 20;
                    else whle += 10;
                }
            }
            if (BK1 > 0) BK1 -= 5;
        }
        //DRAW

        setlinewidth(3);
        if (DarkMode)putimage_alphablend(NULL, backImage, 0, 0, 80);
        else putimage_alphablend(NULL, backImage, 0, 0, 150);
        for (int i = 0; i < 8; i++)
        {
            if (chooseLG == i) {
                setcolor(EGERGBA(155, 255, 200, 150));
                if (DarkMode) setfillcolor(EGERGBA(50, 50, 50, 100));
                else setfillcolor(EGERGBA(240, 240, 240, 140));
            }
            else {
                if (DarkMode)
                {
                    setcolor(EGERGBA(50, 50, 50, 100));
                    setfillcolor(EGERGBA(50, 50, 50, 100));
                }
                else
                {
                    setcolor(EGERGBA(240, 240, 240, 140));
                    setfillcolor(EGERGBA(240, 240, 240, 140));
                }
            }

            if (fontRectChange)
            {

                int totalFrames = 360;
                double startValue = 0.0;
                double endValue = 30.0;


                if (frame2 < totalFrames)
                {
                    double t = frame2 / (double)totalFrames;
                    double v = startValue + (endValue - startValue) * ease::easeOutBack(t, 4);
                    if (i == chooseLG)
                    {
                        ege_roundrect(w / 2 + 45 - v / 2, h / 7 * (i + 1) + whle / 5 - h / 20 + 15 - v / 2, w / 2 - 90 + v, h / 9 - 30 + v, screen.x / 90);
                        if (i != 3)
                        {
                            setfont(v * 2, 0, "Aa吧唧一口小可耐");
                        }
                        else
                        {
                            setfont(v * 2, 0, "Ownglyph Soomini Regular");
                        }
                        setcolor(EGERGB(150, 250, 255));
                        xyprintf(w / 4 + w / 10 - 120 + v * 4, h / 2 + h / 20 - 50 + v * 2 * 5 / 6, L"%s", ys[i].c_str());
                    }
                    else
                    {
                        ege_roundrect(w / 2 + 30, h / 7 * (i + 1) + whle / 5 - h / 20, w / 2 - 60, h / 9, screen.x / 90);
                    }
                    frame2++;
                }
                else
                {
                    frame2 = 0;
                    fontRectChange = false;
                }
            }
            else
            {
                ege_roundrect(w / 2 + 30, h / 7 * (i + 1) + whle / 5 - h / 20, w / 2 - 60, h / 9, screen.x / 90);
                if (i == chooseLG)
                {
                    if (i != 3)
                    {
                        setfont(59, 0, "Aa吧唧一口小可耐");
                    }
                    else
                    {
                        setfont(59, 0, "Ownglyph Soomini Regular");
                    }
                    setcolor(EGERGB(150, 250, 255));
                    xyprintf(w / 4 + w / 10, h / 2 + h / 20, L"%s", ys[i].c_str());
                }
            }

            DrawManager::fillroundrectwithrect(w / 2 + 30, h / 7 * (i + 1) + whle / 5 - h / 20, w / 2 - 60, h / 9, screen.x / 90);
        }
        if (DarkMode) setcolor(WHITE);
        else setcolor(EGERGB(20, 20, 20));

        for (int i = 0; i < 8; i++)
        {
            if (clockStart > 10 * i)
            {
                if (FontFrame[i] < FontTotalFrame)
                {
                    double startValue_font = 0.0;
                    double endValue_font = w / 36;

                    double t = FontFrame[i] / (double)FontTotalFrame; // 用自己的帧数
                    double v = startValue_font + (endValue_font - startValue_font) * ease::easeOutBack(t, 2);
                    int r_font = static_cast<int>(v);

                    if (i != 3)
                    {
                        setfont(r_font, 0, "Aa吧唧一口小可耐");
                    }
                    else
                    {
                        setfont(r_font, 0, "Ownglyph Soomini Regular");
                    }

                    // 只显示当前i的内容
                    switch (i) {
                    case 0: xyprintf(w * 3 / 4, h / 7 + whle / 5, zh2[1].c_str()); break;
                    case 1: xyprintf(w * 3 / 4, h / 7 * 2 + whle / 5, en2[1].c_str()); break;
                    case 2: xyprintf(w * 3 / 4, h / 7 * 3 + whle / 5, jp2[1].c_str()); break;
                    case 3: xyprintf(w * 3 / 4, h / 7 * 4 + whle / 5, kr2[1].c_str()); break;
                    case 4: xyprintf(w * 3 / 4, h / 7 * 5 + whle / 5, fr2[1].c_str()); break;
                    case 5: xyprintf(w * 3 / 4, h / 7 * 6 + whle / 5, de2[1].c_str()); break;
                    case 6: xyprintf(w * 3 / 4, h / 7 * 7 + whle / 5, ru2[1].c_str()); break;
                    case 7: xyprintf(w * 3 / 4, h / 7 * 8 + whle / 5, es2[1].c_str()); break;
                    }
                    FontFrame[i]++;
                }
                else
                {
                    // 动画结束后保持最终大小
                    int r_font = static_cast<int>(w / 36);
                    if (i != 3)
                    {
                        setfont(r_font, 0, "Aa吧唧一口小可耐");
                    }
                    else
                    {
                        setfont(r_font, 0, "Ownglyph Soomini Regular");
                    }
                    switch (i) {
                    case 0: xyprintf(w * 3 / 4, h / 7 + whle / 5, zh2[1].c_str()); break;
                    case 1: xyprintf(w * 3 / 4, h / 7 * 2 + whle / 5, en2[1].c_str()); break;
                    case 2: xyprintf(w * 3 / 4, h / 7 * 3 + whle / 5, jp2[1].c_str()); break;
                    case 3: xyprintf(w * 3 / 4, h / 7 * 4 + whle / 5, kr2[1].c_str()); break;
                    case 4: xyprintf(w * 3 / 4, h / 7 * 5 + whle / 5, fr2[1].c_str()); break;
                    case 5: xyprintf(w * 3 / 4, h / 7 * 6 + whle / 5, de2[1].c_str()); break;
                    case 6: xyprintf(w * 3 / 4, h / 7 * 7 + whle / 5, ru2[1].c_str()); break;
                    case 7: xyprintf(w * 3 / 4, h / 7 * 8 + whle / 5, es2[1].c_str()); break;
                    }
                }
            }
        }





        //缓动动画

        if (!lgs)
        {
            lgs = first::start1(w, h);
        }
        else
        {
            const double endScale = 0.5;
            // 位置动画参数
            int logoW = getwidth(logoImage[0]);
            int logoH = getheight(logoImage[0]);
            int endX = w / 5 - int(logoW * endScale) / 2;
            int endY = h / 2 - int(logoH * endScale) / 2; // 居中

            int drawW = static_cast<int>(logoW * endScale);
            int drawH = static_cast<int>(logoH * endScale);
            // 临时缩放，不破坏原图
            PIMAGE tempLogo = newimage(drawW, drawH);
            putimage(tempLogo, 0, 0, drawW, drawH, logoImage[0], 0, 0, logoW, logoH);
            putimage_withalpha(NULL, tempLogo, endX, endY, 0, 0, 0, 0);
            delimage(tempLogo);
        }

        if (chooseLG > -1)
        {

            POINT K = Window::GetWindowSize();
            int totalFrames3 = 80;
            double startValue3 = 0.0;
            double endValue3 = 150.0;
            int s = 0;

            if (frame3 < totalFrames3)
            {
                double t = frame3 / (double)totalFrames3;
                double v = startValue3 + (endValue3 - startValue3) * ease::easeOutBack(t, 3);
                s = v;
                frame3++;
            }
            else
            {
                s = endValue3;
            }

            if (DarkMode)
            {
                setcolor(EGERGBA(20, 20, 20, 150));
                setfillcolor(EGERGBA(20, 20, 20, 150));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, 150));
                setfillcolor(EGERGBA(240, 240, 240, 150));
            }
            DrawManager::fillroundrectwithrect(K.x - 470 + s / 2, K.y - 150 + s * 2 / 7, s * 2, s * 4 / 7, K.x / 100);
            if (!DarkMode)
            {
                setcolor(EGERGB(20, 20, 20));
            }
            else
            {
                setcolor(EGERGB(240, 240, 240));
            }
            if (s != endValue3)
            {
                if (chooseLG != 3)
                {
                    setfont(s / 3, 0, "Aa吧唧一口小可耐");
                }
                else
                {
                    setfont(s / 3, 0, "Ownglyph Soomini Regular");
                }
            }
            else
            {
                int totalFrames4 = 80;
                double startValue4 = 0.0;
                double endValue4 = 150.0;
                int fontS = 0;
                if (frame4 < totalFrames4)
                {
                    double t2s = frame4 / (double)totalFrames4;
                    double v2s = startValue4 + (endValue4 - startValue4) * ease::easeOutBack(t2s, 3);
                    fontS = v2s;
                    frame4++;
                }
                else
                {
                    fontS = endValue4;
                }

                if (chooseLG != 3)
                {
                    setfont(fontS / 3, 0, "Aa吧唧一口小可耐");
                }
                else
                {
                    setfont(fontS / 3, 0, "Ownglyph Soomini Regular");
                }
            }
            if (chooseLG == 0) xyprintf(K.x - 250, K.y - 65, zh2[0].c_str());
            if (chooseLG == 1) xyprintf(K.x - 250, K.y - 65, en2[0].c_str());
            if (chooseLG == 2) xyprintf(K.x - 250, K.y - 65, jp2[0].c_str());
            if (chooseLG == 3) xyprintf(K.x - 250, K.y - 65, kr2[0].c_str());
            if (chooseLG == 4) xyprintf(K.x - 250, K.y - 65, fr2[0].c_str());
            if (chooseLG == 5) xyprintf(K.x - 250, K.y - 65, de2[0].c_str());
            if (chooseLG == 6) xyprintf(K.x - 250, K.y - 65, ru2[0].c_str());
            if (chooseLG == 7) xyprintf(K.x - 250, K.y - 65, es2[0].c_str());
        }

        getimage(OG, 0, 0, w, 70);
        img::BlurWithDownscale(OG, 2, 1); // 2倍降采样
        putimage(0, 0, OG);

        if (BK1 > 0 && !EXIT2)
        {
            setcolor(EGERGBA(0, 0, 0, BK1));
            setfillcolor(EGERGBA(0, 0, 0, BK1));
            ege_fillrect(0, 0, w, h);
        }

        if (EXIT2)
        {
            if (BK1 < 255)BK1 += 15;
            setcolor(EGERGBA(0, 0, 0, BK1));
            setfillcolor(EGERGBA(0, 0, 0, BK1));
            ege_fillrect(0, 0, w, h);

            if (BK1 == 255)break;
        }

    }

    if (!is_run())
    {
        first::backOut();
    }

    delimage(OG);
    int BK2 = 255;
    if (DarkMode)setbkcolor(EGERGB(0, 0, 0));
    else setbkcolor(EGERGB(255, 255, 255));
    for (int i = 0; i < 160; i++)
    {
        delay_fps(60);
        cleardevice();

        if (!is_run())
        {
            first::backOut();
        }

        // 处理消息队列
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (BK2 > 0 && i < 144) BK2 -= 15;
        if (i >= 144) BK2 += 15;

        if (BK2 > 0 && i >= 144)
        {
            if (DarkMode)
            {
                setcolor(EGERGBA(0, 0, 0, BK2));
                setfillcolor(EGERGBA(0, 0, 0, BK2));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, BK2));
                setfillcolor(EGERGBA(240, 240, 240, BK2));
            }
            ege_fillrect(0, 0, w, h);
        }


        static int animFrame2 = 0;
        const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
        const double startScale02 = 0.1;
        const double endScale02 = 0.6;

        // 位置动画参数
        int logoW = getwidth(logoImage[0]);
        int logoH = getheight(logoImage[0]);
        int startX = w / 2 - int(logoW * startScale02) / 2;
        int endX = w / 2 - int(logoW * endScale02) / 2;
        int startY = h / 2 - int(logoH * startScale02) / 2; // 居中
        int endY = h / 2 - int(logoH * endScale02) / 2; // 居中

        double scale = endScale02;
        int drawX = endX, drawY = endY;
        if (animFrame2 < animTotalFrames2) {
            double t = animFrame2 / (double)(animTotalFrames2 - 1);
            double easeT = ease::easeOutBack(t, 5);
            scale = startScale02 + (endScale02 - startScale02) * easeT;
            drawX = startX + int((endX - startX) * easeT);
            drawY = startY + int((endY - startY) * easeT);
            animFrame2++;
        }
        else {
            scale = endScale02;
            drawX = endX;
            drawY = endY;
        }

        int drawW = static_cast<int>(logoW * scale);
        int drawH = static_cast<int>(logoH * scale);

        // 临时缩放，不破坏原图
        PIMAGE tempLogo = newimage(drawW, drawH);
        putimage(tempLogo, 0, 0, drawW, drawH, logoImage[1], 0, 0, logoW, logoH);
        putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
        delimage(tempLogo);

        if (BK2 > 0 && i < 144)
        {
            if (DarkMode)
            {
                setcolor(EGERGBA(0, 0, 0, BK2));
                setfillcolor(EGERGBA(0, 0, 0, BK2));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, BK2));
                setfillcolor(EGERGBA(240, 240, 240, BK2));
            }
            ege_fillrect(0, 0, w, h);
        }
    }

    int BK3 = 255;
    int clock2 = 0;

    DarkMode = Window::IsDarkModeEnabled();
    if (DarkMode)setbkcolor(EGERGB(40, 40, 40));
    else setbkcolor(EGERGB(240, 240, 240));

    PIMAGE OG2 = newimage();

    whle = 0;
    EXIT2 = false;
    chooseLG = 2;
    bool isNONE = false;

    while (is_run())
    {
        delay_fps(60);
        cleardevice();

        //KEY
        {
            // 处理消息队列
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            if (keystate(0x01))
            {
                POINT mousePos = Window::GetMousePosX();
                POINT WindowSize = Window::GetWindowSize();
                //MessageBoxA(NULL, "code:0x01", "Debug", MB_OK + MB_SYSTEMMODAL + 48);
                for (int i = 1; i < 8; i++)
                {
                    if (mousePos.x > w / 2 + 30 && mousePos.x < w - 30 && mousePos.y < WindowSize.y - 110 && mousePos.y > 50)
                    {
                        if (mousePos.y > h / 7 * (i + 1) + whle / 5 - h / 20 && mousePos.y < h / 7 * (i + 1) + whle / 5 - h / 20 + h / 9)
                        {
                            if (userFileLock != i)
                            {
                                userFileLock = i;
                                frameC = 0;
                                if (i == 1 && isNONE == false)
                                {
                                    frameA = 0;
                                    frameB = 0;
                                    isNONE = true;
                                }
                                else
                                {
                                    if (isNONE == true)
                                    {
                                        frameA = 0;
                                        isNONE = false;
                                    }
                                }
                            }
                        }
                    }
                }
                if (mousePos.x > w - 470 + 150 / 2 && mousePos.x < w - 470 + 150 / 2 + 300 && mousePos.y > h - 150 + 150 * 2 / 7 && mousePos.y < h - 150 + 150 * 2 / 7 + 150 * 4 / 7)
                {
                    EXIT2 = true;
                }
            }
            if (mousemsg())
            {
                mouseMsg = getmouse();
                if (mouseMsg.is_wheel())   whle_temp = mouseMsg.wheel / 2;
            }
        }
        //MATH
        {
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
                const int min = 1500;
                if (whle < -min)
                {
                    if (whle < -4500 - min) whle = -4500;
                    else if (whle < -1500 - min) whle += 100;
                    else if (whle < -1000 - min) whle += 60;
                    else if (whle < -800 - min)whle += 50;
                    else if (whle < -600 - min) whle += 40;
                    else if (whle < -400 - min)whle += 30;
                    else if (whle < -200 - min)whle += 20;
                    else whle += 10;
                }
            }
            if (whle_temp != 0)
            {
                whle += whle_temp;
                if (whle_temp > 0)whle_temp -= 2;
                else whle_temp += 2;
            }

            if (clock2 < 1000)clock2 += 1;
            if (BK3 > 0 && !EXIT2) BK3 -= 5;

            if (BK3 < 255 && EXIT2)  BK3 += 15;
        }

        //DRAW
        {

            if (DarkMode)putimage_alphablend(NULL, backImage, 0, 0, 80);
            else putimage_alphablend(NULL, backImage, 0, 0, 150);

            if (clock2 > 60)
            {
                first::OBI(2, w, h);
            }
            first::OBI(4, w, h);
            first::OBI(3, w, h);
            first::OBI(8, w, h);
            if (clock2 > 180)
            {
                if (isNONE)
                {
                    first::OBI(5, w, h);
                }
                else
                {
                    first::OBI(6, w, h);
                }
            }

            first::OBI(7, w, h);



            getimage(OG2, 0, 0, w, 70);
            img::BlurWithDownscale(OG2, 2, 1); // 2倍降采样
            putimage(0, 0, OG2);

            if (BK3 > 0 && !EXIT2)
            {
                if (DarkMode)
                {
                    setcolor(EGERGBA(0, 0, 0, BK3));
                    setfillcolor(EGERGBA(0, 0, 0, BK3));
                }
                else
                {
                    setcolor(EGERGBA(240, 240, 240, BK3));
                    setfillcolor(EGERGBA(240, 240, 240, BK3));
                }
                ege_fillrect(0, 0, w, h);
            }

            first::OBI(1, w, h);

            if (BK3 > 0 && EXIT2)
            {
                if (BK3 >= 255) break;
                if (DarkMode)
                {
                    setcolor(EGERGBA(0, 0, 0, BK3));
                    setfillcolor(EGERGBA(0, 0, 0, BK3));
                }
                else
                {
                    setcolor(EGERGBA(240, 240, 240, BK3));
                    setfillcolor(EGERGBA(240, 240, 240, BK3));
                }
                ege_fillrect(0, 0, w, h);
            }
        }

    }
    delimage(OG2);

    if (!is_run())
    {
        first::backOut();
    }

    int BK4 = 255;
    if (DarkMode)setbkcolor(EGERGB(0, 0, 0));
    else setbkcolor(EGERGB(255, 255, 255));
    for (int i = 0; i < 160; i++)
    {
        delay_fps(60);
        cleardevice();

        // 处理消息队列
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!is_run())
        {
            first::backOut();
        }

        if (BK4 > 0 && i < 144) BK4 -= 15;
        if (i >= 144) BK4 += 15;

        if (BK4 > 0 && i >= 144)
        {
            if (DarkMode)
            {
                setcolor(EGERGBA(0, 0, 0, BK4));
                setfillcolor(EGERGBA(0, 0, 0, BK4));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, BK4));
                setfillcolor(EGERGBA(240, 240, 240, BK4));
            }
            ege_fillrect(0, 0, w, h);
        }


        static int animFrame2 = 0;
        const int animTotalFrames2 = 120; // 动画帧数（1.5秒）
        const double startScale02 = 0.1;
        const double endScale02 = 0.4;

        // 位置动画参数
        int logoW = getwidth(logoImage[0]);
        int logoH = getheight(logoImage[0]);
        int startX = w / 2 - int(logoW * startScale02) / 2;
        int endX = w / 2 - int(logoW * endScale02) / 2;
        int startY = h / 2 - int(logoH * startScale02) / 2; // 居中
        int endY = h / 2 - int(logoH * endScale02) / 2; // 居中

        double scale = endScale02;
        int drawX = endX, drawY = endY;
        if (animFrame2 < animTotalFrames2) {
            double t = animFrame2 / (double)(animTotalFrames2 - 1);
            double easeT = ease::easeOutBack(t, 5);
            scale = startScale02 + (endScale02 - startScale02) * easeT;
            drawX = startX + int((endX - startX) * easeT);
            drawY = startY + int((endY - startY) * easeT);
            animFrame2++;
        }
        else {
            scale = endScale02;
            drawX = endX;
            drawY = endY;
        }

        int drawW = static_cast<int>(logoW * scale);
        int drawH = static_cast<int>(logoH * scale);

        // 临时缩放，不破坏原图
        PIMAGE tempLogo = newimage(drawW, drawH);
        putimage(tempLogo, 0, 0, drawW, drawH, logoImage[2], 0, 0, logoW, logoH);
        putimage_withalpha(NULL, tempLogo, drawX, drawY, 0, 0, 0, 0);
        delimage(tempLogo);

        if (BK4 > 0 && i < 144)
        {
            if (DarkMode)
            {
                setcolor(EGERGBA(0, 0, 0, BK4));
                setfillcolor(EGERGBA(0, 0, 0, BK4));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, BK4));
                setfillcolor(EGERGBA(240, 240, 240, BK4));
            }
            ege_fillrect(0, 0, w, h);
        }
    }

    int MSY = 255;
    ///////////////////////////
    EXIT2 = false;
    ////////////////////////////
    clockStart = 0;

    bool isChoose = false;
    bool isChoose2 = false;

    DarkMode = Window::IsDarkModeEnabled();
    PIMAGE OG3 = newimage();

    int keyClock_0x01 = 0;
    int keyClock_0x08 = 0;
    while (is_run())
    {
        delay_fps(60);
        cleardevice();
        if (clockStart < 1000)clockStart += 1;
        // 处理消息队列
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (keystate(0x01))
        {
            keyClock_0x01 += 1;
            POINT mouseP = Window::GetMousePosX();
            if (!isChoose)
            {
                isChoose = true;
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {

                        if (mouseP.x > w / 2 + w / 10 + 10 + j * 120 && mouseP.x < w / 2 + w / 10 + 10 + j * 120 + 100 && mouseP.y > h / 2 - h / 8 + 10 + i * 120 && mouseP.y < h / 2 - h / 8 + 10 + i * 120 + 100)
                        {
                            int s = 1 + j + i * 3;
                            for (int k = 0; k < 6; k++)
                            {
                                if (userLock[k] == -1)
                                {
                                    userLock[k] = userLock_temp[k] = s;
                                    cgos = s;
                                    cgos_frame = 0;
                                    userLock_fontFrame[k] = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (mouseP.x > w / 2 + w / 10 + 10 + 3 * 120 && mouseP.x < w / 2 + w / 10 + 10 + 3 * 120 + 100 && mouseP.y > h / 2 - h / 8 + 10 + 2 * 120 && mouseP.y < h / 2 - h / 8 + 10 + 2 * 120 + 100)
                {
                    for (int k = 0; k < 6; k++)
                    {
                        if (userLock[k] == -1)
                        {
                            userLock[k] = userLock_temp[k] = 0;
                            cgos = 0;
                            cgos_frame = 0;
                            userLock_fontFrame[k] = 0;
                            break;
                        }
                    }
                }
                if (mouseP.x > w / 2 + w / 10 + 10 + 3 * 120 && mouseP.x < w / 2 + w / 10 + 10 + 3 * 120 + 100 && mouseP.y > h / 2 - h / 8 + 10 + 0 * 120 && mouseP.y < h / 2 - h / 8 + 10 + 0 * 120 + 220)
                {
                    for (int k = 5; k >= 0; k--)
                    {
                        if (userLock[k] != -1)
                        {
                            userLock[k] = -1;
                            userLock_fontFrame[k] = 0;
                            cgos = 100;
                            cgos_frame = 0;
                            break;
                        }
                    }
                }
            }
            if (keyClock_0x01 > 60 && mouseP.x > w / 2 + w / 10 + 10 + 3 * 120 && mouseP.x < w / 2 + w / 10 + 10 + 3 * 120 + 100 && mouseP.y > h / 2 - h / 8 + 10 + 0 * 120 && mouseP.y < h / 2 - h / 8 + 10 + 0 * 120 + 220)
            {
                for (int k = 5; k >= 0; k--)
                {
                    if (userLock[k] != -1)
                    {
                        userLock[k] = -1;
                        userLock_fontFrame[k] = 0;
                        cgos = 100;
                        cgos_frame = 0;
                    }
                }
            }

            if (mouseP.x > w - 420 + 150 / 2 && mouseP.x < w - 420 + 150 / 2 + 300 && mouseP.y > h - 150 + 150 * 2 / 7 && mouseP.y < h - 150 + 150 * 2 / 7 + 150 * 4 / 7)
            {
                break;
            }
            if (mouseP.x > w - 770 + 150 / 2 && mouseP.x < w - 770 + 150 / 2 + 300 && mouseP.y > h - 150 + 150 * 2 / 7 && mouseP.y < h - 150 + 150 * 2 / 7 + 150 * 4 / 7)
            {
                for (int k = 5; k >= 0; k--)
                {
                    if (userLock[k] != -1)
                    {
                        userLock[k] = -1;
                        userLock_fontFrame[k] = 0;
                        cgos = 100;
                        cgos_frame = 0;
                    }
                }
                break;
            }
        }
        else
        {
            keyClock_0x01 = 0;
            isChoose = false;
        }

        if (keystate(0x08))
        {
            keyClock_0x08 += 1;
            POINT mouseP = Window::GetMousePosX();
            if (!isChoose2)
            {
                isChoose2 = true;
                for (int k = 5; k >= 0; k--)
                {
                    if (userLock[k] != -1)
                    {
                        userLock[k] = -1;
                        userLock_fontFrame[k] = 0;
                        break;
                    }
                }
            }
            if (keyClock_0x08 > 60)
            {
                for (int k = 5; k >= 0; k--)
                {
                    if (userLock[k] != -1)
                    {
                        userLock[k] = -1;
                        userLock_fontFrame[k] = 0;
                    }
                }
            }
        }
        else
        {
            keyClock_0x08 = 0;
            isChoose2 = false;
        }

        if (kbhit())
        {
            char ch = getch(); // 获取按键，不显示在屏幕上

            if (ch >= '0' && ch <= '9') {
                int num = ch - '0';

                for (int k = 0; k < 6; k++)
                {
                    if (userLock[k] == -1)
                    {
                        userLock[k] = userLock_temp[k] = num;
                        userLock_fontFrame[k] = 0;
                        break;
                    }
                }
            }
        }

        if (MSY > 0 && !EXIT2)MSY -= 5;

        if (DarkMode)putimage_alphablend(NULL, backImage, 0, 0, 80);
        else putimage_alphablend(NULL, backImage, 0, 0, 150);

        getimage(OG3, 0, 0, w, 70);
        img::BlurWithDownscale(OG3, 2, 1); // 2倍降采样
        putimage(0, 0, OG3);

        if (clockStart > 90)
        {
            first::OBI(10, w, h);
            first::OBI(11, w, h);
            first::OBI(12, w, h);
        }
        if (clockStart > 150)
        {
            first::OBI(13, w, h);
            first::OBI(14, w, h);
            first::OBI(15, w, h);
        }

        if (MSY > 0 && !EXIT2)
        {
            if (DarkMode)
            {
                setcolor(EGERGBA(0, 0, 0, MSY));
                setfillcolor(EGERGBA(0, 0, 0, MSY));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, MSY));
                setfillcolor(EGERGBA(240, 240, 240, MSY));
            }
            ege_fillrect(0, 0, w, h);
        }

        if (clockStart <= 90) first::OBI(9, w, h);
    }

    if (!is_run())
    {
        first::backOut();
    }

    clockStart = 0;
    keyClock_0x08 = 0;
    isChoose2 = false;

    while (is_run())
    {
        delay_fps(60);
        cleardevice();
        if (clockStart < 1000)clockStart += 1;
        // 处理消息队列
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        if (keystate(0x01) && clockStart > 120)
        {
            POINT mouseP = Window::GetMousePosX();
            if (mouseP.x > w - 420 + 150 / 2 && mouseP.x < w - 420 + 150 / 2 + 300 && mouseP.y > h - 150 + 150 * 2 / 7 && mouseP.y < h - 150 + 150 * 2 / 7 + 150 * 4 / 7)
            {
                break;
            }
            if (mouseP.x > w - 770 + 150 / 2 && mouseP.x < w - 770 + 150 / 2 + 300 && mouseP.y > h - 150 + 150 * 2 / 7 && mouseP.y < h - 150 + 150 * 2 / 7 + 150 * 4 / 7)
            {
                for (int k = 7; k >= 0; k--)
                {
                    if (linkCode[k] != '?')
                    {
                        linkCode[k] = '?';

                    }
                }
                break;
            }
        }


        if (keystate(0x08))
        {
            keyClock_0x08 += 1;
            POINT mouseP = Window::GetMousePosX();
            if (!isChoose2)
            {
                isChoose2 = true;
                for (int k = 7; k >= 0; k--)
                {
                    if (linkCode[k] != '?')
                    {
                        linkCode[k] = '?';
                        userLink_fontFrame[k] = 0;
                        break;
                    }
                }
            }
            if (keyClock_0x08 > 60)
            {
                for (int k = 7; k >= 0; k--)
                {
                    if (linkCode[k] != '?')
                    {
                        linkCode[k] = '?';
                        userLink_fontFrame[k] = 0;
                    }
                }
            }
        }
        else
        {
            keyClock_0x08 = 0;
            isChoose2 = false;
        }
        if (kbhit())
        {
            char s = getch();
            if (tool::isValidChar(s))
            {
                for (int i = 0; i < 8; i++)
                {
                    if (linkCode[i] == '?')
                    {
                        linkCode[i] = s;
                        linkCode_temp[i] = s;
                        userLink_fontFrame[i] = 0;
                        break;
                    }
                }
            }
        }

        if (DarkMode)putimage_alphablend(NULL, backImage, 0, 0, 80);
        else putimage_alphablend(NULL, backImage, 0, 0, 150);

        getimage(OG3, 0, 0, w, 70);
        img::BlurWithDownscale(OG3, 2, 1); // 2倍降采样
        putimage(0, 0, OG3);

        if (clockStart < 121) first::OBI(16, w, h);
        if (clockStart > 120)
        {
            first::OBI(19, w, h);
            first::OBI(20, w, h);
        }
        first::OBI(17, w, h);
        first::OBI(18, w, h);
    }

    if (!is_run())
    {
        first::first::backOut();
    }

    EXIT2 = false;
    clockStart = 0;
    int Kg = 0;

    userset[0] = to_string(userLg);
    userset[1] = to_string(userFileLock);

    int lastLock = 0;
    if (userLock[0] == -1 || userLock[1] == -1 || userLock[2] == -1 || userLock[3] == -1 || userLock[4] == -1 || userLock[5] == -1)
    {
        userset[2] = "0";
    }
    else
    {
        ostringstream oss;
        for (int num : userLock) {
            oss << num;  // 自动处理0和其他整数
        }
        userset[2] = oss.str();
    }
    for (int i = 3; i < 15; i++)
    {
        userset[i] = "0";
    }


    bool ax = false;
    for (int i = 0; i < 8; i++)
    {
        if (linkCode[i] == '?')
        {
            ax = true;
            break;
        }
    }
    string s(linkCode, 8);
    if (!ax) saveData_link[0] = s;
    else saveData_link[0] = "NULL";

    ini::SaveData();

    while (1)
    {
        delay_fps(60);
        cleardevice();
        if (clockStart < 200)clockStart += 1;
        else EXIT2 = true;
        // 处理消息队列
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (DarkMode)putimage_alphablend(NULL, backImage, 0, 0, 80);
        else putimage_alphablend(NULL, backImage, 0, 0, 150);

        getimage(OG3, 0, 0, w, 70);
        img::BlurWithDownscale(OG3, 2, 1); // 2倍降采样
        putimage(0, 0, OG3);

        if (clockStart < 121)
        {
            first::OBI(21, w, h);
            first::OBI(22, w, h);
        }
        if (clockStart < 61) first::OBI(23, w, h);
        else
        {
            first::OBI(24, w, h);
            first::OBI(25, w, h);
        }

        if (EXIT2 && Kg < 255)
        {
            Kg += 15;
            if (DarkMode)
            {
                setcolor(EGERGBA(0, 0, 0, Kg));
                setfillcolor(EGERGBA(0, 0, 0, Kg));
            }
            else
            {
                setcolor(EGERGBA(240, 240, 240, Kg));
                setfillcolor(EGERGBA(240, 240, 240, Kg));
            }
            ege_fillrect(0, 0, w, h);
        }
        if (EXIT2 && Kg >= 255) break;
    }


    ofstream lcin("lcin.dll");
    lcin.close();

    first::firststartA = true;
    return 1;
}

#pragma endregion