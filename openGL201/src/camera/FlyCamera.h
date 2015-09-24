#pragma once
#include <glm/glm.hpp>
#include <glfw/glfw3.h>
#include "Camera.h"


class FlyCamera : public Camera {
	float speed;
	glm::vec2 mouse_old;

public:
	FlyCamera(const float speed);
	~FlyCamera();

	virtual void Update() override;
};