//      triangles.cpp
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


#include "triangles.h"
#include <cstdlib>

Triangles::Triangles(unsigned n)
    : Scene(), triangles(n)
{
    srand(0);
}

void Triangles::createVertices()
{
    verticesCount = triangles * 9;

    vertices = new GLfloat[verticesCount];
    colors = new GLfloat[verticesCount];

    for(unsigned i = 0; i < verticesCount; i += 3)
    {
        colors[i] = rand() / (float) RAND_MAX;
        colors[i + 1] = rand() / (float) RAND_MAX;
        colors[i + 2] = rand() / (float) RAND_MAX;

        vertices[i] = rand() / ((float) RAND_MAX / 2);
        vertices[i + 1] = rand() / ((float) RAND_MAX / 2);
        vertices[i + 2] = 0.0;
    }

    initVBO();
}

void Triangles::displayBE()
{
    glBegin(GL_TRIANGLES);
    for (unsigned i = 0; i < verticesCount; i += 3)
    {
        glColor3fv(colors + i);
        glVertex3fv(vertices + i);
    }
    glEnd();
}

void Triangles::displayVA()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*3, vertices);
    glColorPointer(3, GL_FLOAT, sizeof(GLfloat)*3, colors);
    drawArrays();
}

void Triangles::displayVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*3, 0);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glColorPointer(3, GL_FLOAT, sizeof(GLfloat)*3, 0);
    drawArrays();
}

void Triangles::drawArrays()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawArrays(GL_TRIANGLES, 0, triangles * 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
