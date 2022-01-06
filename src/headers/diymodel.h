#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "bezierface.h"
#include "shader.h"
#include "camera.h"

//总模型类
//一个模型由多个贝叶斯旋转面连接而成



class DIYmodel
{
private:
    /* data */
    vector<glm::vec3> vertices;//锚点
    int active_point;
    
    vector<BezierFace> faces;

    



public:
    int load_model(vector<float>* pvalues,vector<float>* tvalues,vector<float>* nvalues);
    int load_frame(vector<float>* pvalues);
    bool load_active(vector<float>* pvalues);
    void modify_point(float dx,float dy);
    int get_point(float x,float y,Camera camera);
    int get_line_start_point(float x,float y,Camera camera);
    void active(int index);
    void makeFaces();
    void split_point(int pid);
    void remove_point(int pid);
    void init();
    void remake();
    void save_file(string filename);
    void load_from_file(string filename);

    DIYmodel();
    ~DIYmodel();

};




