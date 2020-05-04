/* ---------------------------------------------------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: Functions that define the shapes
Language: C++ Visual Studio 2015
Platform: Windows 7
Project: cs300_ivan.iglesias_1
Author: Ivan Iglesias, ivan.iglesias
Creation date: 5/21/2018
----------------------------------------------------------------------------------------------------------*/
#pragma once
#include <GL/gl3w.h>
#include <GL/GL.h>
//#include <SDL2/SDL.h>
#include <vector>
#include <glm\glm.hpp>
#include "GLSLProgram.hpp"


class Texture
{
public:
	unsigned int id;
	std::string type;
	std::string path;

};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct MatProperties
{
	//Material
	glm::vec3 K_Ambient, K_Diffuse, K_Specular;
	float NS;

};

class Mesh
{
public:
	//void CreateMesh();

	//For not applying shader
	bool NotShader = true;

	GLuint VertexData;
	GLuint vao;
	GLuint vaoNormals;

	bool DrawObjNorms = true;
	Mesh();
	void DeleteMesh();

	/*void CreateCube();
	void Increase();
	void BindVertPos();
	void CreateCylinder();
	void CreateCone();*/
	void SetBuffers();
	void CreateSphere(float radius = 0.5f);
	/*void ComputeFaceNormals();
	void ComputeVertexNormals();
	glm::vec3 ComputeCenterTrig(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);*/

	GLuint positionBufferObject;
	GLuint colorBufferObject;
	GLuint indexBufferObject;
	GLuint TexCordBufferObject;
	GLuint NormalsBufferObj;
	GLuint VertexNormalsBuffer;

	std::vector<glm::vec4> vertexPositions;
	std::vector<float> vertexColors;
	std::vector<float> vertexTexCoords;
	std::vector<glm::vec3> normals;
	//std::vector<short unsigned> vertexIndex;
	std::vector<glm::vec3> normalsFace;
	std::vector<glm::vec3> VertexNormals;

	//For assimp
	GLuint VAO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, MatProperties mat_props)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->mat_props = mat_props;
		setupMesh();
	}
	void setupMesh();

	void Draw(GLSLProgram & GLProg);
	void Draw_Picking(GLSLProgram & GLProg);


	//temporal
	glm::vec3 K_Ambient, K_Diffuse, K_Specular;
	float NS;
	MatProperties mat_props;
private:
	GLuint VBO, EBO;
};

