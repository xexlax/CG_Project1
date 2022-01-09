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
    UI();
    ~UI();
    void init();
    void Draw();

};


