#include <iostream>
#include <functional>
#include <utility>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "headers/ball.hpp"
#include "headers/paddles.hpp"

enum GameState
{
    LoadingState,
    InitialState,
    ModeState,
    DifficultyState,
    GameRunningState,
    OptionsState,
    PauseState
};

enum ButtonState
{
    IDOL = 0,
    HOVER = 1,
    PRESSED = 2
};

const float SCREEN_WIDTH = 1920;
const float SCREEN_HEIGHT = 1080;
const float smallButtonWidth = 130.0f;
const float largeButtonWidth = 224.0f;
const float smallButtonFrameHeight = 185.0f / 3;
const float largeButtonFrameHeight = 318.0f / 3;
const float listButtonSpace = 50;
const float buttonWidth = 220;
const float buttonHeight = 60;
const int LOADING_DELAY_FRAMES = 15;

Rectangle listButtonRec1 = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 1 * listButtonSpace, largeButtonWidth, largeButtonFrameHeight};
Rectangle listButtonRec2 = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 3 * listButtonSpace, largeButtonWidth, largeButtonFrameHeight};
Rectangle listButtonRec3 = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 5 * listButtonSpace, largeButtonWidth, largeButtonFrameHeight};
Rectangle backButtonRec = {5, 30, smallButtonWidth, smallButtonFrameHeight};
Rectangle pauseButtonRec = {SCREEN_WIDTH - 245, 30, smallButtonWidth, smallButtonFrameHeight};
Rectangle musicButtonRec = {SCREEN_WIDTH - 120, 30, smallButtonWidth, smallButtonFrameHeight};

float playerSpeed = 10;
float ballSpeed = 10;
float instructionTextSpace = 50;
int framesCounter = 0;
int score = 10;
float scoretemp = 10;

Paddle player1Paddle(KEY_UP, KEY_DOWN);
Paddle player2Paddle(KEY_W, KEY_S);
CpuPaddle cpuPaddle;

class Game
{
private:
    Music music;
    Texture2D bg;
    Texture2D pong;
    Texture2D ballTexture;
    Sound buttonClickSound;

public:
    int optionsTwoPlayerInstructionTextX = 480;
    int optionsTwoPlayerInstructionTextSpeedX = 2;
    int playerScore;
    int cpuScore;
    Ball ball;
    bool pause;

    bool isBackButtonPressed;
    bool isMusicButtonPressed;
    bool isGuideButtonPressed;
    bool isPlayButtonPressed;
    bool isOptionButtonPressed;
    bool isExitButtonPressed;
    bool isVsCompButtonPressed;
    bool isVsHumanButtonPressed;
    bool isEndlessButtonPressed;
    bool isEasyButtonPressed;
    bool isMediumButtonPressed;
    bool isHardButtonPressed;

    Game()
    {
        pause = false;
        playerScore = 0;
        cpuScore = 0;

        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong Game");
        InitAudioDevice();
        SetTargetFPS(60);
        ToggleFullscreen();

        music = LoadMusicStream("resources/music.mp3");
        bg = LoadTexture("resources/bg.png");
        pong = LoadTexture("resources/pong 1.png");
        ballTexture = LoadTexture("resources/pong 2.png");
        buttonClickSound = LoadSound("resources/buttonfx.wav");

        ball.setBallTexture(LoadTexture("resources/ball 1.png"));

        PlayMusicStream(music);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

        loadingScreen(
            150, [this]()
            { start(); },
            true);

        ClearBackground(Blue);
        DrawTexture(bg, 0, 0, WHITE);
    }

    void reset()
    {
        playerScore = 0;
        cpuScore = 0;
        ball.ResetBall();
    }

    void toggleMusic()
    {
        pause = !pause;
        if (pause)
            PauseMusicStream(music);
        else
            ResumeMusicStream(music);
    }

    void versesComputerMode(int ballspeedx, int ballspeedy, int playerspeed, int cpuspeed)
    {
        ball.radius = 20;
        ball.x = SCREEN_WIDTH / 2;
        ball.y = SCREEN_HEIGHT / 2;
        ball.speed_x = ballspeedx;
        ball.speed_y = ballspeedx;

        player1Paddle.width = 30;
        player1Paddle.height = 140;
        player1Paddle.x = SCREEN_WIDTH - player1Paddle.width - 10;
        player1Paddle.y = SCREEN_HEIGHT / 2 - player1Paddle.height / 2;
        player1Paddle.speed = ballspeedx;

        cpuPaddle.height = 140;
        cpuPaddle.width = 30;
        cpuPaddle.x = 10;
        cpuPaddle.y = SCREEN_HEIGHT / 2 - cpuPaddle.height / 2;
        cpuPaddle.speed = cpuspeed;

        Sound jump = LoadSound("resources/jump.mp3");
        Texture2D endles = LoadTexture("resources/endless.png");
        Texture2D button4 = LoadTexture("resources/back.png");  // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png"); // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/pause.png"); // Load button 6 texture
        Rectangle sourceRec1 = {0, 0, (float)button4.width, smallButtonFrameHeight};
        Rectangle btnBounds4 = {5, 30, (float)button4.width, smallButtonFrameHeight};
        Rectangle btnBounds5 = {SCREEN_WIDTH - 120, 30, (float)button5.width, smallButtonFrameHeight};
        Rectangle btnBounds6 = {SCREEN_WIDTH - 245, 30, (float)button6.width, smallButtonFrameHeight};
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
            {
                btnState4 = 0;
            }

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
            {
                btnState5 = 0;
            }

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
            {
                btnState6 = 0;
            }

            if (btnAction4)
            {
                PlaySound(buttonClickSound);
                break;
            }

            if (btnAction5)
            {
                PlaySound(buttonClickSound);
                toggleMusic();
            }

            if (btnAction6)
            {
                PlaySound(buttonClickSound);
                guideScreen();
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

            player1Paddle.Update();
            cpuPaddle.Update(ball.y);

            // Checking for collisions
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player1Paddle.x, player1Paddle.y, player1Paddle.width, player1Paddle.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpuPaddle.x, cpuPaddle.y, cpuPaddle.width, cpuPaddle.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            // Drawing
            ClearBackground(Blue);
            DrawTexture(endles, 0, 0, GRAY);
            DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
            ball.Draw();
            cpuPaddle.Draw();
            player1Paddle.Draw();
            DrawText(TextFormat("%i", cpuScore), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore), 3 * SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * smallButtonFrameHeight;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * smallButtonFrameHeight;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * smallButtonFrameHeight;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            if (cpuScore >= score || playerScore >= score)
            {

                versesComputerWinScreen();
            }

            EndDrawing();
        }
    }

    void endlessMode(int ballspeedx, int ballspeedy, int playerspeed, int cpuspeed)
    {

        ball.radius = 20;
        ball.x = SCREEN_WIDTH / 2;
        ball.y = SCREEN_HEIGHT / 2;
        ball.speed_x = ballspeedx;
        ball.speed_y = ballspeedx;

        player1Paddle.width = 30;
        player1Paddle.height = 140;
        player1Paddle.x = SCREEN_WIDTH - player1Paddle.width - 10;
        player1Paddle.y = SCREEN_HEIGHT / 2 - player1Paddle.height / 2;
        player1Paddle.speed = ballspeedx;

        cpuPaddle.height = 140;
        cpuPaddle.width = 30;
        cpuPaddle.x = 10;
        cpuPaddle.y = SCREEN_HEIGHT / 2 - cpuPaddle.height / 2;
        cpuPaddle.speed = cpuspeed;

        Sound jump = LoadSound("resources/jump.mp3");
        Texture2D endles = LoadTexture("resources/endless.png");
        Sound fxButton = LoadSound("resources/buttonfx.wav");   // Load button sound
        Texture2D button4 = LoadTexture("resources/back.png");  // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png"); // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/pause.png"); // Load button 6 texture

        Rectangle sourceRec1 = {0, 0, (float)button4.width, smallButtonFrameHeight};
        Rectangle btnBounds4 = {5, 30, (float)button4.width, smallButtonFrameHeight};
        Rectangle btnBounds5 = {SCREEN_WIDTH - 120, 30, (float)button5.width, smallButtonFrameHeight};
        Rectangle btnBounds6 = {SCREEN_WIDTH - 245, 30, (float)button6.width, smallButtonFrameHeight};
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
                toggleMusic();
            }

            if (btnAction6)
            {
                PlaySound(fxButton);
                guideScreen();
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

            player1Paddle.Update();
            cpuPaddle.Update(ball.y);

            // Checking for collisions
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player1Paddle.x, player1Paddle.y, player1Paddle.width, player1Paddle.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpuPaddle.x, cpuPaddle.y, cpuPaddle.width, cpuPaddle.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            // Drawing
            ClearBackground(Blue);
            DrawTexture(endles, 0, 0, GRAY);
            DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
            ball.Draw();
            cpuPaddle.Draw();
            player1Paddle.Draw();
            DrawText(TextFormat("%i", cpuScore), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore), 3 * SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * smallButtonFrameHeight;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * smallButtonFrameHeight;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * smallButtonFrameHeight;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void versesHumanMode()
    {
        ball.radius = 20;
        ball.x = SCREEN_WIDTH / 2;
        ball.y = SCREEN_HEIGHT / 2;
        ball.speed_x = ballSpeed;
        ball.speed_y = ballSpeed;

        player1Paddle.width = 25;
        player1Paddle.height = 120;
        player1Paddle.x = SCREEN_WIDTH - player1Paddle.width - 10;
        player1Paddle.y = SCREEN_HEIGHT / 2 - player1Paddle.height / 2;
        player1Paddle.speed = playerSpeed;

        player2Paddle.height = 120;
        player2Paddle.width = 25;
        player2Paddle.x = 10;
        player2Paddle.y = SCREEN_HEIGHT / 2 - player2Paddle.height / 2;
        player2Paddle.speed = playerSpeed;

        Sound jump = LoadSound("resources/jump.mp3");
        Sound fxButton = LoadSound("resources/buttonfx.wav"); // Load button sound
        Texture2D endles = LoadTexture("resources/endless.png");
        Texture2D button4 = LoadTexture("resources/back.png");  // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png"); // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/pause.png"); // Load button 6 texture

        Rectangle sourceRec1 = {0, 0, (float)button4.width, smallButtonFrameHeight};
        Rectangle btnBounds4 = {5, 30, (float)button4.width, smallButtonFrameHeight};
        Rectangle btnBounds5 = {SCREEN_WIDTH - 120, 30, (float)button5.width, smallButtonFrameHeight};
        Rectangle btnBounds6 = {SCREEN_WIDTH - 245, 30, (float)button6.width, smallButtonFrameHeight};
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
                toggleMusic();
            }

            if (btnAction6)
            {
                PlaySound(fxButton);
                guideScreen();
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

            player1Paddle.Update();
            player2Paddle.Update();

            // Checking for collisions
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player1Paddle.x, player1Paddle.y, player1Paddle.width, player1Paddle.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player2Paddle.x, player2Paddle.y, player2Paddle.width, player2Paddle.height}))
            {
                ball.speed_x *= -1;
                PlaySound(jump);
            }

            // Drawing
            ClearBackground(Blue);
            DrawTexture(endles, 0, 0, GRAY);
            DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);

            ball.Draw();
            player2Paddle.Draw();
            player1Paddle.Draw();
            DrawText(TextFormat("%i", cpuScore), SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore), 3 * SCREEN_WIDTH / 4 - 20, 20, 80, WHITE);

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * smallButtonFrameHeight;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * smallButtonFrameHeight;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * smallButtonFrameHeight;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            if (cpuScore >= score || playerScore >= score)
            {
                versesHumanWinScreen();
            }

            EndDrawing();
        }
    }

    void loadingScreen(int waitFrames, std::function<void()> function, bool showBall = false)
    {
        int ball_x = SCREEN_WIDTH / 2 - 260;
        int ball_y = SCREEN_HEIGHT / 2 - 250;
        int ball_speed = 5;
        int framesCounter = 0;

        while (true)
        {
            BeginDrawing();
            UpdateMusicStream(music);

            ball_y += ball_speed;

            if (ball_y >= SCREEN_HEIGHT / 2 - 210 || ball_y <= 50)
            {
                ball_speed *= -1;
            }

            ClearBackground(Blue);
            DrawTexture(bg, 0, 0, GRAY);
            if (showBall)
            {
                DrawTexture(ballTexture, ball_x, ball_y, Yellow);
            }
            DrawTexture(pong, SCREEN_WIDTH / 2 - 260, SCREEN_HEIGHT / 2 - 100, WHITE);
            DrawText(TextFormat("LOADING..."), SCREEN_WIDTH - MeasureText("LOADING...", 40) - 30, SCREEN_HEIGHT - 60, 40, WHITE);

            framesCounter++;
            if (framesCounter >= waitFrames)
            {
                break;
            }

            EndDrawing();
        }
        function();
    }

    void guideScreen()
    {
        Texture2D bg = LoadTexture("resources/bg.png");
        while (true)
        {
            BeginDrawing();
            UpdateMusicStream(music);

            ClearBackground(Blue);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT, Blue);

            DrawText(TextFormat("Instructions"), SCREEN_WIDTH / 6 - MeasureText("Instructions", 60) / 2, SCREEN_HEIGHT / 6, 60, WHITE);
            DrawText(TextFormat("Player 1 (Right Paddle):"), SCREEN_WIDTH / 12, SCREEN_HEIGHT / 4 + 3 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("UP: Arrow up"), SCREEN_WIDTH / 8, SCREEN_HEIGHT / 4 + 5 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("DOWN: Arrow down"), SCREEN_WIDTH / 8, SCREEN_HEIGHT / 4 + 6 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("Player 2 (Left Paddle):"), SCREEN_WIDTH / 12, SCREEN_HEIGHT / 4 + 8 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("UP: W"), SCREEN_WIDTH / 8, SCREEN_HEIGHT / 4 + 10 * instructionTextSpace, 30, WHITE);
            DrawText(TextFormat("DOWN: S"), SCREEN_WIDTH / 8, SCREEN_HEIGHT / 4 + 11 * instructionTextSpace, 30, WHITE);

            DrawText(TextFormat("Press ENTER to return"), SCREEN_WIDTH - MeasureText("Press ENTER to return", 30) - instructionTextSpace, SCREEN_HEIGHT - instructionTextSpace, 30, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                break;
            }

            EndDrawing();
        }
    }

    void optionsScreen()
    {

        Rectangle ballButtonRec = {SCREEN_WIDTH - 850, 605, 200, 30};
        Rectangle playerButtonRec = {SCREEN_WIDTH - 850, 685, 200, 30};
        Rectangle scoreButtonRec = {SCREEN_WIDTH - 850, 765, 200, 30};
        Rectangle resetButtonRec = {SCREEN_WIDTH - 790, 835, 80, 40};

        Texture2D bg = LoadTexture("resources/bg.png");
        Texture2D button4 = LoadTexture("resources/back.png"); // Load button 4 texture

        Rectangle sourceRec1 = {0, 0, (float)button4.width, smallButtonFrameHeight};
        Rectangle btnBounds4 = {50, 30, (float)button4.width, smallButtonFrameHeight};
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
                PlaySound(buttonClickSound);
                break;
            }

            ClearBackground(Blue);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(SCREEN_WIDTH / 6, 0, SCREEN_WIDTH - 2 * (SCREEN_WIDTH / 6), SCREEN_HEIGHT, Blue);

            DrawText("PONG", SCREEN_WIDTH / 2 - MeasureText("PONG", 140) / 2, SCREEN_HEIGHT / 12, 140, WHITE);

            DrawText("Options", SCREEN_WIDTH / 2 - MeasureText("Options", 60) / 2, 330, 60, Yellow);

            DrawText(TextFormat("Ball Speed"), SCREEN_WIDTH / 2 - 300, 600, 40, WHITE);
            DrawText(TextFormat("Paddle Speed"), SCREEN_WIDTH / 2 - 300, 680, 40, WHITE);
            DrawText(TextFormat("Score Points"), SCREEN_WIDTH / 2 - 300, 760, 40, WHITE);
            DrawText(TextFormat("Reset"), SCREEN_WIDTH / 2 - 300, 840, 40, WHITE);

            DrawText(TextFormat("%i", (int)ballSpeed), SCREEN_WIDTH - 630, 610, 20, WHITE);
            DrawText(TextFormat("%i", (int)playerSpeed), SCREEN_WIDTH - 630, 690, 20, WHITE);
            DrawText(TextFormat("%i", score), SCREEN_WIDTH - 630, 770, 20, WHITE);

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * smallButtonFrameHeight;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            optionsTwoPlayerInstructionTextX += optionsTwoPlayerInstructionTextSpeedX;

            if (optionsTwoPlayerInstructionTextX + MeasureText("Speed settings are only for Two Player Pong", 20) >= SCREEN_WIDTH - SCREEN_WIDTH / 4 || optionsTwoPlayerInstructionTextX <= SCREEN_WIDTH / 4)
            {
                optionsTwoPlayerInstructionTextSpeedX *= -1;
            }

            DrawText(TextFormat("Speed settings are only for Two Player Pong"), optionsTwoPlayerInstructionTextX, SCREEN_HEIGHT - 30, 20, WHITE);

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

    void versesHumanWinScreen()
    {
        Texture2D bg = LoadTexture("resources/bg.png");

        if (playerScore >= score)
        {

            while (true)
            {

                BeginDrawing();

                ClearBackground(Blue);
                DrawTexture(bg, 0, 0, GRAY);
                DrawText(TextFormat("Congradulations!!!"), SCREEN_WIDTH / 2 - MeasureText("Congradulations!!!", 80) / 2, 120, 80, WHITE);
                DrawText(TextFormat("Player 1 wins..."), SCREEN_WIDTH / 2 - MeasureText("Player 1 wins..", 60) / 2, 300, 60, WHITE);
                DrawText(TextFormat("PLAYER 1 SCORE: %i", playerScore), SCREEN_WIDTH / 2 - MeasureText("PLAYER 1 SCORE:   ", 50) / 2, 500, 50, WHITE);
                DrawText(TextFormat("PLAYER 2 SCORE: %i", cpuScore), SCREEN_WIDTH / 2 - MeasureText("PLAYER 2 SCORE:   ", 50) / 2, 600, 50, WHITE);
                DrawText(TextFormat("Press ENTER to return"), SCREEN_WIDTH - MeasureText("Press ENTER to return", 30) - 60, SCREEN_HEIGHT - 80, 30, WHITE);

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
                DrawText(TextFormat("Congradulations!!!"), SCREEN_WIDTH / 2 - MeasureText("Congradulations!!!", 80) / 2, 120, 80, WHITE);
                DrawText(TextFormat("Player 2 wins..."), SCREEN_WIDTH / 2 - MeasureText("Player 2 wins..", 60) / 2, 300, 60, WHITE);
                DrawText(TextFormat("PLAYER 1 SCORE: %i", playerScore), SCREEN_WIDTH / 2 - MeasureText("PLAYER 1 SCORE:   ", 50) / 2, 500, 50, WHITE);
                DrawText(TextFormat("PLAYER 2 SCORE: %i", cpuScore), SCREEN_WIDTH / 2 - MeasureText("PLAYER 2 SCORE:   ", 50) / 2, 600, 50, WHITE);
                DrawText(TextFormat("Press ENTER to return"), SCREEN_WIDTH - MeasureText("Press ENTER to return", 30) - 60, SCREEN_HEIGHT - 80, 30, WHITE);

                if (IsKeyPressed(KEY_ENTER))
                {
                    break;
                }

                EndDrawing();
            }

            reset();
        }
    }

    void versesComputerWinScreen()
    {
        Texture2D bg = LoadTexture("resources/bg.png");

        if (playerScore >= score)
        {

            while (true)
            {

                BeginDrawing();

                ClearBackground(Blue);
                DrawTexture(bg, 0, 0, GRAY);
                DrawText(TextFormat("Congradulations!!!"), SCREEN_WIDTH / 2 - MeasureText("Congradulations!!!", 80) / 2, 120, 80, WHITE);
                DrawText(TextFormat("Player 1 wins..."), SCREEN_WIDTH / 2 - MeasureText("Player 1 wins..", 60) / 2, 300, 60, WHITE);
                DrawText(TextFormat("YOUR SCORE: %i", playerScore), SCREEN_WIDTH / 2 - MeasureText("YOUR SCORE:   ", 50) / 2, 500, 50, WHITE);
                DrawText(TextFormat("Press ENTER to return"), SCREEN_WIDTH - MeasureText("Press ENTER to return", 30) - 60, SCREEN_HEIGHT - 80, 30, WHITE);

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
                DrawText(TextFormat("BETTER LUCK NEXT TIME :)"), SCREEN_WIDTH / 2 - MeasureText("BETTER LUCK NEXT TIME :)", 80) / 2, 120, 80, WHITE);
                DrawText(TextFormat("CPU wins..."), SCREEN_WIDTH / 2 - MeasureText("CPU wins..", 60) / 2, 300, 60, WHITE);
                DrawText(TextFormat("YOUR SCORE: %i", playerScore), SCREEN_WIDTH / 2 - MeasureText("YOUR SCORE:   ", 50) / 2, 500, 50, WHITE);
                DrawText(TextFormat("Press ENTER to return"), SCREEN_WIDTH - MeasureText("Press ENTER to return", 30) - 60, SCREEN_HEIGHT - 80, 30, WHITE);

                if (IsKeyPressed(KEY_ENTER))
                {
                    break;
                }

                EndDrawing();
            }

            reset();
        }
    }

    void compDifficulity()
    {
        Texture2D button1 = LoadTexture("resources/easy.png");   // Load button 1 texture
        Texture2D button2 = LoadTexture("resources/medium.png"); // Load button 2 texture
        Texture2D button3 = LoadTexture("resources/hard.png");   // Load button 3 texture
        Texture2D button4 = LoadTexture("resources/back.png");   // Load button 4 texture
        Texture2D button5 = LoadTexture("resources/music.png");  // Load button 5 texture
        Texture2D button6 = LoadTexture("resources/guide.png");  // Load button 6 texture

        // Define frame rectangle for drawing
        Rectangle sourceRec = {0, 0, (float)button1.width, largeButtonFrameHeight};
        Rectangle sourceRec1 = {0, 0, (float)button4.width, smallButtonFrameHeight};

        // Define button bounds on screen
        Rectangle btnBounds1 = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + listButtonSpace, (float)button1.width, largeButtonFrameHeight};
        Rectangle btnBounds2 = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 3 * listButtonSpace, (float)button2.width, largeButtonFrameHeight};
        Rectangle btnBounds3 = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 5 * listButtonSpace, (float)button3.width, largeButtonFrameHeight};
        Rectangle btnBounds4 = {5, 30, (float)button4.width, smallButtonFrameHeight};
        Rectangle btnBounds5 = {SCREEN_WIDTH - 120, 30, (float)button5.width, smallButtonFrameHeight};
        Rectangle btnBounds6 = {SCREEN_WIDTH - 245, 30, (float)button6.width, smallButtonFrameHeight};

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
                PlaySound(buttonClickSound);
                reset();
                versesComputerMode(10, 10, 10, 8);
            }

            if (btnAction2)
            {
                PlaySound(buttonClickSound);
                reset();
                versesComputerMode(10, 10, 10, 10);
            }

            if (btnAction3)
            {
                PlaySound(buttonClickSound);
                reset();
                versesComputerMode(12, 12, 11, 11);
            }

            if (btnAction4)
            {
                PlaySound(buttonClickSound);
                break;
            }

            if (btnAction5)
            {
                PlaySound(buttonClickSound);
                toggleMusic();
            }

            if (btnAction6)
            {
                PlaySound(buttonClickSound);
                guideScreen();
            }
            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT, Blue);
            DrawText("PONG", SCREEN_WIDTH / 6 - MeasureText("PONG", 140) / 2, SCREEN_HEIGHT / 8, 140, WHITE);
            DrawText("Select Difficulity", SCREEN_WIDTH / 6 - MeasureText("Select Difficulity", 60) / 2, SCREEN_HEIGHT / 3, 60, Yellow);

            // Calculate button 1 frame rectangle to draw depending on button 1 state
            sourceRec.y = btnState1 * largeButtonFrameHeight;
            DrawTextureRec(button1, sourceRec, (Vector2){btnBounds1.x, btnBounds1.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            sourceRec.y = btnState2 * largeButtonFrameHeight;
            DrawTextureRec(button2, sourceRec, (Vector2){btnBounds2.x, btnBounds2.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            sourceRec.y = btnState3 * largeButtonFrameHeight;
            DrawTextureRec(button3, sourceRec, (Vector2){btnBounds3.x, btnBounds3.y}, WHITE); // Draw button 3 frame

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * smallButtonFrameHeight;
            DrawTextureRec(button4, sourceRec1, (Vector2){btnBounds4.x, btnBounds4.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * smallButtonFrameHeight;
            DrawTextureRec(button5, sourceRec1, (Vector2){btnBounds5.x, btnBounds5.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * smallButtonFrameHeight;
            DrawTextureRec(button6, sourceRec1, (Vector2){btnBounds6.x, btnBounds6.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void endDifficulity()
    {

        Texture2D easyButton = LoadTexture("resources/easy.png");     // Load button 1 texture
        Texture2D mediumButton = LoadTexture("resources/medium.png"); // Load button 2 texture
        Texture2D hardButton = LoadTexture("resources/hard.png");     // Load button 3 texture
        Texture2D backButton = LoadTexture("resources/back.png");     // Load button 4 texture
        Texture2D musicButton = LoadTexture("resources/music.png");   // Load button 5 texture
        Texture2D guideButton = LoadTexture("resources/guide.png");   // Load button 6 texture

        // Define frame rectangle for drawing
        Rectangle sourceRec = {0, 0, (float)easyButton.width, largeButtonFrameHeight};
        Rectangle sourceRec1 = {0, 0, (float)backButton.width, smallButtonFrameHeight};

        // Define button bounds on screen
        Rectangle easyButtonBound = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + listButtonSpace, (float)easyButton.width, largeButtonFrameHeight};
        Rectangle mediumButtonBound = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 3 * listButtonSpace, (float)mediumButton.width, largeButtonFrameHeight};
        Rectangle hardButtonBound = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 5 * listButtonSpace, (float)hardButton.width, largeButtonFrameHeight};
        Rectangle backButtonBound = {5, 30, (float)backButton.width, smallButtonFrameHeight};
        Rectangle musicButtonBound = {SCREEN_WIDTH - 120, 30, (float)musicButton.width, smallButtonFrameHeight};
        Rectangle guideButtonBound = {SCREEN_WIDTH - 245, 30, (float)guideButton.width, smallButtonFrameHeight};

        ButtonState btnState1 = IDOL;
        ButtonState btnState2 = IDOL;
        ButtonState btnState3 = IDOL;
        ButtonState btnState4 = IDOL;
        ButtonState btnState5 = IDOL;
        ButtonState btnState6 = IDOL;

        bool isEasyButtonPressed = false;   // Button 1 action should be activated
        bool isMediumButtonPressed = false; // Button 2 action should be activated
        bool isHardButtonPressed = false;   // Button 3 action should be activated
        bool isBackButtonPressed = false;   // Button 4 action should be activated
        bool isMusicButtonPressed = false;  // Button 5 action should be activated
        bool isGuideButtonPressed = false;  // Button 6 action should be activated

        Vector2 mousePoint = {0.0f, 0.0f};
        while (!WindowShouldClose()) // Detect window close button or ESC key
        {
            // Update
            //----------------------------------------------------------------------------------
            UpdateMusicStream(music);
            mousePoint = GetMousePosition();
            isEasyButtonPressed = false;
            isMediumButtonPressed = false;
            isHardButtonPressed = false;
            isBackButtonPressed = false;
            isMusicButtonPressed = false;
            isGuideButtonPressed = false;

            // Check button 1 state
            if (CheckCollisionPointRec(mousePoint, easyButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState1 = PRESSED;
                else
                    btnState1 = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isEasyButtonPressed = true;
            }
            else
                btnState1 = IDOL;

            // Check button 2 state
            if (CheckCollisionPointRec(mousePoint, mediumButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState2 = PRESSED;
                else
                    btnState2 = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isMediumButtonPressed = true;
            }
            else
                btnState2 = IDOL;

            // Check button 3 state
            if (CheckCollisionPointRec(mousePoint, hardButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState3 = PRESSED;
                else
                    btnState3 = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isHardButtonPressed = true;
            }
            else
                btnState3 = IDOL;

            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, backButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState4 = PRESSED;
                else
                    btnState4 = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isBackButtonPressed = true;
            }
            else
                btnState4 = IDOL;

            // Check button 5 state
            if (CheckCollisionPointRec(mousePoint, musicButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState5 = PRESSED;
                else
                    btnState5 = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isMusicButtonPressed = true;
            }
            else
                btnState5 = IDOL;

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, guideButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    btnState6 = PRESSED;
                else
                    btnState6 = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isGuideButtonPressed = true;
            }
            else
                btnState6 = IDOL;

            if (isEasyButtonPressed)
            {
                PlaySound(buttonClickSound);
                reset();
                endlessMode(10, 10, 10, 8);
            }

            if (isMediumButtonPressed)
            {
                PlaySound(buttonClickSound);
                reset();
                endlessMode(10, 10, 10, 10);
            }

            if (isHardButtonPressed)
            {
                PlaySound(buttonClickSound);
                reset();
                endlessMode(12, 12, 11, 11);
            }

            if (isBackButtonPressed)
            {
                PlaySound(buttonClickSound);
                break;
            }

            if (isMusicButtonPressed)
            {
                PlaySound(buttonClickSound);
                toggleMusic();
            }

            if (isGuideButtonPressed)
            {
                PlaySound(buttonClickSound);
                guideScreen();
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT, Blue);
            DrawText("PONG", SCREEN_WIDTH / 6 - MeasureText("PONG", 140) / 2, SCREEN_HEIGHT / 8, 140, WHITE);
            DrawText("Select Difficulity", SCREEN_WIDTH / 6 - MeasureText("Select Difficulity", 60) / 2, SCREEN_HEIGHT / 3, 60, Yellow);

            // Calculate button 1 frame rectangle to draw depending on button 1 state
            sourceRec.y = btnState1 * largeButtonFrameHeight;
            DrawTextureRec(easyButton, sourceRec, (Vector2){easyButtonBound.x, easyButtonBound.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            sourceRec.y = btnState2 * largeButtonFrameHeight;
            DrawTextureRec(mediumButton, sourceRec, (Vector2){mediumButtonBound.x, mediumButtonBound.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            sourceRec.y = btnState3 * largeButtonFrameHeight;
            DrawTextureRec(hardButton, sourceRec, (Vector2){hardButtonBound.x, hardButtonBound.y}, WHITE); // Draw button 3 frame

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = btnState4 * smallButtonFrameHeight;
            DrawTextureRec(backButton, sourceRec1, (Vector2){backButtonBound.x, backButtonBound.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = btnState5 * smallButtonFrameHeight;
            DrawTextureRec(musicButton, sourceRec1, (Vector2){musicButtonBound.x, musicButtonBound.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = btnState6 * smallButtonFrameHeight;
            DrawTextureRec(guideButton, sourceRec1, (Vector2){guideButtonBound.x, guideButtonBound.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void mode()
    {
        Texture2D vsCompButton = LoadTexture("resources/vsComp.png");
        Texture2D vsHumanButton = LoadTexture("resources/vsHuman.png");
        Texture2D endlessButton = LoadTexture("resources/endless1.png");
        Texture2D backButton = LoadTexture("resources/back.png");
        Texture2D musicButton = LoadTexture("resources/music.png");
        Texture2D guideButton = LoadTexture("resources/guide.png");

        Rectangle largeButtonAreaRec = {0, 0, (float)vsCompButton.width, largeButtonFrameHeight};
        Rectangle smallButtonAreaRec = {0, 0, (float)backButton.width, smallButtonFrameHeight};

        ButtonState vsCompState = IDOL;
        ButtonState vsHumanState = IDOL;
        ButtonState endlessState = IDOL;
        ButtonState backButtonState = IDOL;
        ButtonState musicButtonState = IDOL;
        ButtonState guideButtonState = IDOL;

        Vector2 mousePoint = {0.0f, 0.0f};

        while (!WindowShouldClose()) // Detect window close button or ESC key
        {
            // Update
            //----------------------------------------------------------------------------------
            UpdateMusicStream(music);
            mousePoint = GetMousePosition();
            isVsCompButtonPressed = false;
            isVsHumanButtonPressed = false;
            isEndlessButtonPressed = false;
            isBackButtonPressed = false;
            isMusicButtonPressed = false;
            isGuideButtonPressed = false;

            // Check button 1 state
            if (CheckCollisionPointRec(mousePoint, listButtonRec1))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    vsCompState = PRESSED;
                else
                    vsCompState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isVsCompButtonPressed = true;
            }
            else
            {
                vsCompState = IDOL;
            }

            // Check button 2 state
            if (CheckCollisionPointRec(mousePoint, listButtonRec2))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    vsHumanState = PRESSED;
                else
                    vsHumanState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isVsHumanButtonPressed = true;
            }
            else
            {
                vsHumanState = IDOL;
            }

            // Check button 3 state
            if (CheckCollisionPointRec(mousePoint, listButtonRec3))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    endlessState = PRESSED;
                else
                    endlessState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isEndlessButtonPressed = true;
            }
            else
            {
                endlessState = IDOL;
            }

            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, backButtonRec))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    backButtonState = PRESSED;
                else
                    backButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isBackButtonPressed = true;
            }
            else
            {
                backButtonState = IDOL;
            }

            // Check button 5 state
            if (CheckCollisionPointRec(mousePoint, musicButtonRec))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    musicButtonState = PRESSED;
                else
                    musicButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isMusicButtonPressed = true;
            }
            else
            {
                musicButtonState = IDOL;
            }

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, pauseButtonRec))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    guideButtonState = PRESSED;
                else
                    guideButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isGuideButtonPressed = true;
            }
            else
            {
                guideButtonState = IDOL;
            }

            if (isVsCompButtonPressed)
            {
                PlaySound(buttonClickSound);
                loadingScreen(LOADING_DELAY_FRAMES, [this]()
                              { compDifficulity(); });
            }

            if (isVsHumanButtonPressed)
            {
                PlaySound(buttonClickSound);
                loadingScreen(LOADING_DELAY_FRAMES, [this]()
                              { versesHumanMode(); });
            }

            if (isEndlessButtonPressed)
            {
                PlaySound(buttonClickSound);
                loadingScreen(LOADING_DELAY_FRAMES, [this]()
                              { endDifficulity(); });
            }

            if (isBackButtonPressed)
            {
                PlaySound(buttonClickSound);
                break;
            }

            if (isMusicButtonPressed)
            {
                PlaySound(buttonClickSound);
                toggleMusic();
            }

            if (isGuideButtonPressed)
            {
                PlaySound(buttonClickSound);
                guideScreen();
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT, Blue);
            DrawText("PONG", SCREEN_WIDTH / 6 - MeasureText("PONG", 140) / 2, SCREEN_HEIGHT / 8, 140, WHITE);
            DrawText("Select Mode", SCREEN_WIDTH / 6 - MeasureText("Select Mode", 60) / 2, SCREEN_HEIGHT / 3, 60, Yellow);

            // Calculate button 1 frame rectangle to draw depending on button 1 state
            largeButtonAreaRec.y = vsCompState * largeButtonFrameHeight;
            DrawTextureRec(vsCompButton, largeButtonAreaRec, (Vector2){listButtonRec1.x, listButtonRec1.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            largeButtonAreaRec.y = vsHumanState * largeButtonFrameHeight;
            DrawTextureRec(vsHumanButton, largeButtonAreaRec, (Vector2){listButtonRec2.x, listButtonRec2.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            largeButtonAreaRec.y = endlessState * largeButtonFrameHeight;
            DrawTextureRec(endlessButton, largeButtonAreaRec, (Vector2){listButtonRec3.x, listButtonRec3.y}, WHITE); // Draw button 3 frame

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            smallButtonAreaRec.y = backButtonState * smallButtonFrameHeight;
            DrawTextureRec(backButton, smallButtonAreaRec, (Vector2){backButtonRec.x, backButtonRec.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            smallButtonAreaRec.y = musicButtonState * smallButtonFrameHeight;
            DrawTextureRec(musicButton, smallButtonAreaRec, (Vector2){musicButtonRec.x, musicButtonRec.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            smallButtonAreaRec.y = guideButtonState * smallButtonFrameHeight;
            DrawTextureRec(guideButton, smallButtonAreaRec, (Vector2){pauseButtonRec.x, pauseButtonRec.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void start()
    {
        Texture2D playButton = LoadTexture("resources/play.png");       // Load button 1 texture
        Texture2D optionsButton = LoadTexture("resources/options.png"); // Load button 2 texture
        Texture2D exitButton = LoadTexture("resources/exit.png");       // Load button 3 texture

        PlayMusicStream(music);

        // Define frame rectangle for drawing
        Rectangle sourceRec = {0, 0, (float)playButton.width, largeButtonFrameHeight};

        ButtonState playButtonState = IDOL;
        ButtonState optionButtonState = IDOL;
        ButtonState exitButtonState = IDOL;

        Vector2 mousePoint = {0.0f, 0.0f};

        while (!WindowShouldClose())
        {
            UpdateMusicStream(music);
            mousePoint = GetMousePosition();
            isPlayButtonPressed = false;
            isOptionButtonPressed = false;
            isExitButtonPressed = false;

            if (CheckCollisionPointRec(mousePoint, listButtonRec1))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    playButtonState = PRESSED;
                else
                    playButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isPlayButtonPressed = true;
            }
            else
            {
                playButtonState = IDOL;
            }

            if (CheckCollisionPointRec(mousePoint, listButtonRec2))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    optionButtonState = PRESSED;
                else
                    optionButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isOptionButtonPressed = true;
            }
            else
            {
                optionButtonState = IDOL;
            }

            if (CheckCollisionPointRec(mousePoint, listButtonRec3))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    exitButtonState = PRESSED;
                else
                    exitButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isExitButtonPressed = true;
            }
            else
            {
                exitButtonState = IDOL;
            }

            if (isPlayButtonPressed)
            {
                PlaySound(buttonClickSound);
                loadingScreen(LOADING_DELAY_FRAMES, [this]()
                              { mode(); });
            }

            if (isOptionButtonPressed)
            {
                PlaySound(buttonClickSound);
                loadingScreen(LOADING_DELAY_FRAMES, [this]()
                              { optionsScreen(); });
            }

            if (isExitButtonPressed)
            {
                PlaySound(buttonClickSound);
                break;
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(bg, 0, 0, WHITE);
            DrawRectangle(0, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT, Blue);
            DrawText("PONG", SCREEN_WIDTH / 6 - MeasureText("PONG", 140) / 2, SCREEN_HEIGHT / 8, 140, WHITE);
            DrawText("Main Menu", SCREEN_WIDTH / 6 - MeasureText("Main Menu", 60) / 2, SCREEN_HEIGHT / 3, 60, Yellow);

            // Calculate button 1 frame rectangle to draw depending on button 1 state
            sourceRec.y = playButtonState * largeButtonFrameHeight;
            DrawTextureRec(playButton, sourceRec, (Vector2){listButtonRec1.x, listButtonRec1.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            sourceRec.y = optionButtonState * largeButtonFrameHeight;
            DrawTextureRec(optionsButton, sourceRec, (Vector2){listButtonRec2.x, listButtonRec2.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            sourceRec.y = exitButtonState * largeButtonFrameHeight;
            DrawTextureRec(exitButton, sourceRec, (Vector2){listButtonRec3.x, listButtonRec3.y}, WHITE); // Draw button 3 frame

            EndDrawing();
        }
    }
};

int main()
{
    Game game;
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
