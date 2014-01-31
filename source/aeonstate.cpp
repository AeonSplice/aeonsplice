#include "aeonstate.hpp"

namespace aeon
{
    void aeonstate::init(aeon::aeonstack * currentStack){}
    void aeonstate::cleanUp(){}

    void aeonstate::pause(){}
    void aeonstate::resume(){}

    void aeonstate::render(){}
    void aeonstate::update(){}
    void aeonstate::processInput(){}

    bool aeonstate::isPaused(){}
    bool aeonstate::isReady(){}
}
