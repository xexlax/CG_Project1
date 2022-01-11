#include "headers/shader.h"

Shader ourShader,frameShader,skyShader,smoothShader,particleShader,simpleDepthShader;
void Shader_Init(){
    ourShader=Shader("..\\..\\src\\shaders\\shader.vs", "..\\..\\src\\shaders\\shader.fs");  
    frameShader=Shader("..\\..\\src\\shaders\\edge.vs", "..\\..\\src\\shaders\\edge.fs");
    skyShader=Shader("..\\..\\src\\shaders\\skybox.vs", "..\\..\\src\\shaders\\skybox.fs");
    particleShader=Shader("..\\..\\src\\shaders\\shader_particles.vs", "..\\..\\src\\shaders\\shader_particles.fs");
    simpleDepthShader=Shader("..\\..\\src\\shaders\\simple_depth_shader.vs", "..\\..\\src\\shaders\\simple_depth_shader.fs");

     particleShader.use();
     particleShader.setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
    
    // ourShader.use();
     ourShader.setFloat("material.shininess", 16.0f);
    // ourShader.setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
    // ourShader.setVec3("light.diffuse", 0.6f, 0.6f, 0.65f);
    // ourShader.setVec3("light.specular", 0.8f, 0.8f, 0.8f);
        



}
