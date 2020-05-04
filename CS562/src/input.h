#pragma once
#include <GLFW/glfw3.h>
#include "camera.h"

class Input
{
public:
	static Input& getInstance()
	{
		static Input    instance;							  
		return instance;
	}
	Input(Input const&) = delete;
	void operator=(Input const&) = delete;
	
	Camera * cam;

	void processInput(GLFWwindow *window_input);
	void mouse_buttom_callback(GLFWwindow* window_mouse, int button, int action, int mods);

	bool rotate{ false };
	bool firstMouse = true;	
	
	int SCR_WIDTH = 1280;
	int SCR_HEIGHT = 720;


private:
	Input();

	double mouse_xpos, mouse_ypos;
	vec3 mouse_pos;
	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;



	float lastX;
	float lastY;

};

static Input & g_input = Input::getInstance();



