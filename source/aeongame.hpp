#ifndef _AEONGAME_
#define _AEONGAME_

#include "aeoncontext.hpp"
#include "aeonstate.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

namespace aeon
{
    // Overwrite default context, and make it load a state (TestState)
    class TestContext : public Context
    {
    public:
        virtual ~TestContext();
        virtual void load();
    };

    // Overwrite default state, and make it render something useful. (a pretty triangle)
    class TestState : public State
    {
    public:
        virtual ~TestState();

        virtual void load(Config * settings, GLFWwindow* window, function<void(State * newState)> handler);

        virtual void processInput();
        virtual void update();
        virtual void render();
    private:
        GLuint vertexbuffer;
        GLuint VertexArrayID;
        GLuint programID;
    };
}

#endif // _AEONGAME_
