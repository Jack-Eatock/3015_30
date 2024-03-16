#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>

class CamController
{
public:

	float speed = .15f;
	glm::vec3 position = glm::vec3(6.0f, 7.0f, 7.5f);
	glm::vec3 Orientation = glm::vec3(-0.6f, -0.5f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	void Inputs(glm::vec3 camMovement);
};
#endif