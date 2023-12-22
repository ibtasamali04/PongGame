#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define NUM_FRAMES 3

#include "headers/ball.hpp"

float screenWidth = 1920;
float screenHeight = 1080;
float playerSpeed = 10;
float ballSpeed = 10;
float buttonWidth = 220;
float buttonHeight = 60;
float buttonSpace = 50;
float instructionTextSpace = 50;
int framesCounter = 0;
bool pause = false;
int score = 10;
float scoretemp = 10;
int playerScore = 0;
int cpuScore = 0;

Rectangle ballButtonRec = {screenWidth - 850, 605, 200, 30};
Rectangle playerButtonRec = {screenWidth - 850, 685, 200, 30};
Rectangle scoreButtonRec = {screenWidth - 850, 765, 200, 30};
Rectangle resetButtonRec = {screenWidth - 790, 835, 80, 40};

class Paddle
{
protected:
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

public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, SkyBlue);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN))
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

class Paddle1 : public Paddle
{
public:
    void Update()
    {
        if (IsKeyDown(KEY_W))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_S))
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
Paddle1 player2;
CpuPaddle cpu;

class Game
{
public:
    int setx = 480;
    int speedX = 2;

    void reset()
    {

        playerScore = 0;
        cpuScore = 0;
        ball.ResetBall();
    }

    void pausemusic(Music music)
    {

        pause = !pause;

        if (pause)
            PauseMusicStream(music);
        else
            ResumeMusicStream(music);
    }

    void vscomp(int ballspeedx, int ballspeedy, int playerspeed, int cpuspeed, Music music)
    {

        ball.radius = 20;
        ball.x = screenWidth / 2;
        ball.y = screenHeight / 2;
        ball.speed_x = ballspeedx;
        ball.speed_y = ballspeedx;

        player.width = 30;
        player.height = 140;
        player.x = screenWidth - player.width - 10;
        player.y = screenHeight / 2 - player.height / 2;
        player.speed = ballspeedx;

        cpu.height = 140;
        cpu.width = 30;
        cpu.x = 10;
        cpu.y = screenHeight / 2 - cpu.height / 2;
        cpu.speed = cpuspeed;

        Sound fxButton = LoadSound("resources/buttonfx.wav"); // Load button sound
        Sound jump = LoadSound("resources/jump.mp3");
        Texture2D endles = LoadTexture("resources/endless.png");
        Texture2D button4 = LoadTexture("resources/back.png");  // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png"); // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/pause.png"); // Load button 6 texture
        float frameHeight1 = (float)button4.height / NUM_FRAMES;
        Rectangle sourceRec1 = {0, 0, (float)button4.width, frameHeight1};
        Rectangle btnBounds4 = {5, 30, (float)button4.width, frameHeight1};
        Rectangle btnBounds5 = {screenWidth - 120, 30, (float)button5.width, frameHeight1};
        Rectangle btnBounds6 = {screenWidth - 245, 30, (float)button6.width, frameHeight1};
        int btnState4 = 0;       // Button 4 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState5 = 0;       // Button 5 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState6 = 0;       // Button 6 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        bool btnAction4 = false; // Button 4 action should be activated
        bool btnAction5 = false; // Button 5 action should be activated
        bool btnAction6 = false; // Button 6 action should be activated

        Vector2 mousePoint = {0.0f, 0.0f};

        while (true)
        {

            mousePoint = GetMousePosition();
            btnAction4 = false;
            btnAction5 = false;
            btnAction6 = false;

            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, btnBounds4))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState4 = 2;
                else
                    btnState4 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction4 = true;
            }
            else
                btnState4 = 0;

            // Check button 5 state
            if (CheckCollisionPointRec(mousePoint, btnBounds5))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState5 = 2;
                else
                    btnState5 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction5 = true;
            }
            else
                btnState5 = 0;

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, btnBounds6))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState6 = 2;
                else
                    btnState6 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction6 = true;
            }
            else
                btnState6 = 0;

            if (btnAction4)
            {
                PlaySound(fxButton);
                break;
            }

            if (btnAction5)
            {
                PlaySound(fxButton);
                pausemusic(music);
            }

            if (btnAction6)
            {
                PlaySound(fxButton);
                guide(music);
            }

            BeginDrawing();

            UpdateMusicStream(music);

            // Updating

            ScoreUpdate update = ball.Update();
            if (update == CPU_WON)
            {
                cpuScore++;
            }
            else if (update == PLAYER_WON)
            {
                playerScore++;
            }

            player.Update();
            cpu.Update(ball.y);

            // Checking for collisions
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpu.x, cpu.y, cpu.width, cpu.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            // Drawing
            ClearBackground(Blue);
            DrawTexture(endles, 0, 0, GRAY);
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
            ball.Draw();
            cpu.Draw();
            player.Draw();
            DrawText(TextFormat("%i", cpuScore), screenWidth / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * frameHeight1;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * frameHeight1;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * frameHeight1;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            if (cpuScore >= score || playerScore >= score)
            {

                wincomp();
            }

            EndDrawing();
        }
    }

    void endless(int ballspeedx, int ballspeedy, int playerspeed, int cpuspeed, Music music)
    {

        ball.radius = 20;
        ball.x = screenWidth / 2;
        ball.y = screenHeight / 2;
        ball.speed_x = ballspeedx;
        ball.speed_y = ballspeedx;

        player.width = 30;
        player.height = 140;
        player.x = screenWidth - player.width - 10;
        player.y = screenHeight / 2 - player.height / 2;
        player.speed = ballspeedx;

        cpu.height = 140;
        cpu.width = 30;
        cpu.x = 10;
        cpu.y = screenHeight / 2 - cpu.height / 2;
        cpu.speed = cpuspeed;

        Sound jump = LoadSound("resources/jump.mp3");
        Texture2D endles = LoadTexture("resources/endless.png");
        Sound fxButton = LoadSound("resources/buttonfx.wav");   // Load button sound
        Texture2D button4 = LoadTexture("resources/back.png");  // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png"); // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/pause.png"); // Load button 6 texture
        float frameHeight1 = (float)button4.height / NUM_FRAMES;
        Rectangle sourceRec1 = {0, 0, (float)button4.width, frameHeight1};

        Rectangle btnBounds4 = {5, 30, (float)button4.width, frameHeight1};
        Rectangle btnBounds5 = {screenWidth - 120, 30, (float)button5.width, frameHeight1};
        Rectangle btnBounds6 = {screenWidth - 245, 30, (float)button6.width, frameHeight1};
        int btnState4 = 0;       // Button 4 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState5 = 0;       // Button 5 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState6 = 0;       // Button 6 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        bool btnAction4 = false; // Button 4 action should be activated
        bool btnAction5 = false; // Button 5 action should be activated
        bool btnAction6 = false; // Button 6 action should be activated

        Vector2 mousePoint = {0.0f, 0.0f};

        while (true)
        {

            mousePoint = GetMousePosition();
            btnAction4 = false;
            btnAction5 = false;
            btnAction6 = false;

            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, btnBounds4))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState4 = 2;
                else
                    btnState4 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction4 = true;
            }
            else
                btnState4 = 0;

            // Check button 5 state
            if (CheckCollisionPointRec(mousePoint, btnBounds5))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState5 = 2;
                else
                    btnState5 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction5 = true;
            }
            else
                btnState5 = 0;

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, btnBounds6))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState6 = 2;
                else
                    btnState6 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction6 = true;
            }
            else
                btnState6 = 0;

            if (btnAction4)
            {
                PlaySound(fxButton);
                break;
            }

            if (btnAction5)
            {
                PlaySound(fxButton);
                pausemusic(music);
            }

            if (btnAction6)
            {
                PlaySound(fxButton);
                guide(music);
            }

            BeginDrawing();

            UpdateMusicStream(music);

            // Updating

            ball.Update();
            player.Update();
            cpu.Update(ball.y);

            // Checking for collisions
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpu.x, cpu.y, cpu.width, cpu.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            // Drawing
            ClearBackground(Blue);
            DrawTexture(endles, 0, 0, GRAY);
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
            ball.Draw();
            cpu.Draw();
            player.Draw();
            DrawText(TextFormat("%i", cpuScore), screenWidth / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * frameHeight1;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * frameHeight1;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * frameHeight1;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void vsplayer(Music music)
    {
        ball.radius = 20;
        ball.x = screenWidth / 2;
        ball.y = screenHeight / 2;
        ball.speed_x = ballSpeed;
        ball.speed_y = ballSpeed;

        player.width = 25;
        player.height = 120;
        player.x = screenWidth - player.width - 10;
        player.y = screenHeight / 2 - player.height / 2;
        player.speed = playerSpeed;

        player2.height = 120;
        player2.width = 25;
        player2.x = 10;
        player2.y = screenHeight / 2 - player2.height / 2;
        player2.speed = playerSpeed;

        Sound jump = LoadSound("resources/jump.mp3");
        Sound fxButton = LoadSound("resources/buttonfx.wav"); // Load button sound
        Texture2D endles = LoadTexture("resources/endless.png");
        Texture2D button4 = LoadTexture("resources/back.png");  // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png"); // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/pause.png"); // Load button 6 texture
        float frameHeight1 = (float)button4.height / NUM_FRAMES;
        Rectangle sourceRec1 = {0, 0, (float)button4.width, frameHeight1};

        Rectangle btnBounds4 = {5, 30, (float)button4.width, frameHeight1};
        Rectangle btnBounds5 = {screenWidth - 120, 30, (float)button5.width, frameHeight1};
        Rectangle btnBounds6 = {screenWidth - 245, 30, (float)button6.width, frameHeight1};
        int btnState4 = 0;       // Button 4 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState5 = 0;       // Button 5 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState6 = 0;       // Button 6 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        bool btnAction4 = false; // Button 4 action should be activated
        bool btnAction5 = false; // Button 5 action should be activated
        bool btnAction6 = false; // Button 6 action should be activated

        Vector2 mousePoint = {0.0f, 0.0f};

        while (true)
        {

            mousePoint = GetMousePosition();
            btnAction4 = false;
            btnAction5 = false;
            btnAction6 = false;

            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, btnBounds4))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState4 = 2;
                else
                    btnState4 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction4 = true;
            }
            else
                btnState4 = 0;

            // Check button 5 state
            if (CheckCollisionPointRec(mousePoint, btnBounds5))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState5 = 2;
                else
                    btnState5 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction5 = true;
            }
            else
                btnState5 = 0;

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, btnBounds6))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState6 = 2;
                else
                    btnState6 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction6 = true;
            }
            else
                btnState6 = 0;

            if (btnAction4)
            {
                PlaySound(fxButton);
                break;
            }

            if (btnAction5)
            {
                PlaySound(fxButton);
                pausemusic(music);
            }

            if (btnAction6)
            {
                PlaySound(fxButton);
                guide(music);
            }

            BeginDrawing();
            UpdateMusicStream(music);

            // Updating

            ball.Update();
            player.Update();
            player2.Update();

            // Checking for collisions
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player2.x, player2.y, player2.width, player2.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            // Drawing
            ClearBackground(Blue);
            DrawTexture(endles, 0, 0, GRAY);
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

            ball.Draw();
            player2.Draw();
            player.Draw();
            DrawText(TextFormat("%i", cpuScore), screenWidth / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * frameHeight1;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * frameHeight1;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * frameHeight1;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            if (cpuScore >= score || playerScore >= score)
            {
                win();
            }

            EndDrawing();
        }
    }

    void loadingscreen(Music music)
    {

        Texture2D pong = LoadTexture("resources/pong 1.png");
        Texture2D ball = LoadTexture("resources/pong 2.png");
        Texture2D bg = LoadTexture("resources/bg.png");

        int ball_x = screenWidth / 2 - 260;
        int ball_y = screenHeight / 2 - 250;
        int ball_speed = 5;

        while (true)
        {
            BeginDrawing();
            UpdateMusicStream(music);

            ball_y += ball_speed;

            if (ball_y >= screenHeight / 2 - 210 || ball_y <= 50)
            {
                ball_speed *= -1;
            }

            ClearBackground(Blue);
            DrawTexture(bg, 0, 0, GRAY);
            DrawTexture(ball, ball_x, ball_y, Yellow);
            DrawTexture(pong, screenWidth / 2 - 260, screenHeight / 2 - 100, WHITE);
            DrawText(TextFormat("LOADING..."), screenWidth - MeasureText("LOADING...", 40) - 30, screenHeight - 60, 40, WHITE);

            framesCounter++;

            if (framesCounter > 200)
            {
                break;
            }

            EndDrawing();
        }
    }

    void guide(Music music)
    {

        Texture2D bg = LoadTexture("resources/bg.png");

        while (true)
        {
            BeginDrawing();
            UpdateMusicStream(music);

            ClearBackground(Blue);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, screenWidth / 3, screenHeight, Blue);

            DrawText(TextFormat("Instructions"), screenWidth / 6 - MeasureText("Instructions", 60) / 2, screenHeight / 6, 60, WHITE);
            DrawText(TextFormat("Player 1 (Right Paddle):"), screenWidth / 12, screenHeight / 4 + 3 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("UP: Arrow up"), screenWidth / 8, screenHeight / 4 + 5 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("DOWN: Arrow down"), screenWidth / 8, screenHeight / 4 + 6 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("Player 2 (Left Paddle):"), screenWidth / 12, screenHeight / 4 + 8 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("UP: W"), screenWidth / 8, screenHeight / 4 + 10 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("DOWN: S"), screenWidth / 8, screenHeight / 4 + 11 * instructionTextSpace, 30, WHITE);

            DrawText(TextFormat("Press ENTER to return"), screenWidth - MeasureText("Press ENTER to return", 30) - instructionTextSpace, screenHeight - instructionTextSpace, 30, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                break;
            }

            EndDrawing();
        }
    }

    void options(Music music, Sound fxButton)
    {

        Texture2D bg = LoadTexture("resources/bg.png");
        Texture2D button4 = LoadTexture("resources/back.png"); // Load button 4 texture
        float frameHeight1 = (float)button4.height / NUM_FRAMES;
        Rectangle sourceRec1 = {0, 0, (float)button4.width, frameHeight1};
        Rectangle btnBounds4 = {50, 30, (float)button4.width, frameHeight1};
        int btnState4 = 0;       // Button 4 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        bool btnAction4 = false; // Button 4 action should be activated
        Vector2 mousePoint = {0.0f, 0.0f};

        while (true)
        {
            BeginDrawing();
            UpdateMusicStream(music);

            mousePoint = GetMousePosition();
            btnAction4 = false;
            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, btnBounds4))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState4 = 2;
                else
                    btnState4 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction4 = true;
            }
            else
                btnState4 = 0;

            if (btnAction4)
            {
                PlaySound(fxButton);
                break;
            }

            ClearBackground(Blue);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(screenWidth / 6, 0, screenWidth - 2 * (screenWidth / 6), screenHeight, Blue);

            DrawText("PONG", screenWidth / 2 - MeasureText("PONG", 140) / 2, screenHeight / 12, 140, WHITE);

            DrawText("Options", screenWidth / 2 - MeasureText("Options", 60) / 2, 330, 60, Yellow);

            DrawText(TextFormat("Ball Speed"), screenWidth / 2 - 300, 600, 40, WHITE);
            DrawText(TextFormat("Paddle Speed"), screenWidth / 2 - 300, 680, 40, WHITE);
            DrawText(TextFormat("Score Points"), screenWidth / 2 - 300, 760, 40, WHITE);
            DrawText(TextFormat("Reset"), screenWidth / 2 - 300, 840, 40, WHITE);

            DrawText(TextFormat("%i", (int)ballSpeed), screenWidth - 630, 610, 20, WHITE);
            DrawText(TextFormat("%i", (int)playerSpeed), screenWidth - 630, 690, 20, WHITE);
            DrawText(TextFormat("%i", score), screenWidth - 630, 770, 20, WHITE);

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * frameHeight1;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            setx += speedX;

            if (setx + MeasureText("Speed settings are only for Two Player Pong", 20) >= screenWidth - screenWidth / 4 || setx <= screenWidth / 4)
            {
                speedX *= -1;
            }

            DrawText(TextFormat("Speed settings are only for Two Player Pong"), setx, screenHeight - 30, 20, WHITE);

            /*if (GuiButton(backbuttonRec, "BACK")) {
            break;
            }*/

            GuiSliderBar(ballButtonRec, NULL, NULL, &ballSpeed, 0, 25);

            GuiSliderBar(playerButtonRec, NULL, NULL, &playerSpeed, 0, 25);

            GuiSliderBar(scoreButtonRec, NULL, NULL, &scoretemp, 0, 50);

            score = (int)scoretemp;

            if (GuiButton(resetButtonRec, "Reset"))
            {

                playerSpeed = 10;
                ballSpeed = 10;
                scoretemp = 10;
            }

            EndDrawing();
        }
    }

    void win()
    {

        Texture2D bg = LoadTexture("resources/bg.png");

        if (playerScore >= score)
        {

            while (true)
            {

                BeginDrawing();

                ClearBackground(Blue);
                DrawTexture(bg, 0, 0, GRAY);
                DrawText(TextFormat("Congradulations!!!"), screenWidth / 2 - MeasureText("Congradulations!!!", 80) / 2, 120, 80, WHITE);
                DrawText(TextFormat("Player 1 wins..."), screenWidth / 2 - MeasureText("Player 1 wins..", 60) / 2, 300, 60, WHITE);
                DrawText(TextFormat("PLAYER 1 SCORE: %i", playerScore), screenWidth / 2 - MeasureText("PLAYER 1 SCORE:   ", 50) / 2, 500, 50, WHITE);
                DrawText(TextFormat("PLAYER 2 SCORE: %i", cpuScore), screenWidth / 2 - MeasureText("PLAYER 2 SCORE:   ", 50) / 2, 600, 50, WHITE);
                DrawText(TextFormat("Press ENTER to return"), screenWidth - MeasureText("Press ENTER to return", 30) - 60, screenHeight - 80, 30, WHITE);

                if (IsKeyPressed(KEY_ENTER))
                {
                    break;
                }

                EndDrawing();
            }

            reset();
        }
        else if (cpuScore >= score)
        {

            while (true)
            {

                BeginDrawing();

                ClearBackground(Blue);
                DrawTexture(bg, 0, 0, GRAY);
                DrawText(TextFormat("Congradulations!!!"), screenWidth / 2 - MeasureText("Congradulations!!!", 80) / 2, 120, 80, WHITE);
                DrawText(TextFormat("Player 2 wins..."), screenWidth / 2 - MeasureText("Player 2 wins..", 60) / 2, 300, 60, WHITE);
                DrawText(TextFormat("PLAYER 1 SCORE: %i", playerScore), screenWidth / 2 - MeasureText("PLAYER 1 SCORE:   ", 50) / 2, 500, 50, WHITE);
                DrawText(TextFormat("PLAYER 2 SCORE: %i", cpuScore), screenWidth / 2 - MeasureText("PLAYER 2 SCORE:   ", 50) / 2, 600, 50, WHITE);
                DrawText(TextFormat("Press ENTER to return"), screenWidth - MeasureText("Press ENTER to return", 30) - 60, screenHeight - 80, 30, WHITE);

                if (IsKeyPressed(KEY_ENTER))
                {
                    break;
                }

                EndDrawing();
            }

            reset();
        }
    }

    void wincomp()
    {

        Texture2D bg = LoadTexture("resources/bg.png");

        if (playerScore >= score)
        {

            while (true)
            {

                BeginDrawing();

                ClearBackground(Blue);
                DrawTexture(bg, 0, 0, GRAY);
                DrawText(TextFormat("Congradulations!!!"), screenWidth / 2 - MeasureText("Congradulations!!!", 80) / 2, 120, 80, WHITE);
                DrawText(TextFormat("Player 1 wins..."), screenWidth / 2 - MeasureText("Player 1 wins..", 60) / 2, 300, 60, WHITE);
                DrawText(TextFormat("YOUR SCORE: %i", playerScore), screenWidth / 2 - MeasureText("YOUR SCORE:   ", 50) / 2, 500, 50, WHITE);
                DrawText(TextFormat("Press ENTER to return"), screenWidth - MeasureText("Press ENTER to return", 30) - 60, screenHeight - 80, 30, WHITE);

                if (IsKeyPressed(KEY_ENTER))
                {
                    break;
                }

                EndDrawing();
            }
            reset();
        }
        else if (cpuScore >= score)
        {

            while (true)
            {

                BeginDrawing();

                ClearBackground(Blue);
                DrawTexture(bg, 0, 0, GRAY);
                DrawText(TextFormat("BETTER LUCK NEXT TIME :)"), screenWidth / 2 - MeasureText("BETTER LUCK NEXT TIME :)", 80) / 2, 120, 80, WHITE);
                DrawText(TextFormat("CPU wins..."), screenWidth / 2 - MeasureText("CPU wins..", 60) / 2, 300, 60, WHITE);
                DrawText(TextFormat("YOUR SCORE: %i", playerScore), screenWidth / 2 - MeasureText("YOUR SCORE:   ", 50) / 2, 500, 50, WHITE);
                DrawText(TextFormat("Press ENTER to return"), screenWidth - MeasureText("Press ENTER to return", 30) - 60, screenHeight - 80, 30, WHITE);

                if (IsKeyPressed(KEY_ENTER))
                {
                    break;
                }

                EndDrawing();
            }

            reset();
        }
    }

    // main functions

    void initwindow()
    {

        InitWindow(screenWidth, screenHeight, "Pong Game");
        InitAudioDevice();
        SetTargetFPS(60);
        // ToggleFullscreen();

        Music music = LoadMusicStream("resources/music.mp3");
        Texture2D bg = LoadTexture("resources/bg.png");

        PlayMusicStream(music);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        loadingscreen(music);

        ClearBackground(Blue);
        DrawTexture(bg, 0, 0, WHITE);
    }

    void compDifficulity(Music music, Texture2D bg, Sound fxButton)
    {

        Texture2D button1 = LoadTexture("resources/easy.png");   // Load button 1 texture
        Texture2D button2 = LoadTexture("resources/medium.png"); // Load button 2 texture
        Texture2D button3 = LoadTexture("resources/hard.png");   // Load button 3 texture
        Texture2D button4 = LoadTexture("resources/back.png");   // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png");  // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/guide.png");  // Load button 6 texture

        // Define frame rectangle for drawing
        float frameHeight = (float)button1.height / NUM_FRAMES;
        float frameHeight1 = (float)button4.height / NUM_FRAMES;

        Rectangle sourceRec = {0, 0, (float)button1.width, frameHeight};
        Rectangle sourceRec1 = {0, 0, (float)button4.width, frameHeight1};

        // Define button bounds on screen
        Rectangle btnBounds1 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + buttonSpace, (float)button1.width, frameHeight};
        Rectangle btnBounds2 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + 3 * buttonSpace, (float)button2.width, frameHeight};
        Rectangle btnBounds3 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + 5 * buttonSpace, (float)button3.width, frameHeight};
        Rectangle btnBounds4 = {5, 30, (float)button4.width, frameHeight1};
        Rectangle btnBounds5 = {screenWidth - 120, 30, (float)button5.width, frameHeight1};
        Rectangle btnBounds6 = {screenWidth - 245, 30, (float)button6.width, frameHeight1};

        int btnState1 = 0; // Button 1 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState2 = 0; // Button 2 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState3 = 0; // Button 3 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState4 = 0; // Button 4 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState5 = 0; // Button 5 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState6 = 0; // Button 6 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED

        bool btnAction1 = false; // Button 1 action should be activated
        bool btnAction2 = false; // Button 2 action should be activated
        bool btnAction3 = false; // Button 3 action should be activated
        bool btnAction4 = false; // Button 4 action should be activated
        bool btnAction5 = false; // Button 5 action should be activated
        bool btnAction6 = false; // Button 6 action should be activated

        Vector2 mousePoint = {0.0f, 0.0f};

        while (!WindowShouldClose()) // Detect window close button or ESC key
        {
            // Update
            //----------------------------------------------------------------------------------
            UpdateMusicStream(music);
            mousePoint = GetMousePosition();
            btnAction1 = false;
            btnAction2 = false;
            btnAction3 = false;
            btnAction4 = false;
            btnAction5 = false;
            btnAction6 = false;

            // Check button 1 state
            if (CheckCollisionPointRec(mousePoint, btnBounds1))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState1 = 2;
                else
                    btnState1 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction1 = true;
            }
            else
                btnState1 = 0;

            // Check button 2 state
            if (CheckCollisionPointRec(mousePoint, btnBounds2))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState2 = 2;
                else
                    btnState2 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction2 = true;
            }
            else
                btnState2 = 0;

            // Check button 3 state
            if (CheckCollisionPointRec(mousePoint, btnBounds3))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState3 = 2;
                else
                    btnState3 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction3 = true;
            }
            else
                btnState3 = 0;

            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, btnBounds4))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState4 = 2;
                else
                    btnState4 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction4 = true;
            }
            else
                btnState4 = 0;

            // Check button 5 state
            if (CheckCollisionPointRec(mousePoint, btnBounds5))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState5 = 2;
                else
                    btnState5 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction5 = true;
            }
            else
                btnState5 = 0;

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, btnBounds6))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState6 = 2;
                else
                    btnState6 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction6 = true;
            }
            else
                btnState6 = 0;

            if (btnAction1)
            {
                PlaySound(fxButton);
                reset();
                vscomp(10, 10, 10, 8, music);
            }

            if (btnAction2)
            {
                PlaySound(fxButton);
                reset();
                vscomp(10, 10, 10, 10, music);
            }

            if (btnAction3)
            {
                PlaySound(fxButton);
                reset();
                vscomp(12, 12, 11, 11, music);
            }

            if (btnAction4)
            {
                PlaySound(fxButton);
                break;
            }

            if (btnAction5)
            {
                PlaySound(fxButton);
                pausemusic(music);
            }

            if (btnAction6)
            {
                PlaySound(fxButton);
                guide(music);
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, screenWidth / 3, screenHeight, Blue);
            DrawText("PONG", screenWidth / 6 - MeasureText("PONG", 140) / 2, screenHeight / 8, 140, WHITE);
            DrawText("Select Difficulity", screenWidth / 6 - MeasureText("Select Difficulity", 60) / 2, screenHeight / 3, 60, Yellow);

            // Calculate button 1 frame rectangle to draw depending on button 1 state
            sourceRec.y = btnState1 * frameHeight;
            DrawTextureRec(button1, sourceRec, (Vector2){btnBounds1.x, btnBounds1.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            sourceRec.y = btnState2 * frameHeight;
            DrawTextureRec(button2, sourceRec, (Vector2){btnBounds2.x, btnBounds2.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            sourceRec.y = btnState3 * frameHeight;
            DrawTextureRec(button3, sourceRec, (Vector2){btnBounds3.x, btnBounds3.y}, WHITE); // Draw button 3 frame

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * frameHeight1;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * frameHeight1;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * frameHeight1;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void endDifficulity(Music music, Texture2D bg, Sound fxButton)
    {

        Texture2D button1 = LoadTexture("resources/easy.png");   // Load button 1 texture
        Texture2D button2 = LoadTexture("resources/medium.png"); // Load button 2 texture
        Texture2D button3 = LoadTexture("resources/hard.png");   // Load button 3 texture
        Texture2D button4 = LoadTexture("resources/back.png");   // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png");  // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/guide.png");  // Load button 6 texture

        // Define frame rectangle for drawing
        float frameHeight = (float)button1.height / NUM_FRAMES;
        float frameHeight1 = (float)button4.height / NUM_FRAMES;

        Rectangle sourceRec = {0, 0, (float)button1.width, frameHeight};
        Rectangle sourceRec1 = {0, 0, (float)button4.width, frameHeight1};

        // Define button bounds on screen
        Rectangle btnBounds1 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + buttonSpace, (float)button1.width, frameHeight};
        Rectangle btnBounds2 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + 3 * buttonSpace, (float)button2.width, frameHeight};
        Rectangle btnBounds3 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + 5 * buttonSpace, (float)button3.width, frameHeight};
        Rectangle btnBounds4 = {5, 30, (float)button4.width, frameHeight1};
        Rectangle btnBounds5 = {screenWidth - 120, 30, (float)button5.width, frameHeight1};
        Rectangle btnBounds6 = {screenWidth - 245, 30, (float)button6.width, frameHeight1};

        int btnState1 = 0; // Button 1 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState2 = 0; // Button 2 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState3 = 0; // Button 3 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState4 = 0; // Button 4 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState5 = 0; // Button 5 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState6 = 0; // Button 6 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED

        bool btnAction1 = false; // Button 1 action should be activated
        bool btnAction2 = false; // Button 2 action should be activated
        bool btnAction3 = false; // Button 3 action should be activated
        bool btnAction4 = false; // Button 4 action should be activated
        bool btnAction5 = false; // Button 5 action should be activated
        bool btnAction6 = false; // Button 6 action should be activated

        Vector2 mousePoint = {0.0f, 0.0f};

        while (!WindowShouldClose()) // Detect window close button or ESC key
        {
            // Update
            //----------------------------------------------------------------------------------
            UpdateMusicStream(music);
            mousePoint = GetMousePosition();
            btnAction1 = false;
            btnAction2 = false;
            btnAction3 = false;
            btnAction4 = false;
            btnAction5 = false;
            btnAction6 = false;

            // Check button 1 state
            if (CheckCollisionPointRec(mousePoint, btnBounds1))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState1 = 2;
                else
                    btnState1 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction1 = true;
            }
            else
                btnState1 = 0;

            // Check button 2 state
            if (CheckCollisionPointRec(mousePoint, btnBounds2))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState2 = 2;
                else
                    btnState2 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction2 = true;
            }
            else
                btnState2 = 0;

            // Check button 3 state
            if (CheckCollisionPointRec(mousePoint, btnBounds3))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState3 = 2;
                else
                    btnState3 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction3 = true;
            }
            else
                btnState3 = 0;

            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, btnBounds4))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState4 = 2;
                else
                    btnState4 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction4 = true;
            }
            else
                btnState4 = 0;

            // Check button 5 state
            if (CheckCollisionPointRec(mousePoint, btnBounds5))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState5 = 2;
                else
                    btnState5 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction5 = true;
            }
            else
                btnState5 = 0;

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, btnBounds6))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState6 = 2;
                else
                    btnState6 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction6 = true;
            }
            else
                btnState6 = 0;

            if (btnAction1)
            {
                PlaySound(fxButton);
                reset();
                endless(10, 10, 10, 8, music);
            }

            if (btnAction2)
            {
                PlaySound(fxButton);
                reset();
                endless(10, 10, 10, 10, music);
            }

            if (btnAction3)
            {
                PlaySound(fxButton);
                reset();
                endless(12, 12, 11, 11, music);
            }

            if (btnAction4)
            {
                PlaySound(fxButton);
                break;
            }

            if (btnAction5)
            {
                PlaySound(fxButton);
                pausemusic(music);
            }

            if (btnAction6)
            {
                PlaySound(fxButton);
                guide(music);
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, screenWidth / 3, screenHeight, Blue);
            DrawText("PONG", screenWidth / 6 - MeasureText("PONG", 140) / 2, screenHeight / 8, 140, WHITE);
            DrawText("Select Difficulity", screenWidth / 6 - MeasureText("Select Difficulity", 60) / 2, screenHeight / 3, 60, Yellow);

            // Calculate button 1 frame rectangle to draw depending on button 1 state
            sourceRec.y = btnState1 * frameHeight;
            DrawTextureRec(button1, sourceRec, (Vector2){btnBounds1.x, btnBounds1.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            sourceRec.y = btnState2 * frameHeight;
            DrawTextureRec(button2, sourceRec, (Vector2){btnBounds2.x, btnBounds2.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            sourceRec.y = btnState3 * frameHeight;
            DrawTextureRec(button3, sourceRec, (Vector2){btnBounds3.x, btnBounds3.y}, WHITE); // Draw button 3 frame

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * frameHeight1;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * frameHeight1;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * frameHeight1;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void mode(Music music, Texture2D bg, Sound fxButton)
    {

        Texture2D button1 = LoadTexture("resources/vsComp.png");   // Load button 1 texture
        Texture2D button2 = LoadTexture("resources/vsHuman.png");  // Load button 2 texture
        Texture2D button3 = LoadTexture("resources/endless1.png"); // Load button 3 texture
        Texture2D button4 = LoadTexture("resources/back.png");     // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png");    // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/guide.png");    // Load button 6 texture

        // Define frame rectangle for drawing
        float frameHeight = (float)button1.height / NUM_FRAMES;
        float frameHeight1 = (float)button4.height / NUM_FRAMES;

        Rectangle sourceRec = {0, 0, (float)button1.width, frameHeight};
        Rectangle sourceRec1 = {0, 0, (float)button4.width, frameHeight1};

        // Define button bounds on screen
        Rectangle btnBounds1 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + buttonSpace, (float)button1.width, frameHeight};
        Rectangle btnBounds2 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + 3 * buttonSpace, (float)button2.width, frameHeight};
        Rectangle btnBounds3 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + 5 * buttonSpace, (float)button3.width, frameHeight};
        Rectangle btnBounds4 = {5, 30, (float)button4.width, frameHeight1};
        Rectangle btnBounds5 = {screenWidth - 120, 30, (float)button5.width, frameHeight1};
        Rectangle btnBounds6 = {screenWidth - 245, 30, (float)button6.width, frameHeight1};

        int btnState1 = 0; // Button 1 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState2 = 0; // Button 2 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState3 = 0; // Button 3 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState4 = 0; // Button 4 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState5 = 0; // Button 5 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState6 = 0; // Button 6 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED

        bool btnAction1 = false; // Button 1 action should be activated
        bool btnAction2 = false; // Button 2 action should be activated
        bool btnAction3 = false; // Button 3 action should be activated
        bool btnAction4 = false; // Button 4 action should be activated
        bool btnAction5 = false; // Button 5 action should be activated
        bool btnAction6 = false; // Button 6 action should be activated

        Vector2 mousePoint = {0.0f, 0.0f};

        while (!WindowShouldClose()) // Detect window close button or ESC key
        {
            // Update
            //----------------------------------------------------------------------------------
            UpdateMusicStream(music);
            mousePoint = GetMousePosition();
            btnAction1 = false;
            btnAction2 = false;
            btnAction3 = false;
            btnAction4 = false;
            btnAction5 = false;
            btnAction6 = false;

            // Check button 1 state
            if (CheckCollisionPointRec(mousePoint, btnBounds1))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState1 = 2;
                else
                    btnState1 = 1;

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    btnAction1 = true;
            }
            else
                btnState1 = 0;

            // Check button 2 state
            if (CheckCollisionPointRec(mousePoint, btnBounds2))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState2 = 2;
                else
                    btnState2 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction2 = true;
            }
            else
                btnState2 = 0;

            // Check button 3 state
            if (CheckCollisionPointRec(mousePoint, btnBounds3))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState3 = 2;
                else
                    btnState3 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction3 = true;
            }
            else
                btnState3 = 0;

            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, btnBounds4))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState4 = 2;
                else
                    btnState4 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction4 = true;
            }
            else
                btnState4 = 0;

            // Check button 5 state
            if (CheckCollisionPointRec(mousePoint, btnBounds5))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState5 = 2;
                else
                    btnState5 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction5 = true;
            }
            else
                btnState5 = 0;

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, btnBounds6))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState6 = 2;
                else
                    btnState6 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction6 = true;
            }
            else
                btnState6 = 0;

            if (btnAction1)
            {
                PlaySound(fxButton);
                compDifficulity(music, bg, fxButton);
            }

            if (btnAction2)
            {
                PlaySound(fxButton);
                vsplayer(music);
            }

            if (btnAction3)
            {
                PlaySound(fxButton);
                endDifficulity(music, bg, fxButton);
            }

            if (btnAction4)
            {
                PlaySound(fxButton);
                break;
            }

            if (btnAction5)
            {
                PlaySound(fxButton);
                pausemusic(music);
            }

            if (btnAction6)
            {
                PlaySound(fxButton);
                guide(music);
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, screenWidth / 3, screenHeight, Blue);
            DrawText("PONG", screenWidth / 6 - MeasureText("PONG", 140) / 2, screenHeight / 8, 140, WHITE);
            DrawText("Select Mode", screenWidth / 6 - MeasureText("Select Mode", 60) / 2, screenHeight / 3, 60, Yellow);

            // Calculate button 1 frame rectangle to draw depending on button 1 state
            sourceRec.y = btnState1 * frameHeight;
            DrawTextureRec(button1, sourceRec, (Vector2){btnBounds1.x, btnBounds1.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            sourceRec.y = btnState2 * frameHeight;
            DrawTextureRec(button2, sourceRec, (Vector2){btnBounds2.x, btnBounds2.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            sourceRec.y = btnState3 * frameHeight;
            DrawTextureRec(button3, sourceRec, (Vector2){btnBounds3.x, btnBounds3.y}, WHITE); // Draw button 3 frame

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * frameHeight1;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * frameHeight1;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * frameHeight1;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void start()
    {

        Sound fxButton = LoadSound("resources/buttonfx.wav");     // Load button sound
        Texture2D button1 = LoadTexture("resources/play.png");    // Load button 1 texture
        Texture2D button2 = LoadTexture("resources/options.png"); // Load button 2 texture
        Texture2D button3 = LoadTexture("resources/exit.png");    // Load button 3 texture
        Texture2D bg = LoadTexture("resources/bg.png");
        Music music = LoadMusicStream("resources/music.mp3");
        PlayMusicStream(music);

        // Define frame rectangle for drawing
        float frameHeight = (float)button1.height / NUM_FRAMES;
        Rectangle sourceRec = {0, 0, (float)button1.width, frameHeight};

        // Define button bounds on screen
        Rectangle btnBounds1 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + buttonSpace, (float)button1.width, frameHeight};
        Rectangle btnBounds2 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + 3 * buttonSpace, (float)button2.width, frameHeight};
        Rectangle btnBounds3 = {screenWidth / 6 - buttonWidth / 2, screenHeight / 2 + 5 * buttonSpace, (float)button3.width, frameHeight};

        int btnState1 = 0; // Button 1 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState2 = 0; // Button 2 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
        int btnState3 = 0; // Button 3 state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED

        bool btnAction1 = false; // Button 1 action should be activated
        bool btnAction2 = false; // Button 2 action should be activated
        bool btnAction3 = false; // Button 3 action should be activated

        Vector2 mousePoint = {0.0f, 0.0f};

        while (!WindowShouldClose()) // Detect window close button or ESC key
        {
            // Update
            //----------------------------------------------------------------------------------
            UpdateMusicStream(music);
            mousePoint = GetMousePosition();
            btnAction1 = false;
            btnAction2 = false;
            btnAction3 = false;

            // Check button 1 state
            if (CheckCollisionPointRec(mousePoint, btnBounds1))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState1 = 2;
                else
                    btnState1 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction1 = true;
            }
            else
                btnState1 = 0;

            // Check button 2 state
            if (CheckCollisionPointRec(mousePoint, btnBounds2))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState2 = 2;
                else
                    btnState2 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction2 = true;
            }
            else
                btnState2 = 0;

            // Check button 3 state
            if (CheckCollisionPointRec(mousePoint, btnBounds3))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState3 = 2;
                else
                    btnState3 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    btnAction3 = true;
            }
            else
                btnState3 = 0;

            if (btnAction1)
            {
                PlaySound(fxButton);
                mode(music, bg, fxButton);
            }

            if (btnAction2)
            {
                PlaySound(fxButton);
                options(music, fxButton);
            }

            if (btnAction3)
            {
                PlaySound(fxButton);
                break;
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, screenWidth / 3, screenHeight, Blue);
            DrawText("PONG", screenWidth / 6 - MeasureText("PONG", 140) / 2, screenHeight / 8, 140, WHITE);
            DrawText("Main Menu", screenWidth / 6 - MeasureText("Main Menu", 60) / 2, screenHeight / 3, 60, Yellow);

            // Calculate button 1 frame rectangle to draw depending on button 1 state
            sourceRec.y = btnState1 * frameHeight;
            DrawTextureRec(button1, sourceRec, (Vector2){btnBounds1.x, btnBounds1.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            sourceRec.y = btnState2 * frameHeight;
            DrawTextureRec(button2, sourceRec, (Vector2){btnBounds2.x, btnBounds2.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            sourceRec.y = btnState3 * frameHeight;
            DrawTextureRec(button3, sourceRec, (Vector2){btnBounds3.x, btnBounds3.y}, WHITE); // Draw button 3 frame

            EndDrawing();
        }
    }
};

Game game;

int main()
{

    game.initwindow();
    game.start();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
