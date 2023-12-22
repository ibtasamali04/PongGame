#ifndef PADDLES_HPP
#define PADDLES_HPP

#include "raylib.h"
#include "headers/colors.hpp"

class Paddle
{
public:
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

    float x, y;
    float width, height;
    int speed;
    KeyboardKey upKey, downKey;

    Paddle() {}

    Paddle(KeyboardKey upKey, KeyboardKey downKey)
    {
        this->upKey = upKey;
        this->downKey = downKey;
    }

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, SkyBlue);
    }

    void Update()
    {
        if (IsKeyDown(upKey))
        {
            y = y - speed;
        }
        if (IsKeyDown(downKey))
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

#endif