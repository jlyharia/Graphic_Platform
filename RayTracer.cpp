#define GLM_SWIZZLE
#include "RayTracer.h"
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <thread>
#include "sharedata.h"
using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;
RayTracer::RayTracer()
{
    this->winw = glutGet(GLUT_WINDOW_WIDTH);//current window's width
    this->winh = glutGet(GLUT_WINDOW_HEIGHT);//current window's height
    vector<vector <vec3>> tem_col(winw, vector<vec3> (winh));
    col = move(tem_col);
    //add object to object stack
    iniObjStack();
    if(objstack.empty())
        return;
    //main loop
    this->NumOfThread = 4;
    //maximum 4 threads
    cout<<"Ray tracing using "<<NumOfThread<<" threads"<<endl;
}
void RayTracer::render()
{//run time rendering
    RayShootManager(this->NumOfThread);
    //input number of thread to process ray tracing
    //separate work and execute ray tracing
    float winw = glutGet(GLUT_WINDOW_WIDTH);//current window's width
    float winh = glutGet(GLUT_WINDOW_HEIGHT);//current window's height
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glBegin(GL_POINTS);
        for(int i = 0; i< winw; ++i)
        {   for(int j = 0; j< winh; ++j)
            {
                glColor3f( col[i][j].r,
                           col[i][j].g,
                           col[i][j].b);
                glVertex2i(i, j);
            }
        }
    glEnd();
}
void RayTracer::RayShootManager(const int &NumOfThread)
{//manage number of thread working on ray tracing
    vector<thread> threads;
    if(NumOfThread < 2)
    {// 0 or 1 -> main thread
        RayLoop(0,1);
    }
    else//multi-thread
    {
        for(int i =0; i< NumOfThread; ++i)
        {//create thread to separate program
            threads.push_back(thread(&RayTracer::RayLoop, this, i, NumOfThread));
        }
        for(auto &th:threads)
        {//join the threads
            if(th.joinable())
                th.join();
        }
    }
}
void RayTracer::RayLoop(const int& idx, const int& NumOfThread)
{//Main loop of ray tracing
    vec2 step = vec2(2.0)/vec2(winw, winh); //increment step

    /*calculate block boundary for each thread*/
    vec2 thstep = vec2(2.0, winw)/vec2(NumOfThread);
    vec2 ubound = vec2(thstep.x*idx, thstep.x*(idx+1)) - vec2(1.0);
    vec2 wbound = vec2(glm::floor(thstep.y*idx), glm::floor(thstep.y*(idx+1)));

    int i = wbound.x;    //index of canvas array
    for(float u = ubound.x; u< ubound.y && i< wbound.y; u+=step.x, ++i)
    {
        int j{0};
        for(float v = -1.0; v< 1.0 && j<winh; v+=step.y, ++j)
        {
            vec3 rd = glm::normalize(vec3(u* 1.33,v, -1.0)); //ray direction
            col[i][j] = ray(ro_eye, //ray origin
                            rd,     //ray direction
                            -1,     //object index
                            2);     //reflection times
        }
    }
}

void RayTracer::iniObjStack()
{//initialize object
    //sphere ID = 1
    //(x-a)^2 + (y-b)^2 + (z-c)^2 = r^2
    //vec4(a, b, c, r)
    addObjectStack(vec4(3.0, 1.0, 1.0, 0.7), 1);//reflect object
    addObjectStack(vec4(0.0, 1.0, 0.0, 1.0), 1);//opaque object
    addObjectStack(vec4(0.5, 0.5, 2.5, 0.5), 1);//transparent object
    //plane ID = 2
    //ax + by + cz + d =0
    //vec4(a, b, c, d)
    addObjectStack(vec4(0.0, 1.0, 0.0, 0.0), 2);//opaque object
    addObjectStack(vec4(1.0, 0.0, 0.3, 1.0), 2);//reflect object
}
void RayTracer::addObjectStack(vec4 obj, short type)
{//add object to object stack
    objinfo newobj{obj, type};
    objstack.push_back(move(newobj));
}

vec3 RayTracer::getDiffuse(const vec3 &nor, const vec3 &lightDir, const float kd, const vec3 Is)
{//return diffuse intensity
    //Is -> Intensity of source light
    /*Direct Illumination - diffuse and specular light*/
    float norDotlit = glm::dot(nor, lightDir);
    //diffuse light
    vec3 I_dif = glm::clamp(Is * kd * norDotlit, 0.0, 1.0);
    return I_dif;
}
vec3 RayTracer::getSpecular(const vec3& ro, const vec3 &nor, const vec3 &lightDir, const float ks, const vec3 Is)
{//return specular intensity
    //make light source infinitely distant
    vec3 H = (lightDir + ro) / glm::length(lightDir + ro);
    float spec_k = pow(glm::dot(nor, H),8);
    //vec3 reflight = 2 * nor * norDotlit - lightDir;
    return Is * ks * spec_k;
}
vec3 RayTracer::ShadowRay(const vec3& ro, const vec3& rd, const int& idx)
{//determine if point in shadow and return shadow intensity
    vec3 col_sh = vec3(1.0);
    if(objstack.size() != 1)
    {
        vec2 hit_info = intersect(ro, rd, idx);
        if(hit_info.x == FLT_MAX)//shadow ray no hit
            col_sh = vec3(1.0);
        else//shadow ray hit object
            col_sh = vec3(0.0);//object in shadow
    }
    return col_sh;
}

vec3 RayTracer::ReflectRay(const vec3& ro, const vec3& rd, const int idx, short ref_time)
{//return intensity of reflection ray
    //Stop tracing reflection ray if meet the threshold
    if(ref_time == 0) return vec3(0.0);

    vec2 hit_info = intersect(ro, rd, idx);
    //<t, index>
    //t ->   pos = rp + t* rd   //index -> index for object stack
    if(hit_info.x == FLT_MAX) // reflection ray hit nothing
    {
        return vec3(0.0);
    }
    vec3 pos = ro + hit_info.x * rd;//get ray hitting position
    vec3 nor = get_Obj_Normal(pos, hit_info.y);//get position's normal

    //shadow color
    vec3 COL_shadow =  ShadowRay(pos, lightDir_1, hit_info.y);

    //reflection color
    vec3 COL_ref = ReflectRay(pos, glm::normalize(glm::reflect(rd, nor)),
                              hit_info.y, --ref_time);

    /*Direct Illumination - diffuse and specular light*/
     //diffuse color
    vec3 COL_dif = getDiffuse(nor, lightDir_1,
                              1.0f , Int_sun);
    //specular color
    vec3 COL_spec = getSpecular(ro, nor, lightDir_1,
                                0.3f, Int_sun);
    //light attenuation
    float K_att = 1/pow(glm::length(hit_info.x), 2.0);
    vec3 I_ref = COL_shadow * (COL_dif + COL_spec) + K_att * COL_ref;
    return I_ref;
}

vec3 RayTracer::ray(const vec3& ro, const vec3& rd, const int idx, short ref_time)
{//recursive ray main body //index -> object stack
    vec3 finalcol = col_back;
    vec2 hit_info = intersect(ro, rd, idx);
    //<t, index>
    if(hit_info.x != FLT_MAX)//ray hit object
    {
        vec3 pos = ro + hit_info.x * rd;//get ray hitting position
        vec3 nor = get_Obj_Normal(pos, hit_info.y);//get position's normal

        /*Indirect Illumination - Shadow & reflection ray*/
        //shadow
        vec3 COL_shadow =  ShadowRay(pos, lightDir_1, hit_info.y);

        //reflection color
        vec3 COL_ref = ReflectRay(pos, glm::normalize(glm::reflect(rd, nor)),
                                    hit_info.y, --ref_time);
        /*Direct Illumination - diffuse & specular light*/
        //diffuse color
        vec3 COL_dif = getDiffuse(nor, lightDir_1, 1.0f, Int_sun);
        //specular color
        vec3 COL_spec = getSpecular(ro, nor, lightDir_1,0.3f, Int_sun);
        //light attenuation
        float K_att = 0.5/pow(glm::length(hit_info.x), 2.0);

//        return  col_amb +
//                COL_shadow * (COL_dif + COL_spec);
//                K_att * COL_ref;
        return  col_amb+
                COL_shadow * (COL_dif + COL_spec)+
                K_att * COL_ref;
    }
    //program should never reach here
    return finalcol;
}

vec2 RayTracer::intersect(const vec3 &ro, const vec3 &rd, const int index)
{//calculate the parameter t,(i.e., X = ro + t * rod)
    vector<vec2> dist; //<t, index>
    for(int i=0; i<(int)this->objstack.size(); ++i)
    {//go through all object
        if(i == index)
            continue; //skip object itself when shadow ray is blocked by other object

        switch(objstack[i].type)
        {
            case 1: //sphere
                dist.push_back(iSphere(ro, rd, i));
                break;
            case 2: //plane
                dist.push_back(iPlane(ro, rd, i));
                break;
            default:
                cout<<"out range of object stack\n";
        }
        if((int)dist.size() == 2)
        {//remove item with bigger t
            if(dist[0].x < dist[1].x)
                dist.pop_back();
            else
                dist.erase(dist.begin());
        }
    }
    //return first item
    return dist.front();
}

vec3 RayTracer::get_Obj_Normal(const vec3 &pos, const int &idx)
{//compute object normal
    vec3 nor;
    switch(objstack[idx].type)
    {
        case 1: //sphere
            nor = nSphere(pos, idx);
            break;
        case 2: //plane
            nor = nPlane(idx);
            break;
        default:
            cout<<"out range of object Normal"<<endl;
    }
    return nor;
}

vec2 RayTracer::iSphere(const vec3 &ro, const vec3 &rd, const int &idx)
{//sphere obj id = 1
    const vec4 &sph = this->objstack[idx].vec;
    vec3 co = sph.xyz-ro;
    if(glm::dot(co, rd) < 0) //determine by geometric solution
        return vec2(FLT_MAX, idx); //no intersection
    vec3 oc = ro-sph.xyz;
    float b = 2.0 * glm::dot(oc, rd);
    float c = glm::dot(oc,oc) - sph.w*sph.w;
    float h = b*b - 4.0 *c;
    if(h <0.0) return vec2(FLT_MAX, idx); //no intersection

    //pick smaller one(i.e, close one)
    //not (-b+sqrt(h)) /2
    float sqrth = sqrt(h);
    float t1 = 0.5*(-b - sqrth);
    return vec2(t1, idx);
}
vec3 RayTracer::nPlane(const int &idx)
{//return plane's normal
    vec3 nor = glm::normalize(vec3(objstack[idx].vec.xyz));
    return nor;
}
vec3 RayTracer::nSphere(const vec3 &pos, const int &idx)
{//return sphere's normal
    //sphere center at (l, m, n) radius r
    //normal at intersect point N= ( (x-l)/r, (y-m)/r, (z-n)/r )
    const vec4 &sph = objstack[idx].vec;
    return (pos - sph.xyz)/sph.w;
}

vec2 RayTracer::iPlane(const vec3& ro, const vec3& rd, const int idx)
{
    const vec4 &pln = this->objstack[idx].vec;
    //t = -(ax + by + cz + d)/ (ai + bj + ck)
    vec3 nor = vec3(pln.xyz);
    float denom = glm::dot(rd, nor);
    if(denom > 0) return vec2(FLT_MAX, idx);

    float t = -1.0 *(glm::dot(ro, nor) + pln.w)/denom;

    return vec2(t, idx);
}

void RayTracer::vout(const vec3 v)
{
    cout<<v.x<<'\t'<<v.y<<'\t'<<v.z<<endl;
}
