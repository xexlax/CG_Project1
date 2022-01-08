#include "headers/shader.h"

Shader ourShader,frameShader,skyShader,smoothShader;
void Shader_Init(){
    ourShader=Shader("D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\shader.vs", "D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\shader.fs");  
    frameShader=Shader("D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\edge.vs", "D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\edge.fs");
    skyShader=Shader("D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\skybox.vs", "D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\skybox.fs");

    ourShader.use();
    ourShader.setFloat("material.shininess", 16.0f);
    ourShader.setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
    ourShader.setVec3("light.diffuse", 0.6f, 0.6f, 0.65f);
    ourShader.setVec3("light.specular", 0.8f, 0.8f, 0.8f);
        
    ourShader.setFloat("light.constant", 1.0f);
    ourShader.setFloat("light.linear", 0.05f);
    ourShader.setFloat("light.quadratic", 0.01f);
}
