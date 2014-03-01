#ifndef _AEONSTATE_
#define _AEONSTATE_

#include <GLFW/glfw3.h>
#include <functional>

namespace
{
    class Config;

    class State
    {
    public:
        virtual ~State();

        virtual void load(Config * settings,
                          GLFWwindow* window,
                          function<void(State * newState)> handler)=0;

        virtual void executeFrame();
        virtual void changeState(State * newState);

        virtual void processInput()=0;
        virtual void update()=0;
        virtual void render()=0;
    protected:
        Config * aSettings;
        GLFWwindow* aWindowHandle;
        function<void(State * newState)> updateState;
    };
}

#endif // _AEONSTATE_
