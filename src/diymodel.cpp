#include "headers/diymodel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/camera.h"
#include "headers/settings.h"
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

bool DIYmodel::load_active(vector<float>* pvalues){
    pvalues->clear();
    if(active_point<0) return false;
    auto p=vertices[active_point];
    pvalues->push_back(p.x);
    pvalues->push_back(p.y);
    return true;
}

int DIYmodel::load_frame(vector<float>* pvalues){
    pvalues->clear();
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
    pvalues->clear();
    tvalues->clear();
    nvalues->clear();
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
    active_point=-1;

    
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

bool clamp(float x,float y,float diff){

    if(abs(x-y)<diff){
        return true;
    }
    return false;

}

glm::vec4 wild_screen_baroque(glm::vec4 pos,glm::mat4 view,glm::mat4 projection){
        pos=view*pos;
        pos=projection*pos;

        pos.x=pos.x/pos.w;
        pos.y=pos.y/pos.w;
        pos.z=pos.z/pos.w;
        pos.w=1;

        pos.x=(SCR_WIDTH/2)*(pos.x+1);
        pos.y=(SCR_HEIGHT/2)*(1-pos.y);

        return pos;

}

int DIYmodel::get_point(float x,float y,Camera camera){
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    float zc=camera.Position.z;
    
    for(int i=0;i<vertices.size();i++){
        glm::vec4 pos(vertices[i].x,vertices[i].y,0.0,1.0);
        pos=wild_screen_baroque(pos,view,projection);
        
  //     cout<<"point "<<i<<"at"<<pos.x<<","<<pos.y<<endl;
        

        if(clamp(pos.x,x,10.0)&&clamp(pos.y,y,10.0)){
            active(i);
            return i;
        }


        
    }
    return -1;
}

void DIYmodel::active(int index){
    if(index==active_point){
        active_point=-1;
    }
    else{
        active_point=index;
    }
}

void DIYmodel::modify_point(float dx,float dy){
    if(active_point<0) return;
    else{
        vertices[active_point].x+=dx/50;
        vertices[active_point].y+=dy/50;
    }
}
void DIYmodel::remake(){
    faces.clear();
    for(int i=0;i+3<vertices.size() ;i+=3){
        vector<glm::vec2> vec;
        for(int j=0;j<4;j++)
            vec.push_back(glm::vec2(vertices[i+j].x,vertices[i+j].y));
        
        BezierFace bf=BezierFace(vec);
        faces.push_back(bf);
    }

}

void DIYmodel::remove_point(int pid){
    if(pid<=0) return;
    if(pid==vertices.size()-1) return;
    //首尾不能删除
    if(pid%3!=0) return;
    
    int i=0;
  
    for(vector<glm::vec3>::iterator it=vertices.begin();it!=vertices.end();i++){
        if(i==pid-1){
            vertices.erase(it,it+3);
            return;        
        }
        else{
            it++; 
        }
        

    }

}

void DIYmodel::split_point(int pid){
    if(pid<0) return;
    int i=0;
    for(vector<glm::vec3>::iterator it=vertices.begin();it!=vertices.end();i++){
        if(i==pid+1){
            //在此点和下一个点之间插入三个点
            glm::vec4 pos0(vertices[i-1].x,vertices[i-1].y,0.0,1.0);
            glm::vec4 pos1(vertices[i].x,vertices[i].y,0.0,1.0);
            glm::vec4 posx;
         
            for(float j=1;j<=3;j++){
                posx= (j/4)*pos0+(1-j/4)*pos1;
                it=vertices.insert(it,posx);
                
            }

            return;

            

        }
        else{
            it++;
        }
    }

}

int DIYmodel::get_line_start_point(float x,float y,Camera camera){
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    float zc=camera.Position.z;
    
    for(int i=0;i+1<vertices.size();i++){
        glm::vec4 pos0(vertices[i].x,vertices[i].y,0.0,1.0);
        pos0=wild_screen_baroque(pos0,view,projection);

        glm::vec4 pos1(vertices[i+1].x,vertices[i+1].y,0.0,1.0);
        pos1=wild_screen_baroque(pos1,view,projection);
        
  //     cout<<"point "<<i<<"at"<<pos.x<<","<<pos.y<<endl;
        
        bool c1=clamp((y-pos0.y)/(x-pos0.x),(y-pos1.y)/(x-pos1.x),1.0);//共线
        bool c0=clamp((x-pos0.x)/(y-pos0.y),(x-pos1.x)/(y-pos1.y),1.0);//共线
        bool c2=(x<pos0.x+5&&x>pos1.x-5)||(x<pos1.x+5&&x>pos0.x-5);//在线段上
        bool c3=(y<pos0.y+5&&y>pos1.y-5)||(y<pos1.y+5&&y>pos0.y-5);//在线段上
        if((c1||c0)&&c2&&c3)
            return i;


        
    }
    return -1;

}