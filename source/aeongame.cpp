#include "aeongame.hpp"
// Don't include stuff above here. Because standards.

#include "aeoncontext.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aeon
{
    TestContext::~TestContext()
    {
        glDeleteBuffers(1, &vertexbuffer);
        //glDeleteProgram(programID);
        glDeleteVertexArrays(1, &VertexArrayID);
        glfwDestroyWindow(aWindowHandle);
    }
    void TestContext::load()
    {
        aLock.lock();
        // Dark blue background
        glClearColor(0.0f, 0.5f, 0.0f, 0.0f);

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Create and compile our GLSL program from the shaders
        //programID = aeon::loadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

        //if(programID == 0)
        //{
        //    //aeon::log("ERROR: Failed to load program for aeontrianglestate");
        //    return;
        //}

        const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             0.0f,  1.0f, 0.0f,
        };

        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        aLock.unlock();
    }
    void TestContext::execute()
    {
        while(!shouldClose())
        {
            processInput();
            update();
            render();
        }
    }
    bool TestContext::needsUpdate()
    {
        return false;
    }
    void TestContext::processInput()
    {
        aLock.lock();
        glfwPollEvents();
        try
        {
            if(aInput.isKeyDown("CLOSE"))
            {
                this->closeContext();
            }
        }
        catch(...)
        {
            // Do nothing... Means that CLOSE was not defined.
        }
        aLock.unlock();
    }
    void TestContext::update()
    {
        aLock.lock();
        // TODO: Update scene
        aLock.unlock();
    }
    void TestContext::render()
    {
        aLock.lock();
         glDisable(GL_DEPTH_TEST);
        // Use our shader
        //glUseProgram(programID);

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
        glEnable(GL_DEPTH_TEST);
        glfwSwapBuffers(aWindowHandle);
        aLock.unlock();
    }
}
