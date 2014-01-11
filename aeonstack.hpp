#ifndef _AEONSTACK_
#define _AEONSTACK_
#include <vector>
#include "aeonstate.hpp"
namespace aeon
{
    class aeonstack
    {
    public:
        aeonstack();
        /**
         * Pushes state on top of stack.
         */
        void push(aeonstate * state);
        /**
         * Swaps top state with new state.
         */
        void swap(aeonstate * state);
        /**
         * Removes top state. (Does nothing if only one state is left)
         */
        void pop();

        void render();
        void update();

        void cleanUp();
    private:
        std::vector<aeonstate*> statestack;
    };
}
#endif // _AEONSTACK_
