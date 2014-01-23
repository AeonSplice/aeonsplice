#ifndef _AEONBOXSTATE_
#define _AEONBOXSTATE_
#include "aeonstate.hpp"
namespace aeon
{
    class boxstate : public aeonstate
    {
        public:
            boxstate();

            virtual void init(aeonstack * currentStack);
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
            GLuint MatrixID;
            GLuint vertexbuffer;
            GLuint uvbuffer;
            glm::mat4 ProjectionMatrix;
            glm::mat4 ViewMatrix;
            glm::mat4 ModelMatrix;
            glm::mat4 MVP;
            GLuint Texture;
            GLuint TextureID;
            aeonstack * thisStack;
    };
}
#endif
