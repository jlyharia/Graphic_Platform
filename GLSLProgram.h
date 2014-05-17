/**
    GLSLProgram.h & GLSLProgram.cpp take advantages from the book "OpenGL 4.0 Shading Language Cookbook"
*/
#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H
#include <GL/glut.h>
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

class GLSLProgram
{
    public:
        GLSLProgram();
        virtual ~GLSLProgram();
        int getUniformLocation(const char * name );
        void sendUniform( const char *name, float x, float y, float z);
        void sendUniform( const char *name, const vec2 & v);
        void sendUniform( const char *name, const vec3 & v);
        void sendUniform( const char *name, const vec4 & v);
        void sendUniform( const char *name, const mat4 & m);
        void sendUniform( const char *name, const mat3 & m);
        void sendUniform( const char *name, float val );
        void sendUniform( const char *name, int val );
        void sendUniform( const char *name, bool val );

        void setLocShader(GLuint);
    private:
        GLuint loc_shader;
    protected:
};

#endif // GLSLPROGRAM_H
