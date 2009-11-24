//      scene.cpp
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


#include "scene.h"


Scene::Scene() : vertices(NULL), colors(NULL)
{

}


Scene::~Scene()
{

}

void Scene::freeVertices()
{
    if (vertices)
        delete[] vertices;

    if (colors)
        delete[] colors;
}

void Scene::display(const RenderType rt)
{
    glPushMatrix();
    glTranslatef(-1.0, -1.0, 0.0);

    if (rt == RT_BE)
        displayBE();
    else if (rt == RT_VA)
        displayVA();
    else if (rt == RT_VBO)
        displayVBO();

    glPopMatrix();
}

void Scene::initVBO()
{
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), colors, GL_STATIC_DRAW);
}
