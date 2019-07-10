#include "UI.h"

/**
\file UI.cpp
\brief User interface processor for the program.

\author    Don Spickler
\version   1.2
\date      1/18/2018

*/

/**
\brief Constructor

\param graph --- Pointer to the GraphicsEngine that this interface processor is attached.

Simply stores the pointer of the GraphicsEngine.

*/

UI::UI(GraphicsEngine* graph)
{
    ge = graph;
    mouseDown = false;
}

/**
\brief Destructor

No implementation needed at this point.

*/

UI::~UI() {}

/**
\brief The method handles the SFML event processing and calls the keyboard state processor
method.

This method processes all events in the current SFML event queue and calls the
corresponding processing method.  At the end it calls the keyboard state processor
method, outside the event loop.

*/

void UI::processEvents()
{
    // Process user events
    sf::Event event;
    while (ge->pollEvent(event))
    {
        // Close Window or Escape Key Pressed: exit
        if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            ge->close();

        // Key is pressed.
        if (event.type == sf::Event::KeyPressed)
            keyPressed(event.key);

        // Window is resized.
        if (event.type == sf::Event::Resized)
            ge->resize();

        if (event.type == sf::Event::MouseMoved)
            processMouseMoved(event.mouseMove);

        if (event.type == sf::Event::MouseButtonPressed)
            processMouseButtonPressed(event.mouseButton);

        if (event.type == sf::Event::MouseButtonReleased)
            processMouseButtonReleased(event.mouseButton);
    }

    // Process the state of the keyboard outside of event firing,
    keyboardStateProcessing();

}

/**
\brief The method updates the theta and psi values of the spherical camera
on a click and drag.  If the control key is down the vertical movement will
alter the radius of the camera.

\param mouseMoveEvent --- The SFML mouse move event structure.

*/

void UI::processMouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent)
{
    bool ctrldown = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

    if (mouseDown)
    {
        if (ctrldown)
        {
            ge->getSphericalCamera()->addR((LastPosition.y - mouseMoveEvent.y)*0.25);
        }
        else
        {
            ge->getSphericalCamera()->addTheta((mouseMoveEvent.x - LastPosition.x)*degf*10);
            ge->getSphericalCamera()->addPsi((mouseMoveEvent.y - LastPosition.y)*degf*10);
        }

        LastPosition.x = mouseMoveEvent.x;
        LastPosition.y = mouseMoveEvent.y;
    }
    else if (rightMouseDown)
    {
        ge->getSphericalCamera()->setR(ge->getSphericalCamera()->getR() + (mouseMoveEvent.y - LastPosition.y) * 0.2);
        LastPosition.x = mouseMoveEvent.x;
        LastPosition.y = mouseMoveEvent.y;
    }
}

/**
\brief On a left mouse click it will track the mouse down and tag the current position
of the mouse as the last position the mouse was at.

\param mouseButtonEvent --- The SFML mouse button event structure.

*/

void UI::processMouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    if (mouseButtonEvent.button == sf::Mouse::Left)
    {
        mouseDown = true;
        LastPosition.x = mouseButtonEvent.x;
        LastPosition.y = mouseButtonEvent.y;
    } else if (mouseButtonEvent.button == sf::Mouse::Right)
    {
        rightMouseDown = true;
        LastPosition.x = mouseButtonEvent.x;
        LastPosition.y = mouseButtonEvent.y;
    }

}

/**
\brief If the left mouse button is released this method will track the release and
exit any drag movement.

\param mouseButtonEvent --- The SFML mouse button event structure.

*/

void UI::processMouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    if (mouseButtonEvent.button == sf::Mouse::Left)
    {
        mouseDown = false;
    }
    else if(mouseButtonEvent.button == sf::Mouse::Right)
    {
        rightMouseDown = false;
    }
}

/**
\brief The function handles the keyboard input events from the user.

\param keyevent --- The SFML key code for the key pressed.

\remark

- M: Toggles between fill mode and line mode to draw the triangles.
- F10: Saves a screen shot of the graphics window to a png file.
- F11 or O: Turns on the spherical camera.
- F12 or P: Turns on the yaw-pitch-roll camera.

*/

void UI::keyPressed(sf::Event::KeyEvent keyevent)
{
    int key = keyevent.code;

    switch (key)
    {
    case sf::Keyboard::F10:
        ge->screenshot();
        break;

    case sf::Keyboard::A:
        ge->setDrawAxes();
        break;

    case sf::Keyboard::B:
        ge->setDrawBorder();
        break;

    case sf::Keyboard::F12:

    case sf::Keyboard::M:
        ge->changeMode();
        break;


    default:
        break;
    }
}

/**
\brief Calls the respective method for key processing depending on
which camera, spherical or yaw-pitch-roll, is currently selected.
If Alt or X is pressed the arrow control goes to the light and if
not the arrow control goes to the camera.

*/

void UI::keyboardStateProcessing()
{
    bool altdown = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);

    if (altdown || sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        keyboardStateProcessingLight();
    else
    {
        keyboardStateProcessingSphericalCamera();
    }
}

/**
\brief The method processes the keyboard state if the spherical camera is the one currently
being used.

\remark

If no modifier keys are pressed:

- Left: Increases the camera's theta value.
- Right: Decreases the camera's theta value.
- Up: Increases the camera's psi value.
- Down: Decreases the camera's psi value.

If the control or Z key is down:

- Up: Decreases the camera's radius.
- Down: Increases the camera's radius.

*/

void UI::keyboardStateProcessingSphericalCamera()
{
    bool ctrldown = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

    if (ctrldown || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getSphericalCamera()->addR(-0.1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getSphericalCamera()->addR(0.1);
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ge->getSphericalCamera()->addTheta(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ge->getSphericalCamera()->addTheta(-1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getSphericalCamera()->addPsi(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getSphericalCamera()->addPsi(-1);
    }
}


/**
\brief The method processes the keyboard state if the light is currently
being used.

\remark

If no modifier keys are pressed:

- Left: Increases the light's theta value.
- Right: Decreases the light's theta value.
- Up: Increases the light's psi value.
- Down: Decreases the light's psi value.

If the control or Z key is down:

- Up: Decreases the light's radius.
- Down: Increases the light's radius.

*/

void UI::keyboardStateProcessingLight()
{
    bool ctrldown = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

    if (ctrldown || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
         //   ge->getLtPos()->addR(-0.1);

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
         //   ge->getLtPos()->addR(0.1);
    }
    else
    {
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
         //   ge->getLtPos()->addTheta(1);

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
         //   ge->getLtPos()->addTheta(-1);

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
         //   ge->getLtPos()->addPsi(1);

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
         //   ge->getLtPos()->addPsi(-1);
    }
}
