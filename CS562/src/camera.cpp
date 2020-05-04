/**
* @file		camera.cpp
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
#include "camera.h"
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer.h"

//Function to create camara
/*Camera::Camera()
{
	phi = 0.0;
	theta = 0.0f;
	length = 20.0f;
	AngleMove = 0.1f;
}*/
//Function to return the viewport matrix
glm::mat4 Camera::ViewportMatrix()
{
	ViewVec = vec3(0.0, 0.0, -20.0) - Position;

	/*glm::vec3 right = glm::cross(ViewVec, glm::vec3(0.0f, 1.0f, 0.0f));
	UpVec = glm::cross(right, ViewVec);*/
		
	mat4 view = glm::lookAt(Position, Position + ViewVec, UpVec);
	//return perspect * view;
	return view;
}
//Camara projection matrix
glm::mat4 Camera::ProjectionMatrix()
{
	return glm::perspective(fov, (float)g_render.SCR_WIDTH / (float)g_render.SCR_HEIGHT, near_plane, far_plane);
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	//calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction_cam, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction_cam == FORWARD)
		Position += Front * velocity;
	if (direction_cam == BACKWARD)
		Position -= Front * velocity;
	if (direction_cam == LEFT)
		Position -= Right * velocity;
	if (direction_cam == RIGHT)
		Position += Right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;


	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::UpdateFrustum()
{
	
	float nearHeight = 2 * tan(mFrustum.fov / 2) * mFrustum.mNear;
	float nearWidth = nearHeight * mFrustum.ratio;

	float farHeight = 2 * tan(mFrustum.fov / 2) * mFrustum.mFar;
	float farWidth = farHeight * mFrustum.ratio;

	glm::vec3 farC = Position + Front * mFrustum.mFar;
	glm::vec3 nearC = Position + Front * mFrustum.mNear;

	mFrustum.frustumCorners[0] = nearC + (Up * nearHeight / 2.0f) - (Right * nearWidth / 2.0f);
	mFrustum.frustumCorners[1] = nearC + (Up * nearHeight / 2.0f) + (Right * nearWidth / 2.0f);
	mFrustum.frustumCorners[3] = nearC - (Up * nearHeight / 2.0f) - (Right * nearWidth / 2.0f);
	mFrustum.frustumCorners[2] = nearC - (Up * nearHeight / 2.0f) + (Right * nearWidth / 2.0f);

	mFrustum.frustumCorners[4] = farC + (Up * farHeight / 2.0f) - (Right * farWidth / 2.0f);  
	mFrustum.frustumCorners[5] = farC + (Up * farHeight / 2.0f) + (Right * farWidth / 2.0f);  
	mFrustum.frustumCorners[7] = farC - (Up * farHeight / 2.0f) - (Right * farWidth / 2.0f);  
	mFrustum.frustumCorners[6] = farC - (Up * farHeight / 2.0f) + (Right * farWidth / 2.0f);  

	
	
}