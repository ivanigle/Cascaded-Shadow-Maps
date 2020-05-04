#include "input.h"
#include <iostream>
//#include "renderer.h"
Input::Input()
{
	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

	//cam = g_render.cam;
}
/**
* @brief 	function to process the input from glfw window
* @param	window		pointer to window
*/
void Input::processInput(GLFWwindow *window_input)
{
	//frame time
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//esc input
	if (glfwGetKey(window_input, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window_input, true);

	//Cam input 
	if (glfwGetKey(window_input, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (glfwGetKey(window_input, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cam->ProcessKeyboard(FORWARD, deltaTime * 5);
		else
			cam->ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window_input, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (glfwGetKey(window_input, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cam->ProcessKeyboard(BACKWARD, deltaTime * 5);
		else
			cam->ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window_input, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (glfwGetKey(window_input, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cam->ProcessKeyboard(LEFT, deltaTime * 5);
		else
			cam->ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window_input, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (glfwGetKey(window_input, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cam->ProcessKeyboard(RIGHT, deltaTime * 5);
		else
			cam->ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(window_input, GLFW_KEY_Q) == GLFW_PRESS)
	{		
		cam->Position.y += 0.5f;
	}

	if (glfwGetKey(window_input, GLFW_KEY_E) == GLFW_PRESS)
	{
		cam->Position.y -= 0.5f;
	}

	//delete object
/*	if (glfwGetKey(window_input, GLFW_KEY_DELETE) == GLFW_PRESS && object_num != -1)
	{
		my_octree.delete_object(objects_render[object_num]);
		delete objects_render[object_num];
		objects_render.erase(objects_render.begin() + object_num);
		object_num = -1;
		prev = -1;
		selecting = -1;
	}
	*/
	//compute mouse position in world
	glfwGetCursorPos(window_input, &mouse_xpos, &mouse_ypos);
	auto pos_x_view = mouse_xpos / SCR_WIDTH;
	auto pos_y_view = mouse_ypos / SCR_HEIGHT;
	//NDC
	auto ndc_x = (pos_x_view * 2) - 1;
	auto ndc_y = (pos_y_view * 2) - 1;
	//std::cout << "=================================" << std::endl;
	//std::cout << "ndc_x x" << ndc_x << std::endl;
	//std::cout << "ndc_x y" << ndc_y << std::endl;
	//Viewport
	vec4 ndc_vec = { ndc_x , -ndc_y, 1, 1 };
	auto inv_view = glm::inverse(cam->ProjectionMatrix() * cam->GetViewMatrix());
	vec4 world_pos = inv_view * ndc_vec;
	//perspective division
	world_pos /= world_pos.w;

	//std::cout << "=================================" << std::endl;
	//std::cout << "world x" << world_pos.x << std::endl;
	//std::cout << "world y" << world_pos.y << std::endl;
	//std::cout << "world z" << world_pos.z << std::endl;
	/*mouse_pos = { world_pos.x, world_pos.y, world_pos.z };

	//compute ray between mouse point and camara center
	ray cam_point{ cam->Position, glm::normalize(mouse_pos - cam->Position)};

	//compute intersection between all objects and ray

	unsigned i = 0;
	float min_inter = FLT_MAX;
	int pos = -1;
	for (; i < objects_render.size(); i++)
	{
		//Intersection found

	//	auto result_aabb = aabb_from_aabb_and_transform(objects_render[i].bounding_aabb, objects_render[i].TransformData.Model);
		auto inter = intersection_ray_aabb(cam_point, objects_render[i]->transform_aabb);
		if (inter > 0.0)
		{
			if (inter < min_inter)
			{
				min_inter = inter;
				pos = i;
			}
		//	objects_render[i].TransformData.Position = mouse_pos;
		//	break;
		}
	}

	if (pos != -1)//intersection found
	{
		selecting = pos;
		if (prev!= -1 && prev != pos && prev != object_num)
		{
			objects_render[prev]->color_aabb = default_aabb_color;
		}
		prev = pos;
		objects_render[pos]->color_aabb = {0,1,0,1};
	}
	else
	{
		if (prev != -1 && prev != object_num)//change color to original
		{
			objects_render[prev]->color_aabb = default_aabb_color;
			prev = -1;
			selecting = -1;
		}
	}
	*/
	if (rotate)
	{
		double xpos, ypos;

		glfwGetCursorPos(window_input, &xpos, &ypos);

		if (firstMouse)
		{
			lastX = (float)xpos;
			lastY = (float)ypos;
			firstMouse = false;
		}

		float xoffset = static_cast<float>(xpos - lastX);
		float yoffset = static_cast<float>(lastY - ypos); // reversed since y-coordinates go from bottom to top

		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);

		cam->ProcessMouseMovement(xoffset, yoffset);
		
	}


}


void Input::mouse_buttom_callback(GLFWwindow* window_mouse, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		rotate = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
		rotate = false;
	}

	/*if (render.imgui_hovered)
		return;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (render.selecting != -1 && render.selecting != render.object_num)
		{
			if(render.object_num != -1)
				render.objects_render[render.object_num]->color_aabb = render.default_aabb_color;
			render.object_num = render.selecting;
			render.objects_render[render.object_num]->color_aabb = { 0,0,1,1 };
		}
		else
		{

			if (render.object_num != -1)
				render.objects_render[render.object_num]->color_aabb = render.default_aabb_color;
			render.object_num = -1;
		}

	}*/


}