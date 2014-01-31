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
#include "aeonstack.hpp"
#include "aeonboxstate.hpp"

using namespace glm;

namespace aeon
{
    boxstate::boxstate()
    {
        // do nothing
    }

    void boxstate::init(aeonstack * currentStack)
    {
        if(!ready)
        {
            // Dark blue background
            glClearColor(0.0f, 0.3f, 0.7f, 0.0f);

            // Enable depth test
            glEnable(GL_DEPTH_TEST);
            // Accept fragment if it closer to the camera than the former one
            glDepthFunc(GL_LEQUAL);

            // Cull triangles which normal is not towards the camera
            //glEnable(GL_CULL_FACE);

            glGenVertexArrays(1, &VertexArrayID);
            glBindVertexArray(VertexArrayID);

            // Create and compile our GLSL program from the shaders
            programID = aeon::loadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );

            if(programID == 0)
            {
                //aeon::log("ERROR: Failed to load program for aeonboxstate");
                return;
            }

            MatrixID = glGetUniformLocation(programID, "MVP");

            std::string uvtemp = (getUserDir())+"\\.aeonsplice\\uvtemplate.DDS";

            Texture = loadDDS(uvtemp.c_str());

            TextureID  = glGetUniformLocation(programID, "myTextureSampler");

            // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
            // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
            static const GLfloat g_vertex_buffer_data[] = {
                -1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                 1.0f, 1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f,
                 1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                 1.0f,-1.0f,-1.0f,
                 1.0f, 1.0f,-1.0f,
                 1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                 1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                 1.0f,-1.0f, 1.0f,
                 1.0f, 1.0f, 1.0f,
                 1.0f,-1.0f,-1.0f,
                 1.0f, 1.0f,-1.0f,
                 1.0f,-1.0f,-1.0f,
                 1.0f, 1.0f, 1.0f,
                 1.0f,-1.0f, 1.0f,
                 1.0f, 1.0f, 1.0f,
                 1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f,
                 1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                 1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                 1.0f,-1.0f, 1.0f
            };

            // Two UV coordinatesfor each vertex. They were created withe Blender.
            static const GLfloat g_uv_buffer_data[] = {
                0.000059f, 1.0f-0.000004f,
                0.000103f, 1.0f-0.336048f,
                0.335973f, 1.0f-0.335903f,
                1.000023f, 1.0f-0.000013f,
                0.667979f, 1.0f-0.335851f,
                0.999958f, 1.0f-0.336064f,
                0.667979f, 1.0f-0.335851f,
                0.336024f, 1.0f-0.671877f,
                0.667969f, 1.0f-0.671889f,
                1.000023f, 1.0f-0.000013f,
                0.668104f, 1.0f-0.000013f,
                0.667979f, 1.0f-0.335851f,
                0.000059f, 1.0f-0.000004f,
                0.335973f, 1.0f-0.335903f,
                0.336098f, 1.0f-0.000071f,
                0.667979f, 1.0f-0.335851f,
                0.335973f, 1.0f-0.335903f,
                0.336024f, 1.0f-0.671877f,
                1.000004f, 1.0f-0.671847f,
                0.999958f, 1.0f-0.336064f,
                0.667979f, 1.0f-0.335851f,
                0.668104f, 1.0f-0.000013f,
                0.335973f, 1.0f-0.335903f,
                0.667979f, 1.0f-0.335851f,
                0.335973f, 1.0f-0.335903f,
                0.668104f, 1.0f-0.000013f,
                0.336098f, 1.0f-0.000071f,
                0.000103f, 1.0f-0.336048f,
                0.000004f, 1.0f-0.671870f,
                0.336024f, 1.0f-0.671877f,
                0.000103f, 1.0f-0.336048f,
                0.336024f, 1.0f-0.671877f,
                0.335973f, 1.0f-0.335903f,
                0.667969f, 1.0f-0.671889f,
                1.000004f, 1.0f-0.671847f,
                0.667979f, 1.0f-0.335851f
            };

            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

            glGenBuffers(1, &uvbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

            initCamera();
            computeCameraMatrices();
        }
        ready = true;
    }
    void boxstate::cleanUp()
    {
        if(ready)
        {
            // Cleanup VBO and shader
            glDeleteBuffers(1, &vertexbuffer);
            glDeleteBuffers(1, &uvbuffer);
            glDeleteProgram(programID);
            glDeleteTextures(1, &TextureID);
            glDeleteVertexArrays(1, &VertexArrayID);
        }
        ready = false;
    }

    void boxstate::pause()
    {
        paused = true;
    }
    void boxstate::resume()
    {
        paused = false;
    }

    void boxstate::render()
    {
		// Use our shader
		glUseProgram(programID);

        ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
    }
    void boxstate::update()
    {
        // do nothing
    }
    void boxstate::processInput()
    {
        if(isKeyDown("CLOSE"))
        {
            glfwSetWindowShouldClose(getMainWindowHandle(),GL_TRUE);
        }
        if(isKeyDown("DERP"))
        {
            thisStack->pop();
        }
        computeCameraMatrices();
    }

    bool boxstate::isPaused()
    {
        return paused;
    }
    bool boxstate::isReady()
    {
        return ready;
    }
}
