#ifndef DIYMODEL_H
#define DIYMODEL_H

#include <vector>
#include <string>
#include <cmath>
#include "camera.h"

#define PI2 3.1415927

//储存用于旋转的b样条曲线控制点
typedef struct{
    float x;
    float y;
    //DIYVertex(float a,float b):x(a),y(b){};
} DIYVertex;



//根据v坐标（v）计算半径（r）
float getRadiantNurbs(int h, vector<DIYVertex> vec){
    int lb=0;
    for(int i=0;i<vec.size();i++){
        if(vec[i].y>=h){
            lb=i;
            break;
        }
    }
    return 5.0f;


}

//3d空间中的点
typedef struct{
    double x;
    double y;
    double z;
    glm::vec3 normal;
    

} Point;

Point getpoint(double u,double v){
        Point t;
        t.x = sin(PI2*v)*cos(PI2*u);
        t.y = sin(PI2*v)*sin(PI2*u);
        t.z = cos(PI2*v);
        return t;
    }

class DIYModel
{
    private:
    vector<DIYVertex> vertexs_outside;
    vector<DIYVertex> vertexs_inside;
    int ustepNum=20,vstepNum=20; 
    DIYModel();
    ~DIYModel();
    public:
    void Init();
    void Draw(Camera camera);
    void AddVertex(int);
    void DeleteVertex(int);
    void MoveVertex(int,DIYVertex);
};


void DIYModel::Draw(Camera camera){
    double ustep = 1.0f / ustepNum;
    double vstep = 1.0f / vstepNum;
    double u = 0,v = 0;
    //绘制下端三角形组
    for(int i = 0;i<ustepNum;i++)
    {
        glBegin(GL_LINE_LOOP);
        Point a=getpoint(0,0);
        glVertex3d(a.x,a.y,a.z);
        Point b=getpoint(u,vstep);
        glVertex3d(b.x,b.y,b.z);
        Point c=getpoint(u+ustep,vstep);
        glVertex3d(c.x,c.y,c.z);
        u += ustep;
        glEnd();
    }
    //绘制中间四边形组
    u = 0, v = vstep;
    for(int i=1;i<vstepNum-1;i++)
    {
        for(int j=0;j<ustepNum;j++)
        {
            glBegin(GL_LINE_LOOP);
            Point a=getpoint(u,v);
            Point b=getpoint(u+ustep,v);
            Point c=getpoint(u+ustep,v+vstep);
            Point d=getpoint(u,v+vstep);
            glVertex3d(a.x,a.y,a.z);
            glVertex3d(b.x,b.y,b.z);
            glVertex3d(c.x,c.y,c.z);
            glVertex3d(d.x,d.y,d.z);
            u += ustep;
            glEnd();
        }
        v += vstep;
    }
    //绘制下端三角形组
    u = 0;
    for(int i=0;i<ustepNum;i++)
    {
        glBegin(GL_LINE_LOOP);
        Point a=getpoint(0,1);
        Point b=getpoint(u,1-vstep);
        Point c=getpoint(u+ustep,1-vstep);
        glVertex3d(a.x,a.y,a.z);
        glVertex3d(b.x,b.y,b.z);
        glVertex3d(c.x,c.y,c.z);
        glEnd();
    }

}



#endif