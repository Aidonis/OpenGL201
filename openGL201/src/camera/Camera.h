#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

enum CameraAspectRatio {
	SIXTEEN_NINE,
	SIXTEEN_TEN,
	FOUR_THREE
};

class Camera{
protected:
	glm::mat4 world_transform;
	glm::mat4 view_transform;
	glm::mat4 camera_transform;
	glm::mat4 camera_view_transform;

	glm::vec3 position, lookat, up;
	
	float aspectRatio_width, aspectRatio_height;
	
	void SetAspectRatio(CameraAspectRatio aspect);

public:
	Camera();
	virtual ~Camera();

	virtual void Update() = 0;

	void SetPerspective(const float set_fov, CameraAspectRatio set_aspect_ratio, const float set_near, const float set_far);
	void SetOrthographic(CameraAspectRatio set_aspect_ratio, const float set_near, const float set_far);
	void SetLookAt(const glm::vec3 set_from, const glm::vec3 set_to, const glm::vec3 set_up);
	void SetPosition(const glm::vec3 set_position);

	void UpdateProjectionViewTransform();


	glm::mat4 world_transform1() const{
		return world_transform;
	}
	__declspec(property(get = world_transform1)) glm::mat4 WorldTransform;

	glm::mat4 view_transform1() const{
		return view_transform;
	}
	__declspec(property(get = view_transform1)) glm::mat4 ViewTransform;

	glm::mat4 camera_transform1() const{
		return camera_transform;
	}
	__declspec(property(get = camera_transform1)) glm::mat4 CameraTransform;

	glm::mat4 camera_view_transform1() const{
		return camera_view_transform;
	}
	__declspec(property(get = camera_view_transform1)) glm::mat4 CameraViewTransform;
};