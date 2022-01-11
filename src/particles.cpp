
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

#include "headers/particles.h"
#include "headers/settings.h"
#include "headers/camera.h"
#include "headers/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

//square
float square[] = {
        // positions            // texture coords
         0.5f,  0.5f, 0.0f,   //   // top right
         0.5f, -0.5f, 0.0f,    //  // bottom right
        -0.5f,  0.5f, 0.0f,   //   // top left 

        0.5f, -0.5f, 0.0f,   //  // bottom right
        -0.5f, -0.5f, 0.0f,   //   // bottom left
        -0.5f,  0.5f, 0.0f,  //    // top left 
    };

float texcoords[] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f, 
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f 

};

string pattern[]={
    "..\\..\\resources\\flower.jpg",
    "..\\..\\resources\\maple.jpg"
};

static const float gravity = 20.0f;
 
const float pi = 3.1416;
float randFloat01() {
	return 1.0*rand() / RAND_MAX;
}
float randFloat(float from, float to) {
	return from + (to - from)*randFloat01();
}
int randInt(int from, int to) {
	return from + rand() % (to - from);
}

ParticleSystem::ParticleSystem(/* args */)
{

    init(1000);

}

ParticleSystem::~ParticleSystem()
{
}


void ParticleSystem::init(int n){
    type=0;
    totnum=n;
    maxlifespan=6000;
    
   
    // // position attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // // texcoord attribute
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    glGenTextures(2, texture);
    for(int i=0;i<2;i++){
    string s=pattern[i];
    
    
    glBindTexture(GL_TEXTURE_2D, texture[i]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(s.c_str(), &width, &height, &nrChannels, 0);
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
    }

}

void ParticleSystem::update(GLfloat deltaTime){
    //新粒子的创建
	Particle particle;
    int newNum = randInt(1,10);
    if(newNum==1){
        //if(particles.size()>totnum) break;
        particle.position = glm::vec3(randFloat(-10.0f, 10.0f), randFloat(20.0f, 25.0f), randFloat(-10.0f, 10.0f));
		particle.velocity = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, -2.0f), randFloat(-1.0f, 1.0f));
		particle.rotation = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f));
		particle.rotation_i=randFloat(0.0f,180);
        particle.statical=false;
        particle.life = maxlifespan;
        particles.push_back(particle); 
        //cout<<"new particle created"<<endl;
    }

    for(int i=0;i<particles.size();){
        particles[i].position+=deltaTime*particles[i].velocity;
        //落在桌子上
        if( particles[i].statical==false &&particles[i].position.y<=0.1f && abs(particles[i].position.x)<=16.0f &&abs(particles[i].position.z)<=16.0f ){
            particles[i].position.y=0.1f;
            particles[i].velocity=glm::vec3(0,0,0);
            particles[i].rotation_i=90;
            particles[i].rotation=glm::vec3(1,0,0);
            particles[i].statical=true;
            particles[i].rotation_y=randInt(0,180);
        }
        //判断摧毁
        particles[i].life--;
        if( particles[i].position.y<-10.0f) {
            particles.erase(particles.begin()+i);
           // cout<<"a particle died"<<endl;
        }
        else{
            i++;
        }
    }

	
}


void ParticleSystem::Draw(Shader s,Camera camera){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
    glEnable(GL_DEPTH_TEST);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture[type]);

    glEnable(GL_DEPTH_TEST);
    s.use();
    s.setInt("s",4);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();       

    s.setMat4("projection", projection);
    s.setMat4("view", view);        

    for(int i=0;i<particles.size();i++){
        glm::mat4 model;
	    model = glm::translate(model, particles[i].position);
        model = glm::rotate(model, particles[i].rotation_i, particles[i].rotation);
        if(particles[i].statical)
            model = glm::rotate(model, particles[i].rotation_y, glm::vec3(0,0,1));
	    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        s.setMat4("model",model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0,6);
        //glDrawElements(GL_POINTS, 6, GL_UNSIGNED_INT, 0);


    }

}

void ParticleSystem::Switch_Type(){
    type++;
    if(type>=2) type=0;
}