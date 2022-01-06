#pragma once
#include "shader.h"
enum Cursor_Type{
    CURSOR_SELECT,
    CURSOR_ADD,
    CURSOR_REDUCE
};


class Cursor
{

private:
    unsigned int cVAO, cVBO, cEBO;
    unsigned int cursorTexture;

public:
    float x,y;
    Cursor_Type cursor_type;


    Cursor(/* args */);
    void init();

    void Move(float x,float y);
    void Draw(Shader s);
    ~Cursor();
};


