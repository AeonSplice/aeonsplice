#include "aeonstate.hpp"
// Don't include stuff above here. Because standards.

#include "aeoncontext.hpp"
#include "aeonlog.hpp"
#include "aeonutil.hpp"

#include <thread>

using namespace std;

namespace aeon
{
    void vodoMagic(Context * context, State * newState)
    {
        context->changeState(newState);
    }

    void State::executeFrame()
    {
        this->update();
        this->render();
    }

    void State::changeState(State * newState)
    {
        // TODO: Make this asynchronous, probably causes deadlock atm.
        newState->load(aSettings, aWindowHandle, contextHandle);
        thread aThread (vodoMagic, contextHandle, newState);
        aThread.detach();
    }
    void State::processInput(int key, int scancode, int action, int mods)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        if(action == GLFW_PRESS)
        {
            context.injectKeyDown(glfwToCEGUIKey(key));
            if(toBoolean(this->contextHandle->getSettingValue("debug","debugInput")))
                aeon::log("Key Pressed: "+aeon::toString(key), AEON_INFO);
        }
        else if(action == GLFW_RELEASE)
        {
            context.injectKeyUp(glfwToCEGUIKey(key));
            if(toBoolean(this->contextHandle->getSettingValue("debug","debugInput")))
                aeon::log("Key Released: "+aeon::toString(key), AEON_INFO);
        }
        else if(action == GLFW_REPEAT)
        {
            // TODO: Banana phone
            context.injectKeyDown(glfwToCEGUIKey(key));
            if(toBoolean(this->contextHandle->getSettingValue("debug","debugInput")))
                aeon::log("Key Repeat: "+aeon::toString(key), AEON_INFO);
        }
    }
    void State::processChar(unsigned int codepoint)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        if(toBoolean(this->contextHandle->getSettingValue("debug","debugInput")))
            aeon::log("Char: "+aeon::toString(codepoint), AEON_INFO);
        context.injectChar(codepoint);
    }
    void State::processButtons(int button, int action, int mods)
    {
        try
        {
            CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
            if(action == GLFW_PRESS)
            {
                context.injectMouseButtonDown(glfwToCEGUIButton(button));
            }
            else if(action == GLFW_RELEASE)
            {
                context.injectMouseButtonUp(glfwToCEGUIButton(button));
            }
        }
        catch(...)
        {
            aeon::log("This is not the button state you're looking for.", AEON_ERROR);
        }
    }
    void State::processFocus(int action)
    {
        try
        {
            CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
            if(action == GL_TRUE)
            {
                context.getMouseCursor().show();
                if(toBoolean(this->contextHandle->getSettingValue("debug","debugInput")))
                    log("Window Gained Focus", AEON_INFO);
            }
            else if(action == GL_FALSE)
            {
                context.getMouseCursor().hide();
                if(toBoolean(this->contextHandle->getSettingValue("debug","debugInput")))
                    log("Window Lost Focus", AEON_INFO);
            }
            else
                log("glfw focus callback returned none boolean value.", AEON_ERROR);
        }
        catch(...)
        {
            aeon::log("Failed to focus?", AEON_ERROR);
        }
    }
}
