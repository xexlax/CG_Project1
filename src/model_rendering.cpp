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
#include "headers/model.h"
#include "headers/settings.h"
#include "headers/skybox.h"

//public variants
Camera camera(glm::vec3(0.0f, 5.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool close = false;

void processInput(GLFWwindow *window);
void render_model(GLFWwindow *window, string dir)
{
    glm::vec3 lightPos(10.0f, 19.0f, 10.0f);
    firstMouse = true;
    lastX = SCR_WIDTH / 2.0f;
    lastY = SCR_HEIGHT / 2.0f;
    
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).

    // configure global opengl state
    // -----------------------------
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    //glEnable(GL_PROGRAM_POINT_SIZE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // build and compile shaders
    // -------------------------
    Shader ourShader("D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\shader_model.vs", "D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\shader_model_2d.fs");
    Shader singleShader("D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\edge.vs", "D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\edge.fs");
    Shader skyShader("D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\skybox.vs", "D:\\Study\\OpenGL2020.12\\VSCproj\\src\\shaders\\skybox.fs");
    // load models
    // -----------
    Model ourModel("D:\\Study\\OpenGL2020.12\\VSCproj\\resources\\" + dir);
    //Model ourModel("D:\\Study\\OpenGL2020.12\\VSCproj\\resources\\nanosuit\\nanosuit.obj");

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    ourShader.use();

    ourShader.setVec3("light.position", lightPos);

    ourShader.setVec3("light.ambient", 0.8f, 0.8f, 0.85f);
    ourShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.75f);
    ourShader.setVec3("light.specular", 0.2f, 0.2f, 0.2f);

    ourShader.setFloat("light.constant", 1.0f);
    ourShader.setFloat("light.linear", 0.05f);
    ourShader.setFloat("light.quadratic", 0.01f);

    SkyBox skybox;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        singleShader.use();
        singleShader.setMat4("view", view);
        singleShader.setMat4("projection", projection);

        // view/projection transformations
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setFloat("material.shininess", 4.0f);
        ourShader.setVec3("viewPos", camera.Position);

        // render the loaded model
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        // render the edge
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        // glDisable(GL_DEPTH_TEST);
        singleShader.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        singleShader.setMat4("model", model);
        ourModel.Draw(singleShader);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        // glEnable(GL_DEPTH_TEST);

        skybox.Draw(camera, skyShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
