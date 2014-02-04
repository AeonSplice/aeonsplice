#ifndef _AEONCONTEXT_
#define _AEONCONTEXT_
namespace aeon
{
    class Config; // Prototype config so compiler doesn't flip tables

    class Context
    {
    public:
        Context();
        virtual ~Context();

        virtual void setContextHint(std::string hint, std::string value);
        virtual void setContextHint(int hint, int value);

        virtual bool openContext();
        virtual bool openContext(Config * settings);
        virtual void closeContext();

        virtual void processExtensions(Config * settings);
        virtual void load()=0;

        virtual bool shouldClose();
        virtual bool needsUpdate();

        virtual void processInput();
        virtual void update();
        virtual void render();
    protected:
        GLFWwindow* aWindowHandle;
        Mutex aLock;
    }

    void apiInit();
    void apiTerminate();
}
#endif
