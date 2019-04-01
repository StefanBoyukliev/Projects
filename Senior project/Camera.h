#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <gl/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP
};

class Camera {
private:
	vec3 Position;
	vec3 Direction;
	vec3 Up;
	vec3 Right;
	vec3 Front;
	vec3 WorldUp;
	
	bool onGround;
	
	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;
	GLfloat Speed;
	GLfloat ZOOM;
	GLfloat MouseSensitivity;

	void UpdateCameraVectors() {
		vec3 front;
		front.x = cos(radians(yaw))*cos(radians(pitch));
		front.y = sin(radians(pitch));
		front.z = sin(radians(yaw))*cos(radians(pitch));
		Front = normalize(front);
		Right = normalize(cross(Front, WorldUp));
		Up = normalize(cross(Right, Front));
	}

public:
	bool jumping;

	Camera(vec3 position = vec3(0.0f, 1.0f, 3.0f), vec3 Up = vec3(0.0f, 1.0f, 0.0f), GLfloat mvmtspeed = 6.0f, GLfloat mouse = 0.15f) {
		this->Position = position;
		this->WorldUp = Up;
		this->yaw = -90.0f;
		this->pitch = 0.0f;
		this->jumping = false;
		this->onGround = true;
		Speed = mvmtspeed;
		MouseSensitivity = mouse;
		UpdateCameraVectors();
	}

	mat4 GetViewMatrix() {
		return lookAt(Position, Position + Front, Up);
	}

	void processKeyboard(Camera_Movement direction, GLfloat deltaTime) {
		GLfloat camSpeed = Speed * deltaTime;
		if(direction == FORWARD)
		{
			Position += camSpeed*Front;
		}
		if (direction == BACKWARD)
		{
			Position -= camSpeed * Front;
		}
		if (direction == LEFT)
		{
			Position -= camSpeed * Right;
		}
		if (direction == RIGHT)
		{
			Position += camSpeed * Right;
		}
		if (onGround)
		{
			Position.y = 1.0f;
		}
	}

	vec3 getPosition() { return Position; }

	void JumpUp() {
		
		while (Position.y < 4.0f) { Position += 0.015f*WorldUp; std::cout << "jumping" << " " << onGround << std::endl;
		}
		jumping = false;
	}

	void FallDown() {
		std::cout << "falling" << " " << onGround << std::endl;
		while (Position.y > 1.0f) { Position -= 0.03f*WorldUp; }
		//Position.y = 1.0f;
		onGround = true;
		
	}

	void processJump() {
		std::cout << "processing jump" << std::endl;
		this->jumping = true;
		this->onGround = false;
		if (jumping) { this->JumpUp(); UpdateCameraVectors(); }
		FallDown();
	}

	void ProcessMouse(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		yaw += xoffset;
		pitch += yoffset;

		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}		
		UpdateCameraVectors();
	}

};
#endif // !CAMERA_H

