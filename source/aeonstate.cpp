#include "aeonstate.hpp"
// Don't include stuff above here. Because standards.

namespace aeon
{
    void State::executeFrame()
    {
        this->processInput();
        this->update();
        this->render();
    }

    void State::changeState(State * newState)
    {
        // TODO: Make this asynchronous, probably causes deadlock atm.
        // async(this->updateState, newState);
        // thread aThread (this->updateState, newState).detach();
        this->updateState(newState);
    }
}
