/* ---------------------------------------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: renderer.cpp
Purpose: Functions for the Graphics Manager
Language: C++ Visual Studio 2017
Platform: Windows 10
Project: cs460_ivan.iglesias_1
Author: Ivan Iglesias, ivan.iglesias
Creation date: 9/26/2019
----------------------------------------------------------------------------------------------------------*/
#include "renderer.h"
#include "glm\gtc\constants.hpp"
#include <iostream>
#include "glm/gtx/transform.hpp"
#include <imgui/imgui.h>
#include <imgui/ImGuizmo.h>
#include <stb_image/stb_image.h>
#include <glm/gtc/matrix_inverse.hpp>

#define TOTAL_SPLITS 4

//Hard Coded Cube
float cube[] = {

	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};



//function to the control mouse picking
void Render::MousePicking()
{
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !ImGui::IsMouseHoveringAnyWindow() && !ImGuizmo::IsOver() && !ImGui::GetIO().WantCaptureMouse)
	{
		// Clear the screen in white
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		picking_program.use();

		glEnableVertexAttribArray(0);

		for (unsigned i = 0; i < ObjectsToRender.size(); i++)
		{
			//ObjectsToRender[i]->TransformData.UpdateMatrices();
			mat4 MVP = Projection * Viewport * ObjectsToRender[i]->TransformData.Model;
			picking_program.setUniform("MVP", MVP);

			vec4 color;
			//convert the integer position into a color
			color.r = ((i & 0x000000FF) >> 0) / 255.0f;
			color.g = ((i & 0x0000FF00) >> 8) / 255.0f;
			color.b = ((i & 0x00FF0000) >> 16) / 255.0f;
			color.a = 1.0f;

			picking_program.setUniform("PickingColor", color);

			for (unsigned j = 0; j < ObjectsToRender[i]->meshes.size(); j++)
				ObjectsToRender[i]->meshes[j].Draw_Picking(picking_program);
		}

		for (unsigned i = 0; i < Lights.size(); i++)
		{
			//ObjectsToRender[i]->TransformData.UpdateMatrices();
			mat4 MVP = Projection * Viewport * Lights[i].TransformData.Model;
			picking_program.setUniform("MVP", MVP);

			vec4 color;
			int j = i + static_cast<int>(ObjectsToRender.size());
			//convert the integer position into a color
			color.r = ((j & 0x000000FF) >> 0) / 255.0f;
			color.g = ((j & 0x0000FF00) >> 8) / 255.0f;
			color.b = ((j & 0x00FF0000) >> 16) / 255.0f;
			color.a = 1.0f;

			picking_program.setUniform("PickingColor", color);

			Lights[i].light_mesh.Draw_Picking(picking_program);
		}

		glDisableVertexAttribArray(0);

		glFlush();
		glFinish();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);

		unsigned char data[4];

		glReadPixels(static_cast<int>(xpos), SCR_HEIGHT - static_cast<int>(ypos), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//convert back to id 

		int pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256;

		if (pickedID != 0x00ffffff)
		{
			if (pickedID > (ObjectsToRender.size() - 1))
			{
				selected_light = pickedID - static_cast<int>(ObjectsToRender.size());
				selected_object = -1;
			}
			else
			{
				selected_object = pickedID;
				selected_light = -1;
			}
		}
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
void Render::RenderWindow()
{
	GLenum error = glGetError();


	Viewport = cam.GetViewMatrix();
	Projection = cam.ProjectionMatrix();


	MousePicking();

	mCSMTechnique.renderCSM(VAO);

	glCullFace(GL_BACK);
	//Reset viewport
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	
	geometry_program.use();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//Render the floor
	renderScene();
	glDisable(GL_DEPTH_TEST);
	glUseProgram(0);

	depthQuads();	

	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);
}

Render::Render()
{
	

}

void Render::initializeScene()
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	//Quad coordinates
	float quadVert[] = {		
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	
	glGenVertexArrays(1, &mQuadVAO);
	glGenBuffers(1, &mQuadVBO);
	glBindVertexArray(mQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), &quadVert, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	mCSMTechnique.initCSM(2048);
	

}

void Render::renderScene()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mCSMTechnique.getDepthID());	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	
	glm::mat4 inverseView = glm::inverse(cam2.GetViewMatrix());

	geometry_program.setUniform("inverseViewMatrix", inverseView);


	geometry_program.setUniform("cascadedSplits", glm::vec4(mCSMTechnique.shadowSplits[0], mCSMTechnique.shadowSplits[1], mCSMTechnique.shadowSplits[2], mCSMTechnique.shadowSplits[3]));
	geometry_program.setUniform("numOfCascades", TOTAL_SPLITS);	

	//Store matrices per split
	for (unsigned i = 0; i < TOTAL_SPLITS; ++i)
	{
		auto text_name = "viewProjecMatrices[" + std::to_string(i) + "]";
		geometry_program.setUniform(text_name.c_str(), mCSMTechnique.cascadedMatrices[i]);	}


	geometry_program.setUniform("depthTextures", 0);
	geometry_program.setUniform("angleBias", angleBias);
	
	
	geometry_program.setUniform("lightDirection", light_dir);

	
	for (unsigned i = 0; i < 3; i++)
	{
		//Colors for shadow maps
		if (i  == 0)
		{			
			geometry_program.setUniform("diffuseMaterial", glm::vec3(1.0f, 0.0f, 0.0f));
		}else if(i == 1)
		{
			geometry_program.setUniform("diffuseMaterial", glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (i == 2)
		{
			geometry_program.setUniform("diffuseMaterial", glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else
		{
			geometry_program.setUniform("diffuseMaterial", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		//Compute Normal
		glm::mat3 normalMatrix = glm::mat3(cam.GetViewMatrix()*ObjectsToRender[i]->TransformData.Model);
		normalMatrix = glm::inverseTranspose(normalMatrix); 

		//One model per camera
		glm::mat4 modelViewMatrix = cam.GetViewMatrix() * ObjectsToRender[i]->TransformData.Model;
		glm::mat4 modelViewMatrix2 = cam2.GetViewMatrix() * ObjectsToRender[i]->TransformData.Model;

		
		glm::mat4 inverseProjection = glm::inverse(cam.ProjectionMatrix());

		//Send the matrices
		geometry_program.setUniform("projectionMatrix", cam.ProjectionMatrix());
		geometry_program.setUniform("normalMatrix", normalMatrix);
		geometry_program.setUniform("modelViewMatrix", modelViewMatrix);
		geometry_program.setUniform("modelViewMatrix2", modelViewMatrix2);

		
		geometry_program.setUniform("render_zones", render_zones);

		

		for (unsigned j = 0; j < ObjectsToRender[i]->meshes.size(); j++)
			ObjectsToRender[i]->meshes[j].Draw(geometry_program);

	}

	auto modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(350.0f, 10.0f, 350.0f));
	modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	auto normalMatrix = glm::mat3(cam.GetViewMatrix()*modelMatrix);
	normalMatrix = glm::inverseTranspose(normalMatrix);

	auto modelViewMatrix = cam.GetViewMatrix() * modelMatrix;
	auto modelViewMatrix2 = cam2.GetViewMatrix() * modelMatrix;

	
	geometry_program.setUniform("normalMatrix", normalMatrix);
	geometry_program.setUniform("modelViewMatrix", modelViewMatrix);
	geometry_program.setUniform("modelViewMatrix2", modelViewMatrix2);

	geometry_program.setUniform("diffuseMaterial", glm::vec3(1.0f, 1.0f, 1.0f));


	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	
}

void Render::depthQuads()
{
	global_light_program.use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);

	global_light_program.setUniform("arrayTex", 0);
	global_light_program.setUniform("layer", 0);	

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.f, 3.f, 1.0f));
	global_light_program.setUniform("modelMatrix", modelMatrix);

	glActiveTexture(GL_TEXTURE1);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mCSMTechnique.getDepthID());
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	global_light_program.setUniform("layer", 1);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f, 1.f, 1.0f));

	global_light_program.setUniform("modelMatrix", modelMatrix);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mCSMTechnique.getDepthID());
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	global_light_program.setUniform("layer", 2);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f, -1.f, 1.0f));
	
	global_light_program.setUniform("modelMatrix", modelMatrix);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mCSMTechnique.getDepthID());
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	global_light_program.setUniform("layer", 3);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.f, -3.f, 1.0f));
	global_light_program.setUniform("modelMatrix", modelMatrix);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mCSMTechnique.getDepthID());
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glUseProgram(0);
}