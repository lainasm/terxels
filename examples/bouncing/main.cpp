#include "../../terxels.h"
#include <cstdlib>
#include <thread>
#include <csignal>

void sigint(int signal)
{
    terxel::ShowCursor();
    std::exit(0);
}

int main()
{
    using namespace std::chrono_literals;
    terxel::Terxture tex(16, 16);

    std::signal(SIGINT, sigint);

    float posX = 0.0f;
    float posY = 0.0f;

    float velX = 0.2f;
    float velY = 0.12f;

    terxel::HideCursor();

    while (true)
    {
        int cursorX;
        int cursorY;
        terxel::GetCursorPosition(cursorX, cursorY);

        posX += velX;
        posY += velY;

        if (posX <= 0.0f || posX >= 15)
        {
            velX *= -1.0f;
        }

        if (posY <= 0.0f || posY >= 15)
        {
            velY *= -1.0f;
        }

        tex.Clear();
        tex.SetPixel(posX, posY, (terxel::Color){0, 0, 255});
        tex.Draw();

        terxel::SetCursorPosition(cursorX, cursorY);

        std::this_thread::sleep_for(16ms);
    }
}
