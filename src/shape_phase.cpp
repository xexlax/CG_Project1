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
bool texframedisplay=false;
bool texframelock=false;
DIYmodel diymodel;


extern Shader ourShader,frameShader,skyShader;

extern void Shader_Init();

void display_shape(GLFWwindow *window){
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glfwSetMouseButtonCallback(window, mouse_button_callback);
    Shader_Init();
    glEnable(GL_DEPTH_TEST);
    glLineWidth(5.0);
    glPointSize(15.0);   
    SkyBox skyBox;
    glm::vec3 lightPos(12.0f, 30.0f, 5.0f);
    while(!glfwWindowShouldClose(window)){
        diymodel.remake();
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

        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model",model);

       
        diymodel.Draw(camera,ourShader,lightPos);
        skyBox.Draw(camera,skyShader);
        diymodel.DrawFrame(camera,frameShader,framedisplay);
        diymodel.DrawTexFrame(camera,frameShader,texframedisplay);
        

        glfwSwapBuffers(window);
		glfwPollEvents();
    }
    }
}

bool lorSelection;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS )
    {
        int point,circle;
        bool lor;
        
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

                if(texframedisplay){
                    circle=diymodel.get_circle(lastX,lastY,camera,lor);
                    if(circle>=0){
                        cout<<"click circ "<<circle<<"at"<<lastX<<","<<lastY<<endl;
                    PointSelect=true;
                    lorSelection=lor;
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
        if(framedisplay)
        diymodel.modify_point(xoffset,yoffset,camera);
        if(texframedisplay)
        diymodel.modify_circle(xoffset,yoffset,camera,lorSelection);
        notChange=false;
        cout<<"selecting"<<endl;
    }
}

bool B_lock=false;
bool Com_lock=false,Dot_lock=false;
bool T_lock=false,R_lock=false;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 相机移动区
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

    //显示设置
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS&&framelock==false)
        {
        framedisplay=!framedisplay;
        texframedisplay=false;
        framelock=true;        
        }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
        framelock=false;

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS&&texframelock==false)
        {
        texframedisplay=!texframedisplay;
        framedisplay=false;
        texframelock=true;        
        }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
        texframelock=false;

   
 
    //读写文件
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        diymodel.load_from_file();
        notChange=false;
        }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        diymodel.save_file();

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
        diymodel.add_texture();
        notChange=false;
    }

    

    //材质选项

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS&&B_lock==false)
        {diymodel.switch_material();
        notChange=false;
        B_lock=true;
        
        }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
        B_lock=false;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS&&!T_lock){
        diymodel.trans_tex_type();
        notChange=false;
        T_lock=true;
    }
     if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
        T_lock=false;
        
        
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS&&!R_lock){
        diymodel.remove_texture();
        notChange=false;
        R_lock=true;
    }
     if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
        R_lock=false;
        

    



     if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS&&Com_lock==false)
        {
        diymodel.add_repeat(false);
        notChange=false;
        Com_lock=true;
        }
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_RELEASE)
        Com_lock=false;

     if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS&&Dot_lock==false)
        {
        diymodel.add_repeat(true);
        notChange=false;
        Dot_lock=true;
        }
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_RELEASE)
        Dot_lock=false;
        


    
}
