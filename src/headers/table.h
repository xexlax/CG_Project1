#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
using namespace std;
#include "shader.h"
#include "camera.h"

class Table
{
private:
    /* data */
    unsigned int VAO,VBO[3];
   
    
    

public:
    unsigned int texture;
    Table(/* args */);
    ~Table();

    void Draw(Shader s,Camera camera);
};

