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
#include <cmath>

Terrain::Terrain(unsigned s) : skip(s)
{

}

Terrain::~Terrain()
{

}

void Terrain::loadFromSTRM(const char *filename)
{
    int16_t x;
    std::ifstream file(filename);
    heights.clear();
    freeVertices();

    for (int i = 0; i < 1201; ++i)
    {
        for (int j = 0; j < 1201; ++j)
        {
            char a, b;
            file.get(a).get(b);
            x = a;
            x <<= 8;
            x += (unsigned char) b;

            if (x == -32768)
                x = heights.back();

            if (j % skip == 0 && i % skip == 0)
                heights.push_back(x);
        }
    }

    w = h = sqrt(heights.size());

    createVertices();
}

void Terrain::createVertices()
{
    int v = 0, c = 0;
    verticesCount = 6 * w * (h - 1);

    vertices = new GLfloat[verticesCount];
    colors = new GLfloat[verticesCount];

    for (unsigned i = 0; i < h - 1; ++i)
    {
        for (unsigned j = 0; j < w; ++j)
        {
            GLfloat x = GLfloat(2*j) / (w - 1), y2 = GLfloat(2*(i + 1)) / (h - 1),
                y1 = GLfloat(2*i) / (h - 1), h2 = heights[w * (i + 1) + j],
                h1 = heights[w * i + j], r, g, b;

            setColor(h2, r, g, b);
            colors[c] = r; colors[c + 1] = g; colors[c + 2] = b;
            c += 3;

            setColor(h1, r, g, b);
            colors[c] = r; colors[c + 1] = g; colors[c + 2] = b;
            c += 3;

            vertices[v] = x; vertices[v + 1] = y2; vertices[v + 2] = h2 / 60000.0;
            v += 3;

            vertices[v] = x; vertices[v + 1] = y1; vertices[v + 2] = h1 / 60000.0;
            v += 3;
        }
    }

    initVBO();
}

void Terrain::setColor(GLfloat h, GLfloat &r, GLfloat &g, GLfloat &b)
{
    if (h <= 0)
    {
        r = 0.0;
        g = 0.0;
        b = 1.0;
    }
    else if (h <= 500)
    {
        r = 0.0;
        g = h / 500.0;
        b = 0.0;
    }
    else if (h <= 1000)
    {
        r = (h - 500.0) / 500.0;
        g = 1.0;
        b = 0.0;
    }
    else if (h <= 1500)
    {
        r = 1.0;
        g = 1.0 - (h - 1000.0) / 500.0;
        b = 0.0;
    }
    else
    {
        r = 1.0 - (h - 1500.0) / 6000.0;
        g = 0.0;
        b = 0.0;
    }
}

void Terrain::displayBE()
{
    for (unsigned i = 0; i < h - 1; ++i)
    {
        glBegin(GL_QUAD_STRIP);
        for (unsigned j = 0; j < 2 * w; ++j)
        {
            int index = 3 * (i * 2 * w + j);

            glColor3fv(colors + index);
            glVertex3fv(vertices + index);
        }
        glEnd();
    }
}

void Terrain::displayVA()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*3, vertices);
    glColorPointer(3, GL_FLOAT, sizeof(GLfloat)*3, colors);
    drawArrays();
}

void Terrain::displayVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, 0);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glColorPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, 0);
    drawArrays();
}

void Terrain::drawArrays()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    for (unsigned i = 0; i < h - 1; ++i)
    {
        glDrawArrays(GL_QUAD_STRIP, i * 2 * w, 2 * w);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
