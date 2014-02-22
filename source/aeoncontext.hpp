#ifndef _AEONCONTEXT_
#define _AEONCONTEXT_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <mutex>

#include "aeoninput.hpp"

namespace aeon
{
    class Config;       // Prototype config so compiler doesn't flip tables

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

        virtual void processExtensions(Config * settings);
        virtual void load()=0;
        virtual void execute()=0;

        virtual bool shouldClose();
        virtual bool needsUpdate()=0;

        virtual void processInput()=0;
        virtual void update()=0;
        virtual void render()=0;
    protected:
        GLFWwindow* aWindowHandle;
        std::mutex aLock;
        InputHandler aInput;
    };

    void apiInit();
    void apiTerminate();
}
#endif
