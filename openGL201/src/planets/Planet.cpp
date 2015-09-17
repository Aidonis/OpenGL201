#include "Planet.h"


Planet::Planet() : Planet(2.f, 15){}

Planet::~Planet()
{
}

Planet::Planet(float a_radius, int a_quality)
{
	radius = a_radius;
	rows = a_quality;
	cols = a_quality;
	color = glm::vec4(0);
	position = glm::mat4(1);
	transform = glm::mat4(1);

	parent = nullptr;

}

void Planet::Update()
{
	transform = CalculateGlobalTransform() * CalculateLocalTransform();
}

void Planet::Draw()
{
	Gizmos::addSphere(glm::vec3(0), radius, rows, cols, color, &transform);
}

void Planet::setQuality(int a_quality){
	radius = a_quality;
	rows = a_quality;
}

void Planet::setRadius(float a_radius)
{
	radius = a_radius;
}

void Planet::setColor(glm::vec4 a_color)
{
	color = a_color;
}

void Planet::setTransform(glm::mat4& a_transform)
{
	transform = a_transform;
}

void Planet::setPosition(glm::vec3 a_center)
{
	position = glm::translate(a_center);
}

void Planet::setGlobalRotation(float degrees, glm::vec3 set_rotation){
	rotation_global = glm::rotate(glm::radians(degrees), set_rotation);
}

void Planet::setLocalRotation(float degrees, glm::vec3 set_rotation){
	rotation_local = glm::rotate(glm::radians(degrees), set_rotation);
}

glm::mat4 Planet::CalculateGlobalTransform() const{
	glm::mat4 matrix = glm::mat4(1);
	if(parent != nullptr){
		matrix = parent->CalculateGlobalTransform();
	}

	return matrix * rotation_global * position;
}

glm::mat4 Planet::CalculateLocalTransform() const{
	return rotation_local;
}
