#include "aeonstate.hpp"
// Don't include stuff above here. Because standards.

#include "aeoncontext.hpp"

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
        this->processInput();
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
}
