#include "SphereWorldTile.h"
/*
SphereWorldTile::SphereWorldTile(glm::vec3 tl, glm::vec3 tr, glm::vec3 br, glm::vec3 bl, glm::vec4 color)
{
    Color = color;
    BorderColor = glm::vec4(0, 0, 0, 1);

    topLeftPos = tl;
    topRightPos = tr;
    bottomRightPos = br;
    bottomLeftPos = bl;
    anchor = glm::vec3(0, 0, 0);

    glGenVertexArrays(1, &vboptr);
    glGenBuffers(1, &eboptr);
    glGenBuffers(1, &bufptr);
    glGenVertexArrays(1, &vboptrborder);
    glGenBuffers(1, &eboptrborder);
    glGenBuffers(1, &bufptrborder);

    LoadDataToGraphicsCard();
}
*/

SphereWorldTile::SphereWorldTile(GLfloat Ppsi, GLfloat Ptheta, GLfloat radius, GLfloat objectPsi, GLfloat objectTheta, glm::vec4 color)
{
    Color = color;
    BorderColor = glm::vec4(1, 1, 1, 1);

    GLfloat r = radius;
    topLeftPos = glm::vec3(r*sin(Ppsi)*cos(Ptheta),r*sin(Ptheta),r*cos(Ppsi)*cos(Ptheta));
    topRightPos = glm::vec3(r*sin(Ppsi+objectPsi)*cos(Ptheta),r*sin(Ptheta),r*cos(Ppsi+objectPsi)*cos(Ptheta));
    bottomLeftPos = glm::vec3(r*sin(Ppsi)*cos(Ptheta+objectTheta),r*sin(Ptheta+objectTheta),r*cos(Ppsi)*cos(Ptheta+objectTheta));
    bottomRightPos = glm::vec3(r*sin(Ppsi+objectPsi)*cos(Ptheta+objectTheta),r*sin(Ptheta+objectTheta),r*cos(Ppsi+objectPsi)*cos(Ptheta+objectTheta));

    anchor = glm::vec3(0, 0, 0);

    glGenVertexArrays(1, &vboptr);
    glGenBuffers(1, &eboptr);
    glGenBuffers(1, &bufptr);
    glGenVertexArrays(1, &vboptrborder);
    glGenBuffers(1, &eboptrborder);
    glGenBuffers(1, &bufptrborder);

    LoadDataToGraphicsCard();
}

SphereWorldTile::~SphereWorldTile()
{
    glBindVertexArray(vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
    glBindVertexArray(vboptrborder);
    glDeleteBuffers(1, &eboptrborder);
    glDeleteBuffers(1, &bufptrborder);
}

void SphereWorldTile::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    Color.r = r;
    Color.g = g;
    Color.b = b;
    Color.a = a;
    //LoadDataToGraphicsCard();
}

void SphereWorldTile::LoadDataToGraphicsCard()
{
    GLuint vPosition = 0;
    GLuint vColor = 1;
    GLuint vNormal = 2;


    GLushort indices[] = {0, 1, 2,
                          0, 2, 3,
                          0, 3, 4,
                          0, 4, 1,
                          1, 2, 3,
                          1, 3, 4
                          };

    GLushort border_indices[] = {1, 2,
                                 2, 3,
                                 3, 4,
                                 4, 1
                                 };

    GLfloat points[] = {anchor.x, anchor.y, anchor.z, 1,
                        topLeftPos.x, topLeftPos.y, topLeftPos.z, 1,
                        topRightPos.x, topRightPos.y, topRightPos.z, 1,
                        bottomRightPos.x, bottomRightPos.y, bottomRightPos.z, 1,
                        bottomLeftPos.x, bottomLeftPos.y, bottomLeftPos.z, 1
                        };

    GLfloat xn = (topLeftPos.x + topRightPos.x + bottomRightPos.x + bottomLeftPos.x)/4.0;
    GLfloat yn = (topLeftPos.y + topRightPos.y + bottomLeftPos.y + bottomRightPos.y)/4.0;
    GLfloat zn = (topLeftPos.z + topRightPos.z + bottomLeftPos.z + bottomRightPos.z)/4.0;

    GLfloat normals[] = {0, 0, 0,
                         xn, yn, zn,
                         xn, yn, zn,
                         xn, yn, zn,
                         xn, yn, zn
                         };

    GLfloat colors[18];
    for(int i = 0; i < 6; i++)
    {
        colors[3*i] = Color.r;
        colors[3*i+1] = Color.g;
        colors[3*i+2] = Color.b;
    }


    GLfloat border_colors[15];
    for(int i = 0; i < 5; i++)
    {
        border_colors[3*i] = BorderColor.r;
        border_colors[3*i+1] = BorderColor.g;
        border_colors[3*i+2] = BorderColor.b;
    }

    glBindVertexArray(vboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);

    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);

    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);

    glBindVertexArray(vboptrborder);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptrborder);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(border_indices), border_indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufptrborder);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(border_colors), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(border_colors), border_colors);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);


}

void SphereWorldTile::draw()
{
    glBindVertexArray(vboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, NULL);

    glBindVertexArray(vboptrborder);
    glLineWidth(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptrborder);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, NULL);

}
