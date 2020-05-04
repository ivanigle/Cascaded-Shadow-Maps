#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <fstream>
#include <string>
#include <iostream>
#include "GLSLProgram.hpp"
#include <vector>
#include "renderer.h"
#include "input.h"
#include "globals.h"
#include "model.h"
#include <sstream>
#include <nlohmann/json.hpp>
#include <imgui/ImGuizmo.h>


using json = nlohmann::json;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_buttom_callback(GLFWwindow* window, int button, int action, int mods);

void setWindowFPS(GLFWwindow* win);



GLFWwindow* window;

std::string m_win_name;
ImGuiWindowFlags m_flags;
/**
* @brief 	function to init imgui
* @return	if failed or succes
*/
bool InitImGui()
{
	//call imgui functions
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
		return false;
	
	g_render.window = window;


	const char * glsl_version = "#version 130";

	if (!ImGui_ImplOpenGL3_Init(glsl_version))
		return false;

	m_win_name = "Editor Render";
	//NOt moving windons!!!
	//m_flags = ImGuiWindowFlags_NoMove;

	return true;
}
/**
* @brief 	function to render imgui
*/

void EditTransform(const float *cameraView, float *cameraProjection, float* matrix)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	if (ImGui::IsKeyPressed(90))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(69))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(82)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation, 3);
	ImGui::InputFloat3("Rt", matrixRotation, 3);
	ImGui::InputFloat3("Sc", matrixScale, 3);


	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}
	/*if (ImGui::IsKeyPressed(83))
		useSnap = !useSnap;*/
	ImGui::Checkbox("", &useSnap);
	ImGui::SameLine();

	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}
	ImGui::Checkbox("Bound Sizing", &boundSizing);
	if (boundSizing)
	{
		ImGui::PushID(3);
		ImGui::Checkbox("", &boundSizingSnap);
		ImGui::SameLine();
		ImGui::InputFloat3("Snap", boundsSnap);
		ImGui::PopID();
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	//ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

	//Set values to object
	/*g_render.ObjectsToRender[g_render.selected_object]->TransformData.Position = vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
	g_render.ObjectsToRender[g_render.selected_object]->TransformData.Rotate = vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
	g_render.ObjectsToRender[g_render.selected_object]->TransformData.Scale = vec3(matrixScale[0], matrixScale[1], matrixScale[2]);*/

}
void render_imgui()
{
	ImGui::Begin(m_win_name.c_str(), nullptr, m_flags);
	
	ImGui::DragFloat("lambda", &g_render.mCSMTechnique.lambda, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("minDistance", &g_render.mCSMTechnique.minDist, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("maxDistance", &g_render.mCSMTechnique.maxDist, 0.01f, 0.0f, 1.0f);


	ImGui::Checkbox("FreeCamera", &g_render.free_camera);
	ImGui::Checkbox("RenderZones", &g_render.render_zones);
	ImGui::DragFloat3("light_dir", &g_render.light_dir.x, 0.001f);
	ImGui::Checkbox("Round Matrix", &g_render.round_matrix);

	ImGui::DragFloat("ShadowAngleBias", &g_render.angleBias, 0.0001f, 0.0f, 1.0f);

	int res_temp = g_render.SHADOW_MAP_RES;

	ImGui::InputInt("ShadowsMapResolution", &res_temp);

	if (res_temp != g_render.SHADOW_MAP_RES)
	{
		g_render.SHADOW_MAP_RES = res_temp;
		g_render.mCSMTechnique.initCSM(static_cast<float>(res_temp));
	}

	//ImGui::ListBox("Central Object", &g_render.selected_light, listbox_items.c_str(), listbox_items.size(), listbox_items.size());
	if (g_render.selected_object != -1 || g_render.selected_light != -1)
	{

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::BeginFrame();

		if (g_render.selected_object != -1)
			EditTransform(&g_render.Viewport[0].x, &g_render.Projection[0].x, &g_render.ObjectsToRender[g_render.selected_object]->TransformData.Model[0].x);
		else
		{
			EditTransform(&g_render.Viewport[0].x, &g_render.Projection[0].x, &g_render.Lights[g_render.selected_light].TransformData.Model[0].x);
			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(&g_render.Lights[g_render.selected_light].TransformData.Model[0].x, matrixTranslation, matrixRotation, matrixScale);
			g_render.Lights[g_render.selected_light].TransformData.Position = vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
		}
		/*ImGui::Separator();

		ImGui::DragFloat3("Position", &g_render.ObjectsToRender[g_render.selected_object]->TransformData.Position.x, 0.01f);
		ImGui::DragFloat3("Rotate", &g_render.ObjectsToRender[g_render.selected_object]->TransformData.Rotate.x, 1.0f, 0.0001f, 1000.f);
		ImGui::DragFloat3("Scale", &g_render.ObjectsToRender[g_render.selected_object]->TransformData.Scale.x, 0.01f, 0.0001f, 1000.f);*/



	}

	ImGui::End();
}
/**
* @brief 	function to update imgui
*/
void update_imgui()
{
	//call imgui funcions on update
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	render_imgui();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
/**
* @brief 	function to shutdown imgui
*/
void shutdown_imgui()
{
	//shutdown imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
/**
* @brief 	function to create window
*/
void WindowCreation()
{
	// glfw window creation
	window = glfwCreateWindow(g_render.SCR_WIDTH, g_render.SCR_HEIGHT, "Framework", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_buttom_callback);

}
/**
* @brief 	function to init glfw
*/
void InitGlfw()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	WindowCreation();
}
/**
* @brief 	function to load opengl
*/
void LoadOpenGL()
{
	// glad: load all OpenGL function pointers	
	if (gl3wInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
}
/**
* @brief 	function to compile shaders
*/
void BuildAndCompileShaders()
{
	glEnable(GL_DEPTH_TEST);

	g_render.geometry_program.compileShaderFromFile("shaders/FragmentShader.frag", GLSLShader::GLSLShaderType::FRAGMENT);
	g_render.geometry_program.compileShaderFromFile("shaders/VertexShader.vert", GLSLShader::GLSLShaderType::VERTEX);
	g_render.geometry_program.link();	

	g_render.global_light_program.compileShaderFromFile("shaders/debug_quads.fs", GLSLShader::GLSLShaderType::FRAGMENT);
	g_render.global_light_program.compileShaderFromFile("shaders/debug_quads.vs", GLSLShader::GLSLShaderType::VERTEX);
	g_render.global_light_program.link();

	g_render.shadow_map_shader.compileShaderFromFile("shaders/shadow_map.frag", GLSLShader::GLSLShaderType::FRAGMENT);
	g_render.shadow_map_shader.compileShaderFromFile("shaders/shadow_map.vert", GLSLShader::GLSLShaderType::VERTEX);
	g_render.shadow_map_shader.link();

	g_render.picking_program.compileShaderFromFile("shaders/picking_fragment.frag", GLSLShader::GLSLShaderType::FRAGMENT);
	g_render.picking_program.compileShaderFromFile("shaders/picking_vertex.vert", GLSLShader::GLSLShaderType::VERTEX);
	g_render.picking_program.link();

	

}

std::string path("models/sponza/meshes/sponza.obj");

/**
* @brief 	function to load all the meshes
*/
void load_all_meshes()
{
	Model * nano = new Model(path.c_str());

	//quad.MeshData.CreateCube();
	g_render.ObjectsToRender.push_back(nano);
	//g_render.GenerateTexture();
}

void read_Json()
{
	std::ifstream ifs("scenes/scene.json");

	json j = json::parse(ifs);

	json object = j["objects"];

	// iterate the array
	for (json::iterator it = object.begin(); it != object.end(); ++it)
	{
		//new object
		std::string mesh_j = (*it)["mesh"];
		json translation = (*it)["translation"];
		json rotate = (*it)["rotate"];
		json scale = (*it)["scale"];
		Model * obj = new Model(mesh_j.c_str());
		obj->TransformData.Position = glm::vec3(translation["x"], translation["y"], translation["z"]);
		obj->TransformData.Scale = glm::vec3(scale["x"], scale["y"], scale["z"]);
		obj->TransformData.Rotate = glm::radians(glm::vec3(rotate["x"], rotate["y"], rotate["z"]));

		obj->TransformData.UpdateMatrices();
		g_render.ObjectsToRender.push_back(obj);
	}

}
void CheckWindowSize()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if ((width != g_render.SCR_WIDTH || g_render.SCR_HEIGHT != height) && (width > 0 && height > 0) )
	{
		g_input.SCR_HEIGHT = g_render.SCR_HEIGHT = height;
		g_input.SCR_WIDTH = g_render.SCR_WIDTH = width;
	}
}

int main()
{
	//cal init and load functions
	InitGlfw();
	LoadOpenGL();
	InitImGui();
	BuildAndCompileShaders();
	//set initial camera
	g_input.cam = &g_render.cam;

	read_Json();

	g_render.initializeScene();
	//load_all_meshes();
	//Read JsonObjects
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		//Check size window
		CheckWindowSize();
		// input
		g_input.processInput(window);	
		//render scene
		g_render.RenderWindow();
		

		update_imgui();
		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
		setWindowFPS(window);
	}
	return 0;
}

/**
* @brief 	function to change glfw window size
* @param	window		pointer to window
*/
void framebuffer_size_callback(GLFWwindow* window_framebuffer, int width, int height)
{
	//change viewport
	glViewport(0, 0, width, height);
}

void mouse_buttom_callback(GLFWwindow* window_mouse, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		g_input.rotate = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		g_input.firstMouse = true;
		g_input.rotate = false;
	}

}

int nbFrames = 0;
int lastTime = 0;
void setWindowFPS(GLFWwindow* win)
{
	// Measure speed
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	nbFrames++;
	if (delta >= 1.0) { 
		double fps = double(nbFrames) / delta;

		std::stringstream ss;
		ss << " [" << fps << " FPS]";

		glfwSetWindowTitle(win, ss.str().c_str());

		nbFrames = 0;
		lastTime = static_cast<int>(currentTime);
	}
}
