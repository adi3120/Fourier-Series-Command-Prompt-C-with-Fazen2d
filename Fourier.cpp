#define s_width 200
#define s_height 100
#include "Fazen.hpp"
int main()
{
    float time = 0;
    Fazen2d game;
    game.make2DConsole(8, 8, "Fourier Series");
    TIMEVARS
    float realx = 30;
    float realy = 50;
    std::vector<float> wave{0};
    float radius = 0;
    float m = 4;

    while (1)
    {
        TIMER
        game.background();
        float x = realx;
        float y = realy;
        for (float i = 0; i < m; i++)
        {
            float n = i * 2 + 1;

            float prevx = x;
            float prevy = y;
            radius = 15 * (4 / (n * pi));
            x += radius * cos(n * time);
            y += radius * sin(n * time);
            game.drawCircle(prevx, prevy, radius, whiteF | FOREGROUND_INTENSITY);
            game.drawLine(x, y, prevx, prevy, redF);
        }
        wave.insert(wave.begin(), y);

        for (int i = 0; i < wave.size(); i += 1)
        {
            game.Plot(55 - time * 10, wave[0], blueF);
        }
        //game.drawTextFromInt(60,10,L"Number of Circles : ",m,whiteF|FOREGROUND_INTENSITY);
        game.display();
        time -= 1 * felapsedTime;
        }
}