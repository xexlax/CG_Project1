#pragma once

#include <iostream>
#include <string>
using namespace std;
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/shader.h"
#include "headers/camera.h"
#include "headers/bezierface.h"
#include "headers/diymodel.h"
#include "headers/skybox.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"




extern float deltaTime;
extern float lastFrame;
void processInput(GLFWwindow *window);
extern Camera camera;

bool notChange=true;


void display(GLFWwindow *window){

    GLuint VAO, VBO[3];
    GLuint VAO2, VBO2;
    GLuint texture;
    glEnable(GL_DEPTH_TEST);
    glLineWidth(5.0);
    glPointSize(15.0);
     

    DIYmodel diymodel;
    SkyBox skyBox;

    vector<float> pvalues;  //顶点坐标
	vector<float> tvalues;  //纹理坐标
	vector<float> nvalues;  //法线
    vector <float> fpvalues;//锚点坐标
    
    int totalindex,findex;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("D:\\Study\\OpenGL2020.12\\VSCproj\\resources\\container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    Shader ourShader("D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\shader.vs", "D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\shader.fs");
    Shader frameShader("D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\edge.vs", "D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\edge.fs");
    Shader skyShader("D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\skybox.vs", "D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\skybox.fs");

    ourShader.setFloat("material.shininess", 16.0f);
    ourShader.setVec3("light.ambient", 0.8f, 0.8f, 0.85f);
    ourShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.75f);
    ourShader.setVec3("light.specular", 0.2f, 0.2f, 0.2f);
    
    ourShader.setFloat("light.constant", 1.0f);
    ourShader.setFloat("light.linear", 0.05f);
    ourShader.setFloat("light.quadratic", 0.01f);
    glm::vec3 lightPos(10.0f, 10.0f, 10.0f);
    while(!glfwWindowShouldClose(window)){
        
        totalindex=diymodel.load_model(&pvalues,&tvalues,&nvalues);

        glGenVertexArrays(1, &VAO);
	    glGenBuffers(3, VBO);
	    glBindVertexArray(VAO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	    glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	    glBufferData(GL_ARRAY_BUFFER, tvalues.size()*4, &tvalues[0], GL_STREAM_DRAW);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	    glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STREAM_DRAW);


        
       


    
    while (notChange&&!glfwWindowShouldClose(window))
    {
        float currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        ourShader.use();


        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);
        
        ourShader.setVec3("viewPos",camera.Position);
        ourShader.setVec3("light.position", lightPos);

       
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        //glEnable(GL_CULL_FACE);
        //glFrontFace(GL_CCW);


        //glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, totalindex);

        skyBox.Draw(camera,skyShader);


        findex= diymodel.load_frame(&fpvalues);
        glGenVertexArrays(1, &VAO2);
	    glGenBuffers(1, &VBO2);
	    glBindVertexArray(VAO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	    glBufferData(GL_ARRAY_BUFFER, fpvalues.size()*4, &fpvalues[0], GL_STREAM_DRAW);


        

        glDisable(GL_DEPTH_TEST);
        frameShader.use();
        frameShader.setMat4("projection", projection);
        frameShader.setMat4("view", view);
        frameShader.setMat4("model", model);
        frameShader.setVec3("color",glm::vec3(1.0,0.7,0.3));
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(0);
      
        glDrawArrays(GL_LINE_STRIP, 0, findex);
        
        frameShader.setVec3("color",glm::vec3(1.0,1.0,1.0));

        glDrawArrays(GL_POINTS, 0, findex);
        glEnable(GL_DEPTH_TEST);

        

        glfwSwapBuffers(window);
		glfwPollEvents();
    }


    }




}