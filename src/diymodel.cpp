#include "headers/diymodel.h"
#include <glm/glm.hpp>

float defPoints[] = {
   
    0.0, 0.0, 1,
    1.0, 0.0, 0,
    2.0, 0.0, 0,
    3.0, 0.0, 1,    
    4.0, 2.0, 0,
    4.0, 4.0, 0,
    3.0, 6.0, 1,
    2.0, 7.0, 0,
    1.0, 5.0, 0,
	0.0, 3.0, 1,
	
    	
};


int DIYmodel::load_frame(vector<float>* pvalues){
    int index=0;
    for (auto p:vertices){
        pvalues->push_back(p.x);
        pvalues->push_back(p.y);
        pvalues->push_back(0.0);
        index++;
    }
    return index;

}

int DIYmodel::load_model(vector<float>* pvalues,vector<float>* tvalues,vector<float>* nvalues){
    int index=0;
    for(auto myBezier: faces){
        vector<int> ind = myBezier.getIndices();
	    vector<glm::vec3> verts = myBezier.getVertices();
	    vector<glm::vec2> tex = myBezier.getTexCoords();
	    vector<glm::vec3> norm = myBezier.getNormals();
        for (int i = 0; i < myBezier.getNumIndices(); i++)
            {
                pvalues->push_back(verts[ind[i]].x);
                pvalues->push_back(verts[ind[i]].y);
                pvalues->push_back(verts[ind[i]].z);
                tvalues->push_back(tex[ind[i]].s);
                tvalues->push_back(tex[ind[i]].t);
                nvalues->push_back(norm[ind[i]].x);
                nvalues->push_back(norm[ind[i]].y);
                nvalues->push_back(norm[ind[i]].z);
                
            }
        index+=myBezier.getNumIndices();
    }
    return index;
}

void DIYmodel::init(){

    
    for(int i=0;i<10;i++){
        vertices.push_back(glm::vec3(defPoints[3*i],defPoints[3*i+1],defPoints[3*i+2]));
    }
    makeFaces();
}

void DIYmodel::makeFaces(){
    faces.clear();
    for(int i=0;i+9 <30;i+=9){
        vector<glm::vec2> vec;
        for(int j=0;j<4;j++)
            vec.push_back(glm::vec2(defPoints[i+3*j],defPoints[i+3*j+1]));
        
        BezierFace bf=BezierFace(vec);
        faces.push_back(bf);
    }

}

DIYmodel::DIYmodel(){
    init();
}

DIYmodel::~DIYmodel(){
    
}

