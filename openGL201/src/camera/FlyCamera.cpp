#include "FlyCamera.h"

#include <glm/ext.hpp>

FlyCamera::FlyCamera(const float speed) : speed(speed), mouse_old(0) {};

FlyCamera::~FlyCamera() {};

void FlyCamera::Update(){
	position.z += speed;
	position.x += speed;

	SetLookAt(position, lookat, up);
}
