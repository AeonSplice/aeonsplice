#ifndef _AEONWINDOW_
#define _AEONWINDOW_
// Get window Width
int aeonWindowWidth();
// Get window Height
int aeonWindowHeight();

// Set the title of the window
void aeonSetWindowTitle(std::string title)
{
	glfwSetWindowTitle(title.c_str());
}

void aeonSetWindowHint(int target,int hint)
{
	glfwOpenWindowHint(target,hint);
}

// Set Anti-Aliasing Sampling
void aeonSetFSAA(int aavalue);

// Enable or Disable window resizing
void aeonSetResizable(bool canResize);

void aeonEnableFaceCulling();

// Initalize underlying API's
bool aeonAPIInit();

// Terminate underlying API's
void aeonAPITerminate();

// Force OpenGL to use specific version
void aeonSetGLVersion(int major,int minor);

// Open fullscreen window at desktop resolution
bool aeonOpenWindow();

// Open window with given resolution (true = fullscreen | false = windowed)
bool aeonOpenWindow(int width,int height,bool fullscreen);

// Open window with specific parameters. (resolution, bit depths, and fullscreen mode)
bool aeonOpenWindow(int width, int height, int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, bool fullscreen);

// (re)center the mouse
void aeonCenterMouse();
#endif