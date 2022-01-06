#pragma once
#include "shader.h"
#include <string>



class UI
{
private:
    /* data */
    unsigned int uiVAO, uiVBO;
    unsigned int uiTexture;
    
    



public:
    float px,py;
    float width,height;
    string texture;

    UI();
    
    ~UI();

    void init(float,float,float,float);

    void Draw();

};


