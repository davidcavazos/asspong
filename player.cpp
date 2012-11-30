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
#include <cstdlib>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

using namespace std;

const int AUDIO_FREQUENCY = 44100;
const int AUDIO_CHANNELS = 2; // stereo
const int AUDIO_BUFFER_SIZE = 4096;

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Error: too few arguments, specify audio file" << endl;
        return EXIT_FAILURE;
    }

    // SDL initialization
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { // 0 success, -1 failure
        cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    // SDL_mixer initialization
    Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFER_SIZE);
    Mix_Music* music = Mix_LoadMUS(argv[1]);

    // play music
    if (music == 0)
        cerr << "Unable to load music: " << Mix_GetError() << endl;
    else
        Mix_PlayMusic(music, -1);

    while (true) {
    }

    // shutdown
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    return EXIT_SUCCESS;
}
