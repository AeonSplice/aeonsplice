#ifndef _AEONTRIANGLESTATE_
#define _AEONTRIANGLESTATE_
#include "aeonstate.hpp"
namespace aeon
{
    class trianglestate : public aeonstate
    {
        public:
            trianglestate();

            virtual void init();
            virtual void cleanUp();

            virtual void pause();
            virtual void resume();

            virtual void render();
            virtual void update();
            virtual void processInput();

            virtual bool isPaused();
            virtual bool isReady();
        private:
            bool paused,ready;
            GLuint VertexArrayID;
            GLuint programID;
            GLuint vertexbuffer;
    };
}
#endif
