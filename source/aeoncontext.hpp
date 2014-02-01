#ifndef _AEONCONTEXT_
#define _AEONCONTEXT_
namespace aeon
{
    class Config; // Prototype config so compiler doesn't flip tables
    
    class Context
    {
    public:
        virtual void setContextHint(std::string hint, std::string value);
        
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
    }
    
    void apiInit();
    void apiTerminate();
}
#endif
