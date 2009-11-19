//      main.cpp
//
//      Copyright 2009 Tomasz Maciejewski <ponton@jabster.pl>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include <vector>

#include "../config.h"
#include "terrain.h"

class Game
{
    GLsizei screenWidth, screenHeight;
    std::vector<bool> keyPressed;
    bool fullScreen;
    Terrain terrain;

    void update()
    {

    }

    void resize()
    {
        // set perspectivic view
        glViewport(0, 0, screenWidth, screenHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
    }

    void display()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        glTranslatef(0, 0, -1);

        terrain.display(Terrain::RT_VA);

        //glColor3f(1.0, 0.0, 0.0);
        //glBegin(GL_TRIANGLES);
            //glVertex2f(0.0, 0.7);
            //glVertex2f(0.7, -0.7);
            //glVertex2f(-0.7, -0.7);
        //glEnd();

        glFlush();
    }

    SDL_Surface * setVideoMode()
    {
        SDL_Surface *surface;
        int flags = SDL_OPENGL | SDL_RESIZABLE | (fullScreen ? SDL_FULLSCREEN : 0);
        if ((surface = SDL_SetVideoMode(screenWidth, screenHeight, 0, flags)) == NULL)
        {
            std::cerr << "Unable to create OpenGL screen: " << SDL_GetError() << '\n';
            SDL_Quit();
            exit(-1);
        }

        return surface;
    }

    void initGL()
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);

        // depth buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    public:

    Game(unsigned w, unsigned h, bool fullscreen = false)
        : screenWidth(w), screenHeight(h), keyPressed(SDLK_LAST, false),
          fullScreen(fullscreen), terrain()
    {
        srand(time(0));
    }

    int run(const char *filename)
    {
        SDL_Surface *surface = NULL;
        SDL_Event event;
        unsigned frames = 0, ticks = 0;

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Unable to initialize SDL: " << SDL_GetError() << '\n';
            return -1;
        }

        surface = setVideoMode();

        SDL_WM_SetCaption(PACKAGE_STRING, NULL);

        terrain.loadFromSTRM(filename);

        initGL();

        while (!keyPressed[SDLK_ESCAPE] && event.type != SDL_QUIT)
        {
            unsigned t;

            update();
            t = SDL_GetTicks();
            display();
            ticks += SDL_GetTicks() - t;
            ++frames;

            if (ticks > 1000)
            {
                std::string s;
                std::ostringstream str;
                float FPS;
                FPS = (1000.0 * frames) / ticks;
                ticks = frames = 0;
                str << PACKAGE_STRING << ", FPS: " << FPS;
                s = str.str();
                SDL_WM_SetCaption(s.c_str(), NULL);
            }

            SDL_GL_SwapBuffers();

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
                {
                    keyPressed[event.key.keysym.sym] = static_cast<bool>(event.key.state);
                }

                if (event.type == SDL_VIDEORESIZE)
                {
                    screenWidth = event.resize.w;
                    screenHeight = event.resize.h;

                    if (surface)
                        SDL_FreeSurface(surface);

                    surface = setVideoMode();
                    resize();
                }
            }

            usleep(50000);
        }

        SDL_Quit();

        return 0;
    }
};

int main(int argc, char **argv)
{
    Game game(600, 600);
    return game.run(argc < 2 ? "N45E006.hgt" : argv[1]);
}
