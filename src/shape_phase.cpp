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
#include "headers/particles.h"
#include "headers/table.h"
#include "headers/sun.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"
Camera camera(glm::vec3(0.0f, 5.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool close = false;

extern bool MouseDown,PointSelect,firstMouse;
void mouse_callback_shape(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


void processInput(GLFWwindow *window);

bool timeflow=false;
bool partdisplay=false;
bool notChange=true;
bool framedisplay=false;
bool framelock=false;
bool texframedisplay=false;
bool texframelock=false;
vector<DIYmodel> diymodels;
int active_model_index=0;
DIYmodel diymodel;
unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;
unsigned int depthMap;

extern Shader ourShader,frameShader,skyShader,particleShader,simpleDepthShader;

extern void Shader_Init();

void display_shape(GLFWwindow *window){
  //  glEnable(GL_FRAMEBUFFER_SRGB);
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    diymodels.clear();
    diymodels.push_back(diymodel);
    active_model_index=0;

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    Shader_Init();
    glEnable(GL_DEPTH_TEST);
    glLineWidth(5.0);
    glPointSize(15.0);   
    SkyBox skyBox;
    Sun sun;
    

    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    
    Table table;
    ParticleSystem particleSys;
    
    while(!glfwWindowShouldClose(window)){
        
       diymodels[active_model_index].remake();
        notChange=true;
    
    while (notChange&&!glfwWindowShouldClose(window))
    {
        float currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if(timeflow){
        sun.ChangeTime(deltaTime);}
        sun.Apply(ourShader,0);
        sun.Apply(skyShader,1);
        sun.Apply(particleShader,1);
        glm::vec3 lightPos=sun.lightDir;

        

        processInput(window);
        if(notChange==false)break;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model",model);
        ourShader.setVec3("viewPos",camera.Position);  

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 20.0f;
        lightProjection = glm::ortho(-18.0f, 18.0f, -18.0f, 18.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        simpleDepthShader.setMat4("model", model);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glCullFace(GL_FRONT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            table.Draw(simpleDepthShader,camera);       
            for(auto x:diymodels)
            x.Draw(camera,simpleDepthShader,lightPos);            
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.use();
        ourShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        ourShader.setInt("shadowMap",7);
        if(partdisplay){
        particleSys.update(deltaTime);

        particleSys.Draw(particleShader,camera);}

        
        table.Draw(ourShader,camera);       

        //ourShader.setInt("shadowdisplay",0);
        for(auto x:diymodels)
            x.Draw(camera,ourShader,lightPos);       

        skyBox.Draw(camera,skyShader);         
        if(framedisplay)
          for(auto x:diymodels)
            x.DrawFrame(camera,frameShader,framedisplay);     
        if(texframedisplay)
          for(auto x:diymodels)
            x.DrawTexFrame(camera,frameShader,texframedisplay);  
        
        
        

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
                    for(int i=0;i<diymodels.size();i++){
                        point=diymodels[i].get_point(lastX,lastY,camera);
                        if(point>=0){
                            
                            PointSelect=true;
                            active_model_index=i;
                            break;
                        }
                        else{
                            PointSelect=false;
                        }}
                }

                if(texframedisplay){
                for(int i=0;i<diymodels.size();i++){
                    circle=diymodels[i].get_circle(lastX,lastY,camera,lor);
                    if(circle>=0){                       
                        PointSelect=true;
                        lorSelection=lor;
                        active_model_index=i;
                        break;
                    }
                    else{                      
                      PointSelect=false;
                    }
                }
                
                }

                
                
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
            //中键,创建新节点
            if(framedisplay){

                
                point=diymodels[active_model_index].get_line_start_point(lastX,lastY,camera);
                if(point>=0){
                    diymodels[active_model_index].split_point(point);
                    notChange=false;
                }
                
                
                }
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
            //右键，删除节点
                if(framedisplay){
                point=diymodels[active_model_index].get_point(lastX,lastY,camera);
                if(point>=0){
                    diymodels[active_model_index].remove_point(point);
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

        
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            diymodels[active_model_index].modify_offset(xoffset,yoffset,camera,0);
        else 
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
            diymodels[active_model_index].modify_offset(xoffset,yoffset,camera,1);
        else 
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            diymodels[active_model_index].modify_offset(xoffset,yoffset,camera,2);
        else
            diymodels[active_model_index].modify_point(xoffset,yoffset,camera);
        if(texframedisplay)
            diymodels[active_model_index].modify_circle(xoffset,yoffset,camera,lorSelection);
        notChange=false;
        
        cout<<"selecting"<<endl;
    }
}

bool B_lock=false,Del_lock=false;
bool Com_lock=false,Dot_lock=false;
bool T_lock=false,R_lock=false,V_lock=false;
bool O_lock=false,P_lock=false;

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

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS&&O_lock==false)
        {
        timeflow=!timeflow;
        O_lock=true;        
        }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE)
        O_lock=false;

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS&&P_lock==false)
        {
        partdisplay=!partdisplay;
        P_lock=true;        
        }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
        P_lock=false;
 
    //读写文件
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        DIYmodel newdiymodel;
        newdiymodel.load_from_file();
        active_model_index=diymodels.size();
        diymodels.push_back(newdiymodel);
        
        notChange=false;
        }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        diymodels[active_model_index].save_file();

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
        diymodels[active_model_index].add_texture();
        notChange=false;
    }
    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS&&!Del_lock){
        if(diymodels.size()>active_model_index)
        {
        
            diymodels.erase(diymodels.begin()+active_model_index);
            active_model_index=0;
             
           
            Del_lock=true;
        }
        
        notChange=false;
    }
    if(glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_RELEASE){
        Del_lock=false;
    }

    

    //材质选项

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS&&B_lock==false)
        {diymodels[active_model_index].switch_material();
        notChange=false;
        B_lock=true;
        
        }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
        B_lock=false;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS&&!T_lock){
        diymodels[active_model_index].trans_tex_type();
        notChange=false;
        T_lock=true;
    }
     if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
        T_lock=false;

     if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS&&!V_lock){
        diymodels[active_model_index].reverse_tex();
        notChange=false;
        V_lock=true;
    }
     if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE)
        V_lock=false;
        
        
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS&&!R_lock){
        diymodels[active_model_index].remove_texture();
        notChange=false;
        R_lock=true;
    }
     if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
        R_lock=false;

     if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS&&Com_lock==false)
        {
        diymodels[active_model_index].add_repeat(false);
        notChange=false;
        Com_lock=true;
        }
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_RELEASE)
        Com_lock=false;

     if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS&&Dot_lock==false)
        {
        diymodels[active_model_index].add_repeat(true);
        notChange=false;
        Dot_lock=true;
        }
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_RELEASE)
        Dot_lock=false;
        


    
}
