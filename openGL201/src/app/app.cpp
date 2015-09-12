#include "app.h"

App::App(): App(""){}

App::App(std::string name_cs) : App(name_cs, 1280, 720){}

App::App(std::string name_cs, int width_i, int height_i) : time(60.0){
	//Window
	name = name_cs;
	width = width_i;
	height = height_i;
	window = nullptr;
	//camera = nullptr;

	//View
	view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	//Time
	time.current = 0.0f;
	time.elapsed = 0.0f;
	time.lag = 0.0f;
	time.previous = 0.0f;

}

App::~App(){}

ApplicationFail App::Init(){
	//Init GLFW
	if(glfwInit() == false){
		return ApplicationFail::GLFW_INIT;
	}
	
	//Generate Window
	CreateGLWindow();

	//Load Gizmos
	Gizmos::create();

	//Camera

	//Load + Bind Texture File

	// Load Model File

	//Set Clear Screen

	//Texture Shader

	//Time
	return ApplicationFail::NONE;
}

void App::Shutdown(){
	//Gizmos::destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool App::Update(){
	if(glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		return false;
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
	
	//Time
	time.current = glfwGetTime();
	time.elapsed = time.current - time.previous;
	time.previous = time.current;
	time.lag += time.elapsed;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Gizmos::clear();
	Gizmos::addTransform(mat4(1));
	
	return true;
}

void App::Tick(){
	while(time.lag >= time.TICK_PER_SEC_D_){
		time.lag -= time.TICK_PER_SEC_D_;
	}
}

void App::Draw(){
	Gizmos::draw();
}

bool App::CreateGLWindow(){
	
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	return true;
}