#define GLEW_STATIC
#include "AeonIncludes.hpp"
using namespace std;

bool init();
void load();
bool isRunning();
void render();
void cleanUp();

int main(int argc, char *argv[])
{
    // Initilizes underlying API's and retrieves the configuration file
    cout << "Initilizing..." << endl;
    if(!init())
    {
        return -1;
    }
    // Loads the assets required at startup (intro animation, main menu textures, first rendering objects, etc)
    cout << "Loading assets..." << endl;
    load();
    // Run the main game loop
    cout << "Running..." << endl;
	while(isRunning())
	{
        // Should render current context (main menu, etc)
		render();
        break;
	}
    cout << "Cleaning up..." << endl;
    // Terminates underlying APIs and releases all memory
	cleanUp();
    cout << "Exiting." << endl;
    // Everything ran and didn't throw an error, return 0 (successful)
	return 0;
}

void render()
{
    cout << "loljk" << endl;
}

void load()
{
    // Probably going to implement loading functions for each rendering context, and call the first few loading functions in here
}

bool init()
{
	// Initialise GLFW (OpenGL), and any other APIs (OpenAL?)
	if( !aeonAPIInit() )
	{
		fprintf( stderr, "Failed to initialize underlying API(s), exiting.\n" );
		return false;
	}
    // Set openGL version to 4.3
    aeonSetGLVersion(4,3);
    // Set anti-aliasing to 4 samples.
    aeonSetFSAA(4);
    // Disable resizing the window. (Prevent user from breaking stuff)
	aeonSetResizable(false);
    
    // Attempt to open window context
    if( !aeonOpenWindow("Aeon Splice Alpha",800,600,false) )
    {
        fprintf( stderr, "Failed to open OpenGL window, exiting.\n" );
		aeonAPITerminate();
		return false;
    }
    
    // GLEW must be declared after the GLFW window context is available :(
    
	glewExperimental = true; // Needed for core profile
    GLenum GlewInitResult;
    GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult) {
        fprintf(stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
        return false;
    }

    fprintf(
        stdout,
        "INFO: OpenGL Version: %s\n",
        glGetString(GL_VERSION)
    );

    glClearColor(0.2f, 1.0f, 0.8f, 0.3f);
	aeonCenterCursor();
    
    // Init camera with default settings
    //initCamera();
    return true;
}

void cleanUp()
{
    aeonAPITerminate();
}

bool isRunning()
{
    if(aeonWindowShouldClose())
    {
        return false;
    }
    else
    {
        return true;
    }
}