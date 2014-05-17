#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <glm/glm.hpp>
#include <vector>
using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;

class RayTracer
{
    public:
        RayTracer();

        void render();
        int NumOfThread;
        virtual ~RayTracer(){};

    private:
        /*windows info*/
        float winw;//current window's width
        float winh;//current window's height
        /*object info*/
        vec2 iSphere(const vec3&, const vec3&, const int&);//intersect of sphere
        vec3 nSphere(const vec3 &, const int&);
        vec2 iPlane(const vec3&, const vec3&, const int);
        vec3 nPlane(const int&);
        vec3 get_Obj_Normal(const vec3 &, const int&);
        void addObjectStack(vec4, short);

        struct{
            bool operator()(const vec2 a, const vec2 b){return a.x < b.x;}
        }mysort;
        struct objinfo{//analytic object
            vec4 vec;
            short type;     //specify object type, 1 -> sphere, 2 -> plane

            bool trsp;      //transparent 1 - transparent, 0 - opaque
            float snell_cff; //snell coefficient for transparent object

        };
        vector<objinfo> objstack;
        void iniObjStack();

        /*Ray*/
        void RayLoop(const int&, const int&);
        void RayShootManager(const int&);
        vec2 intersect(const vec3&, const vec3&, const int);

        vec3 ray(const vec3&, const vec3&, const int, short);
        vec3 ReflectRay(const vec3&, const vec3&, const int, short);
        vec3 ShadowRay(const vec3&, const vec3&, const int&);
        vec3 RefractRay(const vec3&, const vec3&, const int, short);
        const vec3 ro_eye = vec3(0.0, 0.5, 4.0); //ray (eye) origin

        /*color*/
        const vec3 col_back = vec3(0.0);//back ground color
        const vec3 col_amb = vec3(0.2);//ambient color
        const vec3 Int_sun = vec3(0.7); //Intensity of light source
        vector<vector <vec3>> col;

        /*Light*/
        const vec3 lightDir_1 = glm::normalize(vec3(0.57703));

        /*Phong Model*/
        vec3 getDiffuse(const vec3&, const vec3&, const float, const vec3 );
        vec3 getSpecular(const vec3&, const vec3&, const vec3&, const float, const vec3);
        /*utility*/
        void vout(const vec3 v);

};

#endif // RAYTRACER_H
