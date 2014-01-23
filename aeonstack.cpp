#include <vector>
#include <glfw/glfw3.h>
#include "aeonstate.hpp"
#include "aeonconfig.hpp"
#include "aeonlog.hpp"
#include "aeonwindow.hpp"
#include "aeonstack.hpp"

using namespace std;

namespace aeon
{
    aeonstack::aeonstack()
    {}
    void aeonstack::push(aeonstate * state)
    {
        // my race condition sense is tingling
        if(!state->isReady())
        {
            state->init(this);
        }
        aeonstack::statestack.push_back(state);
    }
    void aeonstack::pop()
    {
        if(!aeonstack::statestack.empty())
        {
            if(aeonstack::statestack.size()>1)
            {
                aeonstack::statestack.at(aeonstack::statestack.size()-1)->cleanUp();
                aeonstack::statestack.pop_back();
            }
            else
            {
                aeon::log("WARNING: Tried to pop only state.");
            }
        }
        else
        {
            aeon::log("WARNING: Tried to pop empty state stack.");
        }
    }
    void aeonstack::update()
    {
        glfwPollEvents();
        if(!aeonstack::statestack.empty())
        {
            aeonstack::statestack.at(aeonstack::statestack.size()-1)->processInput();
            if(aeonstack::statestack.size() > 1)
            {
                for(unsigned int stateIter = 0; stateIter<aeonstack::statestack.size(); stateIter++)
                {
                    aeonstack::statestack.at(stateIter)->update();
                }
            }
            else
            {
                aeonstack::statestack.at(0)->update();
            }
        }
    }
    void aeonstack::render()
    {
        // Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(!aeonstack::statestack.empty())
        {
            if(aeonstack::statestack.size() > 1)
            {
                for(unsigned int stateIter = 0; stateIter<aeonstack::statestack.size(); stateIter++)
                {
                    aeonstack::statestack.at(stateIter)->render();
                }
            }
            else
            {
                aeonstack::statestack.at(0)->render();
            }
        }
        glfwSwapBuffers(getMainWindowHandle());
    }
    void aeonstack::cleanUp()
    {
        if(!aeonstack::statestack.empty())
        {
            if(aeonstack::statestack.size()>1)
            {
                for(unsigned int stateIter = 0; stateIter<aeonstack::statestack.size(); stateIter++)
                {
                    aeonstack::statestack.at(stateIter)->cleanUp();
                }
            }
            else
            {
                aeonstack::statestack.at(0)->cleanUp();
            }
        }
        else
        {
            aeon::log("INFO: State stack is empty, no need to clean.");
        }
    }
}
