#include "aeonshader.hpp"
// Don't include stuff above here. Because standards.

#include "aeonutil.hpp"
#include "aeonlog.hpp"

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

namespace aeon
{
    GLuint initProgram(string vertexFile, string fragmentFile)
    {
        // Create shaders
        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        string tempV = getAeonDir()+vertexFile;
        string tempF = getAeonDir()+fragmentFile;

        string vertexCode;
        string fragmentCode;

        // Read vertex shader from file
        ifstream vertexShaderStream(tempV.c_str(), std::ios::in);
        if(vertexShaderStream.is_open())
        {
            string vLine = "";
            while(getline(vertexShaderStream, vLine))
                vertexCode += "\n" + vLine;
            vertexShaderStream.close();
        }
        else
        {
            log("Failed to open vertex shader at: \""+tempV+"\"", AEON_ERROR);
            return 0;
        }

        // Read fragment shader from file
        ifstream fragmentShaderStream(tempF.c_str(), std::ios::in);
        if(fragmentShaderStream.is_open())
        {
            string fLine = "";
            while(getline(fragmentShaderStream, fLine))
                fragmentCode += "\n" + fLine;
            fragmentShaderStream.close();
        }
        else
        {
            log("Failed to open fragment shader at: \""+tempF+"\"", AEON_ERROR);
            return 0;
        }

        GLint result = GL_FALSE;
        int infoLogLength;

        // Compile Vertex Shader
        log("Compiling vertex shader: \""+vertexFile+"\"", AEON_INFO);
        char const * vertexSourcePointer = vertexCode.c_str();
        glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
        glCompileShader(vertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if ( infoLogLength > 0 )
        {
            std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
            glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
            if(vertexShaderErrorMessage.size() > 2)
                log(vertexShaderErrorMessage, AEON_ERROR);
        }

        // Compile Fragment Shader
        log("Compiling fragment shader: \""+fragmentFile+"\"", AEON_INFO);
        char const * fragmentSourcePointer = fragmentCode.c_str();
        glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
        glCompileShader(fragmentShaderID);

        // Check Vertex Shader
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if ( infoLogLength > 0 )
        {
            std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
            glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
            if(fragmentShaderErrorMessage.size() > 2)
                log(fragmentShaderErrorMessage, AEON_ERROR);
        }

        // Link program
        log("Linking program.", AEON_INFO);
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        // Check the program
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if ( infoLogLength > 0 )
        {
            std::vector<char> programErrorMessage(infoLogLength+1);
            glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
            if(programErrorMessage.size() > 2)
                log(programErrorMessage, AEON_ERROR);
        }

        // Clean up
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        return programID;
    }
}
