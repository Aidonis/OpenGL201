#pragma once

#include <gizmos/Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Planet
{
public:
	//Constructors
	Planet();
	~Planet();
	Planet(float a_radius, int a_quality);

	//Vars
	float radius;
	int rows, cols;
	glm::vec4 color;
	glm::mat4 transform;
	glm::mat4 position;
	

	glm::mat4 rotation_local;
	glm::mat4 rotation_global;

	//
	void Update();
	void Draw();

	//
	Planet* parent;

	//Set Values
	void setQuality(int a_quality);
	void setRadius(float a_radius);
	void setColor(glm::vec4 a_color);
	void setTransform(glm::mat4& a_transform);
	void setPosition(glm::vec3 a_center);

	void setGlobalRotation(float degrees, glm::vec3 set_rotation);
	void setLocalRotation(float degrees, glm::vec3 set_rotation);

	private:
		glm::mat4 CalculateGlobalTransform() const;
		glm::mat4 CalculateLocalTransform() const;

};