/* ---------------------------------------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: light.h
Purpose: light information
Language: C++ Visual Studio 2017
Platform: Windows 10
Project: cs460_ivan.iglesias_1
Author: Ivan Iglesias, ivan.iglesias
Creation date: 9/26/2019
----------------------------------------------------------------------------------------------------------*/
#pragma once
#include <glm\glm.hpp>
#include "mesh.h"
#include "transform.h"
//enum TypeOfLight { Innactive, Positional, Directional, SpotLight };

class Light
{
public:

	glm::vec3 Color;
	glm::vec3 diffuse{ 1.0f };
	glm::vec3 specular{ 1.0f };
	float Linear{ 0.05f };
	float Quadratic{ 0.3f };
	float constant{ 1.f };
	Mesh light_mesh;
	Transform TransformData;

private:

};