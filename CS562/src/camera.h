/**
* @file		camera.h
* @date 	2/11/2019
* @author	Ivan Iglesias
* @par		Login: ivan.iglesias
* @par		Course: CS350
* @par		Assignment #2
* @brief 	Implementation of the camera
*
* Hours spent on this assignment: 20h
*
*/
#pragma once
#include <glm\glm.hpp>
#include <glm/gtc/constants.hpp>
#include "Frustum.h"
using namespace glm;
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	//Camera();
	mat4 ViewportMatrix();
	mat4 ProjectionMatrix();
	vec3 Position = { 0,0,0 };
	vec3 ViewVec = {0,0,-20};
	vec3 UpVec = { 0,1,0 };
	
	// Camera Attributes
//	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	float far_plane{ 750.f };
	float near_plane{ 1.0f };

	float fov;


	Frustum mFrustum;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		fov = 45.0f;
		mFrustum.mNear = near_plane;
		mFrustum.mFar = far_plane;
		mFrustum.fov = fov;

		updateCameraVectors();

	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		fov = 45.0f;
		mFrustum.mNear = near_plane;
		mFrustum.mFar = far_plane;
		mFrustum.fov = fov;
		updateCameraVectors();
	}

	glm::mat4  InitTranslationTransform(float x, float y, float z)
	{
		glm::mat4 m;
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
		return m;
	}

	glm::mat4 InitCameraTransform(const glm::vec3& Target, const glm::vec3& Up)
	{
		glm::vec3 N = Target;
		N = glm::normalize(N);
		glm::vec3 U = Up;
		U = glm::cross(U,N);
		U = glm::normalize(U);
		glm::vec3 V = glm::cross(N,U);
		glm::mat4 m;

		m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
		m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
		m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
		m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;

		return m;
	}


	const glm::mat4 GetViewTrans()
	{
		glm::mat4 CameraTranslationTrans, CameraRotateTrans;

		CameraTranslationTrans = InitTranslationTransform(-Position.x, -Position.y, -Position.z);
		CameraRotateTrans = InitCameraTransform(Front, Up);

		glm::mat4 m_Vtransformation = CameraTranslationTrans * CameraRotateTrans;

		return m_Vtransformation;
	}
	void UpdateFrustum();

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	// Processes input received from a mouse 
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

private:
	glm::vec3 direction;	

	// Calculates the front vector from the Camera
	void updateCameraVectors();


};