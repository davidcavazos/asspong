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

typedef int BOOL;
const BOOL TRUE = 1;
const BOOL FALSE = 0;

// constants
const double PI = 3.1415926535897932384626433832795028841971693993751058;

const int FRAMERATE_CAP = 60;
const int MILLISECONDS_CAP = 1000 / 60;

const unsigned int CLEAR_COLOR =      0x111111;
const unsigned int BALL_COLOR =       0x00d636;
const unsigned int PLAYER1_COLOR =    0x00d636;
const unsigned int PLAYER2_COLOR =    0x00d636;
const unsigned int BACKGROUND_COLOR = 0x00d636;

const char* PLAY_MUSIC = "./asspong_player still_alive.ogg &";
const char* STOP_MUSIC = "killall asspong_player";

// function prototypes
void initialize_ASM();
void initializeVideoContext_ASM();
void shutdown_ASM();
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
BOOL g_isKeyDownUP;
BOOL g_isKeyDownDOWN;



// main function
int main(int argc, char** argv) {
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
    printf("    |        [UP]    Mover jugador 2 arriba                                |\n");
    printf("    |        [DOWN]  Mover jugador 2 abajo                                 |\n");
    printf("    |                                                                      |\n");
    printf("    +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n");

    initialize_ASM();

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
    g_isKeyDownUP = FALSE;
    g_isKeyDownDOWN = FALSE;

    // main loop
//     Uint32 startTime;
    BOOL isRunning = TRUE;
    while (isRunning) {
//         startTime = SDL_GetTicks();

        // update
        processEvents_ASM(&isRunning);
        update();

        // draw
        clearScreen_ASM();
        drawEverything();

        // framerate cap

        // use clock() and CLOCKS_PER_SEC

//         g_deltaTime = SDL_GetTicks() - startTime;
//         if (MILLISECONDS_CAP > g_deltaTime)
//             SDL_Delay((Uint32)(MILLISECONDS_CAP - g_deltaTime));
//         g_deltaTime = (SDL_GetTicks() - startTime) * 0.001;

        // show framerate
//        stringstream title;
//        title << "Pong - " << setprecision(1) << fixed <<
//                        (g_deltaTime == 0.0? double(FRAMERATE_CAP) : 1.0 / g_deltaTime) << " fps";
//        SDL_WM_SetCaption(title.str().c_str(), "");
    }

    // shutdown
    shutdown_ASM();
    system(STOP_MUSIC);

    // show winner
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



void initialize_ASM() {
    printf("Using ASM context\n");
}

void initializeVideoContext_ASM() {
    g_windowWidth = 800;
    g_windowHeight = 600;
}

void shutdown_ASM() {
}

void processEvents_ASM(BOOL* isRunning) {
    getchar();
    *isRunning = FALSE;
}

void clearScreen_ASM() {
}

void drawPixel_ASM(const int posX, const int posY, const unsigned int color) {
}



void initializeDimensions() {
    g_virtualScreenPixelSize = g_windowWidth / 160;
    g_virtualScreenWidth = g_windowWidth / g_virtualScreenPixelSize;
    g_virtualScreenHeight = g_windowHeight / g_virtualScreenPixelSize;

    g_ballRadius = g_virtualScreenWidth / 40;
    g_playerWidth = g_virtualScreenWidth / 40;
    g_playerHeight = g_virtualScreenHeight / 6;

    g_ballSpeedIncrease = g_virtualScreenWidth * 0.05;
    g_playerSpeed = g_virtualScreenWidth * 1.3;

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
    if (g_isKeyDownUP) {
        g_player2PosY -= g_playerSpeed * g_deltaTime;
        if (g_player2PosY < 2.0)
            g_player2PosY = 2.0;
    }
    if (g_isKeyDownDOWN) {
        g_player2PosY += g_playerSpeed * g_deltaTime;
        if (g_player2PosY > g_virtualScreenHeight - g_playerHeight - 2.0)
            g_player2PosY = g_virtualScreenHeight - g_playerHeight - 2.0;
    }

    // update ball
    g_ballPosX += g_ballSpeed * cos(g_ballAngle) * g_deltaTime;
    g_ballPosY -= g_ballSpeed * sin(g_ballAngle) * g_deltaTime;

    // check ball collisions with players
    double distX, distY;
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
        }
        else if (g_ballPosX <= g_ballRadius) {
            printf("Jugador 2\n");
            g_isGameOver = TRUE;
            ++g_player2Wins;
        }
    }
}

BOOL hasBallCollidedPlayer1() {
    if (g_ballPosX - g_ballRadius < g_player1PositionX + g_playerWidth + 1) {
        if (g_ballPosY >= g_player1PosY && g_ballPosY <= g_player1PosY + g_playerHeight) // racket 1 body
            return TRUE;
        double radiusSqr = g_ballRadius * g_ballRadius;
        double distX = g_player1PositionX + g_playerWidth - g_ballPosX;
        double distY = g_player1PosY - g_ballPosY;
        double distSqr = distX * distX + distY * distY;
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
    if (g_ballPosX + g_ballRadius > g_player2PositionX - 1) {
        if (g_ballPosY >= g_player2PosY && g_ballPosY <= g_player2PosY + g_playerHeight) // racket 2 body
            return TRUE;
        double radiusSqr = g_ballRadius * g_ballRadius;
        double distX = g_player2PositionX - g_ballPosX;
        double distY = g_player2PosY - g_ballPosY;
        double distSqr = distX * distX + distY * distY;
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
    int x, y, p;

    // fill square with background color
    int endX = posX + 2 * radius - 4;
    int endY = posY + 2 * radius - 4;
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
