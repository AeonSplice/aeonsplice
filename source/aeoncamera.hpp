#ifndef _AEONCAMERA_
#define _AEONCAMERA_

namespace aeon
{
    class Camera
    {
    public:
        Camera();
        Camera(GLFWwindow* window);
        ~Camera();

        void updateProjection(GLFWwindow* window);

        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewMatrix();

        void setCamera(double x, double y, double z, double azimuth, double polar);

        void moveCameraLeft(double distance);
        void moveCameraRight(double distance);
        void moveCameraForward(double distance);
        void moveCameraBackward(double distance);
        void moveCameraUp(double distance);
        void moveCameraDown(double distance);
        void rotateCamera(double azimuth, double polar);
    protected:
        double camX,camY,camZ;
        double camAzimuth,camPolar;
    }
}

#endif // _AEONCAMERA_
