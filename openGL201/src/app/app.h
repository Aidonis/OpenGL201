#pragma once

#include <gl_core_4_4.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <gizmos/Gizmos.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "../camera/Camera.h"
#include "../camera/FlyCamera.h"
#include "../planets/Planet.h"/

#include <tinyBuild/tiny_obj_loader.h>
#include <stb/stb_image.h>
#include <fbx/FBXFile.h>

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
	vec4 normal;
	vec4 Tangent;
	vec2 uv;
};

struct Model{
	Vertex* verts;
	unsigned vertSize;
	unsigned* tris;
	unsigned trisSize;
};

struct Time{
	const double TICK_PER_SEC_D_;
	Time(double value) : TICK_PER_SEC_D_(1.0 / value) {};
	float elapsed, current, previous, lag;
};

struct RenderObject{
	unsigned int VAO, VBO, IBO, size;
};

struct FrameBufferObject{
	unsigned int FBO, textureID, depth;
};

struct Image{
	int width, height, format;
	unsigned handle;
	unsigned char* data;
};

struct GLInfo{
	unsigned int VAO, VBO, IBO, indexCount;
};


class App{
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

	//FBX
	FBXFile* fbx;
	Model model;
	RenderObject renderOBJ;
	FrameBufferObject frameOBJ;

	//
	unsigned int m_program;
	unsigned int programID;
	unsigned int textureID;
	unsigned int normalID;
	unsigned int specularID;

	//Loading FBX
	void CreateOpenGLBuffers(FBXFile* fbx);
	void CleanupOpenGLBuffers(FBXFile* fbx);
	void RenderModel(RenderObject render_object);

	//
	Model LoadFBX(const char* path);
	RenderObject CreateRenderObject(const Model& model);
	
	//
	void CreateFrameBuffer();
	void DrawPlane();

	//Temp
	FBXTexture* model_texture;

	//
	bool CreateGLWindow();
	bool LoadTexture();

	unsigned int CreateShader(GLenum shader_type_GLenum, const char* shader_file_str);
	unsigned int CreateProgram(const char* vertex_str, const char* fragment_str);
	void CreateShaderProgram();

	bool BindTexture();
	void CreateCamera();

public:
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

	Image imageInfo;
};
