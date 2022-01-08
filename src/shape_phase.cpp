#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
using namespace std;
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/shader.h"
#include "headers/settings.h"
#include "headers/camera.h"
#include "headers/bezierface.h"
#include "headers/diymodel.h"
#include "headers/skybox.h"
#include "headers/cursor.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

extern float lastX ,lastY;
extern bool MouseDown,PointSelect,firstMouse;
void mouse_callback_shape(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

extern float deltaTime;
extern float lastFrame;
void processInput(GLFWwindow *window);
extern Camera camera;


bool notChange=true;
bool framedisplay=false;
bool framelock=false;
DIYmodel diymodel;


extern Shader ourShader,frameShader,skyShader;

extern void Shader_Init();

void display_shape(GLFWwindow *window){
    glEnable(GL_FRAMEBUFFER_SRGB);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    Shader_Init();

    GLuint VAO, VBO[3];
    GLuint VAO2, VBO2;
    GLuint VAO3, VBO3;
    
    glEnable(GL_DEPTH_TEST);
    glLineWidth(5.0);
    glPointSize(15.0);
     

    
    SkyBox skyBox;

    vector<float> pvalues;  //顶点坐标
	vector<float> tvalues;  //纹理坐标
	vector<float> nvalues;  //法线
    vector <float> fpvalues;//锚点坐标
    vector <float> apvalues;//锚点坐标
    
    int totalindex,findex;
    
    GLuint texture=diymodel.load_texture("D:\\Study\\OpenGL2020.12\\VSCproj\\resources\\purple.png");
  
    glm::vec3 lightPos(12.0f, 30.0f, 5.0f);
    while(!glfwWindowShouldClose(window)){
        diymodel.remake();
      
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
        notChange=true;


        
       


    
    while (notChange&&!glfwWindowShouldClose(window))
    {
        float currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        if(notChange==false)break;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        ourShader.use();


        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
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

        bool active = diymodel.load_active(&apvalues);
        if(active){
        glGenVertexArrays(1, &VAO3);
	    glGenBuffers(1, &VBO3);
	    glBindVertexArray(VAO3);
        glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	    glBufferData(GL_ARRAY_BUFFER, apvalues.size()*4, &apvalues[0], GL_STREAM_DRAW);
        }




        glDisable(GL_DEPTH_TEST);
        frameShader.use();
        frameShader.setMat4("projection", projection);
        frameShader.setMat4("view", view);
        frameShader.setMat4("model", model);
        frameShader.setVec3("color",glm::vec3(0.5,0.5,0.0));
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
      


        if(framedisplay)        
        glDrawArrays(GL_LINE_STRIP, 0, findex);
        
        frameShader.setVec3("color",glm::vec3(1.0,1.0,1.0));

        if(framedisplay)   
        glDrawArrays(GL_POINTS, 0, findex);


        if(active){
        glBindBuffer(GL_ARRAY_BUFFER, VBO3);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
        frameShader.setVec3("color",glm::vec3(0.0,1.0,0.0));
            if(framedisplay)   
            glDrawArrays(GL_POINTS, 0, 1);
        }

      
        glEnable(GL_DEPTH_TEST);

        

        glfwSwapBuffers(window);
		glfwPollEvents();
    }


    }




}



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS )
    {
        int point;
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
            //左键：选择拖拽
                MouseDown=true;
                
                if(framedisplay){
                point=diymodel.get_point(lastX,lastY,camera);
                if(point>=0){
                    cout<<"click point "<<point<<"at"<<lastX<<","<<lastY<<endl;
                    PointSelect=true;
                }
                else{
                      cout<<"click no point "<<"at"<<lastX<<","<<lastY<<endl;
                      PointSelect=false;
                }}
                
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
            //中键,创建新节点
            if(framedisplay){
                point=diymodel.get_line_start_point(lastX,lastY,camera);
                if(point>=0){
                    diymodel.split_point(point);
                    notChange=false;
                }
                else{
                      
                }}
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
            //右键，删除节点
                if(framedisplay){
                point=diymodel.get_point(lastX,lastY,camera);
                if(point>=0){
                    diymodel.remove_point(point);
                    notChange=false;
                   
                }
                else{
                      
                }}
                break;
            default:
                return;
        }
    }
    else{
        MouseDown=false;
        PointSelect=false;}
    return;
}



void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = float(xpos);
        lastY = float(ypos);
        firstMouse = false;
    }

    float xoffset = float(xpos - lastX);
    float yoffset = float(lastY - ypos); // reversed since y-coordinates go from bottom to top

    lastX = float(xpos);
    lastY = float(ypos);
    
    if(MouseDown&&!PointSelect)
    camera.ProcessMouseMovement(xoffset, yoffset);

    if(PointSelect){
        diymodel.modify_point(xoffset,yoffset);
        notChange=false;
    }
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UPWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWNWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        diymodel.load_from_file();
        notChange=false;
        
        }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        diymodel.save_file();

     if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS&&framelock==false)
        {framedisplay=!framedisplay;
        framelock=true;        
        }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
        framelock=false;
}
