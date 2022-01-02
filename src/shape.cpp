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


#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

float controlPoints[] = {
   
	4.0, 3.0, 0.0,
    3.0, 4.0, 0.0,
    3.0, 1.0, 0.0,
    3.0, 0.0, 0.0,
    4.0,-1.0,0.0
    	
};
float controlPoints2[] = {
   
	4.0, 3.0, 0.0,
    4.0, 4.0, 0.0,
    4.0, 1.0, 0.0,
    4.0, 0.0, 0.0
    	
};

extern float deltaTime;
extern float lastFrame;
void processInput(GLFWwindow *window);
extern Camera camera;



void display(GLFWwindow *window){

    GLuint VAO, VBO[3];
    GLuint texture;
    glEnable(GL_DEPTH_TEST);

    BezierFace myBezier = BezierFace(4, controlPoints);

    vector<float> pvalues;  //顶点坐标
	vector<float> tvalues;  //纹理坐标
	vector<float> nvalues;  //法线
	vector<int> ind = myBezier.getIndices();
	vector<glm::vec3> verts = myBezier.getVertices();
	vector<glm::vec2> tex = myBezier.getTexCoords();
	vector<glm::vec3> norm = myBezier.getNormals();
	for (int i = 0; i < myBezier.getNumIndices(); i++)
	{
		pvalues.push_back(verts[ind[i]].x);
		pvalues.push_back(verts[ind[i]].y);
		pvalues.push_back(verts[ind[i]].z);
		tvalues.push_back(tex[ind[i]].s);
		tvalues.push_back(tex[ind[i]].t);
		nvalues.push_back(norm[ind[i]].x);
		nvalues.push_back(norm[ind[i]].y);
		nvalues.push_back(norm[ind[i]].z);
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

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
   
    ourShader.setFloat("material.shininess", 16.0f);
    ourShader.setVec3("light.ambient", 0.8f, 0.8f, 0.85f);
    ourShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.75f);
    ourShader.setVec3("light.specular", 0.2f, 0.2f, 0.2f);

    ourShader.setFloat("light.constant", 1.0f);
    ourShader.setFloat("light.linear", 0.05f);
    ourShader.setFloat("light.quadratic", 0.01f);
    glm::vec3 lightPos(10.0f, 10.0f, 10.0f);

    while (!glfwWindowShouldClose(window))
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


        glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, myBezier.getNumIndices());

        glfwSwapBuffers(window);
		glfwPollEvents();
    }




}