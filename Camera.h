#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include <vector>

#include "Game.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum CollisionDirections {
	NORTH,
	SOUTH,
	EAST,
	WEST,
	TOP,
	BOTTOM,
	NONE
};

// Default camera values
const float YAW         =  0.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 TempPosition;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float fov;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), fov(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), fov(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
	
	glm::mat4 GetMirroredViewMatrix()
	{
		Yaw += 180.0f;
		Pitch += 180.0f;
		ProcessMouseMovement(0, 0, false);
		return glm::lookAt(Position, Position + Front, Up);
		Yaw -= 180.0f;
		Pitch -= 180.0f;
		ProcessMouseMovement(0, 0, true);
	}

	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, CollisionDirections collision, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			if (Position.y < 0.5 && Front.y < 0.0)
			{
				switch (collision) {
					case NORTH:
						if (Front.x < 0.0) {
							Position.z += Front.z * velocity;
						}
						break;
					case SOUTH:
						if (Front.x > 0.0) {
							Position.z += Front.z * velocity;
						}
						break;
					case EAST:
						if (Front.z < 0.0) {
							Position.x += Front.x * velocity;
						}
						break;
					case WEST:
						if (Front.z > 0.0) {
							Position.x += Front.x * velocity;
						}
						break;
					default:
						Position.x += Front.x * velocity;			//alternatively TempPosition for other sol
						Position.z += Front.z * velocity;
						break;
				}
			}
			else
			{
				switch (collision) {
					case NORTH:
						if (Front.x < 0.0) {
							Position.y += Front.y * velocity;
							Position.z += Front.z * velocity;
						}
						break;
					case SOUTH:
						if (Front.x > 0.0) {
							Position.y += Front.y * velocity;
							Position.z += Front.z * velocity;
						}
						break;
					case EAST:
						if (Front.z < 0.0) {
							Position.y += Front.y * velocity;
							Position.x += Front.x * velocity;
						}
						break;
					case WEST:
						if (Front.z > 0.0) {
							Position.y += Front.y * velocity;
							Position.x += Front.x * velocity;
						}
						break;
					case TOP:
						if (Front.y < 0.0) {
							Position.x += Front.x * velocity;
							Position.z += Front.z * velocity;
						}
						break;
					case BOTTOM:
						if (Front.y > 0.0) {
							Position.x += Front.x * velocity;
							Position.z += Front.z * velocity;
						}
						break;
					default:
						Position += Front * velocity;
						break;
				}
			}
		}		
		if (direction == BACKWARD)
		{
			if (Position.y < 0.5 && Front.y > 0.0)
			{
				switch (collision) {
					case NORTH:
						if (Front.x > 0.0) {
							Position.z -= Front.z * velocity;
						}
						break;
					case SOUTH:
						if (Front.x < 0.0) {
							Position.z -= Front.z * velocity;
						}
						break;
					case EAST:
						if (Front.z > 0.0) {
							Position.x -= Front.x * velocity;
						}
						break;
					case WEST:
						if (Front.z < 0.0) {
							Position.x -= Front.x * velocity;
						}
						break;
					default:
						Position.x -= Front.x * velocity;			//alternatively TempPosition for other sol
						Position.z -= Front.z * velocity;
						break;
				}
			}
			else
			{
				switch (collision) {
					case NORTH:
						if (Front.x > 0.0) {
							Position.y -= Front.y * velocity;
							Position.z -= Front.z * velocity;
						}
						break;
					case SOUTH:
						if (Front.x < 0.0) {
							Position.y -= Front.y * velocity;
							Position.z -= Front.z * velocity;
						}
						break;
					case EAST:
						if (Front.z > 0.0) {
							Position.y -= Front.y * velocity;
							Position.x -= Front.x * velocity;
						}
						break;
					case WEST:
						if (Front.z < 0.0) {
							Position.y -= Front.y * velocity;
							Position.x -= Front.x * velocity;
						}
						break;
					case TOP:
						if (Front.y > 0.0) {
							Position.x -= Front.x * velocity;
							Position.z -= Front.z * velocity;
						}
						break;
					case BOTTOM:
						if (Front.y < 0.0) {
							Position.x -= Front.x * velocity;
							Position.z -= Front.z * velocity;
						}
						break;
					default:
						Position -= Front * velocity;
						break;
				}
			}
		}
		if (direction == LEFT)
			switch (collision) {
				case NORTH:
					if (Right.x > 0.0) {
						Position.y -= Right.y * velocity;
						Position.z -= Right.z * velocity;
					}
					break;
				case SOUTH:
					if (Right.x < 0.0) {
						Position.y -= Right.y * velocity;
						Position.z -= Right.z * velocity;
					}
					break;
				case EAST:
					if (Right.z > 0.0) {
						Position.y -= Right.y * velocity;
						Position.x -= Right.x * velocity;
					}
					break;
				case WEST:
					if (Right.z < 0.0) {
						Position.y -= Right.y * velocity;
						Position.x -= Right.x * velocity;
					}
					break;
				case TOP:
					if (Right.y > 0.0) {
						Position.x -= Right.x * velocity;
						Position.z -= Right.z * velocity;
					}
					break;
				case BOTTOM:
					if (Right.y < 0.0) {
						Position.x -= Right.x * velocity;
						Position.z -= Right.z * velocity;
					}
					break;
				default:
					Position -= Right * velocity;
					break;
			}
		if (direction == RIGHT)
			switch (collision) {
				case NORTH:
					if (Right.x > 0.0) {
						Position.y += Right.y * velocity;
						Position.z += Right.z * velocity;
					}
					break;
				case SOUTH:
					if (Right.x < 0.0) {
						Position.y += Right.y * velocity;
						Position.z += Right.z * velocity;
					}
					break;
				case EAST:
					if (Right.z > 0.0) {
						Position.y += Right.y * velocity;
						Position.x += Right.x * velocity;
					}
					break;
				case WEST:
					if (Right.z < 0.0) {
						Position.y += Right.y * velocity;
						Position.x += Right.x * velocity;
					}
					break;
				case TOP:
					if (Right.y > 0.0) {
						Position.x += Right.x * velocity;
						Position.z += Right.z * velocity;
					}
					break;
				case BOTTOM:
					if (Right.y < 0.0) {
						Position.x += Right.x * velocity;
						Position.z += Right.z * velocity;
					}
					break;
				default:
					Position += Right * velocity;
					break;
			}
		//std::cout << collision << std::endl;
	}
	
	void UpdateCamPos(glm::vec3 offset) {
		Position = TempPosition + offset;
		TempPosition = Position;
	}
	

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw   += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		fov -= (float)yoffset;
		if (fov < 1.0f)
			fov = 1.0f;
		if (fov > 45.0f)
			fov = 45.0f;
	}

private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up    = glm::normalize(glm::cross(Right, Front));
	}
};
#endif
