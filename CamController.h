#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>



class CamController
{
public:

	float sensitivity = 100.0f;
	float speed = .15f;
	glm::vec3 position = glm::vec3(-24.0f, 10.0f, 18.0f);
	glm::vec3 Orientation = glm::vec3(0.9f, -0.3f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	void Inputs(glm::vec3 camMovement, glm::vec2 mouseMovement);
};
#endif