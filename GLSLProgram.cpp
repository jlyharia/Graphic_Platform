#include <GL/glew.h>
#include <GL/glut.h>
#include "GLSLProgram.h"
#include <iostream>
#include <cstdio>
using namespace std;
GLSLProgram::GLSLProgram()
{
    //ctor
}
void GLSLProgram::sendUniform( const char *name, float val )
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1f(loc, val);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}
void GLSLProgram::sendUniform( const char *name, float x, float y, float z)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform3f(loc,x,y,z);
    }
    else
    {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::sendUniform( const char *name, const vec3 & v)
{
    this->sendUniform(name,v.x,v.y,v.z);
}
void GLSLProgram::sendUniform( const char *name, const vec4 & v)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ) {
        glUniform4f(loc,v.x,v.y,v.z,v.w);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}
void GLSLProgram::sendUniform( const char *name, const mat3 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}
void GLSLProgram::sendUniform( const char *name, const mat4 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}
int GLSLProgram::getUniformLocation(const char *name )
{
    return glGetUniformLocation(this->loc_shader, name);
}
void GLSLProgram::setLocShader(GLuint loc)
{
    this->loc_shader = loc;
}
GLSLProgram::~GLSLProgram()
{
    //dtor
}
