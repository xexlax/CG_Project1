#pragma once
#include <vector>

#include <glm/glm.hpp>
#include "bezierface.h"
#include "shader.h"
#include "camera.h"

//总模型类
//一个模型由多个贝叶斯旋转面连接而成

class DIYtexture{
    public:
    int type; //0 环绕 //1 顶部
    int repeat;//一圈的重复数 顶部忽略
    float l,r;//范围
    GLuint map;
    DIYtexture(int t,int re,float l0,float r0,GLuint m){
        type=t;
        repeat=re;
        l=l0;
        r=r0;
        map=m;
    }
};

class DIYmodel
{
private:
    /* data */
    vector<glm::vec3> vertices;//锚点
    int active_point;
    vector<BezierFace> faces;
    vector<DIYtexture> textures;

    int VAO, VBO[3];
    int VAO2, VBO2;
    int VAO3, VBO3;

    vector<float> pvalues;  //顶点坐标
	vector<float> tvalues;  //纹理坐标
	vector<float> nvalues;  //法线
    vector <float> fpvalues;//锚点坐标
    vector <float> apvalues;//锚点坐标




public:
    int load_model(vector<float>* pvalues,vector<float>* tvalues,vector<float>* nvalues);
    int load_frame(vector<float>* pvalues);
    int load_circle(vector<float>* pvalues);
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
    void save_file();
    void load_from_file();
    GLuint load_texture(string s);
    void add_texture();
    void edit_texture();
    void remove_texture();
    

    void Draw(Camera c,Shader s);

    DIYmodel();
    ~DIYmodel();

};




