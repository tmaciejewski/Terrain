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

class Terrain
{
    public:

        enum RenderType {RT_BE, RT_VA, RT_VBO};

        Terrain();
        virtual ~Terrain();

        void display(const RenderType rt = RT_BE);
        void loadFromSTRM(const char *filename);

    private:

        int *vertices, w, h;

        void freeVertices();
        void displayBE();
        void displayVA();
        void displayVBO();
        void setColor(GLfloat h);

};

#endif /* TERRAIN_H */
