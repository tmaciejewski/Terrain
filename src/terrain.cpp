//      terrain.cpp
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


#include "terrain.h"
#include <fstream>
#include <iostream>
#include <climits>

Terrain::Terrain() : vertices(NULL)
{

}

Terrain::~Terrain()
{
    freeVertices();
}

void Terrain::loadFromSTRM(const char *filename)
{
    std::ifstream file(filename);
    freeVertices();
    vertices = new int[1201*1201];
    for (int i = 0; i < 1201*1201; ++i)
    {
        char a, b;
        file.get(a).get(b);
        vertices[i] = a;
        vertices[i] <<= 8;
        vertices[i] += b;
    }

    w = h = 1201;
    //w = h = 100;
}

void Terrain::freeVertices()
{
    if (vertices)
        delete[] vertices;
}

void Terrain::display(const RenderType rt)
{
    if (rt == RT_BE)
        displayBE();
}

void Terrain::setColor(GLfloat h)
{
    if (h <= 0)
        glColor3f(0.0, 0.0, 1.0);
    else if (h < 500)
        glColor3f(0.0, h / 500.0, 0.0);
    else if (h < 1000)
        glColor3f((h - 500.0) / 500.0, 1.0, 0.0);
    else if (h < 1500)
        glColor3f(1.0, 1.0 - (h - 1000.0) / 500.0, 0.0);
    else
        glColor3f(1.0 - (h - 1500.0) / 8000.0, 0.0, 0.0);
}

void Terrain::displayBE()
{
    if (h < 2 || w < 2)
        return;

    glPushMatrix();
    glTranslatef(-1.0, -1.0, 0.0);
    for (int i = 0; i < h - 1; ++i)
    {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < w; ++j)
        {
            GLfloat x = GLfloat(2*j) / (w - 1), y2 = GLfloat(2*(i + 1)) / (h - 1),
                y1 = GLfloat(2*i) / (h - 1), h2 = vertices[w * (i + 1) + j],
                h1 = vertices[w * i + j];

            if (h1 == SHRT_MIN || h2 == SHRT_MIN)
                continue;

            setColor(h2);
            glVertex3f(x, y2, h2 / 60000.0);
            setColor(h1);
            glVertex3f(x, y1, h1 / 60000.0);
        }
        glEnd();
    }
    glPopMatrix();
}
