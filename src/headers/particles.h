#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
using namespace std;
#include "shader.h"
#include "camera.h"


struct Particle{
    glm::vec3 position;
    glm::vec3 rotation;
    float rotation_i;
    float rotation_y;
    glm::vec3 velocity;
    bool statical;
    int life;
};

class ParticleSystem{
private:
    /* data */
    GLuint texture[3];
    int totnum;
    int maxlifespan;
    unsigned int VAO, VBO[2];
    vector<Particle> particles;
    vector<float> pvalues;  //顶点坐标
	vector<float> tvalues;  //纹理坐标

    int type;

protected:
    
    


public:
    ParticleSystem(/* args */);

    ~ParticleSystem();

    void init(int totnum);
    void update(GLfloat deltaTime);
    void Draw(Shader s,Camera c);
    void Switch_Type();
};

