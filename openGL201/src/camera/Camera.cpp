#include "Camera.h"

void Camera::SetAspectRatio(CameraAspectRatio aspect){
	switch (aspect) {
	case CameraAspectRatio::FOUR_THREE:
		aspectRatio_width = 4.f;
		aspectRatio_height = 3.f;
		break;
	case CameraAspectRatio::SIXTEEN_NINE:
		aspectRatio_width = 16.f;
		aspectRatio_height = 9.f;
		break;
	case CameraAspectRatio::SIXTEEN_TEN:
		aspectRatio_width = 16.f;
		aspectRatio_height = 10.f;
		break;
	}
}

Camera::Camera(){
	world_transform = glm::mat4(1);
	view_transform = glm::mat4(1);
	camera_transform = glm::mat4(1);
	camera_view_transform = glm::mat4(1);
	
	position = glm::vec3(0);
	lookat = glm::vec3(0);
	up = glm::vec3(0);

	aspectRatio_width = 16.f;
	aspectRatio_height = 9.f;
}

Camera::~Camera() {}

void Camera::SetPerspective(const float set_fov, CameraAspectRatio set_aspect_ratio, const float set_near, const float set_far){
	SetAspectRatio(set_aspect_ratio);
	camera_transform = glm::perspective(set_fov, aspectRatio_width / aspectRatio_height, set_near, set_far);
}

void Camera::SetOrthographic(CameraAspectRatio set_aspect_ratio, const float set_near, const float set_far){
	SetAspectRatio(set_aspect_ratio);
	camera_transform = glm::ortho(aspectRatio_width / -2.f, aspectRatio_height / 2.f, set_near / 2.f, set_far);
}

void Camera::SetLookAt(const glm::vec3 set_from, const glm::vec3 set_to, const glm::vec3 set_up){
	position = set_from;
	lookat = set_to;
	up = set_up;
	view_transform = glm::lookAt(position, lookat, up);
	world_transform = glm::inverse(view_transform);
}

void Camera::SetPosition(const glm::vec3 set_position){
	position = set_position;
	view_transform = glm::lookAt(position, lookat, up);
	world_transform = glm::inverse(view_transform);
}

void Camera::UpdateProjectionViewTransform(){
	camera_view_transform = camera_transform * view_transform;
}