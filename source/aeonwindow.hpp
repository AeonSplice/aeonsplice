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
    void setDecorated(bool isDecorated);

    bool openWindow(std::string title);
    bool openWindow(std::string title,int width,int height,bool fullscreen);
    bool openWindow(config * settings);
    void closeWindow();

    void setWindowTitle(std::string title);     // Retitles the main window
    void centerMouse();                         // Centers the underlying mouse only
    void hideCursor(bool hide);                 // Hides or shows the cursor

    void getMousePos(int * x, int * y);
    void getMousePos(double * x, double * y);
    void setMousePos(int x,int y);
    void setMousePos(double x,double y);

    float getWindowWidth();
    float getWindowHeight();
    float getWindowRatio();

    bool windowShouldClose();

    GLFWwindow* getMainWindowHandle();
}
#endif
