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
	fbx = nullptr;

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

Image loadImageGL(const char *path);

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

	// Load Model File
	model = LoadFBX("./rsc/models/soulspear/soulspear.fbx");
	renderOBJ = CreateRenderObject(model);

	//Load + Bind Texture File
	LoadTexture();
	//BindTexture();

	//Set Clear Screen
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	//Texture Shader
	CreateShaderProgram();

	//Time


	return ApplicationFail::NONE;
}

void App::Shutdown(){
	Gizmos::destroy();
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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	RenderModel(renderOBJ);

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

Image loadImageGL(const char *path)
{
	Image imageInfo;
	imageInfo.width = 0;
	imageInfo.height = 0;
	imageInfo.format = 0;
	auto data = stbi_load(path, &imageInfo.width, &imageInfo.height, &imageInfo.format, STBI_default);

	glGenTextures(1, &imageInfo.handle);
	glBindTexture(GL_TEXTURE_2D, imageInfo.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageInfo.width, imageInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	return imageInfo;
}

void freeImageGL(Image &imageInfo)
{
	glDeleteTextures(1, &(imageInfo.handle));
	imageInfo = {0,0,0,0,0};
}

//Rename to load maps
bool App::LoadTexture(/*const char *path*/) {
	//Texture Load
	imageInfo.width = 0;
	imageInfo.height = 0;
	imageInfo.format = 0;
	imageInfo.data = stbi_load("./rsc/models/soulspear/soulspear_diffuse.tga", &imageInfo.width, &imageInfo.height, &imageInfo.format, STBI_default);
	if (imageInfo.data == nullptr) {
		return LOAD_TEXTURE; // god bless hotdogs
	}

	//Texture Bind
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageInfo.width, imageInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageInfo.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Free Data
	stbi_image_free(imageInfo.data);

	//Normal Load
	imageInfo.data = stbi_load("./rsc/models/soulspear/soulspear_normal.tga", &imageInfo.width, &imageInfo.height, &imageInfo.format, STBI_default);
	if (imageInfo.data == nullptr) {
		return LOAD_TEXTURE; // god bless hotdogs
	}

	//Normal Bind
	glGenTextures(1, &normalID);
	glBindTexture(GL_TEXTURE_2D, normalID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageInfo.width, imageInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageInfo.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Free Data
	stbi_image_free(imageInfo.data);

	//Specular Load
	imageInfo.data = stbi_load("./rsc/models/soulspear/soulspear_specular.tga", &imageInfo.width, &imageInfo.height, &imageInfo.format, STBI_default);
	if (imageInfo.data == nullptr) {
		return LOAD_TEXTURE; // god bless hotdogs
	}

	//Specular Bind
	glGenTextures(1, &specularID);
	glBindTexture(GL_TEXTURE_2D, specularID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageInfo.width, imageInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageInfo.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Free Data
	stbi_image_free(imageInfo.data);

	return ApplicationFail::NONE;
}
bool App::BindTexture() {


	//Normal Load

	return true;
}

//Based on lighting tutorial
void App::CreateOpenGLBuffers(FBXFile* fbx){
	//create data for each mesh (VAO/VBO/IBO)
	for (unsigned int i = 0; i < fbx->getMeshCount(); i++){
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		//storage for opengl data in unsigned int[3]
		unsigned int* glData = new unsigned int[3];

		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);

		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);

		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

		glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size() * sizeof(FBXVertex), mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(unsigned int), mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);

		glEnableVertexAttribArray(1); //normal
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;
	}
}
//Based on lighting tutorial
void App::CleanupOpenGLBuffers(FBXFile* fbx){
	//cleanup vertex data attached to each hotdog and mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); i++){
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);

		delete[] glData;
	}
}
//Based on lighting tutorial
void App::RenderModel(RenderObject render_object){
	glUseProgram(programID);

	//bind camera
	int loc = glGetUniformLocation(programID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera->CameraViewTransform));

	//Camera Position
	loc = glGetUniformLocation(programID, "CameraPos");
	glUniform3fv(loc, 1, glm::value_ptr(camera->position1()));
	
	//Set Texture Diffuse
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//Set texture to address
	loc = glGetUniformLocation(programID, "DiffuseMap");
	glUniform1i(loc, 0);
	

	//Set Texture Normal
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalID);
	loc = glGetUniformLocation(programID, "NormalMap");
	glUniform1i(loc, 1);

	//Set Texture Specular
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, specularID);
	loc = glGetUniformLocation(programID, "SpecularMap");
	glUniform1i(loc, 2);


	//Draw
	glBindVertexArray(render_object.VAO);
	glDrawElements(GL_TRIANGLES, render_object.size, GL_UNSIGNED_INT, 0);

}

Model App::LoadFBX(const char* path) {
	//Based on Esme
	FBXFile file;
	file.load(path, FBXFile::UNITS_METER, false, false, true);
	
	FBXMeshNode* mesh = file.getMeshByIndex(0);
	//mesh->m_userData = new RenderObject{vao,vbo,ibo,size};
	unsigned int vertSize, triSize;
	Vertex* verts = new Vertex[vertSize = mesh->m_vertices.size()];
	unsigned *tris = new unsigned[triSize = mesh->m_indices.size()];

	//Load vert data
	for (int i = 0; i < mesh->m_vertices.size(); i++) {
		verts[i] = { mesh->m_vertices[i].position, mesh->m_vertices[i].normal, mesh->m_vertices[i].tangent, mesh->m_vertices[i].texCoord1 };
	}

	//Load tri data
	for (int i = 0; i < mesh->m_indices.size(); i++) {
		tris[i] = mesh->m_indices[i];
	}

	file.unload();

	return{ verts, vertSize, tris, triSize };

}

RenderObject App::CreateRenderObject(const Model& model){
	unsigned vbo, ibo, vao, size = model.trisSize;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBufferData(GL_ARRAY_BUFFER, model.vertSize * sizeof(Vertex), &model.verts[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.trisSize * sizeof(unsigned int), &model.tris[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return{ vao,vbo,ibo,size };
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

void App::CreateCamera(){
	camera = new FlyCamera(0.01f);
	camera->SetPerspective(glm::pi<float>() * 0.1f, SIXTEEN_NINE, 0.1f, 1000.f);
	camera->SetLookAt(glm::vec3(7, 6, 7), glm::vec3(0,1,0), glm::vec3(0, 1, 0));
}
