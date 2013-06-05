#ifndef _AEONWINDOW_
#define _AEONWINDOW_

GLFWwindow* aeonWindowHandle;

bool aeonAPIInit();
void aeonAPITerminate();

void aeonSetGLVersion(int,int);
void aeonSetFSAA(int);
void aeonSetResizable(bool);
void aeonSetWindowHint(int,int);

bool aeonOpenWindow(string title);
bool aeonOpenWindow(string title,int width,int height,bool fullscreen);

bool aeonSetWindowTitle(string);    // REtitles the main window
void aeonCenterMouse();             // Centers the underlying mouse only
void aeonCenterMouseCursor();       // Centers both the underlying mouse AND the displayed mouse

#endif