#ifndef GAME_HPP
#define GAME_HPP
// import library
#include <iostream>
#include <functional>
#include <utility>
// import header files
#include "headers/ball.hpp"
#include "headers/paddles.hpp"
#include "headers/rectangles.hpp"

float playerSpeed = 10;
float ballSpeed = 10;
float instructionTextSpace = 50;
int winScore = 10;
float tempWinScore = 10;

enum ButtonState // for button state
{
    IDOL = 0,
    HOVER = 1,
    PRESSED = 2
};

// instances for paddles
Paddle player1Paddle(KEY_UP, KEY_DOWN);
Paddle player2Paddle(KEY_W, KEY_S);
CpuPaddle cpuPaddle;

class Game
{
private:
    // texture and sound variable
    Music music;
    Texture2D bg;
    Texture2D pong;
    Texture2D ballTexture;
    Texture2D playButton;
    Texture2D optionsButton;
    Texture2D exitButton;
    Texture2D vsCompButton;
    Texture2D vsHumanButton;
    Texture2D endlessButton;
    Texture2D backButton;
    Texture2D musicButton;
    Texture2D guideButton;
    Texture2D easyButton;
    Texture2D mediumButton;
    Texture2D hardButton;
    Texture2D endles;
    Texture2D resetButton;
    Sound buttonClickSound;
    Sound jump;
    Ball ball; // object for ball

    int optionsTwoPlayerInstructionTextX = 480; // text movement in option screen
    int optionsTwoPlayerInstructionTextSpeedX = 2;
    int playerScore; // score variables
    int cpuScore;
    // bools for buttons
    bool pauseGame;
    bool pauseMusic;
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
    bool isResetButtonPressed;

public:
    Game()
    {
        pauseMusic = false;
        pauseGame = false;
        playerScore = 0;
        cpuScore = 0;

        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong Game"); // initialize window
        InitAudioDevice();                                    // initialize audio device
        SetTargetFPS(60);                                     // set fps
        ToggleFullscreen();                                   // enter full screen
        // load texture
        bg = LoadTexture("resources/bg.png");
        pong = LoadTexture("resources/pong 1.png");
        ballTexture = LoadTexture("resources/pong 2.png");
        playButton = LoadTexture("resources/play.png");
        optionsButton = LoadTexture("resources/options.png");
        exitButton = LoadTexture("resources/exit.png");
        vsCompButton = LoadTexture("resources/vsComp.png");
        vsHumanButton = LoadTexture("resources/vsHuman.png");
        endlessButton = LoadTexture("resources/endless1.png");
        easyButton = LoadTexture("resources/easy.png");
        mediumButton = LoadTexture("resources/medium.png");
        hardButton = LoadTexture("resources/hard.png");
        backButton = LoadTexture("resources/back.png");
        musicButton = LoadTexture("resources/music.png");
        guideButton = LoadTexture("resources/guide.png");
        endles = LoadTexture("resources/endless.png");
        resetButton = LoadTexture("resources/reset1.png");

        jump = LoadSound("resources/jump.mp3");
        music = LoadMusicStream("resources/music.mp3");
        buttonClickSound = LoadSound("resources/buttonfx.wav");

        ball.setBallTexture(LoadTexture("resources/ball 1.png"));

        PlayMusicStream(music);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0xFFFFFF00);
        GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0XF9CC0FFF);
        GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0XFC9C0FFF);

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
        pauseMusic = !pauseMusic;
        if (pauseMusic)
            PauseMusicStream(music);
        else
            ResumeMusicStream(music);
    }

    void versesComputerMode(int ballSpeedX, int ballSpeedY, int playerSpeed, int cpuSpeed)
    {
        ball.radius = 20;
        ball.speed_x = ballSpeedX;
        ball.speed_y = ballSpeedY;

        player1Paddle.width = 30;
        player1Paddle.height = 140;
        player1Paddle.x = SCREEN_WIDTH - player1Paddle.width - 10;
        player1Paddle.y = SCREEN_HEIGHT / 2 - player1Paddle.height / 2;
        player1Paddle.speed = ballSpeedX;

        cpuPaddle.height = 140;
        cpuPaddle.width = 30;
        cpuPaddle.x = 10;
        cpuPaddle.y = SCREEN_HEIGHT / 2 - cpuPaddle.height / 2;
        cpuPaddle.speed = cpuSpeed;

        Rectangle sourceRec1 = {0, 0, (float)backButton.width, smallButtonFrameHeight};

        ButtonState musicButtonState = IDOL;
        ButtonState guideButtonState = IDOL;
        ButtonState backButtonState = IDOL;

        Vector2 mousePoint = {0.0f, 0.0f};

        while (true)
        {
            UpdateMusicStream(music);
            if (IsKeyPressed(KEY_P))
            {
                pauseGame = !pauseGame;
            }

            if (pauseGame)
            
            {
                pauseScreen();
            }

            if (!pauseGame)
            {

                mousePoint = GetMousePosition();
                isBackButtonPressed = false;
                isMusicButtonPressed = false;
                isGuideButtonPressed = false;

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
                if (CheckCollisionPointRec(mousePoint, guideButtonRec))
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

                BeginDrawing();

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
                sourceRec1.y = backButtonState * smallButtonFrameHeight;
                DrawTextureRec(backButton, sourceRec1, (Vector2){backButtonRec.x, backButtonRec.y}, WHITE); // Draw button 4 frame

                // Calculate button 5 frame rectangle to draw depending on button 5 state
                sourceRec1.y = musicButtonState * smallButtonFrameHeight;
                DrawTextureRec(musicButton, sourceRec1, (Vector2){musicButtonRec.x, musicButtonRec.y}, WHITE); // Draw button 5 frame

                // Calculate button 6 frame rectangle to draw depending on button 6 state
                sourceRec1.y = guideButtonState * smallButtonFrameHeight;
                DrawTextureRec(guideButton, sourceRec1, (Vector2){guideButtonRec.x, guideButtonRec.y}, WHITE); // Draw button 6 frame

                if (cpuScore >= winScore || playerScore >= winScore)
                {

                    versesComputerWinScreen();
                }

                EndDrawing();
            }
        }
    }

    void endlessMode(int ballSpeedX, int ballSpeedY, int playerSpeed, int cpuSpeed)
    {
        ball.radius = 20;
        ball.x = SCREEN_WIDTH / 2;
        ball.y = SCREEN_HEIGHT / 2;
        ball.speed_x = ballSpeedX;
        ball.speed_y = ballSpeedY;

        player1Paddle.width = 30;
        player1Paddle.height = 140;
        player1Paddle.x = SCREEN_WIDTH - player1Paddle.width - 10;
        player1Paddle.y = SCREEN_HEIGHT / 2 - player1Paddle.height / 2;
        player1Paddle.speed = ballSpeedX;

        cpuPaddle.height = 140;
        cpuPaddle.width = 30;
        cpuPaddle.x = 10;
        cpuPaddle.y = SCREEN_HEIGHT / 2 - cpuPaddle.height / 2;
        cpuPaddle.speed = cpuSpeed;

        Rectangle sourceRec1 = {0, 0, (float)backButton.width, smallButtonFrameHeight};

        ButtonState backButtonState = IDOL;
        ButtonState musicButtonState = IDOL;
        ButtonState guideButtonState = IDOL;

        Vector2 mousePoint = {0.0f, 0.0f};

        while (true)
        {
            UpdateMusicStream(music);
            if (IsKeyPressed(KEY_P))
            {
                pauseGame = !pauseGame;
            }

            if (pauseGame)
            {
                pauseScreen();
            }

            if (!pauseGame)
            {

                mousePoint = GetMousePosition();
                isBackButtonPressed = false;
                isMusicButtonPressed = false;
                isGuideButtonPressed = false;

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
                    backButtonState = IDOL;

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
                    musicButtonState = IDOL;

                // Check button 6 state
                if (CheckCollisionPointRec(mousePoint, guideButtonRec))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        guideButtonState = PRESSED;
                    else
                        guideButtonState = HOVER;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                        isGuideButtonPressed = true;
                }
                else
                    guideButtonState = IDOL;

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

                BeginDrawing();
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
                sourceRec1.y = backButtonState * smallButtonFrameHeight;
                DrawTextureRec(backButton, sourceRec1, (Vector2){backButtonRec.x, backButtonRec.y}, WHITE); // Draw button 4 frame

                // Calculate button 5 frame rectangle to draw depending on button 5 state
                sourceRec1.y = musicButtonState * smallButtonFrameHeight;
                DrawTextureRec(musicButton, sourceRec1, (Vector2){musicButtonRec.x, musicButtonRec.y}, WHITE); // Draw button 5 frame

                // Calculate button 6 frame rectangle to draw depending on button 6 state
                sourceRec1.y = guideButtonState * smallButtonFrameHeight;
                DrawTextureRec(guideButton, sourceRec1, (Vector2){guideButtonRec.x, guideButtonRec.y}, WHITE); // Draw button 6 frame

                EndDrawing();
            }
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

        Rectangle sourceRec1 = {0, 0, (float)backButton.width, smallButtonFrameHeight};

        ButtonState backButtonState = IDOL;
        ButtonState musicButtonState = IDOL;
        ButtonState guideButtonState = IDOL;

        Vector2 mousePoint = {0.0f, 0.0f};

        while (true)
        {
            UpdateMusicStream(music);
            if (IsKeyPressed(KEY_P))
            {
                pauseGame = !pauseGame;
            }

            if (pauseGame)
            {
                pauseScreen();
            }

            if (!pauseGame)
            {

                mousePoint = GetMousePosition();
                isBackButtonPressed = false;
                isMusicButtonPressed = false;
                isGuideButtonPressed = false;

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
                    backButtonState = IDOL;

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
                    musicButtonState = IDOL;

                // Check button 6 state
                if (CheckCollisionPointRec(mousePoint, guideButtonRec))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        guideButtonState = PRESSED;
                    else
                        guideButtonState = HOVER;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                        isGuideButtonPressed = true;
                }
                else
                    guideButtonState = IDOL;

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

                BeginDrawing();
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
                sourceRec1.y = backButtonState * smallButtonFrameHeight;
                DrawTextureRec(backButton, sourceRec1, (Vector2){backButtonRec.x, backButtonRec.y}, WHITE); // Draw button 4 frame

                // Calculate button 5 frame rectangle to draw depending on button 5 state
                sourceRec1.y = musicButtonState * smallButtonFrameHeight;
                DrawTextureRec(musicButton, sourceRec1, (Vector2){musicButtonRec.x, musicButtonRec.y}, WHITE); // Draw button 5 frame

                // Calculate button 6 frame rectangle to draw depending on button 6 state
                sourceRec1.y = guideButtonState * smallButtonFrameHeight;
                DrawTextureRec(guideButton, sourceRec1, (Vector2){guideButtonRec.x, guideButtonRec.y}, WHITE); // Draw button 6 frame

                if (cpuScore >= winScore || playerScore >= winScore)
                {
                    versesHumanWinScreen();
                }

                EndDrawing();
            }
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
        while (!WindowShouldClose())
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
            DrawText(TextFormat("Pause: P"), SCREEN_WIDTH / 12, SCREEN_HEIGHT / 4 + 13 * instructionTextSpace, 30, WHITE);

            DrawText(TextFormat("Press ENTER to return"), SCREEN_WIDTH - MeasureText("Press ENTER to return", 30) - instructionTextSpace, SCREEN_HEIGHT - instructionTextSpace, 30, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                break;
            }

            EndDrawing();
        }
    }

    void pauseScreen()
    {
        BeginDrawing();
        PauseMusicStream(music);
        DrawText("GAME IS PAUSED", SCREEN_WIDTH / 2 - MeasureText("GAME IS PAUSED", 50) / 2, SCREEN_HEIGHT / 2, 50, WHITE);
        EndDrawing();
    }

    void optionsScreen()
    {
        Rectangle sourceRec1 = {0, 0, (float)backButton.width, smallButtonFrameHeight};

        ButtonState backButtonState = IDOL;
        ButtonState resetButtonState = IDOL;

        Vector2 mousePoint = {0.0f, 0.0f};

        while (!WindowShouldClose())
        {
            BeginDrawing();
            UpdateMusicStream(music);

            mousePoint = GetMousePosition();
            isBackButtonPressed = false;
            isResetButtonPressed = false;

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
                backButtonState = IDOL;

            if (isBackButtonPressed)
            {
                PlaySound(buttonClickSound);
                break;
            }

            if (CheckCollisionPointRec(mousePoint, resetButtonRec))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    resetButtonState = PRESSED;
                else
                    resetButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isResetButtonPressed = true;
            }
            else
                resetButtonState = IDOL;

            if (isResetButtonPressed)
            {
                PlaySound(buttonClickSound);
                playerSpeed = 10;
                ballSpeed = 10;
                tempWinScore = 10;
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
            DrawText(TextFormat("%i", winScore), SCREEN_WIDTH - 630, 770, 20, WHITE);

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = backButtonState * smallButtonFrameHeight;
            DrawTextureRec(backButton, sourceRec1, (Vector2){backButtonRec.x, backButtonRec.y}, WHITE); // Draw button 4 frame
            sourceRec1.y = resetButtonState * smallButtonFrameHeight;
            DrawTextureRec(resetButton, sourceRec1, (Vector2){resetButtonRec.x, resetButtonRec.y}, WHITE); // Draw button 4 frame

            optionsTwoPlayerInstructionTextX += optionsTwoPlayerInstructionTextSpeedX;

            if (optionsTwoPlayerInstructionTextX + MeasureText("Speed settings are only for Two Player Pong", 20) >= SCREEN_WIDTH - SCREEN_WIDTH / 4 || optionsTwoPlayerInstructionTextX <= SCREEN_WIDTH / 4)
            {
                optionsTwoPlayerInstructionTextSpeedX *= -1;
            }

            DrawText(TextFormat("Speed settings are only for Two Player Pong"), optionsTwoPlayerInstructionTextX, SCREEN_HEIGHT - 30, 20, WHITE);
            GuiSliderBar(ballButtonRec, NULL, NULL, &ballSpeed, 0, 25);

            GuiSliderBar(playerButtonRec, NULL, NULL, &playerSpeed, 0, 25);

            GuiSliderBar(scoreButtonRec, NULL, NULL, &tempWinScore, 0, 50);

            winScore = (int)tempWinScore;

            EndDrawing();
        }
    }

    void versesHumanWinScreen()
    {
        Texture2D bg = LoadTexture("resources/bg.png");

        if (playerScore >= winScore)
        {

            while (!WindowShouldClose())
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
        else if (cpuScore >= winScore)
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

        if (playerScore >= winScore)
        {

            while (!WindowShouldClose())
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
        else if (cpuScore >= winScore)
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
        Rectangle sourceRec = {0, 0, (float)easyButton.width, largeButtonFrameHeight};
        Rectangle sourceRec1 = {0, 0, (float)backButton.width, smallButtonFrameHeight};

        ButtonState easyButtonState = IDOL;
        ButtonState mediumButtonState = IDOL;
        ButtonState hardButtonState = IDOL;
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
            isEasyButtonPressed = false;
            isMediumButtonPressed = false;
            isHardButtonPressed = false;
            isBackButtonPressed = false;
            isMusicButtonPressed = false;
            isGuideButtonPressed = false;

            // Check button 1 state
            if (CheckCollisionPointRec(mousePoint, listButtonRec1))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    easyButtonState = PRESSED;
                else
                    easyButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isEasyButtonPressed = true;
            }
            else
                easyButtonState = IDOL;

            // Check button 2 state
            if (CheckCollisionPointRec(mousePoint, listButtonRec2))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    mediumButtonState = PRESSED;
                else
                    mediumButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isMediumButtonPressed = true;
            }
            else
                mediumButtonState = IDOL;

            // Check button 3 state
            if (CheckCollisionPointRec(mousePoint, listButtonRec3))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    hardButtonState = PRESSED;
                else
                    hardButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isHardButtonPressed = true;
            }
            else
                hardButtonState = IDOL;

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
                backButtonState = IDOL;

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
                musicButtonState = IDOL;

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, guideButtonRec))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    guideButtonState = PRESSED;
                else
                    guideButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isGuideButtonPressed = true;
            }
            else
                guideButtonState = IDOL;

            if (isEasyButtonPressed)
            {
                PlaySound(buttonClickSound);
                reset();
                versesComputerMode(10, 10, 10, 8);
            }

            if (isMediumButtonPressed)
            {
                PlaySound(buttonClickSound);
                reset();
                versesComputerMode(10, 10, 10, 10);
            }

            if (isHardButtonPressed)
            {
                PlaySound(buttonClickSound);
                reset();
                versesComputerMode(12, 12, 11, 11);
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
            sourceRec.y = easyButtonState * largeButtonFrameHeight;
            DrawTextureRec(easyButton, sourceRec, (Vector2){listButtonRec1.x, listButtonRec1.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            sourceRec.y = mediumButtonState * largeButtonFrameHeight;
            DrawTextureRec(mediumButton, sourceRec, (Vector2){listButtonRec2.x, listButtonRec2.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            sourceRec.y = hardButtonState * largeButtonFrameHeight;
            DrawTextureRec(hardButton, sourceRec, (Vector2){listButtonRec3.x, listButtonRec3.y}, WHITE); // Draw button 3 frame

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = backButtonState * smallButtonFrameHeight;
            DrawTextureRec(backButton, sourceRec1, (Vector2){backButtonRec.x, backButtonRec.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = musicButtonState * smallButtonFrameHeight;
            DrawTextureRec(musicButton, sourceRec1, (Vector2){musicButtonRec.x, musicButtonRec.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = guideButtonState * smallButtonFrameHeight;
            DrawTextureRec(guideButton, sourceRec1, (Vector2){guideButtonRec.x, guideButtonRec.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void endDifficulity()
    {
        Rectangle sourceRec = {0, 0, (float)easyButton.width, largeButtonFrameHeight};
        Rectangle sourceRec1 = {0, 0, (float)backButton.width, smallButtonFrameHeight};

        // Define button bounds on screen
        Rectangle easyButtonBound = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + listButtonSpace, (float)easyButton.width, largeButtonFrameHeight};
        Rectangle mediumButtonBound = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 3 * listButtonSpace, (float)mediumButton.width, largeButtonFrameHeight};
        Rectangle hardButtonBound = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 5 * listButtonSpace, (float)hardButton.width, largeButtonFrameHeight};
        Rectangle backButtonBound = {5, 30, (float)backButton.width, smallButtonFrameHeight};
        Rectangle musicButtonBound = {SCREEN_WIDTH - 120, 30, (float)musicButton.width, smallButtonFrameHeight};
        Rectangle guideButtonBound = {SCREEN_WIDTH - 245, 30, (float)guideButton.width, smallButtonFrameHeight};

        ButtonState easyButtonState = IDOL;
        ButtonState mediumButtonState = IDOL;
        ButtonState hardButtonState = IDOL;
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
                    easyButtonState = PRESSED;
                else
                    easyButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isEasyButtonPressed = true;
            }
            else
                easyButtonState = IDOL;

            // Check button 2 state
            if (CheckCollisionPointRec(mousePoint, mediumButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    mediumButtonState = PRESSED;
                else
                    mediumButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isMediumButtonPressed = true;
            }
            else
                mediumButtonState = IDOL;

            // Check button 3 state
            if (CheckCollisionPointRec(mousePoint, hardButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    hardButtonState = PRESSED;
                else
                    hardButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isHardButtonPressed = true;
            }
            else
                hardButtonState = IDOL;

            // Check button 4 state
            if (CheckCollisionPointRec(mousePoint, backButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    backButtonState = PRESSED;
                else
                    backButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isBackButtonPressed = true;
            }
            else
                backButtonState = IDOL;

            // Check button 5 state
            if (CheckCollisionPointRec(mousePoint, musicButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    musicButtonState = PRESSED;
                else
                    musicButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isMusicButtonPressed = true;
            }
            else
                musicButtonState = IDOL;

            // Check button 6 state
            if (CheckCollisionPointRec(mousePoint, guideButtonBound))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    guideButtonState = PRESSED;
                else
                    guideButtonState = HOVER;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    isGuideButtonPressed = true;
            }
            else
                guideButtonState = IDOL;

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
            sourceRec.y = easyButtonState * largeButtonFrameHeight;
            DrawTextureRec(easyButton, sourceRec, (Vector2){easyButtonBound.x, easyButtonBound.y}, WHITE); // Draw button 1 frame

            // Calculate button 2 frame rectangle to draw depending on button 2 state
            sourceRec.y = mediumButtonState * largeButtonFrameHeight;
            DrawTextureRec(mediumButton, sourceRec, (Vector2){mediumButtonBound.x, mediumButtonBound.y}, WHITE); // Draw button 2 frame

            // Calculate button 3 frame rectangle to draw depending on button 3 state
            sourceRec.y = hardButtonState * largeButtonFrameHeight;
            DrawTextureRec(hardButton, sourceRec, (Vector2){hardButtonBound.x, hardButtonBound.y}, WHITE); // Draw button 3 frame

            // Calculate button 4 frame rectangle to draw depending on button 4 state
            sourceRec1.y = backButtonState * smallButtonFrameHeight;
            DrawTextureRec(backButton, sourceRec1, (Vector2){backButtonBound.x, backButtonBound.y}, WHITE); // Draw button 4 frame

            // Calculate button 5 frame rectangle to draw depending on button 5 state
            sourceRec1.y = musicButtonState * smallButtonFrameHeight;
            DrawTextureRec(musicButton, sourceRec1, (Vector2){musicButtonBound.x, musicButtonBound.y}, WHITE); // Draw button 5 frame

            // Calculate button 6 frame rectangle to draw depending on button 6 state
            sourceRec1.y = guideButtonState * smallButtonFrameHeight;
            DrawTextureRec(guideButton, sourceRec1, (Vector2){guideButtonBound.x, guideButtonBound.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void mode()
    {
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
            if (CheckCollisionPointRec(mousePoint, guideButtonRec))
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
            DrawTextureRec(guideButton, smallButtonAreaRec, (Vector2){guideButtonRec.x, guideButtonRec.y}, WHITE); // Draw button 6 frame

            EndDrawing();
        }
    }

    void start()
    {
        // Define frame rectangle for drawing
        Rectangle sourceRec = {0, 0, (float)playButton.width, largeButtonFrameHeight};

        ButtonState playButtonState = IDOL;
        ButtonState optionButtonState = IDOL;
        ButtonState exitButtonState = IDOL;

        Vector2 mousePoint = {0.0f, 0.0f};

        while (true)
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

    void unload()
    {
        UnloadMusicStream(music);
        UnloadTexture(bg);
        UnloadTexture(pong);
        UnloadTexture(ballTexture);
        UnloadTexture(playButton);
        UnloadTexture(optionsButton);
        UnloadTexture(exitButton);
        UnloadTexture(vsCompButton);
        UnloadTexture(vsHumanButton);
        UnloadTexture(endlessButton);
        UnloadTexture(easyButton);
        UnloadTexture(mediumButton);
        UnloadTexture(hardButton);
        UnloadTexture(backButton);
        UnloadTexture(musicButton);
        UnloadTexture(guideButton);
        UnloadTexture(endles);
        UnloadTexture(resetButton);
        UnloadTexture(ball.gameBall);
        UnloadSound(jump);
        UnloadSound(buttonClickSound);

        CloseAudioDevice();
        CloseWindow();
    }

};

#endif