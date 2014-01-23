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

config * settingsPointer;
aeonstack * myStack;

int main(int argc, char *argv[])
{
    // Initilizes underlying API's and retrieves the configuration file
    cout << "INFO: Initilizing..." << endl;
    if(!init(argc, argv))
    {
        log("FATAL: Failure to initilize.");
        return EXIT_FAILURE;
    }
    // Loads the assets required at startup (intro animation, main menu textures, first rendering objects, etc)
    log("INFO: Loading assets...");
    load();
    // Run the main game loop
    log("INFO: Running...");
	while(isRunning())
	{
        // Should render current context (main menu, etc)
		render();
	}
    log("INFO: Cleaning up...");
    // Terminates underlying APIs and releases all memory
	cleanUp();
    log("INFO: Exiting successfully.");
    // Everything ran and didn't throw an error, return 0 (successful)
	return EXIT_SUCCESS;
}

void render()
{
    myStack->update();
    myStack->render();
}
void load()
{
    myStack = new aeonstack();
    boxstate * box = new boxstate();
    trianglestate * triangle = new trianglestate();
    myStack->push(triangle);
    myStack->push(box);
    centerMouse();
}

void error_callback(int error, const char* description)
{
    string temp = "GLFW ERROR #"+toString(error)+": "+string(description);
    log(temp);
}

bool init(int argc, char *argv[])
{
    createAeonDirectories();
    setLogFile(getUserDir()+"\\.aeonsplice\\log.txt");
    settingsPointer = new config();
    if(!(settingsPointer->loadFromFile((getUserDir())+"\\.aeonsplice\\settings.ini")))
    {
        log("WARNING: Failed to load config.");
    }
    getLogSettings(settingsPointer);
    getInputSettings(settingsPointer);
    glfwSetErrorCallback(error_callback);
	// Initialise GLFW (OpenGL), and any other APIs (OpenAL?)
	if( !APIInit() )
	{
		log("FATAL: Failed to initialize underlying API(s).");
		return false;
	}
	// Command line argument handling
	if(argc > 1)
    {
        for(int argIter = 1; argIter < argc; argIter++)
        {
            stringstream argStream(argv[argIter]);
            string currentArg;
            argStream >> currentArg;
            string temp = "INFO: Argv[";
            temp+=toString(argIter);
            temp+="] = ";
            temp+=argv[argIter];
            log(temp);
            if(currentArg == "-useAnyProfile")
            {
                settingsPointer->setKeyValue("debug","profile","anyProfile");
            }
            else if(currentArg == "-useCoreProfile")
            {
                settingsPointer->setKeyValue("debug","profile","coreProfile");
            }
            else if(currentArg == "-fullscreen")
            {
                settingsPointer->setKeyValue("graphics","fullscreen","true");
            }
        }
    }
    else
    {
        log("INFO: No command line args provided.");
    }
    int fsaa = initKeyPair(settingsPointer, "graphics", "anti-aliasing", 4);
    bool resizable = initKeyPair(settingsPointer, "graphics", "resizable", false);
    bool decorated = initKeyPair(settingsPointer, "graphics", "decorated", true);
    // Set openGL version to 4.3
    setGLVersion(3,3);
    // Set anti-aliasing to X samples.
    setFSAA(fsaa);
    // Disable resizing the window. (Prevent user from breaking stuff)
	setResizable(resizable);
	// Disable window decorations (border, widgets, etc)
	setDecorated(decorated);
    // Attempt to open window context
    if( !openWindow(settingsPointer) )
    {
        log("FATAL: Failed to open OpenGL window.");
		APITerminate();
		return false;
    }

    // GLEW must be declared after the GLFW window context is available :(

	glewExperimental = true; // Needed for core profile
    GLenum GlewInitResult;
    GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        if(settingsPointer->exists("debug","isDebugMode"))
        {
            string temp = settingsPointer->getValue("debug","isDebugMode");
            if(temp == "true" || temp == "1")
            {
                FILE* logFile = getLogFile();
                fprintf(logFile,"%s - INFO: OpenGL Version: %s\n",currentDateTime().c_str(),glGetString(GL_VERSION));
                fclose(logFile);
            }
        }
        else
        {
            FILE* logFile = getLogFile();
            fprintf(logFile,"%s - INFO: OpenGL Version: %s\n",currentDateTime().c_str(),glGetString(GL_VERSION));
            fclose(logFile);
        }
        return false;
    }
    if(settingsPointer->exists("debug","isDebugMode"))
    {
        string temp = settingsPointer->getValue("debug","isDebugMode");
        if(temp == "true" || temp == "1")
        {
            FILE* logFile = getLogFile();
            fprintf(logFile,"%s - INFO: OpenGL Version: %s\n",currentDateTime().c_str(),glGetString(GL_VERSION));
            fclose(logFile);
        }
    }
    else
    {
        FILE* logFile = getLogFile();
        fprintf(logFile,"%s - INFO: OpenGL Version: %s\n",currentDateTime().c_str(),glGetString(GL_VERSION));
        fclose(logFile);
    }
    return true;
}

void cleanUp()
{
    myStack->cleanUp();
    APITerminate();
    if(!settingsPointer->saveToFile((getUserDir())+"/.aeonsplice/settings.ini"))
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
    if(windowShouldClose())
    {
        return false;
    }
    else
    {
        return true;
    }
}
