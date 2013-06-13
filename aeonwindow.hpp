#ifndef _AEONWINDOW_
#define _AEONWINDOW_
bool aeonAPIInit();
void aeonAPITerminate();

void aeonSetGLVersion(int major,int minor);
void aeonSetFSAA(int sampling);
void aeonSetResizable(bool canResize);

bool aeonOpenWindow(std::string title);
bool aeonOpenWindow(std::string title,int width,int height,bool fullscreen);

void aeonSetWindowTitle(std::string title);    // REtitles the main window
void aeonCenterMouse();             // Centers the underlying mouse only
void aeonCenterCursor();            // Centers both the underlying mouse AND the displayed mouse

bool aeonWindowShouldClose();
#endif
