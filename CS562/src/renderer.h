/* ---------------------------------------------------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: GraphManager.h
Purpose: Functions for the Graphics Manager
Language: C++ Visual Studio 2015
Platform: Windows 7
Project: cs300_ivan.iglesias_1
Author: Ivan Iglesias, ivan.iglesias
Creation date: 5/21/2018
----------------------------------------------------------------------------------------------------------*/
#pragma once
#include "GLSLProgram.hpp"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "shadow_map.h"
#include <GLFW/glfw3.h>



class Render
{

public:

	static Render& getInstance()
	{
		static Render    instance; // Guaranteed to be destroyed.
								   // Instantiated on first use.
		return instance;
	}
	Render(Render const&) = delete;
	void operator=(Render const&) = delete;


	void RenderWindow();
	Render();

	//variables
	GLSLProgram geometry_program;	
	GLSLProgram global_light_program;
	GLSLProgram shadow_map_shader;
	GLSLProgram picking_program;

	GLFWwindow * window;
	//deffer
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	
	//temporal lights
	int Total_Lights{ 10 };	

	std::vector<Model*> ObjectsToRender;
	std::vector<Light> Lights;

	Camera cam;
	Camera cam2;
	bool free_camera{ false };

	void initializeScene();
	GLuint VBO, VAO;
	GLuint mQuadVAO, mQuadVBO;

	int render_buffer_temp{ -1 };
	//ImGui values
	int selected_light{ -1 };
	int selected_object{ -1 };
	glm::vec3 ambient_global{ 0.5f };
	//screen values 
	int SCR_WIDTH = 1280;
	int SCR_HEIGHT = 720;

	glm::mat4 Viewport;
	glm::mat4 Projection;
	
	void renderScene();
	void depthQuads();

	//skybox
	unsigned int cubeVAO, cubeVBO;

	CSM mCSMTechnique;
	bool render_zones{ false };
	glm::vec3 light_dir{ -0.1f, -0.5f, 0.0f };
	float angleBias{ 0.006f };
	int SHADOW_MAP_RES{ 2048 };
	unsigned num_split_planes{ 4 };
	bool round_matrix { true };

private:

	void MousePicking();
	

};



static Render & g_render = Render::getInstance();

