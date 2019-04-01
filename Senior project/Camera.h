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
	vec3 velocity;
	
	bool onGround;
	
	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;
	GLfloat Speed;
	GLfloat ZOOM;
	GLfloat MouseSensitivity;
	GLfloat Force;
	const GLfloat gravity = -0.8f;
	const GLfloat velocityLimit = 5.0f;

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
		this->velocity = vec3(0.0f, 0.0f, 0.0f);
		this->Force = 40.0f;
		Speed = mvmtspeed;
		MouseSensitivity = mouse;
		UpdateCameraVectors();
	}

	mat4 GetViewMatrix() {
		return lookAt(Position, Position + Front, Up);
	}

	void processKeyboard(Camera_Movement direction, GLfloat deltaTime) {
		GLfloat camSpeed = Speed*deltaTime;
		if(direction == FORWARD && velocity.x < velocityLimit)
		{
			Position += camSpeed * Front;
		}
		if (direction == BACKWARD && velocity.x > -velocityLimit)
		{
			Position -= camSpeed * Front;
		}
		if (direction == LEFT && velocity.z > -velocityLimit)
		{
			Position -= camSpeed * Right;
		}
		if (direction == RIGHT && velocity.z < velocityLimit)
		{
			Position += camSpeed * Right;
		}
		if (onGround)
		{
			Position.y = 1.0f;
		}
		std::cout << velocity.x << "," << velocity.y << "," << velocity.z << std::endl;
	}

	vec3 getPosition() { return Position; }

	void processJump() {
		std::cout << "processing jump" << std::endl;
		this->jumping = true;
		this->onGround = false;
		velocity = Force * WorldUp;
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

	void Update(GLfloat dt) {
		Position += velocity * dt;
		if (Position.y > 1.0f)
		{
			velocity += gravity * WorldUp;
		}
		else 
		{
			Position.y = 1.0f;
			onGround = true;
			jumping = false;
			velocity = vec3(0.0f, 0.0f, 0.0f);
		}
	}

};
#endif // !CAMERA_H

