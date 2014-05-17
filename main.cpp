/**
    Software Title: Graphics Platform v0.07
    Start: 2013.12.23
    Exclusive Developer: YiHung Lee
    Tasks:
        Add refraction on sphere

*/
#include <GL/freeglut.h>
#include "ogl_trivialfunc.h"
#include "SCENE.h"
#include <iostream>
#include <memory>
#include "sharedata.h"
#include <ctime>
#include <chrono>
#include <string>
using namespace std;
using namespace std::chrono;
void drawScene();
void openglInit(int , char**);
void preProcessing();
//-----------------------------
GLsizei winWidth = 800,winHeight = 600;
unique_ptr<SCENE> scene(new SCENE);
string WIN_TITLE_main{"Graphics Platform v0.07"};
string WIN_TITLE_new{WIN_TITLE_main};
void preProcessing()
{
    scene->CompileTimeProcess();
}
void doFPS()
{//frame per second & millisecond per frame
    static int32_t flag = -1;
    static steady_clock::time_point t_prev;
    steady_clock::time_point t_now = steady_clock::now();
    duration<double> time_span_sum = steady_clock::duration::zero();
    int dtime = 10;
    if(flag != -1)
    {
        duration<double> time_span = duration_cast<duration<double>>(t_now - t_prev);

        time_span_sum += time_span;
        if(flag%dtime == 0)
        {
            time_span_sum /= dtime;
            string FPS = std::to_string((int)(1/time_span_sum.count())) + " fps";
            string MPF = std::to_string(time_span_sum.count() * 1000);
            MPF.resize(4); MPF += " ms";
            //millisecond
            WIN_TITLE_new = WIN_TITLE_main +
                            "   " + MPF +
                            "   " + FPS;
            glutSetWindowTitle(WIN_TITLE_new.c_str());

            time_span_sum = steady_clock::duration::zero();
        }

    }
    t_prev = t_now;
    flag++;
    if(flag == 100000)
        flag = 0;
}
void drawScene()
{//main drawing function
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    scene->RunTimeRender();

    doFPS();

    glutPostRedisplay();
    glutSwapBuffers();
}

void openglInit(int argc, char** argv)
{// initialize OpenGL function
    //Initialize GLUT
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(::winWidth , ::winHeight); //Set the window size

	//Create the window
	glutCreateWindow(WIN_TITLE_new.c_str());
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, key press, and window resizes
	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);

    //keyboard
    glutKeyboardFunc(handleKeypress);

}
int main(int argc, char** argv)
{
    ///OpenGL initialization
    openglInit(argc, argv);
    ///glew function initialization
    //glslInit();
    preProcessing();
    glutMainLoop();
    return 0;
}
