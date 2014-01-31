#define GLEW_STATIC
#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0
#include "aeonincludes.hpp"
using namespace std;
using namespace aeon;

bool init(int argc, char *argv[]);
void load();
bool isRunning();
void render();
void cleanUp();

aeonconfig settings;
aeonscene sceneManager;

int main(int argc, char *argv[])
{
    if(!init(argc, argv))
    {
        log("FATAL: Failed to initialize");
        return EXIT_FAILURE;
    }
    else
    {
        log("INFO: Successfully initialized");
    }
    log("INFO: Loading assets...");
    load();
    log("INFO: Running...");
	while(isRunning())
	{
		render();
	}
    log("INFO: Cleaning up...");
	cleanUp();
    log("INFO: Exiting successfully");
	return EXIT_SUCCESS;
}

void render()
{
    sceneManager.processInput();
    // Checks to see if the game logic is behind schedule. (needsUpdate() should return false if time since last render is greater than some amount. 1s? 16ms?)
    while(sceneManager.needsUpdate())
    {
        sceneManager.update();
    }
    sceneManager.render();
}
void load()
{
    sceneManager.load();
}

bool init(int argc, char *argv[])
{
    initAeonDirectories();
    setLogFile(getAeonDir()+"log.txt");
    settings = new aeonconfig();
    
    // TODO: Replace with try{}catch{}
    if(!(settings.loadFromFile(getAeonDir()+"settings.ini")))
    {
        log("WARNING: Failed to load config");
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
                string temp = "INFO: Argv[";
                temp+=toString(argIter);
                temp+="] = ";
                temp+=arguments.at(argIter);
                log(temp);
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
            log("INFO: No command line args provided.");
    }
    
    // Initialize underlying graphics and audio engines. (GLFW, glew, Ogre3d, that kind of thing)
	if( !apiInit() )
	{
		log("FATAL: Failed to initialize underlying API(s)");
		return false;
	}
    
    // TODO: Replace with try{}catch{}
    if( !sceneManager.openContext(&settings) )
    {
        log("FATAL: Failed to open OpenGL context");
        apiTerminate();
        return false;
    }
    
    return true;
}

void cleanUp()
{
    sceneManager.closeContext();
    APITerminate();
    if(!settings.saveToFile(getAeonDir()+"settings.ini"))
    {
        log("WARNING: Failed to save configuration!");
    }
    else
    {
        log("INFO: Successfully saved configuration.");
    }
}

bool isRunning()
{
    if(sceneManager.shouldClose())
    {
        return false;
    }
    else
    {
        return true;
    }
}
