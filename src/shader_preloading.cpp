#include "headers/shader.h"

Shader ourShader,frameShader,skyShader,smoothShader;
void Shader_Init(){
    ourShader=Shader("..\\..\\src\\shaders\\shader.vs", "..\\..\\src\\shaders\\shader.fs");  
    frameShader=Shader("..\\..\\src\\shaders\\edge.vs", "..\\..\\src\\shaders\\edge.fs");
    skyShader=Shader("..\\..\\src\\shaders\\skybox.vs", "..\\..\\src\\shaders\\skybox.fs");

    ourShader.use();
    ourShader.setFloat("material.shininess", 16.0f);
    ourShader.setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
    ourShader.setVec3("light.diffuse", 0.6f, 0.6f, 0.65f);
    ourShader.setVec3("light.specular", 0.8f, 0.8f, 0.8f);
        
    ourShader.setFloat("light.constant", 1.0f);
    ourShader.setFloat("light.linear", 0.05f);
    ourShader.setFloat("light.quadratic", 0.01f);
}
