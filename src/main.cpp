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
#include "triangles.h"

class Game
{
    GLsizei screenWidth, screenHeight;
    std::vector<bool> keyPressed;
    bool isometric;
    Terrain terrain;
    Triangles triangles;
    Scene::RenderType rt;
    SDL_Surface *surface;

    enum SceneType {S_ALL = 3, S_TERRAIN = 2, S_TRIANGLES = 1} sceneType;

    void update()
    {
        if (keyPressed['1'])
            sceneType = S_TERRAIN;
        else if (keyPressed['2'])
            sceneType = S_TRIANGLES;
        else if (keyPressed['3'])
            sceneType = S_ALL;

        if (keyPressed['5'])
            rt = Scene::RT_BE;
        else if (keyPressed['6'])
            rt = Scene::RT_VA;
        else if (keyPressed['7'])
            rt = Scene::RT_VBO;

        if (keyPressed['i'])
        {
            isometric = !isometric;
            keyPressed['i'] = 0;
        }
    }

    void resize()
    {
        // set perspectivic view
        glViewport(0, 0, screenWidth, screenHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
        //gluPerspective(60.0, screenWidth / screenHeight, 1.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
    }

    void display()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        if (isometric)
            gluLookAt(3.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

        if (sceneType & S_TERRAIN)
            terrain.display(rt);

        glLoadIdentity();

        if (sceneType & S_TRIANGLES)
            triangles.display(rt);

        SDL_GL_SwapBuffers();
    }

    SDL_Surface * setVideoMode()
    {
        SDL_Surface *surface;
        int flags = SDL_OPENGL | SDL_RESIZABLE;
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

    void interactiveRun()
    {
        SDL_Event event = {0};
        unsigned frames = 0, ticks = 0;

        while (!keyPressed[SDLK_ESCAPE] && event.type != SDL_QUIT)
        {
            unsigned t;

            update();
            t = SDL_GetTicks();
            display();
            ticks += SDL_GetTicks() - t;
            ++frames;

            if (ticks > 1000 || frames > 50)
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

            usleep(20000);
        }
    }

    void benchmark()
    {
        unsigned frames = 0, time = 0;
        float seconds = 0.0;

        time = SDL_GetTicks();

        while ((SDL_GetTicks() - time) < 3000)
        {
            display();
            ++frames;
        }

        seconds = (SDL_GetTicks() - time) / 1000.0;

        std::cout << "displayed " << frames << " frames in " << seconds
            << " seconds (" << frames / seconds << " FPS)" << std::endl;
    }

    void benchmarkMode()
    {
        sceneType = S_TRIANGLES;
        std::cout << "Scene 1... ";
        benchmark();

        sceneType = S_TERRAIN;
        std::cout << "Scene 2... ";
        benchmark();

        sceneType = S_ALL;
        std::cout << "Scene 1 + 2... ";
        benchmark();
    }

    void benchmarkRun()
    {
        rt = Scene::RT_BE;
        std::cout << "===== Benchmarking glBegin/glEnd mode ======" << std::endl;
        benchmarkMode();

        rt = Scene::RT_VA;
        std::cout << "\n===== Benchmarking Vertex Array mode ======" << std::endl;
        benchmarkMode();

        rt = Scene::RT_VBO;
        std::cout << "\n===== Benchmarking Vertex Buffer Object mode ======" << std::endl;
        benchmarkMode();
    }

    public:

    Game(unsigned w, unsigned h, unsigned n, unsigned skip)
        : screenWidth(w), screenHeight(h), keyPressed(SDLK_LAST, false),
          isometric(true), terrain(skip), triangles(n), rt(Scene::RT_VBO),
          surface(NULL), sceneType(S_ALL)
    {
        srand(time(0));
    }

    int run(const char *filename, bool interactive = true)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Unable to initialize SDL: " << SDL_GetError() << '\n';
            return -1;
        }

        surface = setVideoMode();

        SDL_WM_SetCaption(PACKAGE_STRING, NULL);

        terrain.loadFromSTRM(filename);
        triangles.init();

        initGL();
        resize();

        if (interactive)
            interactiveRun();
        else
            benchmarkRun();

        SDL_Quit();

        return 0;
    }
};

int main(int argc, char **argv)
{
    int skip, n;
    bool interactive = true;

    if (argc < 4)
        skip = 1;
    else
        skip = atoi(argv[3]);

    if (argc < 3)
    {
        n = 100;
    }
    else
    {
        n = atoi(argv[2]);
        interactive = false;
    }

    Game game(600, 600, n, skip);
    return game.run(argc < 2 ? "N45E006.hgt" : argv[1], interactive);
}
