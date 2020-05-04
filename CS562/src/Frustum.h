#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "glm\vec3.hpp"

//Primarily used in Cascaded Shadow Mapping to calculate frustums for every view partition
struct Frustum
{
	float mNear;
	float mFar;
	float fov;
	//TEMPPP
	float ratio= 1280.0f / 720.0f;
	
	glm::vec3 frustumCorners[8];
};

#endif