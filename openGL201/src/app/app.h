#pragma once

#include <gl_core_4_4.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <gizmos/Gizmos.h>
#include 

#include <string>
#include <vector>
#include <iostream>

//include //Planets
//include // FlyCam

#include <tinyBuild/tiny_obj_loader.h>

#include <stb/stb_image.h>

using glm::mat4;
using glm::vec4;
using glm::vec2;

struct GLFWwindow;

enum ApplicationFail{
	NONE,
	GLFW_INIT,
	GLFW_CREATE_WINDOW,
	OGL_LOAD_FUNCATIONS,
	LOAD_MODEL,
	LOAD_TEXTURE				
};

struct Vertex{
	vec4 position;
	vec4 color;
	vec2 uv;
};

struct Time{
	const double TICK_PER_SEC_D_;
	Time(double value) : TICK_PER_SEC_D_(1.0 / value) {};
	float elapsed, current, previous, lag;
};

struct RenderObject{
	unsigned int VAO, VBO, IBO;
	unsigned int programID;
};

struct Image{
	int width, height, format;
	unsigned char* data;
};

struct GLInfo{
	unsigned int VAO, VBO, IBO, indexCount;
};

class App{
	//Application
	App();
	App(std::string name_cs);
	App(std::string name_cs, int width_i, int height_i);
	~App();
	
	
	//
	ApplicationFail Init();
	void Shutdown();
	bool Update();
	void Tick();
	void Draw();


	//Time
	Time time;

	//Window
	std::string name;
	int width, height;
	GLFWwindow* window;

	//Camera
	Camera* camera;
	mat4 projection;
	mat4 view;

	//
	bool CreateGLWindow();

public:
	
};
