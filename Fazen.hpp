#include <string>
#include <windows.h>
#include <sstream>
#include <vector>
#include <chrono>
#include <cmath>
using std::wstring;
#define redF FOREGROUND_RED | FOREGROUND_INTENSITY
#define redB BACKGROUND_RED | BACKGROUND_INTENSITY
#define blueF FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define blueB BACKGROUNDGROUND_BLUE | BACKGROUND_INTENSITY
#define greenF FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define greenB BACKGROUND_GREEN | BACKGROUND_INTENSITY
#define whiteF FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN
#define whiteB BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY
#define STOP run = false
#define TIMER                                             \
    tp2 = std::chrono::system_clock::now();               \
    std::chrono::duration<float> elapsedTime = tp2 - tp1; \
    tp1 = tp2;                                            \
    float felapsedTime = elapsedTime.count();
#define TIMEVARS                                 \
    auto tp1 = std::chrono::system_clock::now(); \
    auto tp2 = std::chrono::system_clock::now();

const float pi = 3.141592653589793238;
//if some error related to console_font_infoEx occurs comment this part till the last #endif//
typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT FontFamily;
    UINT FontWeight;
    WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

#ifdef __cplusplus
extern "C"
{
#endif
    BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif

class Fazen
{
protected:
    HANDLE outhnd;
    HANDLE inhnd;
    SMALL_RECT rect_win;
    CHAR_INFO *buffscreen = new CHAR_INFO[s_width * s_height];
    COORD characterPos = {0, 0};
    COORD buffersize = {short(s_width), short(s_height)};

public:
    Fazen()
    {
        outhnd = GetStdHandle(STD_OUTPUT_HANDLE);
        inhnd = GetStdHandle(STD_INPUT_HANDLE);

        rect_win = {0, 0, (short)(s_width - 1), short(s_height - 1)};
    }
    const void display()
    {
        WriteConsoleOutputW(outhnd, buffscreen, buffersize, characterPos, &rect_win);
    }
    const void background(short col = 0)
    {
        for (int i = 0; i < s_height; i++)
        {
            for (int j = 0; j < s_width; j++)
            {
                 if (j < 63)
                {
                    buffscreen[j + s_width * i].Char.UnicodeChar = ' ';
                    buffscreen[j + s_width * i].Attributes = col;
                }
            }
        }
    }
    const void Plot(float x, float y, short col = whiteF)
    {
        if (x >= 0 && x <= s_width && y >= 0 && y <= s_height)
        {

            buffscreen[int(x) + s_width * int(y)].Char.UnicodeChar = 0x2588;
            buffscreen[int(x) + s_width * int(y)].Attributes = col;
        }
    }
    void drawLine(int x1, int y1, int x2, int y2, short col = 0x000F)
    {
        int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
        dx = x2 - x1;
        dy = y2 - y1;
        dx1 = std::abs(dx);
        dy1 = std::abs(dy);
        px = 2 * dy1 - dx1;
        py = 2 * dx1 - dy1;
        if (dy1 <= dx1)
        {
            if (dx >= 0)
            {
                x = x1;
                y = y1;
                xe = x2;
            }
            else
            {
                x = x2;
                y = y2;
                xe = x1;
            }

            Plot(x, y, col);

            for (i = 0; x < xe; i++)
            {
                x = x + 1;
                if (px < 0)
                    px = px + 2 * dy1;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                        y = y + 1;
                    else
                        y = y - 1;
                    px = px + 2 * (dy1 - dx1);
                }
                Plot(x, y, col);
            }
        }
        else
        {
            if (dy >= 0)
            {
                x = x1;
                y = y1;
                ye = y2;
            }
            else
            {
                x = x2;
                y = y2;
                ye = y1;
            }

            Plot(x, y, col);

            for (i = 0; y < ye; i++)
            {
                y = y + 1;
                if (py <= 0)
                    py = py + 2 * dx1;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                        x = x + 1;
                    else
                        x = x - 1;
                    py = py + 2 * (dx1 - dy1);
                }
                Plot(x, y, col);
            }
        }
    }
    const void drawTextFromInt(int x, int y, wstring s, int b, short col = whiteF)
    {
        std::wostringstream ws;
        ws << b;

        std::wstring a = s + (ws.str());

        for (size_t i = 0; i < a.size(); i++)
        {
            buffscreen[x + i + s_width * y].Char.UnicodeChar = a[i];
            buffscreen[x + i + s_width * y].Attributes = col;
        }
    }
    const void Text(int x, int y, wstring a, short col = whiteF)
    {
        for (size_t i = 0; i < a.size(); i++)
        {
            buffscreen[x + i + s_width * y].Char.UnicodeChar = a[i];
            buffscreen[x + i + s_width * y].Attributes = col;
        }
    }
}

;

class Fazen2d : public Fazen
{
public:
    const void make2DConsole(int fontw = 8, int fonth = 8, LPCTSTR title = "Your Title Here")
    {

        CONSOLE_FONT_INFOEX fontStructure = {0};
        fontStructure.cbSize = sizeof(fontStructure);
        fontStructure.dwFontSize.X = fontw;
        fontStructure.dwFontSize.Y = fonth;
        fontStructure.FaceName, L"Consolas";
        SetConsoleTitle(title);

        SetCurrentConsoleFontEx(outhnd, true, &fontStructure);

        SetConsoleWindowInfo(outhnd, TRUE, &rect_win);
        SetConsoleScreenBufferSize(outhnd, buffersize);
        //SetConsoleMode(inhnd, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    }

    const void drawRectangle(float x, float y, int b_width, int b_height, float angle = 0, short col = whiteF)
    {
        float tempx;
        float tempy;
        const float xoffset = -b_width / 2;
        const float yoffset = -b_height / 2;
        for (float i = 0; i <= b_height; i += 0.1)
        {
            for (float j = 0; j <= b_width; j += 0.1)
            {
                tempx = ((xoffset + j) * cos(angle) - (yoffset + i) * sin(angle));
                tempy = ((xoffset + j) * sin(angle) + (yoffset + i) * cos(angle));

                Plot(tempx - xoffset + x, tempy - yoffset + y, col);
            }
        }
    }
    const void drawCircle(float xc, float yc, float r, short bordercol = whiteF, bool filled = 0, short innerCol = whiteF)
    {
        float x = -1;
        float y = r;
        if (filled)
        {
            for (float yn = -r; yn < r; yn += 0.1)
                for (float xn = -r; xn < r; xn += 0.1)
                    if (xn * xn + yn * yn < r * r)
                        Plot(xc + xn, yc + yn, innerCol);
        }

        Plot(x + xc, y + yc, bordercol);
        float pk = 3 - 2 * r;
        while (y > x)
        {

            if (pk < 0)
            {
                pk += 4 * x + 6;
                x += 1;
                std::vector<std::pair<float, float>> coords1{{x, y}, {y, x}, {y, -x}, {x, -y}, {-x, -y}, {-y, -x}, {-y, x}, {-x, y}};
                for (auto i : coords1)
                    Plot(i.first + xc, i.second + yc, bordercol);
            }
            else
            {
                pk += 4 * (x - y) + 10;
                x = x + 1;
                y -= 1;
                std::vector<std::pair<float, float>> coords2{{x, y}, {y, x}, {y, -x}, {x, -y}, {-x, -y}, {-y, -x}, {-y, x}, {-x, y}};
                for (auto i : coords2)
                    Plot(i.first + xc, i.second + yc, bordercol);
            }
        }
    }
};
