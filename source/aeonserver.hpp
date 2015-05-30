#ifndef _AEONSERVER_
#define _AEONSERVER_

#include "aeoncontext.hpp"
#include "aeonstate.hpp"
#include "aeoninput.hpp"
#include "aeonnetwork.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 6881
#endif // DEFAULT_PORT

namespace aeon
{
    class ServerInput : public InputHandler
    {
    public:
        virtual void setInputSettings(Config * settings);
    };

    // Overwrites abstract state to something useful.
    class ServerState : public State
    {
    public:
        virtual ~ServerState();

        virtual void load(Config * settings, GLFWwindow* window, Context * context);

        bool TextSubmitted(const CEGUI::EventArgs &e);
        void TextRecieved(CEGUI::String input);
        void registerHandlers();

        virtual void processInput(int key, int scancode, int action, int mods);
        virtual void processChar(unsigned int codepoint);
        virtual void processButtons(int button, int action, int mods);
        virtual void processFocus(int action);
        virtual void update();
        virtual void render();
    protected:
        InputHandler aInput;
        ServerConnection * aServerHandle;
    };

    // Overwrite default context, and make it load a state (ServerState)
    class ServerContext : public Context
    {
    public:
        virtual void load();
    };
}

#endif // _AEONSERVER_
