// textfile.cpp
//
// simple reading and writing for text files
//
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;
char *textFileRead(char *fn)
{


    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL)
    {
        fp = fopen(fn,"rt");

        if (fp != NULL)
        {

            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0)
            {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

int textFileWrite(char *fn, char *s)
{

    FILE *fp;
    int status = 0;

    if (fn != NULL)
    {
        fp = fopen(fn,"w");

        if (fp != NULL)
        {

            if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
                status = 1;
            fclose(fp);
        }
    }
    return(status);
}

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;
    //glGetError ¡X return error information
    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}


void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    //The status of the compile steps can be queried in OpenGL 2.0
    //with the following function:
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    //glGetShaderiv  ßX Returns a parameter from a shader object
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        //glGetShaderInfoLog  ßX Returns the information log for a shader object
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    //The status of the link step can be queried in OpenGL 2.0
    //with the following function:
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    //glGetProgramiv  ßX Returns a parameter from a program object
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        //glGetProgramInfoLog  ßX Returns the information log for a program object
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

GLuint CreateShaders(std::string file_name)
{

	char *vs = NULL,*fs = NULL;

	GLuint v = glCreateShader(GL_VERTEX_SHADER);  //shader step 1. create shader  - vertex shader
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);//shader step 1. create shader  - fragment shader

    std::string vert_shader = file_name + ".vert";
	std::string frag_shader = file_name + ".frag";

	vs = textFileRead((char *)vert_shader.c_str());
	fs = textFileRead((char *)frag_shader.c_str());
	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);//shader step 2. shader source - vertex shader
	glShaderSource(f, 1, &ff,NULL);//shader step 2. shader source - fragment shader

	free(vs);free(fs);

	glCompileShader(v);//shader step 3. compile shader - vertex shader
	glCompileShader(f);//shader step 3. compile shader - fragment shader

    //---------Troubleshooting: The InfoLog - shader------------
	printShaderInfoLog(v);
	printShaderInfoLog(f);
    //----------------------------------------
	GLuint shader_program = glCreateProgram();//program step 1. create program
	glAttachShader(shader_program,v);//program step 2. attach shader - vertex shader
	glAttachShader(shader_program,f);//program step 2. attach shader - fragment shader
	glLinkProgram(shader_program);//program step 3. link program
	//---------Troubleshooting: The InfoLog - program------------
	printProgramInfoLog(shader_program);
    //------------------------------------------
//	glUseProgram(p);//program step 4. use program
    return shader_program;
}
