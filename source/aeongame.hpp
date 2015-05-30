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
    /*class TestInput : public InputHandler
    {
    public:
        virtual void setInputSettings(Config * settings);
    };*/

    // Overwrite default state, and make it render something useful. (a pretty triangle)
    class LoginState : public State
    {
    public:
        virtual ~LoginState();

        virtual void load(Config * settings, GLFWwindow* window, Context * context);

        // Utility functions
        std::string getPasswordSalt();
        bool loginToServer(std::string username, std::string password, bool remember);

        // The fifty billion event subscriber functions
        bool login(const CEGUI::EventArgs &e);
        bool exit(const CEGUI::EventArgs &e);
        bool hideLoginFailure(const CEGUI::EventArgs &e);
        bool usernameSubmitted(const CEGUI::EventArgs &e);
        bool showRegister(const CEGUI::EventArgs &e);
        bool hideRegister(const CEGUI::EventArgs &e);
        bool showForgotPass(const CEGUI::EventArgs &e);
        bool hideForgotPass(const CEGUI::EventArgs &e);

        void registerHandlers();

        virtual void update();
        virtual void render();
    protected:
        InputHandler aInput;
    };

    // Overwrite default context, and make it load a state (LoginState)
    class MainContext : public Context
    {
    public:
        virtual void load();
    };
}

#endif // _AEONGAME_
