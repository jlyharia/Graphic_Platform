#ifndef SCENE_H
#define SCENE_H
//#include <glm/glm.hpp>
//#include <GL/freeglut.h>
//#include "GLSLProgram.h"
#include "RayTracer.h"
class SCENE
{
    public:
        SCENE();
        virtual ~SCENE();
        void CompileTimeProcess();
        void RunTimeRender();

    private:
        RayTracer *raytracer_1_;

};

#endif // SCENE_H
