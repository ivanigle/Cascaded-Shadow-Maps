/* ---------------------------------------------------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.h
Purpose: Transform functions for objects
Language: C++ Visual Studio 2015
Platform: Windows 7
Project: cs300_ivan.iglesias_1
Author: Ivan Iglesias, ivan.iglesias
Creation date: 5/21/2018
----------------------------------------------------------------------------------------------------------*/
#pragma once
#include <glm\glm.hpp>

class Transform
{
public:
	Transform()
	{
		Scale.x = 5.0f;
		Scale.y = 5.0f;
		Scale.z = 5.0f;
		Position.x = 0.0f;
		Position.y = 0.0f;
		Position.z = -20.0f;
		Rotate.x = 0.0f;
		Rotate.y = 0.0f;
		Rotate.z = 0.0f;
	}

	glm::vec3 Position;
	glm::vec3 Rotate;
	glm::vec3 Scale;

	

	bool goUp = true;
	glm::mat4 Model;

	void UpdateMatrices();


private:
};