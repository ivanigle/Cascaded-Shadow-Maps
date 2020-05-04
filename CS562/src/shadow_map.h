#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

#define TOTAL_SPLITS 4

class CSM
{
public:
	CSM();
	~CSM();
	
	void initCSM(float size);	
	void renderCSM(int cubeVAO);
	unsigned getDepthID();

	unsigned mCascadedTextureArray;
	float shadowSplits[TOTAL_SPLITS];
	glm::mat4 cascadedMatrices[TOTAL_SPLITS];
	
	float lambda = 0.5f;
	float minDist = 0.0f;
	float maxDist = 1.0f;

private:

	void computeCascadeSplits(float * cascadeSplits) const;
	glm::vec3 getFrustumCenter(glm::vec3 * frustumCornersWS) const;
	float getRadiusFrustum(glm::vec3 * frustumCornersWS, glm::vec3 center) const;
	glm::mat4 getViewMatrix(glm::vec3 lightDir, glm::vec3 center, float zPos) const;
	glm::mat4 getOrthoMatrix(glm::vec3 minPoint, glm::vec3 maxPoint) const;
	glm::mat4 getRoundMatrix(glm::mat4 Ortho, glm::mat4 View) const;
	void renderToDepthTexture(int cubeVAO) const;

	float ShadowMapSize;

	unsigned mCascadedShadowFBO;

	glm::mat4 lightViewMatrix;
	glm::mat4 lightOrthoMatrix;

};

