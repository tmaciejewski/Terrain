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
        std::cout << vertices[i] << ' ';
    }
}

void Terrain::freeVertices()
{
    if (vertices)
        delete[] vertices;
}
