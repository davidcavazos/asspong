#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <SDL.h>

using namespace std;

const int FRAMERATE_CAP = 60;

void processEvents(bool& isRunning);



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
    cout << "[w]    Mover jugador 1 arriba" << endl;
    cout << "[s]    Mover jugador 1 abajo" << endl;
    cout << "[UP]   Mover jugador 2 arriba" << endl;
    cout << "[DOWN] Mover jugador 2 abajo" << endl;
    cout << endl;
    cout << "Presione [Enter] para continuar...";
    cin.get();

    // initialization
    if (SDL_Init(SDL_INIT_VIDEO) != 0) // 0 success, -1 failure
        return EXIT_FAILURE;
    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF);
    if (screen == 0)
        return EXIT_FAILURE;

    // main loop
    Uint32 startTime;
    Uint32 deltaTime;
    bool isRunning = true;
    while (isRunning) {
        startTime = SDL_GetTicks();

        // update
        processEvents(isRunning);

        // draw
        SDL_Rect rect;
        rect.x = 100;
        rect.y = 50;
        rect.w = 4;
        rect.h = 4;
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 255, 0, 0));
        SDL_Flip(screen);

        // framerate cap
        deltaTime = SDL_GetTicks() - startTime;
        SDL_Delay(1000 / FRAMERATE_CAP - deltaTime);

        // show framerate
        deltaTime = SDL_GetTicks() - startTime;
        stringstream title;
        title << "Pong - " << (deltaTime == 0? FRAMERATE_CAP : 1000 / deltaTime) << " fps";
        SDL_WM_SetCaption(title.str().c_str(), "");
    }

    // shutdown
    SDL_Quit();
    return EXIT_SUCCESS;
}



void processEvents(bool& isRunning) {
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

            // player 1
            case SDLK_w:
                break;
            case SDLK_s:
                break;

            // player 2
            case SDLK_UP:
                break;
            case SDLK_DOWN:
                break;
            }
            break;
        }
    }
}
