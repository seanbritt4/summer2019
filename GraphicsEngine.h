#ifndef GRAPHICSENGINE_H_INCLUDED
#define GRAPHICSENGINE_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LoadShaders.h"
#include "ProgramDefines.h"
#include "SphericalCamera.h"
#include "YPRCamera.h"
#include "Axes.h"
#include "teapot.h"
#include "Material.h"
#include "Light.h"
#include "MaterialPresets.h"
#include "Models.h"
#include "SphereWorld.h"
/**
\file GraphicsEngine.h
\brief Header file for GraphicsEngine.cpp

\author    Don Spickler
\version   1.2
\date      1/20/2018

*/

/**
\class GraphicsEngine

\brief The GraphicsEngine class is an extension of sf::RenderWindow which
handles all of the graphics rendering in the program.

*/

class GraphicsEngine : public sf::RenderWindow
{
private:
    GLenum mode;    ///< Mode, either point, line or fill.
    int sscount;    ///< Screenshot count to be appended to the screenshot filename.
    Axes axes;

    GLboolean drawBorder; // Boolean denoting whether or not to draw borders between tiles
    GLboolean drawAxes; // Boolean for drawing axes or not

    SphereWorld* sphereworld;

    GLuint ModelLoc;   ///< Location ID of the Model matrix in the shader.
    GLuint NormalLoc;  ///< Location ID of the Normal matrix in the shader.
    GLuint PVMLoc;     ///< Location ID of the PVM matrix in the shader.
    GLuint program;    ///< ID of the shader program.

    SphericalCamera sphcamera;   ///< Spherical Camera

    SphericalCamera LtPos[10];   ///< Spherical "Camera" to control position of the light.

    Material mat;   ///< Material of the current object.
    Light lt[10];   ///< Light object.

    glm::mat4 projection;  ///< Projection Matrix
    glm::mat4 model;       ///< Model Matrix

    void printOpenGLErrors();
    void print_GLM_Matrix(glm::mat4 m);
    void print_GLM_Matrix(glm::mat3 m);
    void handleAnimation();
    GLfloat animTime = 1.0f;
    GLfloat currentEmmisive = 0;
    GLint currentEmmisiveObject = 0;

public:
    GraphicsEngine(std::string title = "OpenGL Window", GLint MajorVer = 3, GLint MinorVer = 3,
                   int width = 600, int height = 600);
    ~GraphicsEngine();

    void display();
    void changeMode();
    void screenshot();
    void resize();
    void setSize(unsigned int, unsigned int);
    GLfloat* getScreenBounds();
    void setDrawAxes();
    void setDrawBorder();
    void setEmmissive(int object, GLfloat value);
    sf::Clock animclock;

    GLboolean isSphericalCameraOn();
    void setSphericalCameraOn();
    GLboolean isYPRCameraOn();
    void setYPRCameraOn();

    void loadLight(Light Lt);
    void loadMaterial(Material Mat);

    void turnLightOn();
    void turnLightOff();
    void turnLightOn(std::string name, int i);
    void turnLightOff(std::string name, int i);
    void turnLightsOn(std::string name, int num);
    void turnLightsOff(std::string name, int num);

    void LoadLights(Light Lt[], std::string name, int num);
    void LoadLight(Light Lt, std::string name, int i);

    void addToAnimTime(GLfloat a);
    void resetAnimTime();
    SphericalCamera* getLtPos();
    SphericalCamera* getSphericalCamera();
    YPRCamera* getYPRCamera();
};

#endif // GRAPHICSENGINE_H_INCLUDED
