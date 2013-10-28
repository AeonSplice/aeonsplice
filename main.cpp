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
//aeon::statestack stack;

GLuint vertexbuffer;

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
    // DRAWING CODE BELOW

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);

    //DRAWING CODE ABOVE
    glfwPollEvents();
    glfwSwapBuffers(aeon::getMainWindowHandle());
}

void load()
{
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    static const GLfloat g_vertex_buffer_data[] =
    {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    aeon::centerCursor();
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
