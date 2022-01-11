#pragma once;

#include <vector>
#include <string>
#include <glm/glm.hpp>
using namespace std;
#include "shader.h"
#include "camera.h"
#define PI 3.14159
class Sun
{
private:
    /* data */
    float time;
    
    glm::vec3 moonlightDir;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    glm::vec3 skyColor;


protected:
    void refresh();

public:
    glm::vec3 lightDir;
    Sun(/* args */);
    ~Sun();
    void Apply(Shader shader,bool sky);
    void ChangeTime(float deltaTime);


};

Sun::Sun(){
    time=9.0f;
    refresh();
}
Sun::~Sun(){

}
void Sun::Apply(Shader shader,bool sky){
    shader.use();
    if(sky){
        shader.setVec3("skyColor",skyColor);
    }
    else{
        //if(time>=18.0f||time<6.0f) shader.setVec3("light.direction",moonlightDir);
      shader.setVec3("light.direction",lightDir);
        shader.setVec3("light.ambient", ambientColor);
        shader.setVec3("light.diffuse", diffuseColor);
        shader.setVec3("light.specular",specularColor);

    }
}

void Sun::refresh(){
    float theta = (time-6.0f)/24.0f *2*PI;

    lightDir=glm::vec3(cos(theta),sin(theta),0.1f);
    moonlightDir=glm::vec3(-10*cos(theta),-10*sin(theta),0.1f);

    float r,g,b;
    float rd,gd,bd;
    if(time>=20.0f||time<4.0f){
        r=0.1;
        b=0.5;
        g=0.1;

    
    }
    if(time>=8.0f&&time<16.0f){
        r=1.0;
        b=1.0;
        g=1.0;
   
    }
    if(time>=16.0f&&time<20.0f){
        r=1.0-0.9*((time-16)/4 * (time-16)/4);
        b=1.0-0.5*((24-time)/4 * (time-16)/4);
        g=1.0-0.9*(time-16)/4;
       
    }
    if(time>=4.0f&&time<8.0f){
        r=0.1+0.9*((12-time)/4 * (time-4)/4);
        b=0.5+0.5*((time-4)/4 * (time-4)/4);
        g=0.1+0.9*(time-4)/4;
  
    }
   
    ambientColor=glm::vec3(0.1*r, 0.1*g, 0.1*b);
    diffuseColor=glm::vec3(0.6*r, 0.6*g, 0.6*b);
    specularColor=glm::vec3(0.8*r, 0.8*g, 0.8*b);
    skyColor=glm::vec3(1.0f*r,1.0f*g,1.0f*b);

}



void Sun::ChangeTime(float deltaTime){
    time+=deltaTime/4;
    
    if(time>24.0f){
        time-=24.0f;
    }
    refresh();
}