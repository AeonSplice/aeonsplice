#define GLEW_STATIC
#include "AeonIncludes.hpp"
using namespace std;

bool init();
void load();
bool isRunning();
void render();
void cleanUp();

aeon::config* settings;
//aeon::renderer* intro;

int main(int argc, char *argv[])
{
    // Initilizes underlying API's and retrieves the configuration file
    cout << "INFO: Initilizing..." << endl;
    if(!init())
    {
        cout << "FATAL: Failure to initilize." << endl;
        return -1;
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
	return 0;
}

void render()
{
    glClear( GL_COLOR_BUFFER_BIT );
    glfwSwapBuffers();
}

void load()
{
    /*if(!settings->loadFromFile("C:/Users/Josh/.aeonsplice/config.ini"))
    {
        cout << "Failed to load config..." << endl;
    }*/
}

bool init()
{
    settings = new aeon::config;
    if(!(settings->loadFromFile((aeon::getUserDir())+"/.aeonsplice/settings.ini")))
    {
        fprintf( stderr, "FATAL: Failed to load config.\n" );
        return false;
    }
    settings->print();
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
    if( !aeon::openWindow(settings) )
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
    settings->print();
    if(!settings->saveToFile((aeon::getUserDir())+"/.aeonsplice/settings.ini"))
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
