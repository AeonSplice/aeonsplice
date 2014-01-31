#define GLEW_STATIC
#include <GL/glew.h>

#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "aeonwindow.hpp"
#include "aeontexture.hpp"
#include "aeonshader.hpp"
#include "aeonplatform.hpp"
#include "aeoncamera.hpp"
#include "aeoninput.hpp"
#include "aeonlog.hpp"
#include "aeontrianglestate.hpp"

using namespace glm;

namespace aeon
{
    trianglestate::trianglestate()
    {
        // do nothing
    }

    void trianglestate::init(aeonstack * currentStack)
    {
        if(!ready)
        {
            // Dark blue background
            glClearColor(0.0f, 0.5f, 0.0f, 0.0f);

            glGenVertexArrays(1, &VertexArrayID);
            glBindVertexArray(VertexArrayID);

            // Create and compile our GLSL program from the shaders
            programID = aeon::loadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

            if(programID == 0)
            {
                //aeon::log("ERROR: Failed to load program for aeontrianglestate");
                return;
            }

            static const GLfloat g_vertex_buffer_data[] = {
                -1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 0.0f,  1.0f, 0.0f,
            };

            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        }
        ready = true;
    }
    void trianglestate::cleanUp()
    {
        if(ready)
        {
            // Cleanup VBO and shader
            glDeleteBuffers(1, &vertexbuffer);
            glDeleteProgram(programID);
            glDeleteVertexArrays(1, &VertexArrayID);
        }
        ready = false;
    }

    void trianglestate::pause()
    {
        paused = true;
    }
    void trianglestate::resume()
    {
        paused = false;
    }

    void trianglestate::render()
    {
        glDisable(GL_DEPTH_TEST);
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
		glEnable(GL_DEPTH_TEST);
    }
    void trianglestate::update()
    {
        // do nothing
    }
    void trianglestate::processInput()
    {
        if(isKeyDown("ESCAPE"))
        {
            glfwSetWindowShouldClose(getMainWindowHandle(),GL_TRUE);
        }
    }

    bool trianglestate::isPaused()
    {
        return paused;
    }
    bool trianglestate::isReady()
    {
        return ready;
    }
}
