#ifndef _AEONGAME_
#define _AEONGAME_

#include "aeoncontext.hpp"
#include "aeonstate.hpp"
#include "aeoninput.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

namespace aeon
{
    class TestInput : public InputHandler
    {
    public:
        virtual void setInputSettings(Config * settings);
    };

    // Overwrite default state, and make it render something useful. (a pretty triangle)
    class TestState : public State
    {
    public:
        virtual ~TestState();

        virtual void load(Config * settings, GLFWwindow* window, Context * context);

        void loadGL();

        virtual void processInput(int key, int scancode, int action, int mods);
        virtual void update();
        virtual void render();
    protected:
        InputHandler aInput;
        GLuint vertexbuffer;
        GLuint VertexArrayID;
        GLuint programID;
    };

    // Overwrite default context, and make it load a state (TestState)
    class TestContext : public Context
    {
    public:
        virtual void load();
    };
}

#endif // _AEONGAME_
