#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "bezierface.h"
#include "shader.h"

//总模型类
//一个模型由多个贝叶斯旋转面连接而成



class DIYmodel
{
private:
    /* data */
    vector<glm::vec3> vertices;//锚点，z值1为端点，0为中间点
    vector<BezierFace> faces;

    void makeFaces();


public:
    int load_model(vector<float>* pvalues,vector<float>* tvalues,vector<float>* nvalues);
    int load_frame(vector<float>* pvalues);
    void modify_point(int pid, float nx,float ny);
    void split(int pid);
    void merge(int pid);
    void init();
    DIYmodel();
    ~DIYmodel();
    void Draw(Shader shader);

};




