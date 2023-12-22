#ifndef BALL_CPP
#define BALL_CPP

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

    void Draw()
    {
        Texture2D game_ball = LoadTexture("resources/ball 1.png");
        DrawCircle(x, y, radius, Yellow);
        DrawTexture(game_ball, x - 21, y - 21, Yellow);
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