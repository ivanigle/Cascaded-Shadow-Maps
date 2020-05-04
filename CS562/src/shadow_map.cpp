#include "shadow_map.h"
#include "renderer.h"
#include "Frustum.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>


void CSM::renderCSM(int cubeVAO)
{
	if (!g_render.free_camera)
		g_render.cam2 = g_render.cam;
	//Get the active cameras frustum parameters
	g_render.cam2.UpdateFrustum();
	//Start off by calculating the split distances
	float cascadeSplits[TOTAL_SPLITS] = {};

	computeCascadeSplits(cascadeSplits);

	for (unsigned splitNumber = 0; splitNumber < TOTAL_SPLITS; splitNumber++)
	{
		float prevSplitDistance;
		//Else  
		if (splitNumber == 0)
			prevSplitDistance = minDist;
		else
			prevSplitDistance = cascadeSplits[splitNumber - 1];

		float actualsplitDistance = cascadeSplits[splitNumber];

		glm::vec3 frustumPoints[8] =
		{
			glm::vec3(-1.0f,  1.0f, -1.0f),	glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f,  1.0f,  1.0f),	glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, -1.0f,  1.0f),
		};

		//Move the frustum coordinates to world space
		glm::mat4 invViewProj = glm::inverse(g_render.cam2.ProjectionMatrix() * g_render.cam2.GetViewMatrix());
		
		for (unsigned i = 0; i < 8; i++)
		{
			//Move to world
			glm::vec4 inversePoint = invViewProj * glm::vec4(frustumPoints[i], 1.0f);
			//Perspective division
			frustumPoints[i] = glm::vec3(inversePoint / inversePoint.w);
		}

		//Compute the real corners of the AABB
		for (unsigned i = 0; i < 4; i++)
		{
			glm::vec3 frustumVec = frustumPoints[i + 4] - frustumPoints[i];
			//move vectors to real coordinates
			glm::vec3 farVec = frustumVec * actualsplitDistance;
			glm::vec3 nearVec = frustumVec * prevSplitDistance;
			//Compute real points
			frustumPoints[i + 4] = frustumPoints[i] + farVec;
			frustumPoints[i] = frustumPoints[i] + nearVec;
		}

		//Obtain the Center of the frustum
		glm::vec3 frusCenter = getFrustumCenter(frustumPoints);

		//Obtain the farthest radius
		float radius = getRadiusFrustum(frustumPoints, frusCenter);

		//AABB
		glm::vec3 maxPoint = glm::vec3(radius);
		glm::vec3 minPoint = -maxPoint;

		//Position the viewmatrix looking down the center of the frustum with an arbitrary lighht direction	

		lightViewMatrix = getViewMatrix(g_render.light_dir, frusCenter, minPoint.z);

		//Create the orthographic matrix
		lightOrthoMatrix = getOrthoMatrix(minPoint, maxPoint);

		if (g_render.round_matrix)
		{
			//The rounding matrix that ensures that shadow edges do not shimmer			
			lightOrthoMatrix = getRoundMatrix(lightOrthoMatrix, lightViewMatrix);
		}

		//Store the matrix
		cascadedMatrices[splitNumber] = lightOrthoMatrix * lightViewMatrix;

		//Store the split distances
		float clipDist = g_render.cam2.mFrustum.mFar - g_render.cam2.mFrustum.mNear;
		shadowSplits[splitNumber] = (g_render.cam2.mFrustum.mNear + actualsplitDistance * clipDist) * -1.0f;

		g_render.shadow_map_shader.use();
		glBindFramebuffer(GL_FRAMEBUFFER, mCascadedShadowFBO);
		glViewport(0, 0, static_cast<GLsizei>(ShadowMapSize), static_cast<GLsizei>(ShadowMapSize));

		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mCascadedTextureArray, 0, splitNumber);

		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_CLAMP);
		glCullFace(GL_FRONT);

		g_render.shadow_map_shader.setUniform("lightViewProjectionMatrix", cascadedMatrices[splitNumber]);

		//render objects to store shadows
		renderToDepthTexture(cubeVAO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_CLAMP);
	}
}
unsigned CSM::getDepthID()
{
	return mCascadedTextureArray;
}
void CSM::computeCascadeSplits(float * planeSplits) const
{
	const float nearPlane = g_render.cam2.mFrustum.mNear;
	const float farPlane = g_render.cam2.mFrustum.mFar;

	const float distance = farPlane - nearPlane;

	const float startPos = nearPlane + minDist * distance;
	const float endPos = nearPlane + maxDist * distance;

	const float range = endPos - startPos;
	const float ratio = endPos / startPos;

	for (unsigned i = 0; i < TOTAL_SPLITS; i++)
	{
		const float p = (i + 1) / static_cast<float>(TOTAL_SPLITS);
		const float log = startPos * std::pow(ratio, p);
		const float uniform = startPos + range * p;
		const float d = lambda * (log - uniform) + uniform;
		planeSplits[i] = (d - nearPlane) / distance;
	}
}
glm::vec3 CSM::getFrustumCenter(glm::vec3 * frustumCornersWS) const
{
	//Obtain the Center of the frustum
	glm::vec3 center(0.0f);
	for (unsigned i = 0; i < 8; i++)
		center += frustumCornersWS[i];
	center = center / 8.0f;

	return center;
}
float CSM::getRadiusFrustum(glm::vec3 * frustumCornersWS, glm::vec3 center) const
{
	float radius = 0.0f;
	for (unsigned int i = 0; i < 8; ++i)
	{
		float distance = glm::length(frustumCornersWS[i] - center);
		radius = max(radius, distance);
	}
	radius = std::ceil(radius * 16.0f) / 16.0f;

	return radius;
}
glm::mat4 CSM::getViewMatrix(glm::vec3 lightDir, glm::vec3 center, float zPos) const
{
	glm::vec3 lightDirection = center - glm::normalize(g_render.light_dir) * -zPos;
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::lookAt(lightDirection, center, glm::vec3(0.0f, 1.0f, 0.0f));
	return mat;
}
glm::mat4 CSM::getOrthoMatrix(glm::vec3 minPoint, glm::vec3 maxPoint) const
{
	glm::vec3 cascadeExtents = maxPoint - minPoint;
	//Create the orthographic matrix
	glm::mat4 matOrtho = glm::ortho(minPoint.x, maxPoint.x, minPoint.y, maxPoint.y, 0.0f, cascadeExtents.z);

	return matOrtho;
}
glm::mat4 CSM::getRoundMatrix(glm::mat4 Ortho, glm::mat4 View) const
{	
	glm::mat4 shadowMat = Ortho * View;
	glm::vec4 shadowStartPos(0.0f, 0.0f, 0.0f, 1.0f);
	shadowStartPos = shadowMat * shadowStartPos;
	shadowStartPos = (shadowStartPos * ShadowMapSize) / 2.0f;

	glm::vec4 roundStart = glm::round(shadowStartPos);
	glm::vec4 offset = roundStart - shadowStartPos;
	offset = glm::vec4(glm::vec2(offset * 2.0f / ShadowMapSize)
			, 0.0f, 0.0f);

	Ortho[3] += offset;

	return Ortho;
}
void CSM::renderToDepthTexture(int cubeVAO) const
{
	for (unsigned i = 0; i < 3; i++)
	{
		g_render.shadow_map_shader.setUniform("model", g_render.ObjectsToRender[i]->TransformData.Model);

		for (unsigned j = 0; j < g_render.ObjectsToRender[i]->meshes.size(); j++)
			g_render.ObjectsToRender[i]->meshes[j].Draw_Picking(g_render.shadow_map_shader);

	}
	auto modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(350.0f, 10.0f, 350.0f));
	modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	g_render.shadow_map_shader.setUniform("model", modelMatrix);


	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}

CSM::CSM()
{

}
CSM::~CSM()
{
	glDeleteBuffers(1, &mCascadedShadowFBO);
	glDeleteTextures(1, &mCascadedTextureArray);
}
void CSM::initCSM(float size)
{
	ShadowMapSize = size;

	if (mCascadedShadowFBO)
	{
		glDeleteBuffers(1, &mCascadedShadowFBO);
		glDeleteTextures(1, &mCascadedTextureArray);
	}

	glGenFramebuffers(1, &mCascadedShadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mCascadedShadowFBO);

	glGenTextures(1, &mCascadedTextureArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mCascadedTextureArray);

	//Create depth textures
	glBindTexture(GL_TEXTURE_2D_ARRAY, mCascadedTextureArray);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, static_cast<GLsizei>(ShadowMapSize), static_cast<GLsizei>(ShadowMapSize), TOTAL_SPLITS, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mCascadedTextureArray, 0);

	//Not color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}