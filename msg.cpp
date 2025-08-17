#include "msg.h"
#include "WindowsEv.h"
#include "ease.h"
#include "Main.h"
#include "GraphManager.h"


struct messageTs
{
    int overtime = 0;
    string text = "NULL";
    int color[3] = { 0,0,0 };
    int fontColor[3] = { 0,0,0 };
    int type;
    int time;
    bool isActive;
    int MsgBarAlpha;
    int barY;
    int longT;

    bool loadASlink = false;
    int loadAlink = 0;
    int roundAlink = 0;

    int circle;
    int circleA;
    bool isAddCircle;

    int mouseTime;
    bool isOpen;
    int Cx;
    bool Kill;
};
messageTs msgBox[256];
struct messageMs
{
    int color[3] = { 0,0,0 };
    int FontColor[3] = { 0,0,0 };
    string text = "NULL";
    string bottom[3] = { "NULL","NULL","NULL" };
    bool isActive = false;
    int OSSize = 0;
    int OSme = 0;
};
messageMs msgBar[256];

int msgBarChoose = -1;
bool noBar = true;

bool ex = false;

bool BarisEXIT = false;

int backAlpha = 0;




    int msg::msgBoxManager()
    {
        int mousePosX = mousePos.x;
        int mousePosY = mousePos.y;
        int windowSizeX = windowSize.x;
        int windowSizeY = windowSize.y;
        int screenWidth = screen.x;
        int screenHeight = screen.y;

        for (int i = 0; i < 256; i++)
        {
            if (msgBox[i].isActive == true)
            {
                if (mousePosX > windowSizeX - windowSizeX / 5 - screenWidth / 300 && mousePosX < windowSizeX - screenWidth / 300 && mousePosY > i * (screenHeight / 20 + screenHeight / 40) + screenHeight / 60 && mousePosY < i * (screenHeight / 20 + screenHeight / 40) + screenHeight / 60 + screenHeight / 20)
                {
                    if (msgBox[i].mouseTime < 15)
                        msgBox[i].mouseTime += 1;
                }
                else
                {
                    msgBox[i].mouseTime = 0;
                }
                if (msgBox[i].mouseTime >= 15)
                {
                    int textLen = msgBox[i].text.length();
                    if (textLen > 13) {
                        msgBox[i].isOpen = true;
                    }
                }
                else
                {
                    msgBox[i].isOpen = false;
                }

                if (msgBox[i].type != 3)
                {
                    if (msgBox[i].time >= msgBox[i].overtime)
                    {
                        if (msgBox[i].MsgBarAlpha > 0)
                        {
                            msgBox[i].MsgBarAlpha -= 10;
                        }
                        else
                        {
                            msgBox[i].isActive = false;
                            msgBox[i].time = 0;
                        }
                    }
                    else
                    {
                        msgBox[i].time += 1;
                    }

                    if (msgBox[i].Kill == true)
                    {
                        if (msgBox[i].MsgBarAlpha > 0)
                        {
                            msgBox[i].MsgBarAlpha -= 10;
                        }
                        else
                        {
                            msgBox[i].isActive = false;
                        }
                    }
                }
                else
                {
                    if (msgBox[i].Kill == true)
                    {
                        if (msgBox[i].MsgBarAlpha > 0)
                        {
                            msgBox[i].MsgBarAlpha -= 10;
                        }
                        else
                        {
                            msgBox[i].isActive = false;
                        }
                    }
                }

                if (msgBox[i].type != 3)
                {
                    if (msgBox[i].MsgBarAlpha < 200)
                    {
                        if (200 - msgBox[i].MsgBarAlpha > 5) msgBox[i].MsgBarAlpha += 5;
                        else msgBox[i].MsgBarAlpha = 200;
                    }
                }
                if (msgBox[i].type == 3)
                {
                    if (msgBox[i].MsgBarAlpha < 230)
                    {
                        if (230 - msgBox[i].MsgBarAlpha > 5) msgBox[i].MsgBarAlpha += 5;
                        else msgBox[i].MsgBarAlpha = 230;
                    }
                }
            }
        }
        for (int i = 0; i < 256; i++)
        {
            if (msgBox[i].isActive == false && msgBox[i + 1].isActive == true)
            {
                msgBox[i] = msgBox[i + 1];
                msgBox[i + 1].isActive = false;
            }

        }
        return 1;
    }

    int msg::Message(int type, int color[3], int fontColor[3], int overtime, string text)
    {
        for (int i = 0; i < 256; i++)
        {
            if (msgBox[i].isActive == false)
            {
                msgBox[i].type = type;
                for (int x = 0; x < 3; x++)
                {
                    msgBox[i].color[x] = color[x];
                    msgBox[i], fontColor[x] = fontColor[x];
                }
                msgBox[i].overtime = overtime;
                msgBox[i].text = text;
                msgBox[i].isActive = true;
                msgBox[i].time = 0;
                msgBox[i].barY = -10;
                msgBox[i].longT = 0;
                msgBox[i].circle = 0;
                msgBox[i].circleA = 6;
                msgBox[i].Kill = false;
                msgBox[i].Cx = screen.x / 10;

                if (i > 0)
                {
                    messageTs temp = msgBox[i];
                    for (int j = i; j > 0; j--)
                    {
                        msgBox[j] = msgBox[j - 1];
                    }
                    msgBox[0] = temp;
                }

                return 10;
            }
        }
        return -1;
    }

    void msg::KillBox(string text)
    {
        for (int i = 0; i < 256; i++)
        {
            if (msgBox[i].text == text)
            {
                msgBox[i].Kill = true;
            }
        }
    }

    bool msg::isBox(string text)
    {
        bool isBox = false;
        for (int i = 0; i < 256; i++)
        {
            if (msgBox[i].text == text)
            {
                isBox = true;
            }
        }
        return isBox;
    }

    void msg::MessageBar(int color[3], int fontColor[3], string text, string bottom[3])
    {
        if (!noBar) return;
        noBar = true;
        for (int s = 0; s < 3; s++)
        {
            msgBar[0].color[s] = color[s];
            msgBar[0].FontColor[s] = fontColor[s];
            msgBar[0].bottom[s] = bottom[s];
        }
        msgBar[0].text = text;
        msgBar[0].OSme = 0;
        msgBar[0].isActive = true;

        msgBarChoose = -1;
    }

    int msg::DrawMessageBox()
    {

        for (int i = 0; i < 256; i++)
        {
            if (msgBox[i].isActive == false) break;

            if (msgBox[i].isOpen == true)
            {

                if (msgBox[i].Cx < screen.x / 2)
                {
                    if (screen.x / 2 - msgBox[i].Cx > 20) msgBox[i].Cx += 2;
                    if (screen.x / 2 - msgBox[i].Cx > 40) msgBox[i].Cx += 4;
                    if (screen.x / 2 - msgBox[i].Cx > 60) msgBox[i].Cx += 6;
                    if (screen.x / 2 - msgBox[i].Cx > 80) msgBox[i].Cx += 8;
                    if (screen.x / 2 - msgBox[i].Cx > 100) msgBox[i].Cx += 10;

                    if (screen.x / 2 - msgBox[i].Cx > 3) msgBox[i].Cx += 3;
                    else msgBox[i].Cx = screen.x / 2;
                }
                else
                {
                    if (screen.x / 2 - msgBox[i].Cx < -20) msgBox[i].Cx -= 2;
                    if (screen.x / 2 - msgBox[i].Cx < -40) msgBox[i].Cx -= 4;
                    if (screen.x / 2 - msgBox[i].Cx < -60) msgBox[i].Cx -= 6;
                    if (screen.x / 2 - msgBox[i].Cx < -80) msgBox[i].Cx -= 8;
                    if (screen.x / 2 - msgBox[i].Cx < -100) msgBox[i].Cx -= 10;


                    if (msgBox[i].Cx - screen.x / 2 > 3) msgBox[i].Cx -= 3;
                    else msgBox[i].Cx = screen.x / 2;
                }
            }
            else
            {
                if (msgBox[i].Cx < 0)
                {
                    if (0 - msgBox[i].Cx > 20) msgBox[i].Cx += 2;
                    if (0 - msgBox[i].Cx > 40) msgBox[i].Cx += 4;
                    if (0 - msgBox[i].Cx > 60) msgBox[i].Cx += 6;
                    if (0 - msgBox[i].Cx > 80) msgBox[i].Cx += 8;
                    if (0 - msgBox[i].Cx > 100) msgBox[i].Cx += 10;

                    if (0 - msgBox[i].Cx > 3) msgBox[i].Cx += 3;
                    else msgBox[i].Cx = 0;
                }
                else
                {
                    if (0 - msgBox[i].Cx < -20) msgBox[i].Cx -= 2;
                    if (0 - msgBox[i].Cx < -40) msgBox[i].Cx -= 4;
                    if (0 - msgBox[i].Cx < -60) msgBox[i].Cx -= 6;
                    if (0 - msgBox[i].Cx < -80) msgBox[i].Cx -= 8;
                    if (0 - msgBox[i].Cx < -100) msgBox[i].Cx -= 10;

                    if (msgBox[i].Cx - 0 > 3) msgBox[i].Cx -= 3;
                    else msgBox[i].Cx = 0;
                }
            }

            if (msgBox[i].barY < i * (screen.y / 20 + screen.y / 40) + screen.y / 60)
            {
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY > 600)
                    msgBox[i].barY += 18;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY > 400)
                    msgBox[i].barY += 15;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY > 200)
                    msgBox[i].barY += 12;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY > 100)
                    msgBox[i].barY += 9;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY > 50)
                    msgBox[i].barY += 6;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY > 25)
                    msgBox[i].barY += 3;

                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY > 3)
                    msgBox[i].barY += 3;
                else
                    msgBox[i].barY = i * (screen.y / 20 + screen.y / 40) + screen.y / 60;
            }
            if (msgBox[i].barY > i * (screen.y / 20 + screen.y / 40) + screen.y / 60)
            {
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY < -600)
                    msgBox[i].barY -= 18;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY < -400)
                    msgBox[i].barY -= 15;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY < -200)
                    msgBox[i].barY -= 12;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY < -100)
                    msgBox[i].barY -= 9;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY < -50)
                    msgBox[i].barY -= 6;
                if (i * (screen.y / 20 + screen.y / 40) + screen.y / 60 - msgBox[i].barY < -25)
                    msgBox[i].barY -= 3;


                if (msgBox[i].barY - i * (screen.y / 20 + screen.y / 40) + screen.y / 60 > 3)
                    msgBox[i].barY -= 3;
                else
                    msgBox[i].barY = i * (screen.y / 20 + screen.y / 40) + screen.y / 60;
            }
            setlinewidth(2);
            setcolor(EGERGBA(msgBox[i].color[0], msgBox[i].color[1], msgBox[i].color[2], msgBox[i].MsgBarAlpha));
            if (BKcolor > 128)
                ege::setfillcolor(EGERGBA(BKcolor - 20, BKcolor - 20, BKcolor - 20, msgBox[i].MsgBarAlpha));
            else
                ege::setfillcolor(EGERGBA(BKcolor + 20, BKcolor + 20, BKcolor + 20, msgBox[i].MsgBarAlpha));
            ege_fillroundrect(windowSize.x - windowSize.x / 5 - screen.x / 300 - msgBox[i].Cx, msgBox[i].barY, windowSize.x / 5 + msgBox[i].Cx, screen.y / 20, 5);
            ege_roundrect(windowSize.x - windowSize.x / 5 - screen.x / 300 - msgBox[i].Cx, msgBox[i].barY, windowSize.x / 5 + msgBox[i].Cx, screen.y / 20, 5);

            setlinewidth(5);

            if (msgBox[i].Cx / 5 < screen.x / 150)
            {
                ege_line(windowSize.x - screen.x / 150, msgBox[i].barY + screen.x / 200, windowSize.x - screen.x / 150 - msgBox[i].Cx / 5, msgBox[i].barY + screen.y / 20 / 2);
                ege_line(windowSize.x - screen.x / 150 - msgBox[i].Cx / 5, msgBox[i].barY + screen.y / 20 / 2, windowSize.x - screen.x / 150, msgBox[i].barY + screen.y / 20 - screen.x / 200);
            }
            else
            {
                ege_line(windowSize.x - screen.x / 150, msgBox[i].barY + screen.x / 200, windowSize.x - screen.x / 150 - screen.x / 150, msgBox[i].barY + screen.y / 20 / 2);
                ege_line(windowSize.x - screen.x / 150 - screen.x / 150, msgBox[i].barY + screen.y / 20 / 2, windowSize.x - screen.x / 150, msgBox[i].barY + screen.y / 20 - screen.x / 200);
            }


            {
                if (msgBox[i].type == 1)
                {
                    setlinewidth(6);
                    setlinecap(LINECAP_ROUND);
                    ege_line(windowSize.x - windowSize.x / 5 - screen.x / 400 - msgBox[i].Cx, msgBox[i].barY - 2 + screen.y / 20, windowSize.x - screen.x / 400 - (windowSize.x / 5 + msgBox[i].Cx) * msgBox[i].time / msgBox[i].overtime, msgBox[i].barY - 2 + screen.y / 20);
                    setfillcolor(EGERGBA(msgBox[i].color[0], msgBox[i].color[1], msgBox[i].color[2], msgBox[i].MsgBarAlpha));
                    ege_fillellipse(windowSize.x - windowSize.x / 6 - windowSize.x / 40 - msgBox[i].Cx, msgBox[i].barY + (screen.y / 60), screen.y / 80, screen.y / 80);
                }
                if (msgBox[i].type == 2)
                {
                    setlinewidth(6);
                    setlinecap(LINECAP_ROUND);
                    msgBox[i].longT += 5;

                    int start, end;
                    if (msgBox[i].longT - windowSize.x / 20 >= 0)
                    {
                        start = msgBox[i].longT - windowSize.x / 20;
                    }
                    else
                    {
                        start = 0;
                    }
                    if (msgBox[i].longT <= windowSize.x / 5 - screen.x / 400 + msgBox[i].Cx)
                    {
                        end = msgBox[i].longT - msgBox[i].Cx;
                    }
                    else
                    {
                        end = windowSize.x / 5 - screen.x / 400;
                    }

                    if (msgBox[i].longT - windowSize.x / 20 >= windowSize.x / 5 - screen.x / 200 + msgBox[i].Cx)
                    {
                        msgBox[i].longT = 0;
                    }

                    ege_line(windowSize.x - windowSize.x / 5 - screen.x / 400 + start - msgBox[i].Cx, msgBox[i].barY - 2 + screen.y / 20, windowSize.x - screen.x / 400 - windowSize.x / 5 + end, msgBox[i].barY - 2 + screen.y / 20);
                    setfillcolor(EGERGBA(msgBox[i].color[0], msgBox[i].color[1], msgBox[i].color[2], msgBox[i].MsgBarAlpha));

                    int cx = windowSize.x - windowSize.x / 6 - windowSize.x / 47 - msgBox[i].Cx;
                    int cy = msgBox[i].barY + (screen.y / 42);
                    int r = screen.y / 120;
                    ege_point pts[3] = {
                        {cx, cy - r},
                        {cx - r, cy + r},
                        {cx + r, cy + r}
                    };
                    ege_fillpoly(3, pts);
                }
                if (msgBox[i].type == 3)
                {
                    setlinewidth(6);
                    setlinecap(LINECAP_ROUND);
                    msgBox[i].longT += 5;

                    if (msgBox[i].isAddCircle == true)
                    {
                        if (msgBox[i].circleA < 270)
                        {
                            msgBox[i].circleA += 2;
                        }
                        else
                        {
                            msgBox[i].circleA = 270;
                            msgBox[i].isAddCircle = false;
                        }
                    }
                    else
                    {
                        if (msgBox[i].circleA > 10)
                        {
                            msgBox[i].circleA -= 2;
                        }
                        else
                        {
                            msgBox[i].circleA = 10;
                            msgBox[i].isAddCircle = true;
                        }
                    }

                    msgBox[i].circle -= 8;

                    if (msgBox[i].circle < -720) msgBox[i].circle = 0;


                    int start, end;
                    if (msgBox[i].longT - windowSize.x / 20 >= 0)
                    {
                        start = msgBox[i].longT - windowSize.x / 20;
                    }
                    else
                    {
                        start = 0;
                    }
                    if (msgBox[i].longT <= windowSize.x / 5 - screen.x / 400 + msgBox[i].Cx)
                    {
                        end = msgBox[i].longT - msgBox[i].Cx;
                    }
                    else
                    {
                        end = windowSize.x / 5 - screen.x / 400;
                    }

                    if (msgBox[i].longT - windowSize.x / 20 >= windowSize.x / 5 - screen.x / 200 + msgBox[i].Cx)
                    {
                        msgBox[i].longT = 0;
                    }

                    ege_line(windowSize.x - windowSize.x / 5 - screen.x / 400 + start - msgBox[i].Cx, msgBox[i].barY - 2 + screen.y / 20, windowSize.x - screen.x / 400 - windowSize.x / 5 + end, msgBox[i].barY - 2 + screen.y / 20);
                    setfillcolor(EGERGBA(msgBox[i].color[0], msgBox[i].color[1], msgBox[i].color[2], msgBox[i].MsgBarAlpha));

                    if (msgBox[i].Kill == false)
                        arc(windowSize.x - windowSize.x / 6 - windowSize.x / 58 - msgBox[i].Cx, msgBox[i].barY + screen.y / 40, msgBox[i].circle, msgBox[i].circle + msgBox[i].circleA, screen.y / 70);
                    else
                    {
                        int cx = windowSize.x - windowSize.x / 6 - windowSize.x / 47 - msgBox[i].Cx;
                        int cy = msgBox[i].barY + (screen.y / 42);
                        int r = screen.y / 120;
                        ege_point pts[3] = {
                            {cx, cy - r},
                            {cx - r, cy + r},
                            {cx + r, cy + r}
                        };
                        ege_fillpoly(3, pts);
                    }
                }
            }

            if (msgBox[i].fontColor[0] > 0)
                setcolor(EGERGBA(msgBox[i].fontColor[0], msgBox[i].fontColor[1], msgBox[i].fontColor[2], msgBox[i].MsgBarAlpha));
            else
            {
                if (DarkMode)setcolor(EGERGBA(255, 255, 255, 255));
                else setcolor(EGERGBA(40, 40, 40, 255));
            }

            settextjustify(LEFT_TEXT, CENTER_TEXT);
            DrawManager::setFont(35);

            if (msgBox[i].isOpen == false)
            {
                // 获取消息文本长度
                int textLen = msgBox[i].text.length();
                if (textLen > 13) {
                    // 截取前6个字符并加上"…"
                    string shortText = msgBox[i].text.substr(0, 12) + "..";
                    xyprintf(windowSize.x - windowSize.x / 6 - msgBox[i].Cx, msgBox[i].barY + (screen.y / 40), "%s", shortText.c_str());
                }
                else {
                    xyprintf(windowSize.x - windowSize.x / 6 - msgBox[i].Cx, msgBox[i].barY + (screen.y / 40), "%s", msgBox[i].text.c_str());
                }
            }
            else
            {
                xyprintf(windowSize.x - windowSize.x / 6 - msgBox[i].Cx, msgBox[i].barY + (screen.y / 40), "%s", msgBox[i].text.c_str());
            }
        }
        settextjustify(LEFT_TEXT, TOP_TEXT);
        return 1;
    }

    vector<string> msg::splitByZero(const string& input) {
        vector<string> result;
        const string delimiter = " 0 ";
        size_t start = 0;
        size_t end = input.find(delimiter);

        // 循环查找所有 " 0 " 分隔符
        while (end != string::npos) {
            // 获取分隔符前的子字符串
            string token = input.substr(start, end - start);

            // 去除前后空格
            token.erase(token.find_last_not_of(' ') + 1);
            token.erase(0, token.find_first_not_of(' '));

            // 添加到结果向量（如果非空）
            if (!token.empty()) {
                result.push_back(token);
            }

            // 更新查找位置
            start = end + delimiter.length();
            end = input.find(delimiter, start);
        }

        // 添加最后一个部分
        string lastToken = input.substr(start);
        lastToken.erase(lastToken.find_last_not_of(' ') + 1);
        lastToken.erase(0, lastToken.find_first_not_of(' '));
        if (!lastToken.empty()) {
            result.push_back(lastToken);
        }

        return result;
    }

    void msg::DrawMessageBar()
    {
        const int CircleSize = 15;


        if (msgBar[0].isActive == true)
        {
            msgBarChoose = -1;
            noBar = false;
            PIMAGE back = newimage();
            POINT windowSize = Window::GetWindowSize();



            if (keystate(0x0d))
            {
                if (ex)
                {
                    msgBarChoose = 0;
                    BarisEXIT = true;
                    msgBar[0].OSme = 0;
                }
            }
            else
            {
                ex = true;
            }

            POINT mousePos = Window::GetMousePosX();
            if (!BarisEXIT)
            {
                if (msgBar[0].OSme < 60)
                {
                    double startValue = 0.0;
                    double endValue = windowSize.x / 15;
                    double t = msgBar[0].OSme / (double)60;
                    msgBar[0].OSSize = startValue + (endValue - startValue) * ease::easeOut(t, 10);
                    msgBar[0].OSme++;
                }
                else
                {
                    msgBar[0].OSSize = windowSize.x / 15;
                }
            }
            else
            {
                if (msgBar[0].OSme < 60)
                {
                    double startValue = 0.0;
                    double endValue = windowSize.x / 15;
                    double t = msgBar[0].OSme / (double)60;
                    msgBar[0].OSSize = startValue + (endValue - startValue) * ease::easeInBack(t, 10);
                    msgBar[0].OSme++;
                }

            }

            if (!BarisEXIT)
            {
                if (DarkMode)
                {
                    setcolor(EGERGBA(0, 0, 0, backAlpha));
                    setfillcolor(EGERGBA(0, 0, 0, backAlpha));
                }
                else
                {
                    setcolor(EGERGBA(150, 150, 150, backAlpha));
                    setfillcolor(EGERGBA(150, 150, 150, backAlpha));
                }
                if (backAlpha < 220)
                {
                    backAlpha += 15;
                }
                ege_fillrect(0, 0, windowSize.x, windowSize.y);

                if (DarkMode)
                {
                    setcolor(EGERGBA(50, 50, 50, backAlpha * 220 / 255));
                    setfillcolor(EGERGBA(50, 50, 50, backAlpha * 220 / 255));
                }
                else
                {
                    setcolor(EGERGBA(220, 220, 220, backAlpha * 220 / 255));
                    setfillcolor(EGERGBA(220, 220, 220, backAlpha * 220 / 255));
                }
                ege_fillroundrect(0 + msgBar[0].OSSize, 0 + msgBar[0].OSSize, windowSize.x - 2 * msgBar[0].OSSize, windowSize.y - 2 * msgBar[0].OSSize, CircleSize);
                setcolor(EGERGB(msgBar[0].color[0], msgBar[0].color[1], msgBar[0].color[2]));
                ege_roundrect(0 + msgBar[0].OSSize, 0 + msgBar[0].OSSize, windowSize.x - 2 * msgBar[0].OSSize, windowSize.y - 2 * msgBar[0].OSSize, CircleSize);

                if (userset[0] != "3")
                {
                    setfont(30, 0, "Aa吧唧一口小可耐");
                }
                else
                {
                    setfont(30, 0, "Ownglyph Soomini Regular");
                }
                int ms = 0;
                for (int i = 0; i < 3; i++)
                {
                    if (msgBar[0].bottom[i] == "NULL")
                    {
                        break;
                    }
                    else
                    {
                        ms = i;
                    }
                }
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                setcolor(EGERGB(msgBar[0].FontColor[0], msgBar[0].FontColor[1], msgBar[0].FontColor[2]));
                if (ms == 0)
                {
                    setfillcolor(EGERGBA(150, 255, 200, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - msgBar[0].OSSize - 300, windowSize.y - msgBar[0].OSSize - 100, 280, 80, CircleSize);
                    if (mousePos.x > windowSize.x - msgBar[0].OSSize - 300 && mousePos.x < windowSize.x - msgBar[0].OSSize - 300 + 280 && mousePos.y > windowSize.y - msgBar[0].OSSize - 100 && mousePos.y < windowSize.y - msgBar[0].OSSize - 100 + 80)
                    {
                        if (keystate(0x01))
                        {
                            msgBarChoose = 0;
                            BarisEXIT = true;
                            msgBar[0].OSme = 0;
                        }
                    }
                    xyprintf(windowSize.x - msgBar[0].OSSize - 300 + 140, windowSize.y - msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[0].c_str());
                }
                if (ms == 1)
                {
                    setfillcolor(EGERGBA(150, 255, 200, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - msgBar[0].OSSize - 300, windowSize.y - msgBar[0].OSSize - 100, 280, 80, CircleSize);
                    setfillcolor(EGERGBA(150, 150, 150, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - msgBar[0].OSSize - 600, windowSize.y - msgBar[0].OSSize - 100, 280, 80, CircleSize);
                    xyprintf(windowSize.x - msgBar[0].OSSize - 300 + 140, windowSize.y - msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[0].c_str());
                    xyprintf(windowSize.x - msgBar[0].OSSize - 600 + 140, windowSize.y - msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[1].c_str());
                    for (int i = 0; i < 2; i++)
                    {
                        if (mousePos.x > windowSize.x - msgBar[0].OSSize - 300 * (i + 1) && mousePos.x < windowSize.x - msgBar[0].OSSize - 300 * (i + 1) + 280 && mousePos.y > windowSize.y - msgBar[0].OSSize - 100 && mousePos.y < windowSize.y - msgBar[0].OSSize - 100 + 80)
                        {
                            if (keystate(0x01))
                            {
                                msgBarChoose = i;
                                BarisEXIT = true;
                                msgBar[0].OSme = 0;
                            }
                        }
                    }
                }
                if (ms == 2)
                {
                    setfillcolor(EGERGBA(150, 255, 200, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - msgBar[0].OSSize - 300, windowSize.y - msgBar[0].OSSize - 100, 280, 80, CircleSize);
                    setfillcolor(EGERGBA(150, 255, 200, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - msgBar[0].OSSize - 600, windowSize.y - msgBar[0].OSSize - 100, 280, 80, CircleSize);
                    setfillcolor(EGERGBA(150, 150, 150, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - msgBar[0].OSSize - 900, windowSize.y - msgBar[0].OSSize - 100, 280, 80, CircleSize);
                    xyprintf(windowSize.x - msgBar[0].OSSize - 300 + 140, windowSize.y - msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[0].c_str());
                    xyprintf(windowSize.x - msgBar[0].OSSize - 600 + 140, windowSize.y - msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[1].c_str());
                    xyprintf(windowSize.x - msgBar[0].OSSize - 900 + 140, windowSize.y - msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[2].c_str());
                    for (int i = 0; i < 3; i++)
                    {
                        if (mousePos.x > windowSize.x - msgBar[0].OSSize - 300 * (i + 1) && mousePos.x < windowSize.x - msgBar[0].OSSize - 300 * (i + 1) + 280 && mousePos.y > windowSize.y - msgBar[0].OSSize - 100 && mousePos.y < windowSize.y - msgBar[0].OSSize - 100 + 80)
                        {
                            if (keystate(0x01))
                            {
                                msgBarChoose = i;
                                BarisEXIT = true;
                                msgBar[0].OSme = 0;
                            }
                        }
                    }
                }

                if (userset[0] != "3")
                {
                    setfont(40, 0, "Aa吧唧一口小可耐");
                }
                else
                {
                    setfont(40, 0, "Ownglyph Soomini Regular");
                }
                setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));

                vector<string> a = splitByZero(msgBar[0].text);
                for (int i = 0; i < a.size(); i++)
                {
                    xyprintf(0 + msgBar[0].OSSize + (windowSize.x - 2 * msgBar[0].OSSize) / 2, 0 + msgBar[0].OSSize + 100 + 40 * i, a[i].c_str());
                }


                settextjustify(LEFT_TEXT, TOP_TEXT);
            }
            else
            {
                if (backAlpha > 0)
                {
                    if (backAlpha - 15 > 0) backAlpha -= 15;
                    else
                    {
                        msgBar[0].isActive = false;
                        BarisEXIT = false;
                        backAlpha = 0;
                        ex = false;
                        noBar = false;
                    }
                }
                else
                {
                    msgBar[0].isActive = false;
                    BarisEXIT = false;
                    backAlpha = 0;
                    ex = false;
                    noBar = false;
                }
                if (DarkMode)
                {
                    setcolor(EGERGBA(0, 0, 0, backAlpha * 220 / 255));
                    setfillcolor(EGERGBA(0, 0, 0, backAlpha * 220 / 255));
                }
                else
                {
                    setcolor(EGERGBA(255, 255, 255, backAlpha * 220 / 255));
                    setfillcolor(EGERGBA(255, 255, 255, backAlpha * 220 / 255));
                }

                ege_fillrect(0, 0, windowSize.x, windowSize.y);


                if (DarkMode)
                {
                    setcolor(EGERGBA(50, 50, 50, backAlpha * 220 / 255));
                    setfillcolor(EGERGBA(50, 50, 50, backAlpha * 220 / 255));
                }
                else
                {
                    setcolor(EGERGBA(220, 220, 220, backAlpha * 220 / 255));
                    setfillcolor(EGERGBA(220, 220, 220, backAlpha * 220 / 255));
                }
                ege_fillroundrect(0 + windowSize.x / 15 - msgBar[0].OSSize, 0 + windowSize.x / 15 - msgBar[0].OSSize, windowSize.x - windowSize.x / 15 * 2 + 2 * msgBar[0].OSSize, windowSize.y - windowSize.x / 15 * 2 + 2 * msgBar[0].OSSize, CircleSize);

                if (userset[0] != "3")
                {
                    setfont(30, 0, "Aa吧唧一口小可耐");
                }
                else
                {
                    setfont(30, 0, "Ownglyph Soomini Regular");
                }
                int ms = 0;
                for (int i = 0; i < 3; i++)
                {
                    if (msgBar[0].bottom[i] == "NULL")
                    {
                        break;
                    }
                    else
                    {
                        ms = i;
                    }
                }
                settextjustify(CENTER_TEXT, CENTER_TEXT);
                setcolor(EGERGB(msgBar[0].FontColor[0], msgBar[0].FontColor[1], msgBar[0].FontColor[2]));
                if (ms == 0)
                {
                    setfillcolor(EGERGBA(150, 255, 200, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 300, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100, 280, 80, CircleSize);
                    if (backAlpha > 50)
                        xyprintf(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 300 + 140, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[0].c_str());
                }
                if (ms == 1)
                {
                    setfillcolor(EGERGBA(150, 255, 200, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 300, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100, 280, 80, CircleSize);
                    setfillcolor(EGERGBA(150, 150, 150, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 600, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100, 280, 80, CircleSize);
                    if (backAlpha > 50)
                    {
                        xyprintf(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 300 + 140, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[0].c_str());
                        xyprintf(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 600 + 140, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[1].c_str());
                    }
                }
                if (ms == 2)
                {
                    setfillcolor(EGERGBA(150, 255, 200, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 300, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100, 280, 80, CircleSize);

                    setfillcolor(EGERGBA(150, 255, 200, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 600, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100, 280, 80, CircleSize);

                    setfillcolor(EGERGBA(150, 150, 150, backAlpha * 220 / 255));
                    ege_fillroundrect(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 900, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100, 280, 80, CircleSize);

                    if (backAlpha > 50)
                    {
                        xyprintf(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 300 + 140, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[0].c_str());
                        xyprintf(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 600 + 140, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[1].c_str());
                        xyprintf(windowSize.x - windowSize.x / 15 + msgBar[0].OSSize - 900 + 140, windowSize.y - windowSize.x / 15 + msgBar[0].OSSize - 100 + 40, msgBar[0].bottom[2].c_str());
                    }
                }

                if (userset[0] != "3")
                {
                    setfont(40, 0, "Aa吧唧一口小可耐");
                }
                else
                {
                    setfont(40, 0, "Ownglyph Soomini Regular");
                }
                setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));
                if (backAlpha > 50)
                {
                    vector<string> a = splitByZero(msgBar[0].text);
                    for (int i = 0; i < a.size(); i++)
                    {
                        xyprintf(0 + msgBar[0].OSSize + (windowSize.x - 2 * msgBar[0].OSSize) / 2, windowSize.x / 15 - msgBar[0].OSSize + 100 + 40 * i, a[i].c_str());
                    }
                }

                settextjustify(LEFT_TEXT, TOP_TEXT);

            }
        }
        else
        {
            noBar = true;
        }
    }

    int msg::GetMessageBarChoose()
    {
        return msgBarChoose;
    }

    bool msg::noBarH()
    {
		return noBar;
    }