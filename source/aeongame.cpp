#include "aeongame.hpp"
// Don't include stuff above here. Because standards.

#include "aeoncontext.hpp"
#include "aeonconfig.hpp"
#include "aeonshader.hpp"
#include "aeoninput.hpp"
#include "aeonlog.hpp"
#include "aeonutil.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <string>
#include <thread>

using namespace std;

namespace aeon
{
    // TestInput
    void TestInput::setInputSettings(Config * settings)
    {
        setInput("CLOSE", initKeyPair(settings, "input", "CLOSE", GLFW_KEY_ESCAPE));
    }

    // TestState
    TestState::~TestState()
    {
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &VertexArrayID);
    }

    void TestState::load(Config * settings, GLFWwindow* window, Context * context)
    {
        // TODO: InputHandler
        aSettings = settings;
        aInput = InputHandler();
        aWindowHandle = window;
        contextHandle = context;

        aInput.setWindowHandle(aWindowHandle);
        aInput.setInputSettings(aSettings);

        loadGL();

        // Load GUI

        //OpenGL3Renderer& guiRenderer = OpenGL3Renderer::bootstrapSystem();
        //guiRenderer.enableExtraStateSettings(true);
    }
    void TestState::loadGL()
    {
        // Dark blue background
        glClearColor(0.0f, 0.5f, 0.8f, 0.0f);

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Default shaders
        string vShader = "#version 330 core\n\n// Input vertex data, different for all executions of this shader.\nlayout(location = 0) in vec3 vertexPosition_modelspace;\n\nvoid main(){\n\n    gl_Position.xyz = vertexPosition_modelspace;\n    gl_Position.w = 1.0;\n\n}\n\n";
        string fShader = "#version 330 core\n\n// Ouput data\nout vec3 color;\n\nvoid main()\n{\n\n	// Output color = red \n	color = vec3(1,0,0);\n\n}\n";

        // Create and compile our GLSL program from the shaders
        programID = initProgram( "vert.glsl", "frag.glsl", vShader, fShader );

        if(programID == 0)
        {
            // NOTE: I'm pretty sure this is dangerous atm, but the program shouldn't fail to load anyway.
            throw "Failed to load program for TestState";
        }

        const GLfloat g_vertex_buffer_data[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f,
        };

        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    }
    void TestState::processInput(int key, int scancode, int action, int mods)
    {
        if(key == GLFW_KEY_ESCAPE)
            contextHandle->closeContext();
    }
    void TestState::update()
    {
        glfwPollEvents();
        // Does nothing atm
    }
    void TestState::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glDisable(GL_DEPTH_TEST);
        // Use our shader
        glUseProgram(programID);

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
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);
        //glEnable(GL_DEPTH_TEST);
        glUseProgram(0);

        glfwSwapBuffers(aWindowHandle);
        //System::getSingleton().renderAllGUIContexts();
    }

    // TestContext
    void TestContext::load()
    {
        aLock.lock();
        aState = new TestState;
        aState->load(aSettings,aWindowHandle,this);
        aLock.unlock();
    }
}
