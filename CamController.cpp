#include"CamController.h"
#include<iostream>
#include <glm/ext/matrix_transform.hpp>

void CamController::Inputs(glm::vec3 camMovement, glm::vec2 mouseMovement)
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
	

	// Calculate the movement from the centre of the screen. Uses it to calculate the rotation to apply from sensitivity.
	float rotX = sensitivity * mouseMovement.x;
	float rotY = sensitivity * mouseMovement.y;
	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

	// Locks the rotations
	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		Orientation = newOrientation;

	// Rotates horizontally
	Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);


}