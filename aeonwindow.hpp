#ifndef _AEONWINDOW_
#define _AEONWINDOW_
#include "aeonconfig.hpp"
namespace aeon
{
    bool APIInit();
    void APITerminate();

    void setGLVersion(int major,int minor);
    void setFSAA(int sampling);
    void setResizable(bool canResize);

    bool openWindow(std::string title);
    bool openWindow(std::string title,int width,int height,bool fullscreen);
    bool openWindow(config* settings);
    void closeWindow();

    void setWindowTitle(std::string title);    // Retitles the main window
    void centerMouse();             // Centers the underlying mouse only
    void centerCursor();            // Centers both the underlying mouse AND the displayed mouse

    bool windowShouldClose();
}
#endif
