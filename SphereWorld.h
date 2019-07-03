#ifndef SPHEREWORLD_H
#define SPHEREWORLD_H

#include "SphereWorldTile.h"
#include "ProgramDefines.h"
#include <vector>
#include <iostream>

class SphereWorld
{
    public:
        SphereWorld(GLint latDiv, GLint lonDiv, GLfloat r);
        ~SphereWorld();

        GLboolean changeColor(GLuint lat, GLuint lon, glm::vec3 c); //changes color of a tile, returns true if it worked
        GLboolean changeColorArray(glm::vec3 *c); // passes an array "c" by reference to change tile colors
        GLboolean changeRadius(GLuint lat, GLuint lon, GLfloat r);
        void draw();
        void setDrawBorder(GLboolean b);
        glm::vec3 colorGradient(GLfloat r); // returns a color based on a resource value

    private:
        GLboolean drawBorder; // Flag for drawing borders or not
        GLuint vao;     // Vertex array object for storing EVERYTHING (except the border)
        GLuint borderVao; // Stores the border object

        GLuint vertex_vbo; // VBO for vertex positions
        GLuint normal_vbo; // VBO for normals
        GLuint color_vbo; // VBO for vertex colors

        GLuint vertex_vbo_border;  // ID for faces VBO.
        GLuint normal_vbo_border;  // ID for faces index array.
        GLuint color_vbo_border;  // ID for faces array buffer.

        GLuint numTiles;      // Number of subdivisions of the sphere
        GLuint latDivisions;  // Number of latitudinal subdivisions
        GLuint lonDivisions;  // Number of longitudinal subdivisions
        GLfloat radius;     // Radius of the sphere

        void LoadDataToGraphicsCard();
};

#endif // SPHEREWORLD_H
