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


#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <SDL/SDL.h>

using namespace std;

// constants
const double PI = 3.1415926535897932384626433832795028841971693993751058;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int FRAMERATE_CAP = 60;
const int MILLISECONDS_CAP = 1000 / FRAMERATE_CAP;

const size_t VIRTUAL_SCREEN_PIXEL_SIZE = 6;
const size_t VIRTUAL_SCREEN_WIDTH = WINDOW_WIDTH / VIRTUAL_SCREEN_PIXEL_SIZE;
const size_t VIRTUAL_SCREEN_HEIGHT = WINDOW_HEIGHT / VIRTUAL_SCREEN_PIXEL_SIZE;

const size_t BALL_RADIUS = VIRTUAL_SCREEN_WIDTH / 40;
const size_t PLAYER_WIDTH = VIRTUAL_SCREEN_WIDTH / 40;
const size_t PLAYER_HEIGHT = VIRTUAL_SCREEN_HEIGHT / 6;

const double BALL_SPEED_INCREASE = VIRTUAL_SCREEN_WIDTH * 0.001;
const double PLAYER_SPEED = VIRTUAL_SCREEN_WIDTH * 0.02;

const size_t PLAYER1_POSITION_X = VIRTUAL_SCREEN_WIDTH * 0.08;
const size_t PLAYER2_POSITION_X = VIRTUAL_SCREEN_WIDTH - PLAYER1_POSITION_X - PLAYER_WIDTH;

const unsigned int BALL_COLOR =    0xaa2222;
const unsigned int PLAYER1_COLOR = 0x22aa22;
const unsigned int PLAYER2_COLOR = 0x2222aa;



// function prototypes
void processEventsSDL(bool& isRunning);
void resetEverything();
void update();
bool hasBallCollidedPlayer1();
bool hasBallCollidedPlayer2();
void drawPixelSDL(const size_t posX, const size_t posY, const unsigned int color);
void drawBall(const size_t posX, const size_t posY, const size_t radius, const unsigned int color);
void drawRacket(const size_t posX, const size_t posY, const size_t w, const size_t h, const unsigned int color);
void drawEverything();

// global variables
SDL_Surface* g_screen = 0;
double g_player1PosY;
double g_player2PosY;
double g_ballPosX;
double g_ballPosY;
double g_ballSpeed;
double g_ballAngle;
bool g_isGameOver;
size_t g_player1Wins;
size_t g_player2Wins;

bool g_isKeyDownW;
bool g_isKeyDownS;
bool g_isKeyDownUP;
bool g_isKeyDownDOWN;



// main function
int main(int argc, char** argv) {
    // introduction
    cout << "          *** Equipo 9 ***" << endl;
    cout << endl;
    cout << "             ·--------·" << endl;
    cout << "            /   Pong   \\" << endl;
    cout << "           ·============·" << endl;
    cout << endl;
    cout << "Cavazos Woo David" << endl;
    cout << "Corona Garcia Erick Daniel" << endl;
    cout << endl;
    cout << "Instrucciones:" << endl;
    cout << "[ESC]  Salir" << endl;
    cout << "[r]    Reset" << endl;
    cout << "[w]    Mover jugador 1 arriba" << endl;
    cout << "[s]    Mover jugador 1 abajo" << endl;
    cout << "[UP]   Mover jugador 2 arriba" << endl;
    cout << "[DOWN] Mover jugador 2 abajo" << endl;
    cout << endl;
    cout << "Presione [Enter] para continuar...";
    cin.get();

    // initialization
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) // 0 success, -1 failure
        return EXIT_FAILURE;
    g_screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (g_screen == 0)
        return EXIT_FAILURE;

    srand(time(0));
    resetEverything();

    g_player1Wins = 0;
    g_player2Wins = 0;

    g_isKeyDownW = false;
    g_isKeyDownS = false;
    g_isKeyDownUP = false;
    g_isKeyDownDOWN = false;

    // main loop
    Uint32 startTime;
    Uint32 deltaTime;
    bool isRunning = true;
    while (isRunning) {
        startTime = SDL_GetTicks();

        // update
        processEventsSDL(isRunning);
        update();

        // draw
        SDL_FillRect(g_screen, 0, 0); // clear screen
        drawEverything();
        SDL_Flip(g_screen); // flip buffers

        // framerate cap
        deltaTime = SDL_GetTicks() - startTime;
        if (MILLISECONDS_CAP > deltaTime)
            SDL_Delay(MILLISECONDS_CAP - deltaTime);

        // show framerate
        deltaTime = SDL_GetTicks() - startTime;
        stringstream title;
        title << "Pong - " << (deltaTime == 0? FRAMERATE_CAP : 1000 / deltaTime) << " fps";
        SDL_WM_SetCaption(title.str().c_str(), "");
    }

    // show winner
    cout << endl;
    if (g_player1Wins == g_player2Wins)
        cout << "Empate: ";
    else if (g_player1Wins > g_player2Wins)
        cout << "Ganador: Jugador 1 ";
    else
        cout << "Ganador: Jugador 2 ";
    cout << "(" << g_player1Wins << " : " << g_player2Wins << ")" << endl;

    // shutdown
    SDL_Quit();
    return EXIT_SUCCESS;
}



void processEventsSDL(bool& isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            // quit
            case SDLK_ESCAPE:
                isRunning = false;
                break;
            // reset
            case SDLK_r:
                resetEverything();
                break;

            // player 1
            case SDLK_w:
                g_isKeyDownW = true;
                break;
            case SDLK_s:
                g_isKeyDownS = true;
                break;

            // player 2
            case SDLK_UP:
                g_isKeyDownUP = true;
                break;
            case SDLK_DOWN:
                g_isKeyDownDOWN = true;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            // player 1
            case SDLK_w:
                g_isKeyDownW = false;
                break;
            case SDLK_s:
                g_isKeyDownS = false;
                break;

            // player 2
            case SDLK_UP:
                g_isKeyDownUP = false;
                break;
            case SDLK_DOWN:
                g_isKeyDownDOWN = false;
                break;
            }
            break;
        }
    }
}

void resetEverything() {
    g_player1PosY = (VIRTUAL_SCREEN_HEIGHT - PLAYER_HEIGHT) / 2;
    g_player2PosY = g_player1PosY;
    g_ballPosX = VIRTUAL_SCREEN_WIDTH / 2;
    g_ballPosY = VIRTUAL_SCREEN_HEIGHT / 2;
    g_ballSpeed = VIRTUAL_SCREEN_WIDTH * 0.008;
    g_ballAngle = double(rand()) / double(RAND_MAX) * PI * 0.5 - PI * 0.25;
    if (rand() % 2 == 0)
        g_ballAngle += PI;

    g_isGameOver = false;
}

void update() {
    // update players
    if (g_isKeyDownW) {
        g_player1PosY -= PLAYER_SPEED;
        if (g_player1PosY < 0.0)
            g_player1PosY = 0.0;
    }
    if (g_isKeyDownS) {
        g_player1PosY += PLAYER_SPEED;
        if (g_player1PosY > VIRTUAL_SCREEN_HEIGHT - PLAYER_HEIGHT)
            g_player1PosY = VIRTUAL_SCREEN_HEIGHT - PLAYER_HEIGHT;
    }
    if (g_isKeyDownUP) {
        g_player2PosY -= PLAYER_SPEED;
        if (g_player2PosY < 0.0)
            g_player2PosY = 0.0;
    }
    if (g_isKeyDownDOWN) {
        g_player2PosY += PLAYER_SPEED;
        if (g_player2PosY > VIRTUAL_SCREEN_HEIGHT - PLAYER_HEIGHT)
            g_player2PosY = VIRTUAL_SCREEN_HEIGHT - PLAYER_HEIGHT;
    }

    // update ball
    g_ballPosX += g_ballSpeed * cos(g_ballAngle);
    g_ballPosY -= g_ballSpeed * sin(g_ballAngle);

    // check ball collisions with players
    double distX, distY;
    if (hasBallCollidedPlayer1() && g_ballPosX > PLAYER1_POSITION_X) {
        distY = g_player1PosY + PLAYER_HEIGHT / 2 - g_ballPosY;
        distX = g_ballPosX;
        g_ballAngle = atan(distY / distX);
        g_ballSpeed += BALL_SPEED_INCREASE;
    }
    else if (hasBallCollidedPlayer2() && g_ballPosX < PLAYER2_POSITION_X + PLAYER_WIDTH) {
        distY = g_player2PosY + PLAYER_HEIGHT / 2 - g_ballPosY;
        distX = VIRTUAL_SCREEN_WIDTH - g_ballPosX;
        g_ballAngle = PI - atan(distY / distX);
        g_ballSpeed += BALL_SPEED_INCREASE;
    }
    else if (g_ballPosY < BALL_RADIUS || g_ballPosY > VIRTUAL_SCREEN_HEIGHT - BALL_RADIUS)
        g_ballAngle = -g_ballAngle;

    // check win conditions
    if (!g_isGameOver) {
        if (g_ballPosX >= VIRTUAL_SCREEN_WIDTH - BALL_RADIUS) {
            cout << "Jugador 1" << endl;
            g_isGameOver = true;
            ++g_player1Wins;
        }
        else if (g_ballPosX <= BALL_RADIUS) {
            cout << "Jugador 2" << endl;
            g_isGameOver = true;
            ++g_player2Wins;
        }
    }
}

bool hasBallCollidedPlayer1() {
    if (g_ballPosX - BALL_RADIUS < PLAYER1_POSITION_X + PLAYER_WIDTH + 1) {
        if (g_ballPosY >= g_player1PosY && g_ballPosY <= g_player1PosY + PLAYER_HEIGHT) // racket 1 body
            return true;
        double radiusSqr = BALL_RADIUS * BALL_RADIUS;
        double distX = PLAYER1_POSITION_X + PLAYER_WIDTH - g_ballPosX;
        double distY = g_player1PosY - g_ballPosY;
        double distSqr = distX * distX + distY * distY;
        if (distSqr <= radiusSqr) // racket 1 upper corner
            return true;
        distY = g_player1PosY + PLAYER_HEIGHT - g_ballPosY;
        distSqr = distX * distX + distY * distY;
        if (distSqr <= radiusSqr) // racket 1 lower corner
            return true;
    }
    return false;
}

bool hasBallCollidedPlayer2() {
    if (g_ballPosX + BALL_RADIUS > PLAYER2_POSITION_X - 1) {
        if (g_ballPosY >= g_player2PosY && g_ballPosY <= g_player2PosY + PLAYER_HEIGHT) // racket 2 body
            return true;
        double radiusSqr = BALL_RADIUS * BALL_RADIUS;
        double distX = PLAYER2_POSITION_X - g_ballPosX;
        double distY = g_player2PosY - g_ballPosY;
        double distSqr = distX * distX + distY * distY;
        if (distSqr <= radiusSqr) // racket 2 upper corner
            return true;
        distY = g_player2PosY + PLAYER_HEIGHT - g_ballPosY;
        distSqr = distX * distX + distY * distY;
        if (distSqr <= radiusSqr) // racket 2 lower corner
            return true;
    }
    return false;
}

void drawPixelSDL(const size_t posX, const size_t posY, const unsigned int color) {
    SDL_Rect rect;
    rect.x = posX * VIRTUAL_SCREEN_PIXEL_SIZE;
    rect.y = posY * VIRTUAL_SCREEN_PIXEL_SIZE;
    rect.w = VIRTUAL_SCREEN_PIXEL_SIZE;
    rect.h = VIRTUAL_SCREEN_PIXEL_SIZE;
    SDL_FillRect(g_screen, &rect, color);
}

void drawBall(const size_t posX, const size_t posY, const size_t radius, const unsigned int color) {
    int x = 0;
    int y = radius;
    int p = 1 - radius;

    while(x <= y) {
        drawPixelSDL(posX + x, posY + y, color);
        drawPixelSDL(posX + x, posY - y, color);
        drawPixelSDL(posX - x, posY + y, color);
        drawPixelSDL(posX - x, posY - y, color);
        drawPixelSDL(posX + y, posY + x, color);
        drawPixelSDL(posX + y, posY - x, color);
        drawPixelSDL(posX - y, posY + x, color);
        drawPixelSDL(posX - y, posY - x, color);

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

void drawRacket(const size_t posX, const size_t posY, const size_t w, const size_t h, const unsigned int color) {
    size_t x, y;
    size_t maxX = posX + w - 1;
    size_t maxY = posY + h - 1;
    for (x = posX; x <= maxX; ++x) {
        drawPixelSDL(x, posY, color);
        drawPixelSDL(x, maxY, color);
    }
    for (y = posY + 1; y <= maxY; ++y) {
        drawPixelSDL(posX, y, color);
        drawPixelSDL(maxX, y, color);
    }
}

void drawEverything() {
    drawRacket(PLAYER1_POSITION_X, g_player1PosY, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER1_COLOR);
    drawRacket(PLAYER2_POSITION_X, g_player2PosY, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER2_COLOR);
    drawBall(g_ballPosX, g_ballPosY, BALL_RADIUS, BALL_COLOR);
}
