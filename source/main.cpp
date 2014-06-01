#include "aeonincludes.hpp"
using namespace std;
using namespace aeon;

bool init(int argc, char *argv[]);
void load();
void start();
void cleanUp();

Config * mainConfig;
Context * contextManager;

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
    contextManager = new TestContext();

    try
    {
        initAeonDirectories();
    }
    catch(...)
    {
        log("Failed to init Data Directory at \""+getAeonDir()+"\"", AEON_ERROR);
        return false;
    }

    setLogFile(getAeonDir()+"debugging.log");
    mainConfig = new Config();

    try
    {
        mainConfig->loadFromFile(getAeonDir()+"settings.ini");
    }
    catch(...)
    {
        log("Failed to load configuration at \""+getAeonDir()+"settings.ini\"", AEON_WARNING);
    }

    getLogSettings(mainConfig);

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
                if(toBoolean(mainConfig->getValue("debug","printArgs")))
                {
                    string temp = "Argv["+toString(argIter)+"] = "+arguments.at(argIter);
                    log(temp, AEON_INFO);
                }
            }
            catch(exception& e)
            {
                mainConfig->setKeyValue("debug","printArgs","true");
                log("Debug->printArgs contained non-boolean value, overwriting to true.", AEON_WARNING);
            }
            // does this properly skip the next argument?
            if(arguments.at(argIter) == "-profile")
            {
                // TODO: and next argument is valid
                if( (argIter+1) < arguments.size() )
                {
                    argIter++;
                    mainConfig->setKeyValue("debug","profile",arguments.at(argIter));
                }
                // TODO: else if enough args and argument is NOT valid
                else
                {
                    log("WARNING: Did not provide value for \"-profile\", ignoring");
                }
            }
            else if(arguments.at(argIter) == "-fullscreen")
            {
                mainConfig->setKeyValue("graphics","fullscreen","true");
            }
            else if(arguments.at(argIter) == "-debug")
            {
                mainConfig->setKeyValue("debug","debugging","true");
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
            if(toBoolean(mainConfig->getValue("debug","printArgs")))
                log("No command line args provided.", AEON_INFO);
        }
        catch(invalid_argument e)
        {
            mainConfig->setKeyValue("debug","printArgs","true");
            log("Debug->printArgs contained non-boolean value, overwriting to true.", AEON_WARNING);
        }
    }

    // NOTE: Sets log settings again, to account for arguments like -debug
    getLogSettings(mainConfig);

    // Initialize underlying graphics and audio engines. (GLFW, glew, Ogre3d, that kind of thing)
    try
    {
        apiInit();
    }
    catch(...)
    {
        log("Failed to initialize underlying API(s)", AEON_FATAL);
		return false;
    }

    int fsaa =       initKeyPair(mainConfig, "graphics", "fsaa",      4);
    bool resizable = initKeyPair(mainConfig, "graphics", "resizable", false);
    bool decorated = initKeyPair(mainConfig, "graphics", "decorated", true);

    contextManager->setContextVersion(3,3); // TODO: Leaving OpenGL version hardcoded until the version used becomes important.
    contextManager->setContextHint("FSAA",      toString(fsaa));
    contextManager->setContextHint("resizable", toString(resizable));
    contextManager->setContextHint("decorated", toString(decorated));

    try
    {
        contextManager->openContext(mainConfig);
    }
    catch(...)
    {
        log("Failed to open OpenGL context", AEON_FATAL);
        return false;
    }

    // This will attempt to init Glew (Tells us what extensions are available)
    try
    {
        contextManager->processExtensions(mainConfig);
    }
    catch(...)
    {
        log("Caught exception while initializing Glew.", AEON_ERROR);
    }

    contextManager->setWindowPosition(100,100);

    return true;
}

void cleanUp()
{
    // Obsolete now...I think
    // contextManager->closeContext();
    apiTerminate();
    if(!mainConfig->saveToFile(getAeonDir()+"settings.ini"))
    {
        log("Failed to save configuration!", AEON_WARNING);
    }
    else
    {
        log("Successfully saved configuration.", AEON_INFO);
    }
}
