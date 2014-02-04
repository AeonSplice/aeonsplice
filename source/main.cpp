#define EXIT_CRITICAL_FAILURE 9001
#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0
#define AEON_INFO 3
#define AEON_WARNING 2
#define AEON_ERROR 1
#define AEON_FATAL 0
#include "aeonincludes.hpp"
using namespace std;
using namespace aeon;

bool init(int argc, char *argv[]);
void load();
bool isRunning();
void render();
void cleanUp();

Config settings;
Context contextManager;

int main(int argc, char *argv[])
{
    try
    {
        if(!init(argc, argv))
        {
            apiTerminate();
            log("Failed to initialize", AEON_FATAL);
            return EXIT_FAILURE;
        }
        else
        {
            log("Successfully initialized", AEON_INFO);
        }
        log("Loading assets...", AEON_INFO);
        load();
        log("Running...", AEON_INFO);
        while(isRunning())
        {
            render();
        }
        log("Cleaning up...", AEON_INFO);
        cleanUp();
        log("Exiting successfully", AEON_INFO);
        return EXIT_SUCCESS;
    }
    catch(...)
    {
        return EXIT_CRITICAL_FAILURE;
    }
}

void render()
{
    // TODO: Figure out what method we're using. Standard loop like below, or a simple message pump function

    contextManager.processInput();
    // Checks to see if the game logic is behind schedule. (needsUpdate() should return false if time since last render is greater than some amount. 1s? 16ms?)
    //while(contextManager.needsUpdate())
    //{
        contextManager.update();
    //}
    contextManager.render();
    Sleep(0);
}
void load()
{
    contextManager.load();
}

bool init(int argc, char *argv[])
{
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
    settings = new Config();

    try
    {
        settings.loadFromFile(getAeonDir()+"settings.ini");
    }
    catch(...)
    {
        log("Failed to load configuration at \""+getAeonDir()+"settings.ini\"", AEON_WARNING);
    }

    // TODO: Replace with try{}catch{}
    if(!(settings.loadFromFile(getAeonDir()+"settings.ini")))
    {
        log("Failed to load config", AEON_WARNING);
    }

    getLogSettings(&settings);
    getInputSettings(&settings);

	// Command line argument handling
	if(argc > 1)
    {
        vector<string> arguments;
        for(int argIter = 1; argIter < argc; argIter++)
        {
            stringstream argStream(argv[argIter]);
            string arg;
            argStream >> currentArg;
            arguments.push_back(arg);
        }
        for(int argIter = 0; argIter < arguments.size(); argIter++)
        {
            if(toBoolean(settings.getValue("debug","printArgs")))
            {
                string temp = "Argv["+toString(argIter)+"] = "+arguments.at(argIter);
                log(temp, AEON_INFO);
            }
            // does this properly skip the next argument?
            if(arguments.at(argIter) == "-profile")
            {
                // TODO: and next argument is valid
                if( (argIter+1) < arguments.size() )
                {
                    argIter++;
                    settings.setKeyValue("debug","profile",arguments.at(argIter));
                }
                // TODO: else if enough args and argument is NOT valid
                else
                {
                    log("WARNING: Did not provide value for \"-profile\", ignoring");
                }
            }
            else if(arguments.at(argIter) == "-fullscreen")
            {
                settings.setKeyValue("graphics","fullscreen","true");
            }
            else if(arguments.at(argIter) == "-debug")
            {
                settings.setKeyValue("debug","debugging","true");
            }
            else
            {
                log("WARNING: Unknown argument \""+arguments.at(argIter)+"\"";
            }
        }
    }
    else
    {
        if(toBoolean(settings.getValue("debug","printArgs")))
            log("No command line args provided.", AEON_INFO);
    }

    // Initialize underlying graphics and audio engines. (GLFW, glew, Ogre3d, that kind of thing)
	if( !apiInit() )
	{
		log("Failed to initialize underlying API(s)", AEON_FATAL);
		return false;
	}

    try
    {
        contextManager.openContext(&settings);
    }
    catch(...)
    {
        log("Failed to open OpenGL context", AEON_FATAL);
        return false;
    }

    // This will attempt to init Glew (Tells us what extensions are available)
    try
    {
        contextManager.processExtensions(&settings);
    }
    catch(...)
    {
        log("Caught exception while initializing Glew.", AEON_ERROR);
    }

    return true;
}

void cleanUp()
{
    contextManager.closeContext();
    apiTerminate();
    if(!settings.saveToFile(getAeonDir()+"settings.ini"))
    {
        log("Failed to save configuration!", AEON_WARNING);
    }
    else
    {
        log("Successfully saved configuration.", AEON_INFO);
    }
}

bool isRunning()
{
    if(contextManager.shouldClose())
    {
        return false;
    }
    else
    {
        return true;
    }
}
