#include "app.h"

App::App(): App(""){}

App::App(std::string name_cs) : App(name_cs, 1280, 720){}

App::App(std::string name_cs, int width_i, int height_i) : time(60.0){
	//Window
	name = name_cs;
	width = width_i;
	height = height_i;
	window = nullptr;
	camera = nullptr;

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

	//
	if(ogl_LoadFunctions() == ogl_LOAD_FAILED){
		glfwDestroyWindow(window);
		glfwTerminate();
		return ApplicationFail::OGL_LOAD_FUNCATIONS;
	}

	//Load Gizmos
	Gizmos::create();

	//Camera
	CreateCamera();

	//Load + Bind Texture File
	LoadTexture();
	BindTexture();

	// Load Model File
	LoadFBX("./rsc/models/cube.fbx");

	//Set Clear Screen
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	//Texture Shader
	CreateShaderProgram();

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

	

	
	return true;
}

void App::Tick(){
	while(time.lag >= time.TICK_PER_SEC_D_){
		time.lag -= time.TICK_PER_SEC_D_;
	}
}

void App::Draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Gizmos::clear();
	Gizmos::addTransform(mat4(1));

	//Standard grid draw
	for (int i = 0; i < 21; i++) {
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
			glm::vec3(-10 + i, 0, -10),
			i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 0, 1));
		Gizmos::addLine(glm::vec3(10, 0, -10 + i),
			glm::vec3(-10, 0, -10 + i),
			i == 10 ? glm::vec4(1, 1, 1, 1) : glm::vec4(0, 0, 0, 1));
	}

	//Camera Draw
	camera->UpdateProjectionViewTransform();
	Gizmos::draw(camera->camera_view_transform1());
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

bool App::LoadTexture() {
	imageInfo.width = 0;
	imageInfo.height = 0;
	imageInfo.format = 0;
	imageInfo.data = stbi_load("./rsc/textures/crate.png", &imageInfo.width, &imageInfo.height, &imageInfo.format, STBI_default);
	if (imageInfo.data == nullptr) {
		return LOAD_TEXTURE;
	}
	return ApplicationFail::NONE;
}

void App::LoadFBX(const char* path){
	//TODO
	FBXFile file;
	file.load(path, FBXFile::UNITS_METER, false, false, false);
	std::cout << file.getPath() << std::endl;
}

unsigned int App::CreateShader(GLenum shader_type_GLenum, const char* shader_file_str){
	std::string shaderCode;
	//open shader file
	std::ifstream shaderStream(shader_file_str);

	if(shaderStream.is_open()){
		std::string line = "";
		while(std::getline(shaderStream, line)){
			shaderCode += "\n" + line;
		}
		shaderStream.close();
	}

	//convert to cstring
	char const* shaderSourcePointer = shaderCode.c_str();

	//create shader ID
	unsigned int shader = glCreateShader(shader_type_GLenum);

	//load source code
	glShaderSource(shader, 1, &shaderSourcePointer, NULL);

	//compile shader
	glCompileShader(shader);

	//error check
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		int infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

		const char* shaderType = NULL;
		switch(shader_type_GLenum){
		case GL_VERTEX_SHADER:
			shaderType = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "fragment";
			break;
		}
		fprintf(stderr, "Compile failure in %s shader:\n%s\n", shaderType, infoLog);
		delete[] infoLog;
	}
	return shader;
}

unsigned App::CreateProgram(const char* vertex_str, const char* fragment_str){
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, vertex_str));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, fragment_str));

	//create shader program ID
	GLuint program = glCreateProgram();

	//attach shaders
	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glAttachShader(program, *shader);
	}

	//link program
	glLinkProgram(program);

	//check for link errors and output them
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);

		fprintf(stderr, "Linker failure: %s\n", infoLog);
		delete[] infoLog;
	}

	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glDetachShader(program, *shader);
		glDeleteShader(*shader);
	}
	return program;
}

void App::CreateShaderProgram(){
	programID = CreateProgram("./rsc/shaders/vertexShader.glsl", "./rsc/shaders/FragmentShader.glsl");
}

bool App::BindTexture(){
	glGenTextures(1, &textureID);
	if(textureID == NULL){
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageInfo.width, imageInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageInfo.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	stbi_image_free(imageInfo.data);

	return true;
}

void App::CreateCamera(){
	camera = new FlyCamera(0.01f);
	camera->SetPerspective(glm::pi<float>() * 0.25f, SIXTEEN_NINE, 0.1f, 1000.f);
	camera->SetLookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
}
