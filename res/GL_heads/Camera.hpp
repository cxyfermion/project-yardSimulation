#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	//�������
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	//ŷ����
	float Yaw;
	float Pitch;
	//�������
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	float shiftMode;
	Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->shiftMode = 1.0f;
		updateCameraVectors();
	}
	glm::mat4 GetViewMatrix()
	{
		return lookAt(this->Position, this->Position + this->Front, this->Up);
	}
	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= SENSITIVITY;
		yoffset *= SENSITIVITY;
		Yaw += xoffset;
		Pitch += yoffset;
		if (Pitch > 89.0f)
		{
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f)
		{
			Pitch = -89.0f;
		}
		updateCameraVectors();
	}
	//������֤
	void ProcessKayboard(Camera_Movement direction, float deltaTime)
	{
		const float velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			Position += Front * velocity * this->shiftMode;
		}
		if (direction == BACKWARD)
		{
			Position -= Front * velocity * this->shiftMode;
		}
		if (direction == RIGHT)
		{
			Position += Right * velocity * this->shiftMode;
		}
		if (direction == LEFT)
		{
			Position -= Right * velocity * this->shiftMode;
		}
		if (direction == UP)
		{
			Position += Up * velocity * this->shiftMode;
		}
		if (direction == DOWN)
		{
			Position -= Up * velocity * this->shiftMode;
		}
	}
	void ProcessMouseScroll(float yoffset)
	{
		this->Zoom -= (float)(yoffset * 3.0);
		if (this->Zoom < 1.0f)
		{
			this->Zoom = 1.0f;
		}
		if (this->Zoom > 75.0f)
		{
			this->Zoom = 75.0f;
		}
	}
	void ProcessView(bool direction)
	{
		if (direction)
		{
			this->Zoom -= 0.1f * this->shiftMode;
		}
		else
		{
			this->Zoom += 0.1f * this->shiftMode;
		}
		if (this->Zoom < 1.0f)
		{
			this->Zoom = 1.0f;
		}
		if (this->Zoom > 75.0f)
		{
			this->Zoom = 75.0f;
		}
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		direction.y = sin(glm::radians(Pitch));
		direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(direction);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};