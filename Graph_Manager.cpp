#include "Main.h"
#include "GraphManager.h"

using namespace std;


    void DrawManager::setFillRectColor()
    {
        setlinewidth(1);
        if (DarkMode)
        {
            setcolor(EGERGBA(80, 80, 80, 220));
            setfillcolor(EGERGBA(50, 50, 50, 220));
        }
        else
        {
            setcolor(EGERGBA(230, 230, 230, 220));
            setfillcolor(EGERGBA(200, 200, 200, 220));
        }
    }
    void DrawManager::setFont(int size)
    {
        setcolor(EGERGB(255 - BKcolor, 255 - BKcolor, 255 - BKcolor));
        if (stoi(userset[0]) == 3)
        {
            setfont(size, 0, "Ownglyph Soomini Regular");
        }
        else
        {
            setfont(size, 0, "Aa吧唧一口小可耐");
        }
    }
    void DrawManager::centerText()
    {
        settextjustify(CENTER_TEXT, CENTER_TEXT);
    }
    void DrawManager::startText()
    {
        settextjustify(LEFT_TEXT, TOP_TEXT);
    }
    void DrawManager::fillroundrectwithrect(int x, int y, int w, int h, int r)
        { 
        ege_fillroundrect(x, y, w, h, r);
        ege_roundrect(x, y, w, h, r);
        }
