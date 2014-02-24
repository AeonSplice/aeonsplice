#ifndef _AEONSHADER_
#define _AEONSHADER_

#include <GL/glew.h>

#include <string>

namespace aeon
{
    GLuint initProgram(std::string vertexFile, std::string fragmentFile);

    GLuint initProgram(std::string vertexFile,          // vertex file (just filename/type)
                       std::string fragmentFile,        // i.e. "test.frag" would resolve to "%aeondir%/test.frag"
                       std::string defaultVertex,
                       std::string defaultFragment);
}

#endif // _AEONSHADER_
