// Includes stuff

// Internal Variables (Not exposed in header)
glm::mat4 ViewMatrix;           // View Matrix
glm::mat4 ProjectionMatrix;     // Projection Matrix

glm::vec3 position;             // Current position : on +Z
glm::vec3 initialPos;           // Default position : on +Z

float cameraYaw;                // Current horizontal angle
float initialCameraYaw;         // Default horizontal angle

float cameraPitch;              // Current vertical angle
float initialCameraPitch;       // Default vertical angle

float FoV;                      // Current Field of View
float initialFoV;               // Default Field of View

float minimumFoV;               // Min Field of View (prevents negative FoV and uberwtf narrowscreen)
float maximumFoV;               // Max Field of View (prevents uberwtf widescreen)
float speed;                    // Rate at which the camera moves within the world space
float mouseSpeed;               // Rate at which the camera rotates
bool adjustingView;             // If mouse is being held and camera should be adjustable
int heldX,heldY;                // Location of mouse before adjusting view

// Implementations
glm::mat4 getViewMatrix()
{
    return ViewMatrix;
}

glm::mat4 getProjectionMatrix()
{
    return ProjectionMatrix;
}

float getFoV()
{
    return FoV;
}

void initCamera()
{
    position = glm::vec3( 0, 0, 5 );
    initialPos = glm::vec3( 0, 0, 5 );
    cameraYaw = 3.14f;
    initialCameraYaw = 3.14f;
    cameraPitch = 0.0f;
    initialCameraPitch = 0.0f;
    FoV = 45.0f;
    initialFoV = 45.0f;
    minimumFoV = 20.0f;
    maximumFoV = 80.0f;
    speed = 3.0f;
    mouseSpeed = 0.005f;
    adjustingView=false;
}

void resetCamera()
{
    FoV=initialFoV;
    position=initialPos;
    cameraYaw=initialCameraYaw;
    cameraPitch=initialCameraPitch;
    computeCameraMatrices();
}

void computeCameraMatrices()
{
    // glfwGetTime is called only once, the first time this function is called (this is because of the static declaration, which makes the variable persist)
    static double lastTime = getCurrentTime();

    // Compute time difference between current and last frame
    double currentTime = getCurrentTime();
    float deltaTime = float(currentTime - lastTime);

    // Adjust camera angle based on mouse
    if(isMouseDown('RMB') && adjustingView)
    {
        // Get mouse position
        int xpos, ypos;
        getMousePos(&xpos, &ypos);

        // Reset mouse position for next frame
        centerMouse();

        // Compute new orientation
        cameraYaw += mouseSpeed * float(windowWidth()/2 - xpos );
        cameraPitch   += mouseSpeed * float(windowHeight()/2 - ypos );
    }
    else if(isMouseDown('RMB') && !adjustingView)
    {
        getMousePos(&heldX, &heldY);
        centerMouse();
        adjustingView=true;
        windowDisable(MOUSE_CURSOR);
    }
    else if(!isMouseDown('RMB') && adjustingView)
    {
        windowEnable(MOUSE_CURSOR);
        setMousePos(heldX, heldY);
        adjustingView=false;
    }
    
    // Bounds Check Pitch/Roll/Yaw
    if(cameraPitch>180.0f)
    {
        cameraPitch = 180.0f;
    }
    else if(cameraPitch<0.0f)
    {
        cameraPitch = 0.0f;
    }


    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction
    (
        cos(cameraPitch) * sin(cameraYaw), 
        sin(cameraPitch),
        cos(cameraPitch) * cos(cameraYaw)
    );
    
    // Right vector
    glm::vec3 right = glm::vec3
    (
        sin(cameraYaw - 3.14f/2.0f), 
        0,
        cos(cameraYaw - 3.14f/2.0f)
    );
    
    // Up vector
    glm::vec3 up = glm::cross( right, direction );

    // Move forward
    if (isKeyDown('CAMERA_FORWARD'))
    {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (isKeyDown('CAMERA_BACK'))
    {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (isKeyDown('CAMERA_RIGHT'))
    {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (isKeyDown('CAMERA_LEFT'))
    {
        position -= right * deltaTime * speed;
    }
    // Move Up
    if (isKeyDown('CAMERA_UP'))
    {
        position += up * deltaTime * speed;
    }
    // Move Down
    if (isKeyDown('CAMERA_DOWN'))
    {
        position -= up * deltaTime * speed;
    }

    FoV -= 5 * getMouseWheel();     // Adjust Field of view based on scrolling
    setMouseWheel();                // Set Mouse Wheel position to default
    
    if(FoV>maximumFoV)
    {
        FoV=maximumFoV;
    }
    if(FoV<minimumFoV)
    {
        FoV=minimumFoV;
    }
    
    // Projection matrix : X° Field of View, display ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(FoV, windowWidth()/windowHeight(), 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix       = glm::lookAt(
                                position,           // Camera is here
                                position+direction, // and looks here : at the same position, plus "direction"
                                up                  // Head is up (set to 0,-1,0 to look upside-down)
                           );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}