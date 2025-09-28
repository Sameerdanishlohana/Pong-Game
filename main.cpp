#include <iostream>
#include <raylib.h>
#include <math.h>
using namespace std;
const int screenWidth = 1280;
const int screenHeight = 800;

int gameState = 0; // 0 = Menu, 1 = Difficulty,2= countdown,  3 = Playing, 4 = Game Over
int difficulty;
const int winScore = 5;

int playerScore = 0;
int cpuScore = 0;

Sound hitSound;
Sound scoreSound;
Sound countDown;

Font myFont;

// Modern color scheme
Color DarkBlue = Color{12, 20, 31, 255};        // Dark background
Color NeonBlue = Color{0, 150, 255, 255};       // Bright blue for accents
Color ElectricPurple = Color{125, 0, 255, 255}; // Purple for highlights
Color CyberYellow = Color{255, 213, 0, 255};    // Yellow for text
Color MatrixGreen = Color{0, 255, 100, 255};    // Green for scores
Color PaddleColor = Color{255, 80, 80, 255};    // Reddish paddles
Color BallColor = Color{255, 255, 255, 255};    // White ball

bool isCountdown = false;
float countdownStartTime = 0.0f;
int countdownValue = 3;
bool countdownSoundPlayed = false;

bool isPaused = false;

void StartCountdown()
{
    isCountdown = true;
    countdownStartTime = GetTime(); // raylib time function to get current time
    countdownValue = 3;
}

class Ball
{
public:
    float x, y;
    int speedX, speedY, radius;

    Ball(float x1, float y1, int sX, int sY, int radius) : x(x1), y(y1), speedX(sX), speedY(sY), radius(radius) {}
    void Draw()
    {
        DrawCircle(x, y, radius, BallColor);
    }

    void Update()
    {
        x += speedX;
        y += speedY;

        // collision with roof and btom
        if (y + radius >= screenHeight || y - radius <= 0)
        {
            speedY *= -1;
        }

        // if crossed cpu's paddle
        if (x + radius >= screenWidth)
        {
            playerScore++;
            PlaySound(scoreSound);
            ResetBall();
        }
        // if crossed player's paddle
        else if (x - radius <= 0)
        {
            cpuScore++;
            PlaySound(scoreSound);
            ResetBall();
        }
    }

    void ResetBall()
    {
        x = screenWidth / 2;
        y = screenHeight / 2;

        // for ball to go randomly aftr reseting
        int speedChoices[2] = {1, -1};
        speedX *= speedChoices[GetRandomValue(0, 1)];
        speedY *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
protected:
    // by this func paddle will not  cross walls
    void LimitMovement()
    {
        if (y <= 0)
            y = 0;
        if (y + height >= screenHeight)
            y = screenHeight - height;
    }

public:
    float x, y, width, height;
    int speed; // only y direcion

    Paddle(float x1, float y1, float w, float h, int sp) : x(x1), y(y1), width(w), height(h), speed(sp) {}
    void Draw()
    {
        DrawRectangle(x, y, width, height, PaddleColor);
    }
    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
public:
    CpuPaddle(float x1, float y1, float w, float h, int sp) : Paddle(x1, y1, w, h, sp) {}
    void Update(int ballY)
    {
        if (y + height / 2 > ballY)
        {
            y -= speed;
        }
        if (y + height / 2 <= ballY)
        {
            y += speed;
        }
        LimitMovement();
    }
};

int main()
{

    InitWindow(screenWidth, screenHeight, "Pong 2025!");
    SetTargetFPS(60);
    myFont = LoadFontEx("fonts/newFont.ttf", 96, 0, 0);
    InitAudioDevice();
    hitSound = LoadSound("audio/hit1.wav");
    scoreSound = LoadSound("audio/score.wav");
    countDown = LoadSound("audio/c1.wav");

    Ball ball(screenWidth / 2, screenHeight / 2, 7, 7, 20);
    Paddle player(10, screenHeight / 2 - 60, 25, 120, 8);
    CpuPaddle cpu(screenWidth - 35, screenHeight / 2 - 60, 25, 120, 3);

    while (!WindowShouldClose())
    {
        float alpha = (sinf(GetTime() * 2.5f) * 0.5f + 0.5f) * 255;
        Color fadedWhite = WHITE;
        fadedWhite.a = (unsigned char)alpha;
        Color fadedYellow = CyberYellow;
        fadedYellow.a = (unsigned char)alpha;
        BeginDrawing();
        ClearBackground(DarkBlue);

        // 0 for main starting screen
        if (gameState == 0)
        {
            DrawTextEx(myFont, "PONG 2025", {screenWidth / 2 - MeasureTextEx(myFont, "PONG 2025", 120, 2).x / 2, screenHeight / 2 - 200}, 120, 2, NeonBlue);
            DrawTextEx(myFont, "Press ENTER to Start", {screenWidth / 2 - MeasureTextEx(myFont, "Press ENTER to Start", 50, 2).x / 2, screenHeight / 2 + 150}, 50, 2, fadedYellow);
            if (IsKeyPressed(KEY_ENTER))
            {
                gameState = 1; // for going to difficulty section
            }
        }

        // selecting diffi
        if (gameState == 1)
        {
            DrawTextEx(myFont, "Select Difficulty", {screenWidth / 2 - MeasureTextEx(myFont, "Select Difficulty", 80, 2).x / 2, screenHeight / 2 - 200}, 80, 2, ElectricPurple);

            float optionY = screenHeight / 2 - 50;
            DrawTextEx(myFont, "1 - EASY", {screenWidth / 2 - MeasureTextEx(myFont, "1 - EASY", 50, 2).x / 2, optionY}, 50, 2, WHITE);
            DrawTextEx(myFont, "2 - MEDIUM", {screenWidth / 2 - MeasureTextEx(myFont, "2 - MEDIUM", 50, 2).x / 2, optionY + 70}, 50, 2, WHITE);
            DrawTextEx(myFont, "3 - HARD", {screenWidth / 2 - MeasureTextEx(myFont, "3 - HARD", 50, 2).x / 2, optionY + 140}, 50, 2, WHITE);

            if (IsKeyPressed(KEY_ONE))
            {
                difficulty = 3; // Easy
                gameState = 2;
                isCountdown = true;
                countdownStartTime = GetTime();
                countdownSoundPlayed = false;
            }
            else if (IsKeyPressed(KEY_TWO))
            {
                difficulty = 5; // Medium
                gameState = 2;
                isCountdown = true;
                countdownStartTime = GetTime();
                countdownSoundPlayed = false;
            }
            else if (IsKeyPressed(KEY_THREE))
            {
                difficulty = 8; // Hard
                gameState = 2;
                isCountdown = true;
                countdownStartTime = GetTime();
                countdownSoundPlayed = false;
            }
        }

        // screen of timer 3 2  1
        if (gameState == 2)
        {
            if (!countdownSoundPlayed)
            {
                PlaySound(countDown);
                countdownSoundPlayed = true;
            }
            float timeElapsed = GetTime() - countdownStartTime;
            countdownValue = 3 - (int)timeElapsed;

            if (countdownValue <= 0)
            {
                isCountdown = false;
                gameState = 3;
            }
            else
            {
                const char *countText = TextFormat("%i", countdownValue);
                Vector2 textSize = MeasureTextEx(myFont, countText, 150, 4);
                DrawTextEx(myFont, countText, {screenWidth / 2 - textSize.x / 2, screenHeight / 2 - textSize.y / 2}, 150, 4, MatrixGreen);
            }

            EndDrawing();
            continue;
        }

        // actual game
        if (gameState == 3)
        {
            if (IsKeyPressed(KEY_P))
            {
                isPaused = !isPaused;
            }
            if (!isPaused)
            {
                player.Update();
                cpu.speed = difficulty;
                cpu.Update(ball.y);

                // Paddle collisions with our ball
                if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                            Rectangle{player.x, player.y, player.width, player.height}) &&
                    ball.speedX < 0)
                {
                    PlaySound(hitSound);
                    ball.speedX *= -1.12;
                }

                if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                            Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}) &&
                    ball.speedX > 0)
                {
                    PlaySound(hitSound);
                    ball.speedX *= -1;
                }

                ball.Update();
            }

            // Drawing
            DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, Color{0, 30, 60, 255});
            DrawCircle(screenWidth / 2, screenHeight / 2, 180, Color{NeonBlue.r, NeonBlue.g, NeonBlue.b, 100});
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, Color{NeonBlue.r, NeonBlue.g, NeonBlue.b, 100});
            ball.Draw();
            player.Draw();
            cpu.Draw();

            // displaying scores
            DrawTextEx(myFont, TextFormat("%i", playerScore), {screenWidth / 4 - MeasureTextEx(myFont, TextFormat("%i", playerScore), 100, 2).x / 2, 50}, 100, 2, MatrixGreen);
            DrawTextEx(myFont, TextFormat("%i", cpuScore), {3 * screenWidth / 4 - MeasureTextEx(myFont, TextFormat("%i", cpuScore), 100, 2).x / 2, 50}, 100, 2, MatrixGreen);

            if (isPaused)
            {
                const char *pauseText = "PAUSED";
                const char *escText = "Press E to go back to main menu";
                Vector2 textSize = MeasureTextEx(myFont, pauseText, 100, 2);
                DrawTextEx(myFont, pauseText, {screenWidth / 2 - textSize.x / 2, screenHeight / 2 - textSize.y / 2}, 100, 2, fadedWhite);
                DrawTextEx(myFont, escText, {screenWidth / 2 - textSize.x / 2 - 280, screenHeight / 2 - textSize.y / 2 + 150}, 70, 2, fadedWhite);

                if (IsKeyPressed(KEY_E))
                {
                    gameState = 0;
                    isPaused = false;
                    cpuScore = 0;
                    playerScore = 0;
                }
            }

            // checking who will win
            if (playerScore >= winScore || cpuScore >= winScore)
            {
                gameState = 4;
            }
        }

        // game is over
        if (gameState == 4)
        {
            const char *winner = (playerScore > cpuScore) ? "PLAYER WINS!" : "CPU WINS!";
            Vector2 winnerSize = MeasureTextEx(myFont, winner, 100, 2);

            DrawTextEx(myFont, winner, {screenWidth / 2 - winnerSize.x / 2, screenHeight / 2 - 150}, 100, 2, ElectricPurple);

            const char *restartText = "Press ENTER to Restart";
            Vector2 restartSize = MeasureTextEx(myFont, restartText, 50, 2);
            DrawTextEx(myFont, restartText, {screenWidth / 2 - restartSize.x / 2, screenHeight / 2 + 50}, 50, 2, fadedYellow);

            if (IsKeyPressed(KEY_ENTER))
            {
                playerScore = 0;
                cpuScore = 0;
                ball.ResetBall();
                gameState = 0; // Back to main menu
            }
        }

        EndDrawing();
    }
    UnloadSound(hitSound);
    UnloadSound(scoreSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}