#include "../../terxels.h"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <csignal>
#include <vector>
#include <cmath>

void sigint(int signal)
{
    terxel::ShowCursor();
    std::exit(0);
}

class Ball
{
public:
    Ball()
    {
        this->x = (float)rand() / (float)RAND_MAX * 16.0f;
        this->y = (float)rand() / (float)RAND_MAX * 16.0f;
        this->vx = (float)rand() / (float)RAND_MAX * 0.2f;
        this->vy = (float)rand() / (float)RAND_MAX * 0.2f;
    }

    Ball(float x, float y, float vx, float vy)
    {
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
    }

    void Update()
    {
        x += vx;
        y += vy;

        if (x <= 0.0f || x >= 15)
        {
            vx *= -1.0f;
        }

        if (y <= 0.0f || y >= 15)
        {
            vy *= -1.0f;
        }
    }

    void Draw(terxel::Terxture& tex)
    {
        tex.SetPixel((int)x, (int)y, (terxel::Color){255, 0, 0});
    }

public:
    float x;
    float y;
    float vx;
    float vy;
};

int main()
{
    srand(time(0));

    using namespace std::chrono_literals;
    terxel::Terxture tex(16, 16);

    std::signal(SIGINT, sigint);

    std::vector<Ball> balls;

    for (int i = 0; i < 5; i++)
        balls.push_back({});

    terxel::HideCursor();

    while (true)
    {
        int cursorX;
        int cursorY;
        terxel::GetCursorPosition(cursorX, cursorY);

        for (Ball& b : balls)
        {
            b.Update();
        }

        for (size_t i = 0; i < balls.size(); ++i) {
            for (size_t j = i + 1; j < balls.size(); ++j) {
                Ball& b1 = balls[i];
                Ball& b2 = balls[j];

                float dx = b2.x - b1.x;
                float dy = b2.y - b1.y;
                float distanceSquared = dx * dx + dy * dy;
                float radiiSum = 0.5f + 0.5f;

                if (distanceSquared <= radiiSum * radiiSum) {
                    float distance = std::sqrt(distanceSquared);
                    float nx = dx / distance;  // Normal vector (x component)
                    float ny = dy / distance;  // Normal vector (y component)

                    float dvx = b2.vx - b1.vx;
                    float dvy = b2.vy - b1.vy;

                    float velocityAlongNormal = dvx * nx + dvy * ny;

                    if (velocityAlongNormal > 0) continue;

                    float b1_vn = b1.vx * nx + b1.vy * ny;
                    float b2_vn = b2.vx * nx + b2.vy * ny;

                    float temp = b1_vn;
                    b1_vn = b2_vn;
                    b2_vn = temp;

                    float b1_vt = -b1.vx * ny + b1.vy * nx;
                    float b2_vt = -b2.vx * ny + b2.vy * nx;

                    b1.vx = b1_vn * nx - b1_vt * ny;
                    b1.vy = b1_vn * ny + b1_vt * nx;

                    b2.vx = b2_vn * nx - b2_vt * ny;
                    b2.vy = b2_vn * ny + b2_vt * nx;
                }
            }
        }

        tex.Clear();
        for (Ball& b : balls)
        {
            b.Draw(tex);
        }
        tex.Draw();

        terxel::SetCursorPosition(cursorX, cursorY);

        std::this_thread::sleep_for(16ms);
    }
}
