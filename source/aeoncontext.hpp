#ifndef _AEONCONTEXT_
#define _AEONCONTEXT_
namespace aeon
{
    class Config; // Prototype config so compiler doesn't flip tables
    
    class Context
    {
    public:
        Context();
        ~Context();
        
        void setContextHint(std::string hint, std::string value);
        
        bool openContext();
        bool openContext(Config * settings);
        void closeContext();
        
        void processExtensions(Config * settings);
        void load();
        
        bool shouldClose();
        bool needsUpdate();
        
        void processInput();
        void update();
        void render();
    }
    
    apiInit();
    apiTerminate();
}
#endif
