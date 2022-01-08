#include <windows.h>
#include <commdlg.h>
#include <fstream>
using namespace std;

#include "headers/diymodel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/camera.h"
#include "headers/settings.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"


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
//加载单点到数组
bool DIYmodel::load_active(vector<float>* pvalues){
    pvalues->clear();
    if(active_point<0) return false;
    auto p=vertices[active_point];
    pvalues->push_back(p.x);
    pvalues->push_back(p.y);
    return true;
}
//加载线框模型到数组
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
//加载模型到数组
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

//初始化
void DIYmodel::init(){
    active_point=-1;
    for(int i=0;i<10;i++){
        vertices.push_back(glm::vec3(defPoints[3*i],defPoints[3*i+1],defPoints[3*i+2]));
    }
    makeFaces();
}

//初始化建立面
void DIYmodel::makeFaces(){
    faces.clear();
    float totlength=0;
    for(int i=0;i+9 <30;i+=9){
        vector<glm::vec2> vec;
        for(int j=0;j<4;j++)
        vec.push_back(glm::vec2(defPoints[i+3*j],defPoints[i+3*j+1]));
        
        
        BezierFace bf=BezierFace(vec);
        
        faces.push_back(bf);
    }

}

//刷新
void DIYmodel::remake(){
    faces.clear();
    float totl=0;
    vector<float> lengths;
    for(int i=0;i+3<vertices.size() ;i+=3){
        for(int j=0;j<3;j++){           
            float t=(vertices[i+j+1].x-vertices[i+j].x)
            *(vertices[i+j+1].x-vertices[i+j].x)+
            (vertices[i+j+1].y-vertices[i+j].y)
            *(vertices[i+j+1].y-vertices[i+j].y);
		    totl+=sqrt(t);	
            lengths.push_back(sqrt(t));
        }
    }

    float offset=0;

    for(int i=0;i+3<vertices.size() ;i+=3){
        vector<glm::vec2> vec;
        
        for(int j=0;j<4;j++)
            vec.push_back(glm::vec2(vertices[i+j].x,vertices[i+j].y));
        
        float l=offset;
        float r=l+(lengths[i]+lengths[i+1]+lengths[i+2])/totl;
        BezierFace bf=BezierFace(vec,l,r);
        offset=r;
        
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

void DIYmodel::active(int index){
    if(index==active_point){
        active_point=-1;
    }
    else{
        active_point=index;
    }
}
//移动节点
void DIYmodel::modify_point(float dx,float dy){
    if(active_point<0) return;
    else{
        vertices[active_point].x+=dx/50;
        vertices[active_point].y+=dy/50;
    }
}
//删除节点
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
//新建节点
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

//根据鼠标查找线段
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

//根据鼠标查找线段
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

void DIYmodel::load_from_file(){
    OPENFILENAME ofn;      // 公共对话框结构。   
	TCHAR szFile[MAX_PATH]; // 保存获取文件名称的缓冲区。             
	// 初始化选择文件对话框。   
	ZeroMemory(&ofn, sizeof(OPENFILENAME));  
	ofn.lStructSize = sizeof(OPENFILENAME);  
	ofn.hwndOwner = NULL;  
	ofn.lpstrFile = szFile;  
	ofn.lpstrFile[0] = '\0'; 
	ofn.nMaxFile = sizeof(szFile);  
	ofn.lpstrFilter = "All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";  
	ofn.nFilterIndex = 1;  
	ofn.lpstrFileTitle = NULL;  
	ofn.nMaxFileTitle = 0;  
	ofn.lpstrInitialDir = NULL;  
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  
	//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);  
	// 显示打开选择文件对话框。   
 
	if ( GetOpenFileName(&ofn) )  
	{  
		//显示选择的文件。   
		ifstream is;
        is.open(szFile);
        glm::vec3 t;
        vertices.clear();

        while (is>>t.x)
        {
            is>>t.y>>t.z;
            vertices.push_back(t);

        }
        

        is.close();




	} 
}

void DIYmodel::save_file(){
    OPENFILENAME ofn;      // 公共对话框结构。   
	TCHAR szFile[MAX_PATH]; // 保存获取文件名称的缓冲区。             
	// 初始化选择文件对话框。   
	ZeroMemory(&ofn, sizeof(OPENFILENAME));  
	ofn.lStructSize = sizeof(OPENFILENAME);  
	ofn.hwndOwner = NULL;  
	ofn.lpstrFile = szFile;  
	ofn.lpstrFile[0] = '\0'; 
	ofn.nMaxFile = sizeof(szFile);  
	ofn.lpstrFilter = "All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";  
	ofn.nFilterIndex = 1;  
	ofn.lpstrFileTitle = NULL;  
	ofn.nMaxFileTitle = 0;  
	ofn.lpstrInitialDir = NULL;  
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  
	//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);  
	// 显示打开选择文件对话框。   
 
	if ( GetSaveFileName(&ofn) )  
	{  
		
        ofstream os;
        os.open(strcat(szFile,".diy"));

        for(auto x:vertices){
            os<<x.x<<' '<<x.y<<' '<<x.z<<endl;
        }

        os.close();




        
	} 

}

void DIYmodel::Draw(Camera c,Shader s){

}

GLuint DIYmodel::load_texture(string s){
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(s.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    DIYtexture basic(0,1,0,1,texture);
    textures.push_back(basic);
    return texture;

}

