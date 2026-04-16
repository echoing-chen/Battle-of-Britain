#include "bullet.hpp"
#include "camera.hpp"
#include <iostream>

void Camera::ProcessKeyBoard(Bullets& bullets,Camera& mycamera) {
	if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && (state != -1) && (fuel >0)) {
		Velocity = glm::clamp(Velocity + 3.0f * deltatime, 0.0f, 40.0f);
		fuel = glm::clamp((float)fuel - 1, 0.0f, 100000.0f);
	}
	if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && state != -1) {
		Velocity = glm::clamp(Velocity - 5.0f * deltatime, 0.0f, 40.0f);
	}
	if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && (state == 1)&&(ammunition>0)) {
		bullets.openfire(Position, Front,mycamera);
		ammunition = glm::clamp(ammunition - 1, 0, 5000);
	}
}

void Camera::ProcessMouse() {
	double xpos;
	double ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	Pitch = glm::clamp(Pitch + (float)(ypos - windowheight / 2)*((float)SCR_HEIGHT/windowheight)*deltatime * MouseSensitivity, -75.0f, 75.0f);
	Roll = glm::clamp(-75.0f*(float)((xpos - windowwidth / 2)/(windowwidth/2)), -75.0f, 75.0f);
	Yaw += Roll * deltatime * 0.2f;
}

void Camera::UpdateCamera() {
	if (state == -2) {
		state = 0;
		fuel = 100000;
		ammunition = 5000;
		HP = 100;
		Position = glm::vec3(0.0f, 10.0f, 0.0f);
		Yaw = 0.0f;
		Pitch = 0.0f;
		Roll = 0.0f;
		Velocity = 0.0f;
	}
	if (state == 0) {
		if ((Velocity >= 20) && (Pitch > 0) && (glm::distance(Position, glm::vec3(0.0f, 5.0f, 0.0f)) <= 202.0f)) {
			state = 1;
			Roll = 0;
			return;
		}
		if (glm::distance(Position, glm::vec3(0.0f, 10.0f, 0.0f)) >= 202.0f) {
			Position = glm::vec3(0.0f, 10.0f, 0.0f);
			Velocity = 0;
			Yaw = 0.0f;
			return;
		}
		Pitch = 0;
		Roll = 0;
	}
	if (state == 1) {
		if (Position.y <= 0) {
			HP=0;
			return;
		}
		if ((glm::distance(Position, glm::vec3(0.0f, 10.0f, 0.0f)) <= 202.0f) && (Front.y < 0) && (Position.y < 10.0f)) {
			state = 0;
			Position.y = 10.0f;
			Pitch = 0;
			Roll = 0;
			return;
		}
		if (Velocity < 15) {
			Pitch -= 5 * deltatime;
		}
		if (Velocity < 10) {
			Position -= 6 * deltatime;
		}
	}
	Velocity = glm::clamp(Velocity - 0.5f*Velocity * Front.y * deltatime, 0.0f, 40.0f);
	Position += Front * Velocity * deltatime;
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(FoV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,600.0f);
}

void Camera::UpdateCameraVectors() {
	glm::mat4 Yawrotation = glm::rotate(glm::mat4(1.0f), glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 Pitchaxis = glm::normalize(glm::vec3( Yawrotation* glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
	glm::mat4 Pitchrotation = glm::rotate(glm::mat4(1.0f), glm::radians(Pitch), Pitchaxis);
	Front = glm::normalize(glm::vec3(Pitchrotation * Yawrotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
	glm::mat4 Rollrotation = glm::rotate(glm::mat4(1.0f), glm::radians(-Roll), Front);
	Right = glm::normalize(glm::vec3(Rollrotation * glm::vec4(Pitchaxis, 0.0f)));
	Up = glm::normalize(glm::cross(Right, Front));
}