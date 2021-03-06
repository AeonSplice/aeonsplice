#ifndef _AEONSTATE_
#define _AEONSTATE_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

namespace aeon
{
    class Config;
    class Context;

    class State
    {
    public:
        //virtual ~State()=0;

        virtual void load(Config * settings,
                          GLFWwindow* window,
                          Context * context)=0;

        virtual void executeFrame();
        virtual void changeState(State * newState);

        virtual void processInput(int key, int scancode, int action, int mods);
        virtual void processChar(unsigned int codepoint);
        virtual void processButtons(int button, int action, int mods);
        virtual void processFocus(int action);
        virtual void update()=0;
        virtual void render()=0;
    protected:
        Config * aSettings;
        Context * contextHandle;
        GLFWwindow* aWindowHandle;
    };

    void vodoMagic(Context * context, State * newState);
}

#endif // _AEONSTATE_
