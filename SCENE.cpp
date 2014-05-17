#include <GL/glew.h>
//#include <GL/freeglut.h>
#include "SCENE.h"
#include "shaderloader.h"
#include <iostream>
SCENE::SCENE()
{

}
void SCENE::CompileTimeProcess()
{
    raytracer_1_ = new RayTracer();
}
void SCENE::RunTimeRender()
{
    raytracer_1_->render();

}
SCENE::~SCENE()
{
    delete raytracer_1_;
    std::cout<<"raytracer_1_ deleted\n";
}
