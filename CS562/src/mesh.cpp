/* ---------------------------------------------------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.cpp
Purpose: Functions that define the shapes
Language: C++ Visual Studio 2015
Platform: Windows 7
Project: cs300_ivan.iglesias_1
Author: Ivan Iglesias, ivan.iglesias
Creation date: 5/21/2018
----------------------------------------------------------------------------------------------------------*/
#include "Mesh.h"
#include "glm\gtc\constants.hpp"
#include <glm\glm.hpp>
#include "GLSLProgram.hpp"


/*void Mesh::CreateMesh()
{
	//Set vertices
	vertices = {
		{0.5f, 0.5f, 0.0f, 1.0f },
		{0.5f, -0.5f, 0.0f, 1.0f},
		{-0.5f, -0.5f, 0.0f, 1.0f},
		{-0.5f, 0.5f, 0.0f, 1.0f}
	};
	vertexColors = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f
	};
	vertexTexCoords = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};
	indices = {

		0,1,2,
		0,2,3
	};

	ComputeFaceNormals();
	ComputeVertexNormals();
	SetBuffers();


}*/
Mesh::Mesh()
{
	K_Ambient = glm::vec4(0.1, 0.1, 0.1, 0);
	K_Diffuse = glm::vec4(1, 1, 1, 0);
	K_Specular = glm::vec4(1, 1, 1, 0);
	NS = 100;
}
void Mesh::DeleteMesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}
/*
void Mesh::CreateCube()
{


	//Set vertices
	vertices = {
		{-0.5,-0.5,+0.5,1.0},
		{+0.5,-0.5,+0.5,1.0},
		{+0.5,+0.5,+0.5,1.0},
		{-0.5,+0.5,+0.5,1.0},
		{-0.5,-0.5,-0.5,1.0},
		{+0.5,-0.5,-0.5,1.0},
		{+0.5,+0.5,-0.5,1.0},
		{-0.5,+0.5,-0.5,1.0}
	};
	vertexColors = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f
	};
	vertexTexCoords = {
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
	};
	indices = {

		0,1,2,
		0,2,3,
		1,5,6,
		1,6,2,
		5,4,7,
		5,7,6,
		4,0,3,
		4,3,7,
		3,2,6,
		3,6,7,
		4,5,1,
		4,1,0
	};
	ComputeFaceNormals();
	ComputeVertexNormals();
	SetBuffers();
}

void Mesh::Increase()
{
//	vertices[0] += 5;
}

void Mesh::BindVertPos()
{
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferSubData (GL_ARRAY_BUFFER,0, vertices.size() * sizeof(glm::vec4), vertices.data());
}

void Mesh::CreateCylinder()
{
	vertices.push_back({ 0.0f, 0.0f, -0.5f, 1.0f });

	auto NumberOfPoints = 100;

	for (int i = 0; i <= NumberOfPoints; i++)
	{
		float angle = glm::two_pi<float>()*((float)i / (float)NumberOfPoints);
		float s = sin(angle) / 2;
		float c = cos(angle) / 2;

		vertices.push_back({ c,s,-0.5f,1.0f });
	}

	for (int i = 1; i <= NumberOfPoints; i++)
	{
		indices.push_back(i + 1);
		indices.push_back(i);
		indices.push_back(0);

	}

	vertexTexCoords.push_back(0.5);
	vertexTexCoords.push_back(0.5);

	for (int i = 0; i <= NumberOfPoints; i++)
	{
		vertexTexCoords.push_back(0.5f* std::cos(glm::two_pi<float>()*((float)i / (float)NumberOfPoints)) + 0.5f);
		vertexTexCoords.push_back(0.5f*std::sin(glm::two_pi<float>()*((float)i / (float)NumberOfPoints))+ 0.5f);
	}
	//Second Circle
	vertices.push_back({ 0.0f, 0.0f, 0.5f, 1.0f });

	auto InitPos = NumberOfPoints + 2;

	for (int i = 0; i <= NumberOfPoints; i++)
	{
		float angle = glm::two_pi<float>()*((float)i / (float)NumberOfPoints);
		float s = sin(angle) / 2;
		float c = cos(angle) / 2;

		vertices.push_back({ c,s,0.5,1.0f });
	}
	auto end = NumberOfPoints * 2 + 2;
	for (int i = InitPos + 1; i <= end; i++)
	{
		indices.push_back(InitPos);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	vertexTexCoords.push_back(0.5);
	vertexTexCoords.push_back(0.5);

	for (int i = 0; i <= NumberOfPoints; i++)
	{
		vertexTexCoords.push_back(0.5f* std::cos(glm::two_pi<float>()*((float)i / (float)NumberOfPoints)) + 0.5f);
		vertexTexCoords.push_back(0.5f*std::sin(glm::two_pi<float>()*((float)i / (float)NumberOfPoints)) + 0.5f);
	}

	//Circle in the middle
	auto offset = NumberOfPoints * 3;
	unsigned ending = offset - 3;
	unsigned i;
	unsigned second;
	unsigned second2;
	for (i = 1; i < ending; i += 3)
	{
		second = offset + i;
		second2 = offset + i + 3;
		indices.push_back(indices[i]);
		indices.push_back(indices[second2]);
		indices.push_back(indices[second]);
		indices.push_back(indices[i + 3]);
		indices.push_back(indices[second2]);
		indices.push_back(indices[i]);
	}
	//Last rectangle
	second = offset + i;
	indices.push_back(indices[i]);
	indices.push_back(indices[offset + 1]);
	indices.push_back(indices[second]);
	indices.push_back(indices[1]);
	indices.push_back(indices[offset + 1]);
	indices.push_back(indices[i]);


	ComputeFaceNormals();
	ComputeVertexNormals();
	SetBuffers();



}

void Mesh::CreateCone()
{
	vertices.push_back({ 0.0f, 0.0f, -0.5f, 1.0f });

	auto NumberOfPoints = 100;

	for (int i = 0; i <= NumberOfPoints; i++)
	{
		float angle = glm::two_pi<float>()*((float)i / (float)NumberOfPoints);
		float s = sin(angle) / 2;
		float c = cos(angle) / 2;

		vertices.push_back({ c,s,-0.5f,1.0f });
	}

	for (int i = 1; i <= NumberOfPoints; i++)
	{
		indices.push_back(i + 1);
		indices.push_back(i);
		indices.push_back(0);
	}

	vertexTexCoords.push_back(0.5);
	vertexTexCoords.push_back(0.5);

	for (int i = 0; i <= NumberOfPoints; i++)
	{
		vertexTexCoords.push_back(0.5f* std::cos(glm::two_pi<float>()*((float)i / (float)NumberOfPoints)) + 0.5f);
		vertexTexCoords.push_back(0.5f*std::sin(glm::two_pi<float>()*((float)i / (float)NumberOfPoints)) + 0.5f);
	}

	vertices.push_back({ 0.0f,0.0f,0.5f,1.0f });

	vertexTexCoords.push_back(0.5);
	vertexTexCoords.push_back(0.5);

	auto InitPos = NumberOfPoints + 2;

	for (int i = 1; i <= NumberOfPoints; i++)
	{
		indices.push_back(InitPos);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	ComputeFaceNormals();
	ComputeVertexNormals();
	SetBuffers();



}*/

void Mesh::SetBuffers()
{// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Mesh::CreateSphere(float radius)
{

	int rings = 50;
	int slices = 50;


	float R = 1.0f / (float)(rings - 1);
	float S = 1.0f / (float)(slices - 1);
	int r, s;

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < slices; s++)
		{
			float x = cos(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
			float y = sin(-(glm::pi<float>() / (float)2) + glm::pi<float>() * r * R);
			float z = sin(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

			/*vertexTexCoords.push_back(S*s);
			vertexTexCoords.push_back(R*r);*/
			Vertex new_vert;
			new_vert.Position = glm::vec3(x * radius, y * radius, z * radius);
			vertices.push_back(new_vert);

			if (r < rings - 1)
			{
				int curRow = r * slices;
				int newxtRow = (r + 1) * slices;
				int nextS = (s + 1) % slices;

				indices.push_back(curRow + s);
				indices.push_back(newxtRow + s);
				indices.push_back(newxtRow + nextS);

				indices.push_back(curRow + s);
				indices.push_back(newxtRow + nextS);
				indices.push_back(curRow + nextS);
			}
		}

	}
	/*ComputeFaceNormals();
	ComputeVertexNormals();*/
	setupMesh();

}

/*void Mesh::ComputeFaceNormals()
{
	for (unsigned i = 0; i < indices.size(); i += 3)
	{
		glm::vec3 v1(vertices[indices[i]]);
		glm::vec3 v2(vertices[indices[i + 1]]);
		glm::vec3 v3(vertices[indices[i + 2]]);

		auto center = ComputeCenterTrig(v1, v2, v3);

		normals.push_back({ center.x,center.y,center.z });
		auto a = v2 - v1;
		auto b = v3 - v1;
		auto cross = glm::cross(a, b);
		cross = glm::normalize(cross);

		normalsFace.push_back({ cross.x,cross.y,cross.z });


		normals.push_back({ center.x + cross.x * 0.1,center.y + cross.y * 0.1,center.z + cross.z * 0.1 });
	}
}

void Mesh::ComputeVertexNormals()
{
	VertexNormals.resize(vertices.size());
	unsigned j = 0;
	for (unsigned i = 0; i < indices.size(); i += 3, j++)
	{
		VertexNormals[indices[i]] += normalsFace[j];
		VertexNormals[indices[i + 1]] += normalsFace[j];
		VertexNormals[indices[i + 2]] += normalsFace[j];
	}

	for (unsigned i = 0; i < VertexNormals.size(); i++)
	{
		VertexNormals[i] = glm::normalize(VertexNormals[i]);

		normals.push_back({ vertices[i] });
		VertexNormals[i] *= 0.1;
		normals.push_back({ glm::vec3(vertices[i]) + VertexNormals[i] });
	}
}

glm::vec3 Mesh::ComputeCenterTrig(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	auto result = (v1 + v2 + v3);
	result /= 3;
	return result;
}*/

void Mesh::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);

}

void Mesh::Draw(GLSLProgram & GLProg)
{

	/*for (int i = 0; i < textures.size(); i++)
	{

		//active texture
		glActiveTexture(GL_TEXTURE0);
		string name = textures[0].type;

		GLProg.setUniform(name.c_str(), 0);
		// bind texture
		glBindTexture(GL_TEXTURE_2D, textures[0].id);
	}*/

	//material			
	/*GLProg.setUniform("mat.ambient", mat_props.K_Ambient);
	GLProg.setUniform("mat.diffuse", mat_props.K_Diffuse);
	GLProg.setUniform("mat.specular", mat_props.K_Specular);
	GLProg.setUniform("mat.shininess", mat_props.NS);*/

	

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);

}

void Mesh::Draw_Picking(GLSLProgram & GLProg)
{
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}





