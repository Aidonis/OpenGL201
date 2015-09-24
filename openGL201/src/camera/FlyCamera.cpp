#include "FlyCamera.h"

#include <glm/ext.hpp>

FlyCamera::FlyCamera(const float speed) : speed(speed), mouse_old(0) {};

FlyCamera::~FlyCamera() {};

void FlyCamera::Update(){
	if(glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS){
		position.y += speed;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS) {
		position.y -= speed;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS) {
		position.x += speed;
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position.x -= speed;
	}

	SetLookAt(position, lookat, up);
}
