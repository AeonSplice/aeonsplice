#ifndef _AEONCAMERA_
#define _AEONCAMERA_

namespace aeon
{
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    float getFoV();

    void initCamera();
    void resetCamera();

    void computeCameraMatrices();
}

#endif // _AEONCAMERA_
