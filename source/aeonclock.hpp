#ifndef _AEONCLOCK_
#define _AEONCLOCK_
#include <glfw/glfw3.h>

namespace aeon
{
    double getCurrentTimeMilli()
    {
        return glfwGetTime();
    }
}

#endif
