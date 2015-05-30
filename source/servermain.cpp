#include "aeonserverincludes.hpp"

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 6881
#endif // DEFAULT_PORT

using namespace std;
using namespace aeon;

bool init(int argc, char *argv[]);
void load();
void start();
void cleanUp();

Config * serverConfig;
Context * contextManager;
ServerConnection * aeonServer;

int main(int argc, char *argv[])
{
    try
    {
        if(!init(argc, argv))
        {
            apiTerminate();
            log("Failed to initialize.", AEON_FATAL);
            return EXIT_FAILURE;
        }
        else
        {
            log("Successfully initialized.", AEON_INFO);
        }
        log("Loading assets...", AEON_INFO);
        load();
        log("Running...", AEON_INFO);
        start();
        log("Cleaning up...", AEON_INFO);
        cleanUp();
        log("Exiting successfully.", AEON_INFO);
        return EXIT_SUCCESS;
    }
    catch(exception& e)
    {
        log("Uncaught exception: \""+string(e.what())+"\"", AEON_FATAL);
        return EXIT_CRITICAL_FAILURE;
    }
}

void start()
{
    contextManager->execute();
}

void load()
{
    contextManager->load();
}

bool init(int argc, char *argv[])
{
    contextManager = new ServerContext();

    try
    {
        initAeonDirectories();
    }
    catch(...)
    {
        log("Failed to init Data Directory at \""+getAeonDir()+"\"", AEON_ERROR);
        return false;
    }

    setLogFile(getAeonDir()+"server.log");
    serverConfig = new Config();

    try
    {
        serverConfig->loadFromFile(getAeonDir()+"settings2.ini");
    }
    catch(...)
    {
        log("Failed to load configuration at \""+getAeonDir()+"settings2.ini\"", AEON_WARNING);
    }

    getLogSettings(serverConfig);

    // TODO Command line argument handling. (I.E. Passing port in args)

    // Command line argument handling
	if(argc > 1)
    {
        vector<string> arguments;
        for(int argIter = 1; argIter < argc; argIter++)
        {
            stringstream argStream(argv[argIter]);
            string arg;
            argStream >> arg;
            arguments.push_back(arg);
        }
        for(unsigned int argIter = 0; argIter < arguments.size(); argIter++)
        {
            try
            {
                if(toBoolean(serverConfig->getValue("debug","printArgs")))
                {
                    string temp = "Argv["+toString(argIter)+"] = "+arguments.at(argIter);
                    log(temp, AEON_INFO);
                }
            }
            catch(exception& e)
            {
                serverConfig->setKeyValue("debug","printArgs","true");
                log("Debug->printArgs contained non-boolean value, overwriting to true.", AEON_WARNING);
            }
            if(arguments.at(argIter) == "-debug")
            {
                serverConfig->setKeyValue("debug","debugging","true");
            }
            else
            {
                log("WARNING: Unknown argument \""+arguments.at(argIter)+"\"");
            }
        }
    }
    else
    {
        try
        {
            if(toBoolean(serverConfig->getValue("debug","printArgs")))
                log("No command line args provided.", AEON_INFO);
        }
        catch(invalid_argument e)
        {
            serverConfig->setKeyValue("debug","printArgs","true");
            log("Debug->printArgs contained non-boolean value, overwriting to true.", AEON_WARNING);
        }
    }

    // Initialize underlying graphics and audio engines. (GLFW, glew, CEGUI, that kind of thing)
    try
    {
        apiInit();
    }
    catch(...)
    {
        log("Failed to initialize underlying API(s)", AEON_FATAL);
		return false;
    }

    int fsaa =       initKeyPair(serverConfig, "graphics", "fsaa",      4);
    bool resizable = initKeyPair(serverConfig, "graphics", "resizable", false);
    bool decorated = initKeyPair(serverConfig, "graphics", "decorated", true);

    contextManager->setContextVersion(3,3); // TODO: Leaving OpenGL version hardcoded until the version used becomes important.
    contextManager->setContextHint("FSAA",      toString(fsaa));
    contextManager->setContextHint("resizable", toString(resizable));
    contextManager->setContextHint("decorated", toString(decorated));

    try
    {
        contextManager->openContext(serverConfig);
    }
    catch(...)
    {
        log("Failed to open OpenGL context", AEON_FATAL);
        return false;
    }

    // This will attempt to init Glew (Tells us what extensions are available)
    try
    {
        contextManager->processExtensions(serverConfig);
    }
    catch(...)
    {
        log("Caught exception while initializing Glew.", AEON_ERROR);
    }

    contextManager->setWindowPosition(50,50);

    return true;
}

void cleanUp()
{
    // Obsolete now...I think
    // contextManager->closeContext();
    apiTerminate();
    if(!serverConfig->saveToFile(getAeonDir()+"settings2.ini"))
    {
        log("Failed to save configuration!", AEON_WARNING);
    }
    else
    {
        log("Successfully saved configuration.", AEON_INFO);
    }
}


// =============================================
/*{
    aeonServer = new ServerConnection();
    if(!aeonServer->create(DEFAULT_PORT))
        return 1;
    while(aeonServer->running())
    {
        aeonServer->process();
    }
    aeonServer->destroy();
    return 0;
}*/
