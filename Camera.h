#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

#include "Game.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP
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
	glm::vec3 Fnet;
	glm::vec3 Vi;
	glm::vec3 tempVi;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float fov;
	// other
	const float g = 9.8;
	bool stick = true;

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
						else
						{
							Position.x += Front.x * velocity;
							Position.z += Front.z * velocity;
						}
						break;
					case SOUTH:
						if (Front.x > 0.0) {
							Position.z += Front.z * velocity;
						}
						else
						{
							Position.x += Front.x * velocity;
							Position.z += Front.z * velocity;
						}
						break;
					case EAST:
						if (Front.z < 0.0) {
							Position.x += Front.x * velocity;
						}
						else
						{
							Position.x += Front.x * velocity;
							Position.z += Front.z * velocity;
						}
						break;
					case WEST:
						if (Front.z > 0.0) {
							Position.x += Front.x * velocity;
						}
						else
						{
							Position.x += Front.x * velocity;
							Position.z += Front.z * velocity;
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
						else
						{
							Position += Front * velocity;
						}
						break;
					case SOUTH:
						if (Front.x > 0.0) {
							Position.y += Front.y * velocity;
							Position.z += Front.z * velocity;
						}
						else
						{
							Position += Front * velocity;
						}
						break;
					case EAST:
						if (Front.z < 0.0) {
							Position.y += Front.y * velocity;
							Position.x += Front.x * velocity;
						}
						else
						{
							Position += Front * velocity;
						}
						break;
					case WEST:
						if (Front.z > 0.0) {
							Position.y += Front.y * velocity;
							Position.x += Front.x * velocity;
						}
						else
						{
							Position += Front * velocity;
						}
						break;
					case TOP:
						if (Front.y < 0.0) {
							Position.x += Front.x * velocity;
							Position.z += Front.z * velocity;
						}
						else
						{
							Position += Front * velocity;
						}
						break;
					case BOTTOM:
						if (Front.y > 0.0) {
							Position.x += Front.x * velocity;
							Position.z += Front.z * velocity;
						}
						else
						{
							Position += Front * velocity;
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
						else
						{
							Position.x -= Front.x * velocity;
							Position.z -= Front.z * velocity;
						}
						break;
					case SOUTH:
						if (Front.x < 0.0) {
							Position.z -= Front.z * velocity;
						}
						else
						{
							Position.x -= Front.x * velocity;
							Position.z -= Front.z * velocity;
						}
						break;
					case EAST:
						if (Front.z > 0.0) {
							Position.x -= Front.x * velocity;
						}
						else
						{
							Position.x -= Front.x * velocity;
							Position.z -= Front.z * velocity;
						}
						break;
					case WEST:
						if (Front.z < 0.0) {
							Position.x -= Front.x * velocity;
						}
						else
						{
							Position.x -= Front.x * velocity;
							Position.z -= Front.z * velocity;
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
						else
						{
							Position -= Front * velocity;
						}
						break;
					case SOUTH:
						if (Front.x < 0.0) {
							Position.y -= Front.y * velocity;
							Position.z -= Front.z * velocity;
						}
						else
						{
							Position -= Front * velocity;
						}
						break;
					case EAST:
						if (Front.z > 0.0) {
							Position.y -= Front.y * velocity;
							Position.x -= Front.x * velocity;
						}
						else
						{
							Position -= Front * velocity;
						}
						break;
					case WEST:
						if (Front.z < 0.0) {
							Position.y -= Front.y * velocity;
							Position.x -= Front.x * velocity;
						}
						else
						{
							Position -= Front * velocity;
						}
						break;
					case TOP:
						if (Front.y > 0.0) {
							Position.x -= Front.x * velocity;
							Position.z -= Front.z * velocity;
						}
						else
						{
							Position -= Front * velocity;
						}
						break;
					case BOTTOM:
						if (Front.y < 0.0) {
							Position.x -= Front.x * velocity;
							Position.z -= Front.z * velocity;
						}
						else
						{
							Position -= Front * velocity;
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
					else
					{
						Position -= Right * velocity;
					}
					break;
				case SOUTH:
					if (Right.x < 0.0) {
						Position.y -= Right.y * velocity;
						Position.z -= Right.z * velocity;
					}
					else
					{
						Position -= Right * velocity;
					}
					break;
				case EAST:
					if (Right.z > 0.0) {
						Position.y -= Right.y * velocity;
						Position.x -= Right.x * velocity;
					}
					else
					{
						Position -= Right * velocity;
					}
					break;
				case WEST:
					if (Right.z < 0.0) {
						Position.y -= Right.y * velocity;
						Position.x -= Right.x * velocity;
					}
					else
					{
						Position -= Right * velocity;
					}
					break;
				case TOP:
					if (Right.y > 0.0) {
						Position.x -= Right.x * velocity;
						Position.z -= Right.z * velocity;
					}
					else
					{
						Position -= Right * velocity;
					}
					break;
				case BOTTOM:
					if (Right.y < 0.0) {
						Position.x -= Right.x * velocity;
						Position.z -= Right.z * velocity;
					}
					else
					{
						Position -= Right * velocity;
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
					else
					{
						Position += Right * velocity;
					}
					break;
				case SOUTH:
					if (Right.x < 0.0) {
						Position.y += Right.y * velocity;
						Position.z += Right.z * velocity;
					}
					else
					{
						Position += Right * velocity;
					}
					break;
				case EAST:
					if (Right.z > 0.0) {
						Position.y += Right.y * velocity;
						Position.x += Right.x * velocity;
					}
					else
					{
						Position += Right * velocity;
					}
					break;
				case WEST:
					if (Right.z < 0.0) {
						Position.y += Right.y * velocity;
						Position.x += Right.x * velocity;
					}
					else
					{
						Position += Right * velocity;
					}
					break;
				case TOP:
					if (Right.y > 0.0) {
						Position.x += Right.x * velocity;
						Position.z += Right.z * velocity;
					}
					else
					{
						Position += Right * velocity;
					}
					break;
				case BOTTOM:
					if (Right.y < 0.0) {
						Position.x += Right.x * velocity;
						Position.z += Right.z * velocity;
					}
					else
					{
						Position += Right * velocity;
					}
					break;
				default:
					Position += Right * velocity;
					break;
			}
		//std::cout << collision << std::endl;
	}
	
	
	void ProcessKeyboard(Camera_Movement direction, bool collided[], float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (collided[4] || Position.y <= 0.5) 
		{
			Fnet.y = 0.0;
			if (Position.y < 0.5) {
				Position.y = 0.5;
			}
			stick = true;
			
			if (direction == FORWARD)
			{
				if (collided[0]) {
					if (!(Front.x < 0.0)) {
						Position.x += Front.x * velocity;
						tempVi.x = Front.x * MovementSpeed;
					}
					else {
						tempVi.x = 0.0;
					}
				}
				else if (collided[1]) {
					if (!(Front.x > 0.0)) {
						Position.x += Front.x * velocity;
						tempVi.x = Front.x * MovementSpeed;
					}
					else {
						tempVi.x = 0.0;
					}
				}
				else {
					Position.x += Front.x * velocity;
					tempVi.x = Front.x * MovementSpeed;
				}
				
				if (collided[2]) {
					if (!(Front.z < 0.0)) {
						Position.z += Front.z * velocity;
						tempVi.z = Front.z * MovementSpeed;
					}
					else {
						tempVi.z = 0.0;
					}
				}
				else if (collided[3]) {
					if (!(Front.z > 0.0)) {
						Position.z += Front.z * velocity;
						tempVi.z = Front.z * MovementSpeed;
					}
					else {
						tempVi.z = 0.0;
					}
				}
				else {
					Position.z += Front.z * velocity;
					tempVi.z = Front.z * MovementSpeed;
				}
				
				/*
				if (collided[4]) {
					if (!(Front.y < 0.0)) {
						Position.y += Front.y * velocity;
					}
				}
				else if (collided[5]) {
					if (!(Front.y > 0.0)) {
						Position.y += Front.y * velocity;
					}
				}
				else if (Position.y < 0.5) {
					if (!(Front.y < 0.0)) {
						Position.y += Front.y * velocity;
					}
				}
				else {
					Position.y += Front.y * velocity;
				}
				*/
			}
			
			else if (direction == BACKWARD)
			{
				if (collided[0]) {
					if (!(Front.x > 0.0)) {
						Position.x -= Front.x * velocity;
						tempVi.x = -1.0 * Front.x * MovementSpeed;
					}
					else {
						tempVi.x = 0.0;
					}
				}
				else if (collided[1]) {
					if (!(Front.x < 0.0)) {
						Position.x -= Front.x * velocity;
						tempVi.x = -1.0 * Front.x * MovementSpeed;
					}
					else {
						tempVi.x = 0.0;
					}
				}
				else {
					Position.x -= Front.x * velocity;
					tempVi.x = -1.0 * Front.x * MovementSpeed;
				}
				
				if (collided[2]) {
					if (!(Front.z > 0.0)) {
						Position.z -= Front.z * velocity;
						tempVi.z = -1.0 * Front.z * MovementSpeed;
					}
					else {
						tempVi.z = 0.0;
					}
				}
				else if (collided[3]) {
					if (!(Front.z < 0.0)) {
						Position.z -= Front.z * velocity;
						tempVi.z = -1.0 * Front.z * MovementSpeed;
					}
					else {
						tempVi.z = 0.0;
					}
				}
				else {
					Position.z -= Front.z * velocity;
					tempVi.z = -1.0 * Front.z * MovementSpeed;
				}
				
				/*
				if (collided[4]) {
					if (!(Front.y > 0.0)) {
						Position.y -= Front.y * velocity;
					}
				}
				else if (collided[5]) {
					if (!(Front.y < 0.0)) {
						Position.y -= Front.y * velocity;
					}
				}
				else if (Position.y < 0.5) {
					if (!(Front.y > 0.0)) {
						Position.y -= Front.y * velocity;
					}
				}
				else {
					Position.y -= Front.y * velocity;
				}
				*/
			}
			
			else if (direction == LEFT) {
				if (collided[0]) {
					if (!(Right.x > 0.0)) {
						Position.x -= Right.x * velocity;
						tempVi.x = -1.0 * Right.x * MovementSpeed;
					}
					else {
						tempVi.x = 0.0;
					}
				}
				else if (collided[1]) {
					if (!(Right.x < 0.0)) {
						Position.x -= Right.x * velocity;
						tempVi.x = -1.0 * Right.x * MovementSpeed;
					}
					else {
						tempVi.x = 0.0;
					}
				}
				else {
					Position.x -= Right.x * velocity;
					tempVi.x = -1.0 * Right.x * MovementSpeed;
				}
				
				if (collided[2]) {
					if (!(Right.z > 0.0)) {
						Position.z -= Right.z * velocity;
						tempVi.z = -1.0 * Right.z * MovementSpeed;
					}
					else {
						tempVi.z = 0.0;
					}
				}
				else if (collided[3]) {
					if (!(Right.z < 0.0)) {
						Position.z -= Right.z * velocity;
						tempVi.z = -1.0 * Right.z * MovementSpeed;
					}
					else {
						tempVi.z = 0.0;
					}
				}
				else {
					Position.z -= Right.z * velocity;
					tempVi.z = -1.0 * Right.z * MovementSpeed;
				}
			}
			
			else if (direction == RIGHT) {
				if (collided[0]) {
					if (!(Right.x < 0.0)) {
						Position.x += Right.x * velocity;
						tempVi.x = Right.x * MovementSpeed;
					}
					else {
						tempVi.x = 0.0;
					}
				}
				else if (collided[1]) {
					if (!(Right.x > 0.0)) {
						Position.x += Right.x * velocity;
						tempVi.x = Right.x * MovementSpeed;
					}
					else {
						tempVi.x = 0.0;
					}
				}
				else {
					Position.x += Right.x * velocity;
					tempVi.x = Right.x * MovementSpeed;
				}
				
				if (collided[2]) {
					if (!(Right.z < 0.0)) {
						Position.z += Right.z * velocity;
						tempVi.z = Right.z * MovementSpeed;
					}
					else {
						tempVi.z = 0.0;
					}
				}
				else if (collided[3]) {
					if (!(Right.z > 0.0)) {
						Position.z += Right.z * velocity;
						tempVi.z = Right.z * MovementSpeed;
					}
					else {
						tempVi.z = 0.0;
					}
				}
				else {
					Position.z += Right.z * velocity;
					tempVi.z = Right.z * MovementSpeed;
				}
			}
			if (direction == JUMP) {
				Vi.y = 8.3;
				Fnet.y = -9.8;
				Vi.x = tempVi.x;
				Vi.z = tempVi.z;
				stick = false;
			}
			//std::cout << collision << std::endl;
		}
		else if (collided[5]) {
			Vi = glm::vec3(0.0);
		}
		else {
			Fnet.y = -9.8;
		}
	}
	
	void UpdateCamPos(glm::vec3 offset) {
		Position = TempPosition + offset;
		TempPosition = Position;
	}
	
	void UpdatePosition(bool collided[], float dt) {
		if (collided[0] || collided[1]) {
			Vi.x = 0.0;
		}
		if (collided[2] || collided[3]) {
			Vi.z = 0.0;
		}
		
		if (Fnet.y != 0.0 && Position.y >= 0.5 && (!stick || !collided[4])) {
			Position.x += dt * Vi.x;
			Position.y += dt * Vi.y + 0.5 * Fnet.y * dt * dt;
			Position.z += dt * Vi.z;
			Vi.y = Fnet.y * dt + Vi.y;
		}
		else {
			Fnet.y = 0.0;
		}
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
