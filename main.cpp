#include "AeonIncludes.hpp"
using namespace std;

void init();
void load();
bool isRunning();
void render();
void cleanUp();

int main(int argc, char *argv[])
{
	//init();
    //load();
    cout << "Test!" << endl;
	while(isRunning())
	{
		render();
	}
	//cleanUp();
	return 0; // Everything ran and didn't throw an error, return 0 (successful)
}

void render()
{
    //computeCameraMatrices();
    //skybox.render();
}

void load()
{
    /*
    AeonSkybox* skybox = new AeonSkybox;
    skybox.load("./skybox/sky_",".tga");
    */
}

/*void init()
{
	// Initialise GLFW (OpenGL), and any other API's (OpenAL?)
	if( !aeonAPIInit() )
	{
		fprintf( stderr, "Failed to initialize underlying API(s), exiting.\n" );
		return -1;
	}
    // Set openGL version to 4.3
    aeonSetGLVersion(4,3);
    // Set anti-aliasing to 4 samples.
    aeonSetFSAA(4);
    // ??? idk
    aeonSetWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Disable resizing the window. (Prevent user from breaking stuff)
	aeonSetResizable(false);
    
    // Attempt to open window context
    if( !aeonOpenWindow(800,600,false) )
    {
        fprintf( stderr, "Failed to open OpenGL window, exiting.\n" );
		aeonAPITerminate();
		return -1;
    }
    
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW, exiting.\n");
		return -1;
	}
    
    aeonSetWindowTitle("Aeon Splice Alpha");
    glfwEnable(GLFW_STICKY_KEYS);
	aeonCenterMouse();

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	aeonEnableFaceCulling();
    
    // Init camera with default settings
    //initCamera();
}*/

void cleanUp()
{
    //aeonAPITerminate();
}

bool isRunning()
{
    return false;
}