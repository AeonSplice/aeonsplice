#ifndef _AEONSTATE_
#define _AEONSTATE_
namespace aeon
{
    class aeonstate
    {
    public:
        virtual void init() = 0;
        virtual void cleanUp() = 0;

        virtual void pause() = 0;
        virtual void resume() = 0;

        virtual void render() = 0;
        virtual void update() = 0;
        virtual void processInput() = 0;

        virtual bool isPaused() = 0;
        virtual bool isReady() = 0;
    };
}
#endif
