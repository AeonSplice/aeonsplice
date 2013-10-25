#define GLEW_STATIC
#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0
#include "AeonIncludes.hpp"
using namespace std;

bool init();
void load();
bool isRunning();
void render();
void cleanUp();

aeon::config * settingsPointer;
//aeon::renderer* intro;

int main(int argc, char *argv[])
{
    // Initilizes underlying API's and retrieves the configuration file
    cout << "INFO: Initilizing..." << endl;
    if(!init())
    {
        cout << "FATAL: Failure to initilize." << endl;
        return EXIT_FAILURE;
    }
    // Loads the assets required at startup (intro animation, main menu textures, first rendering objects, etc)
    cout << "INFO: Loading assets..." << endl;
    load();
    // Run the main game loop
    cout << "INFO: Running..." << endl;
	while(isRunning())
	{
        // Should render current context (main menu, etc)
		render();
		//break;
	}
    cout << "INFO: Cleaning up..." << endl;
    // Terminates underlying APIs and releases all memory
	cleanUp();
    cout << "INFO: Exiting successfully." << endl;
    // Everything ran and didn't throw an error, return 0 (successful)
	return EXIT_SUCCESS;
}

void render()
{
    glClear( GL_COLOR_BUFFER_BIT );
    glfwSwapBuffers();
}

void load()
{
    // Things that need to happen after everything is up and running
}

bool init()
{
    //aeon::config settings;
    settingsPointer = new aeon::config();
    if(!(settingsPointer->loadFromFile((aeon::getUserDir())+"\\.aeonsplice\\settings.ini")))
    {
        fprintf( stderr, "WARNING: Failed to load config.\n" );
    }
    //settingsPointer->print();
	// Initialise GLFW (OpenGL), and any other APIs (OpenAL?)
	if( !aeon::APIInit() )
	{
		fprintf( stderr, "FATAL: Failed to initialize underlying API(s).\n" );
		return false;
	}
    // Set openGL version to 4.3
    aeon::setGLVersion(4,3);
    // Set anti-aliasing to 4 samples.
    aeon::setFSAA(4);
    // Disable resizing the window. (Prevent user from breaking stuff)
	aeon::setResizable(false);
    // Attempt to open window context
    if( !aeon::openWindow(*settingsPointer) )
    {
        fprintf( stderr, "FATAL: Failed to open OpenGL window.\n" );
		aeon::APITerminate();
		return false;
    }

    // GLEW must be declared after the GLFW window context is available :(

	glewExperimental = true; // Needed for core profile
    GLenum GlewInitResult;
    GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        fprintf(stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
        return false;
    }

    fprintf(stdout,"INFO: OpenGL Version: %s\n",glGetString(GL_VERSION));

    glClearColor(0.0f, 0.4f, 0.8f, 0.5f);
	aeon::centerCursor();

    // Init camera with default settings
    //initCamera();
    return true;
}

void cleanUp()
{
    aeon::APITerminate();
    settingsPointer->print();
    if(!settingsPointer->saveToFile((aeon::getUserDir())+"/.aeonsplice/settings.ini"))
    {
        cout << "WARNING: Failed to save configuration!" << endl;
    }
    else
    {
        cout << "INFO: Successfully saved configuration." << endl;
    }
}

bool isRunning()
{
    if(aeon::windowShouldClose())
    {
        return false;
    }
    else
    {
        return true;
    }
}
