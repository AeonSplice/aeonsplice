#ifndef _AEONCONTEXT_
#define _AEONCONTEXT_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <mutex>

//#include "aeoninput.hpp"
//#include "aeonstate.hpp"

namespace aeon
{
    class Config;       // Prototype config so compiler doesn't flip tables
    class State;
    class InputHandler; // And inputhandler...

    class Context
    {
    public:
        virtual ~Context();

        virtual void setContextHint(std::string target, std::string hint);
        virtual void setContextHint(int target, int hint);
        virtual void setContextVersion(int major, int minor);

        virtual void openContext();
        virtual void openContext(Config * settings);
        virtual void closeContext();

        virtual void processInput(int key, int scancode, int action, int mods);
        virtual void processExtensions(Config * settings);
        virtual void load()=0;
        virtual void execute();
        virtual void changeState(State * newState);

        virtual bool shouldClose();
    protected:
        GLFWwindow* aWindowHandle;
        Config * aSettings;
        State * aState;
        std::mutex aLock;
    };

    static void vodoInput(GLFWwindow* window, int key, int scancode, int action, int mods);

    void apiInit();
    void apiTerminate();
}
#endif
