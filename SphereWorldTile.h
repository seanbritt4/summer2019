#ifndef SPHEREWORLDTILE_H
#define SPHEREWORLDTILE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ProgramDefines.h"

class SphereWorldTile
{
    public:
        //SphereWorldTile(glm::vec3 tl, glm::vec3 tr, glm::vec3 br, glm::vec3 bl, glm::vec4 color);
        SphereWorldTile(GLfloat Ppsi, GLfloat Ptheta, GLfloat radius, GLfloat objectPsi, GLfloat objectTheta, glm::vec4 color);
        ~SphereWorldTile();

        void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);

        //may want to implement this later
        //void setBorderColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);

        void draw();

    private:
        glm::vec4 Color;
        glm::vec4 BorderColor;

        //vertex positions
        glm::vec3 topLeftPos;
        glm::vec3 topRightPos;
        glm::vec3 bottomRightPos;
        glm::vec3 bottomLeftPos;
        glm::vec3 anchor;

        GLuint vboptr;  ///< ID for faces VBO.
        GLuint eboptr;  ///< ID for faces index array.
        GLuint bufptr;  ///< ID for faces array buffer.

        GLuint vboptrborder;  ///< ID for faces VBO.
        GLuint eboptrborder;  ///< ID for faces index array.
        GLuint bufptrborder;  ///< ID for faces array buffer.

        void LoadDataToGraphicsCard();

};

#endif // SPHEREWORLDTILE_H
