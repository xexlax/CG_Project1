#pragma once
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
using namespace std;
//贝塞尔旋转曲面类
class BezierFace
{
	//顶点数
	int numVertices;
	//索引数(每个四边形片6个顶点)
	int numIndices;

	//uv 分段数
	
	//attributes
	vector<glm::vec3> vertices;
	vector<glm::vec2> texCoords;
	vector<glm::vec3> normals;
	
	vector<int> indices;
	//生成
	void generate(int prec);
	//控制点
	float* controlPoints;
	vector<glm::vec3> controlPointsVector;
	//曲线阶数(控制点个数)
	int step;
	float toRadians(float degrees);
	float Bernstein(float u, int index);
public:
	BezierFace();
	BezierFace(int step, float controlPoints[]);
	int getNumVertices();
	int getNumIndices();
	vector<glm::vec3> getVertices();
	vector<glm::vec3> getNormals();
	vector<glm::vec2> getTexCoords();
	vector<int> getIndices();

};
