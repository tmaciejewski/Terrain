//      terrain.h
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


#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/gl.h>
#include <GL/glext.h>
#include <vector>

#include "scene.h"

class Terrain : public Scene
{
    public:

        Terrain(unsigned s = 1);
        virtual ~Terrain();

        void loadFromSTRM(const char *filename);

    private:

        unsigned w, h, skip;
        std::vector<int> heights;

        void displayBE();
        void displayVA();
        void displayVBO();
        void createVertices();
        void setColor(GLfloat h, GLfloat &r, GLfloat &g, GLfloat &b);

        void drawArrays();
};

#endif /* TERRAIN_H */
