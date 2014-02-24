#ifndef _AEONGAME_
#define _AEONGAME_

#include "aeoncontext.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

namespace aeon
{
    class TestContext : public Context
    {
    public:
        virtual ~TestContext();
        virtual void load();
        virtual void execute();
        virtual bool needsUpdate();
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
