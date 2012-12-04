/*
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    The above copyright notice and this permission notice shall be
 *    included in all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

// Music from http://grayscale.scene.pl/msx_archive.php?lang=en

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <dos.h>        // for delay()
#include <conio.h>      // for kbhit() and getch()


typedef int BOOL;
const BOOL TRUE = 1;
const BOOL FALSE = 0;

// constants
const double PI = 3.1415926535897932384626433832795028841971693993751058;

const int FRAMERATE_CAP = 60;
const double MILLISECONDS_CAP = 1000.0 / 60.0;

const unsigned int CLEAR_COLOR =      0;
const unsigned int BALL_COLOR =       2;
const unsigned int PLAYER1_COLOR =    2;
const unsigned int PLAYER2_COLOR =    2;
const unsigned int BACKGROUND_COLOR = 2;

const char* PLAY_MUSIC = "start asspong_player.exe still_alive.ogg"; //"./asspong_player still_alive.ogg &";
const char* STOP_MUSIC = "taskkill /F /IM asspong_player.exe"; //"killall asspong_player";

// function prototypes
void initializeVideoContext_ASM();
void processEvents_ASM(BOOL* isRunning);
void clearScreen_ASM();
void drawPixel_ASM(const int posX, const int posY, const unsigned int color);

void initializeDimensions();
void resetEverything();
void update();
BOOL hasBallCollidedPlayer1();
BOOL hasBallCollidedPlayer2();
void drawBall(const int posX, const int posY, const size_t radius, const unsigned int color);
void drawRacket(const int posX, const int posY, const size_t w, const size_t h, const unsigned int color);
void drawBackground(const unsigned int color);
void drawEverything();

// global variables
int g_windowWidth;
int g_windowHeight;
size_t g_virtualScreenPixelSize;
size_t g_virtualScreenWidth;
size_t g_virtualScreenHeight;
size_t g_ballRadius;
size_t g_playerWidth;
size_t g_playerHeight;
double g_ballSpeedIncrease;
double g_playerSpeed;
size_t g_player1PositionX;
size_t g_player2PositionX;

double g_deltaTime;
double g_player1PosY;
double g_player2PosY;
double g_ballPosX;
double g_ballPosY;
double g_ballSpeed;
double g_ballAngle;
BOOL g_isGameOver;
unsigned int g_player1Wins;
unsigned int g_player2Wins;

BOOL g_isKeyDownW;
BOOL g_isKeyDownS;
BOOL g_isKeyDownO;
BOOL g_isKeyDownL;



// main function
int main(void) {
    clock_t startTime;
    BOOL isRunning = TRUE;

    // introduction
    printf("    +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n");
    printf("    |               CC322 - Organizacion de Computadoras I                 |\n");
    printf("    |                            .-----------.                             |\n");
    printf("    |                           /   AssPong   \\                            |\n");
    printf("    |                          '==============='                           |\n");
    printf("    |                                                                      |\n");
    printf("    |                          *** Equipo 9 ***                            |\n");
    printf("    |                          Cavazos Woo David                           |\n");
    printf("    |                      Corona Garcia Erick Daniel                      |\n");
    printf("    |                                                                      |\n");
    printf("    |    Instrucciones:                                                    |\n");
    printf("    |        [ESC]   Salir                                                 |\n");
    printf("    |        [SPACE] Reset                                                 |\n");
    printf("    |        [w]     Mover jugador 1 arriba                                |\n");
    printf("    |        [s]     Mover jugador 1 abajo                                 |\n");
    printf("    |        [o]     Mover jugador 2 arriba                                |\n");
    printf("    |        [l]     Mover jugador 2 abajo                                 |\n");
    printf("    |                                                                      |\n");
    printf("    +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n");
    printf("Usando contexto de ASM\n");
    printf("Presione [Enter] para continuar...");
    getchar();

    system(PLAY_MUSIC);
    initializeVideoContext_ASM();

    srand((unsigned int)(time(0)));
    initializeDimensions();
    resetEverything();

    g_deltaTime = 0.0;
    g_player1Wins = 0;
    g_player2Wins = 0;

    g_isKeyDownW = FALSE;
    g_isKeyDownS = FALSE;
    g_isKeyDownO = FALSE;
    g_isKeyDownL = FALSE;

    // main loop
    while (isRunning) {
        startTime = clock();

        // update
        g_isKeyDownW = FALSE;
        g_isKeyDownS = FALSE;
        g_isKeyDownO = FALSE;
        g_isKeyDownL = FALSE;
        processEvents_ASM(&isRunning);
        update();

        // draw
        clearScreen_ASM();
        drawEverything();

        // framerate cap
//         g_deltaTime = MILLISECONDS_CAP;
//         delay((unsigned int)MILLISECONDS_CAP);
//         g_deltaTime *= 0.001;

        g_deltaTime = (double)(clock() - startTime) / (double)CLOCKS_PER_SEC * 1000.0;
        if (MILLISECONDS_CAP > g_deltaTime)
            delay((unsigned int)(MILLISECONDS_CAP - g_deltaTime));
        g_deltaTime = (double)(clock() - startTime) / (double)CLOCKS_PER_SEC;
    }

    // shutdown
    system(STOP_MUSIC);

    // show winner
    system("cls");
    printf("\n");
    printf("                       .--------------------------------.\n");
    printf("                      /   ");
    if (g_player1Wins == g_player2Wins)
        printf("  ~~~~ Empate ~~~~ ");
    else if (g_player1Wins > g_player2Wins)
        printf("Ganador: Jugador 1 ");
    else
        printf("Ganador: Jugador 2 ");
    printf("(%2u : %2u)   \\\n", g_player1Wins, g_player2Wins);
    printf("                     '===================================='\n");
    printf("Presione [Enter] para salir...");
    getchar();

    return EXIT_SUCCESS;
}

void clearScreen_ASM() {
    _asm {
        mov ah, 00h     // Video mode function
        mov al, 12h     // Select VGA/ATI VIP, 16-color, 640x480
        int 10h
    }
}

void drawPixel_ASM(const int posX, const int posY, const unsigned int color) {
    int x, y;
    int endX = posX * g_virtualScreenPixelSize + g_virtualScreenPixelSize;
    int endY = posY * g_virtualScreenPixelSize + g_virtualScreenPixelSize;
    for (x = posX * g_virtualScreenPixelSize; x < endX; ++x) {
        for (y = posY * g_virtualScreenPixelSize; y < endY; ++y) {
            _asm {
                mov ax, color;
                mov ah, 0ch     // Write dot on screen function
                mov bh, 00h     // Dispay page
                mov cx, x       // Column
                mov dx, y       // Row
                int 10h
            }
        }
    }
}

void initializeVideoContext_ASM() {
    g_windowWidth = 640;
    g_windowHeight = 480;
    // Set video mode
    _asm {
        mov ah, 00h     // Video mode function
        mov al, 12h     // Select VGA/ATI VIP, 16-color, 640x480
        int 10h
    }
}

void processEvents_ASM(BOOL* isRunning) {
    char key = 0;
//     if (!kbhit())
//         return;
//     _asm {
//         mov ah, 00h     // Get keystroke
//         int 16h
//         mov key, al     // Get character
//     }

    while (kbhit()) {
    key = getch();
    key = tolower(key);
    switch (key) {
    case 27: // ESC
//     case 'q':
        *isRunning = FALSE;
        break;
    case ' ':
        resetEverything();
        break;

    case 'w':
        g_isKeyDownW = TRUE;
        break;
    case 's':
        g_isKeyDownS = TRUE;
        break;

    case 'o':
        g_isKeyDownO = TRUE;
        break;
    case 'l':
        g_isKeyDownL = TRUE;
        break;

    default:
        break;
    }
    }
}


void initializeDimensions() {
    g_virtualScreenPixelSize = g_windowWidth / 160;
    g_virtualScreenWidth = g_windowWidth / g_virtualScreenPixelSize;
    g_virtualScreenHeight = g_windowHeight / g_virtualScreenPixelSize;

    g_ballRadius = g_virtualScreenWidth / 40;
    g_playerWidth = g_virtualScreenWidth / 40;
    g_playerHeight = g_virtualScreenHeight / 6;

    g_ballSpeedIncrease = g_virtualScreenWidth * 0.05;
    g_playerSpeed = g_virtualScreenWidth * 1.0;

    g_player1PositionX = (size_t)(g_virtualScreenWidth * 0.08);
    g_player2PositionX = g_virtualScreenWidth - g_player1PositionX - g_playerWidth;
}

void resetEverything() {
    g_player1PosY = (g_virtualScreenHeight - g_playerHeight) / 2;
    g_player2PosY = g_player1PosY;
    g_ballPosX = g_virtualScreenWidth / 2;
    g_ballPosY = g_virtualScreenHeight / 2;
    g_ballSpeed = g_virtualScreenWidth * 0.5;
    g_ballAngle = (double)(rand()) / (double)(RAND_MAX) * PI * 0.5 - PI * 0.25;
    if (rand() % 2 == 0)
        g_ballAngle += PI;

    g_isGameOver = FALSE;
}

void update() {
    double distX, distY;

    // update players
    if (g_isKeyDownW) {
        g_player1PosY -= g_playerSpeed * g_deltaTime;
        if (g_player1PosY < 2.0)
            g_player1PosY = 2.0;
    }
    if (g_isKeyDownS) {
        g_player1PosY += g_playerSpeed * g_deltaTime;
        if (g_player1PosY > g_virtualScreenHeight - g_playerHeight - 2.0)
            g_player1PosY = g_virtualScreenHeight - g_playerHeight - 2.0;
    }
    if (g_isKeyDownO) {
        g_player2PosY -= g_playerSpeed * g_deltaTime;
        if (g_player2PosY < 2.0)
            g_player2PosY = 2.0;
    }
    if (g_isKeyDownL) {
        g_player2PosY += g_playerSpeed * g_deltaTime;
        if (g_player2PosY > g_virtualScreenHeight - g_playerHeight - 2.0)
            g_player2PosY = g_virtualScreenHeight - g_playerHeight - 2.0;
    }

    // update ball
    g_ballPosX += g_ballSpeed * cos(g_ballAngle) * g_deltaTime;
    g_ballPosY -= g_ballSpeed * sin(g_ballAngle) * g_deltaTime;

    // check ball collisions with players
    if (hasBallCollidedPlayer1() && g_ballPosX > g_player1PositionX) {
        distY = g_player1PosY + g_playerHeight / 2 - g_ballPosY;
        distX = g_ballPosX;
        g_ballAngle = atan(distY / distX);
        g_ballSpeed += g_ballSpeedIncrease;
        g_ballPosX = g_player1PositionX + g_playerWidth + g_ballRadius;
    }
    else if (hasBallCollidedPlayer2() && g_ballPosX < g_player2PositionX + g_playerWidth) {
        distY = g_player2PosY + g_playerHeight / 2 - g_ballPosY;
        distX = g_virtualScreenWidth - g_ballPosX;
        g_ballAngle = PI - atan(distY / distX);
        g_ballSpeed += g_ballSpeedIncrease;
        g_ballPosX = g_player2PositionX - g_ballRadius - 1;
    }
    else if (g_ballPosY < g_ballRadius + 2.0) {
        g_ballPosY = g_ballRadius + 2.0;
        g_ballAngle = -g_ballAngle;
    }
    else if (g_ballPosY > g_virtualScreenHeight - g_ballRadius - 2.0) {
        g_ballPosY = g_virtualScreenHeight - g_ballRadius - 2.0;
        g_ballAngle = -g_ballAngle;
    }

    // check win conditions
    if (!g_isGameOver) {
        if (g_ballPosX >= g_virtualScreenWidth - g_ballRadius) {
            printf("Jugador 1\n");
            g_isGameOver = TRUE;
            ++g_player1Wins;
            g_ballSpeed = 0.0;
        }
        else if (g_ballPosX <= g_ballRadius) {
            printf("Jugador 2\n");
            g_isGameOver = TRUE;
            ++g_player2Wins;
            g_ballSpeed = 0.0;
        }
    }
}

BOOL hasBallCollidedPlayer1() {
    double radiusSqr = g_ballRadius * g_ballRadius;
    double distX = g_player1PositionX + g_playerWidth - g_ballPosX;
    double distY = g_player1PosY - g_ballPosY;
    double distSqr = distX * distX + distY * distY;
    if (g_ballPosX - g_ballRadius < g_player1PositionX + g_playerWidth + 1) {
        if (g_ballPosY >= g_player1PosY && g_ballPosY <= g_player1PosY + g_playerHeight) // racket 1 body
            return TRUE;
        if (distSqr <= radiusSqr) // racket 1 upper corner
            return TRUE;
        distY = g_player1PosY + g_playerHeight - g_ballPosY;
        distSqr = distX * distX + distY * distY;
        if (distSqr <= radiusSqr) // racket 1 lower corner
            return TRUE;
    }
    return FALSE;
}

BOOL hasBallCollidedPlayer2() {
    double radiusSqr = g_ballRadius * g_ballRadius;
    double distX = g_player2PositionX - g_ballPosX;
    double distY = g_player2PosY - g_ballPosY;
    double distSqr = distX * distX + distY * distY;
    if (g_ballPosX + g_ballRadius > g_player2PositionX - 1) {
        if (g_ballPosY >= g_player2PosY && g_ballPosY <= g_player2PosY + g_playerHeight) // racket 2 body
            return TRUE;
        if (distSqr <= radiusSqr) // racket 2 upper corner
            return TRUE;
        distY = g_player2PosY + g_playerHeight - g_ballPosY;
        distSqr = distX * distX + distY * distY;
        if (distSqr <= radiusSqr) // racket 2 lower corner
            return TRUE;
    }
    return FALSE;
}

void drawBall(const int posX, const int posY, const size_t radius, const unsigned int color) {
    int x, y, p, endX, endY;

    if (posX < 0 || posX > g_virtualScreenWidth || posY < 0 || posY > g_virtualScreenWidth)
        return;

    // fill square with background color
    endX = posX + 2 * radius - 4;
    endY = posY + 2 * radius - 4;
    for (y = posY - radius + 1; y < endY; ++y) {
        for (x = posX - radius + 1; x < endX; ++x)
            drawPixel_ASM(x, y, CLEAR_COLOR);
    }

    // draw circle
    x = 0;
    y = radius;
    p = 1 - radius;
    while(x <= y) {
        drawPixel_ASM(posX + x, posY + y, color);
        drawPixel_ASM(posX + x, posY - y, color);
        drawPixel_ASM(posX - x, posY + y, color);
        drawPixel_ASM(posX - x, posY - y, color);
        drawPixel_ASM(posX + y, posY + x, color);
        drawPixel_ASM(posX + y, posY - x, color);
        drawPixel_ASM(posX - y, posY + x, color);
        drawPixel_ASM(posX - y, posY - x, color);

        if(p < 0) {
            ++x;
            p += 2 * x + 1;
        }
        else {
            ++x;
            --y;
            p += 2 * (x - y) + 1;
        }
    }
}

void drawRacket(const int posX, const int posY, const size_t w, const size_t h, const unsigned int color) {
    size_t x, y;
    size_t maxX = posX + w - 1;
    size_t maxY = posY + h - 1;
    for (x = posX; x <= maxX; ++x) {
        drawPixel_ASM(x, posY, color);
        drawPixel_ASM(x, maxY, color);
    }
    for (y = posY + 1; y <= maxY; ++y) {
        drawPixel_ASM(posX, y, color);
        drawPixel_ASM(maxX, y, color);
    }
}

void drawBackground(const unsigned int color) {
    size_t half = g_virtualScreenWidth / 2;
    size_t hLineEnd = g_virtualScreenWidth - 1;
    size_t vLineEnd = g_virtualScreenHeight - 2;
    size_t i;
    for (i = 1; i < hLineEnd; ++i) {
        drawPixel_ASM(i, 1, color);
        drawPixel_ASM(i, vLineEnd, color);
    }
    for (i = 2; i < vLineEnd; ++i) {
        if (i % 8 != 0)
            drawPixel_ASM(half, i, color);
    }
}

void drawEverything() {
    drawBackground(BACKGROUND_COLOR);
    drawBall((size_t)(g_ballPosX), (size_t)(g_ballPosY), g_ballRadius, BALL_COLOR);
    drawRacket(g_player1PositionX, (size_t)(g_player1PosY), g_playerWidth, g_playerHeight, PLAYER1_COLOR);
    drawRacket(g_player2PositionX, (size_t)(g_player2PosY), g_playerWidth, g_playerHeight, PLAYER2_COLOR);
}
