#include "headers/bezierface.h"
using namespace std;
#define step 50

float defControlPoints[] = {
   
	3.0, 3.0,
    3.0, 2.0,
    3.0, 1.0,
    3.0, 0.0,
	0.0, 3.0,
    1.0, 3.0,
    2.0, 3.0,
    3.0, 3.0,
	0.0, 0.0,
    1.0, 0.0,
    2.0, 0.0,
    3.0, 0.0,
    	
};


void BezierFace::generate(int prec)
{
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	//初始化空白数组
	for (int i = 0; i < numVertices; i++)
	{
		vertices.push_back(glm::vec3());
		normals.push_back(glm::vec3());
		texCoords.push_back(glm::vec2());
	}
	for (int i = 0; i < numIndices; i++)
	{
		indices.push_back(0);
	}
	
	for (int i = 0; i <= prec; i++)//i:从上到下
	{

		for(int j=0; j<=prec; j++){ //旋转一周
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float r = 0.0f;
			
			float u = (float)i / prec;
			float v = (float)j / prec;
			float theta=toRadians(v);

			for (int k = 0;  k<= 3; k++)
			{
					int index = k;
					r += controlPointsVector[index].x * Bernstein(u, k);
					y += controlPointsVector[index].y * Bernstein(u, k);
				
			}
			x=r*cos(theta);
			z=r*sin(theta);

			
		
			vertices[i*(prec+1)+j] = glm::vec3(x, y, z);
			normals[i*(prec+1)+j] = glm::vec3(x, y, z);
			texCoords[i*(prec+1)+j] = glm::vec2(u,v);
		}
	}
	//计算索引
	for (int i = 0; i < prec; i++) {
		for (int j = 0; j < prec; j++) {
			int k=6*(i * prec + j) ;
			indices[k + 0] = i * (prec + 1) + j;
			indices[k + 1] = i * (prec + 1) + j + 1;
			indices[k + 2] = (i + 1) * (prec + 1) + j;
			
			indices[k + 3] = i * (prec + 1) + j + 1;
			indices[k + 4] = (i + 1) * (prec + 1) + j + 1;
			indices[k + 5] = (i + 1) * (prec + 1) + j;
			// for(int q=0;q<6;q++)
			// cout<<indices[k + q]<<"x,y,z:"<<vertices[indices[k + q]].x
			// <<' '<<vertices[indices[k + q]].y<<' '
			// <<vertices[indices[k + q]].z<<' '<<endl;
			
		}
	}
}
float BezierFace::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f); }

float BezierFace::Bernstein(float t, int index)
{
	switch (index)
	{
	default:
	case 0:
		return pow(1.0 - t, 3);
		break;
	case 1:
		return 3 * t * pow(1.0 - t, 2);
		break;
	case 2:
		return 3 * pow(t, 2) * (1 - t);
		break;
	case 3:
		return pow(t, 3);
		break;
	}
	
}
BezierFace::BezierFace(){

}

// BezierFace::BezierFace(int i)
// {
// 	this->step = 3;
// 	this->controlPoints = defControlPoints +8*i;
// 	generate(100);
// }

BezierFace::BezierFace( vector<glm::vec2> vec){

	controlPointsVector=vec;
	generate(step);
}

int BezierFace::getNumVertices()
{
	return numVertices;
}

int BezierFace::getNumIndices()
{
	return numIndices;
}

vector<glm::vec3> BezierFace::getVertices()
{
	return vertices;
}

vector<glm::vec3> BezierFace::getNormals()
{
	return normals;
}

vector<glm::vec2> BezierFace::getTexCoords()
{
	return texCoords;
}

vector<int> BezierFace::getIndices()
{
	return indices;
}

