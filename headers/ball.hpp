#ifndef BALL_HPP
#define BALL_HPP

#include "raylib.h"
#include "headers/colors.hpp"

enum ScoreUpdate
{
    CPU_WON,
    PLAYER_WON,
    NO_ONE_WON
};

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;
    Texture2D gameBall;

    void setBallTexture(Texture2D ballTexture)
    {
        gameBall = ballTexture;
    }

    void Draw()
    {

        DrawCircle(x, y, radius, Yellow);
        DrawTexture(gameBall, x - 21, y - 21, Yellow);
    }

    ScoreUpdate Update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        // Cpu wins
        if (x + radius >= GetScreenWidth())
        {
            ResetBall();
            return CPU_WON;
        }
        if (x - radius <= 0)
        {
            ResetBall();
            return PLAYER_WON;
        }
        return NO_ONE_WON;
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

#endif