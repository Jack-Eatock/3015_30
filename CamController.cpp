#include"CamController.h"
#include<iostream>
#include <glm/ext/matrix_transform.hpp>

void CamController::Inputs(glm::vec3 camMovement)
{
	if (camMovement.z > 0) 
	{
		position += speed * Up;
	}
	else if (camMovement.z < 0)
	{
		position -= speed * Up;
	}

	if (camMovement.y > 0) 
	{
		position += speed * Orientation;
	}
	else if (camMovement.y < 0) 
	{
		position += speed * -Orientation;
	}

	if (camMovement.x > 0)
	{
		position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	else if (camMovement.x < 0) 
	{
		position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	
	//std::cout << cameraMovement.x << std::endl;

}