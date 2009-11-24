//      scene.h
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


#ifndef SCENE_H
#define SCENE_H

#include <GL/gl.h>

class Scene
{
    public:
        Scene();
        virtual ~Scene();

        enum RenderType {RT_BE, RT_VA, RT_VBO};

        void display(const RenderType rt = RT_BE);

    protected:

        GLfloat *vertices, *colors;
        unsigned verticesCount;
        GLuint vertex_buffer, color_buffer;

        void freeVertices();
        virtual void createVertices() = 0;

        virtual void initVBO();
        virtual void displayBE() =0;
        virtual void displayVA() = 0;
        virtual void displayVBO() = 0;

        virtual void setColor(GLfloat h, GLfloat &r, GLfloat &g, GLfloat &b) = 0;
};

#endif /* SCENE_H */
