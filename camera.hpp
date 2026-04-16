#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
-3 failed
-2 revive
-1 die
0 on the groud
1 flying
2 win
*/
class Bullets;
extern GLFWwindow* window;
extern float deltatime;
extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern int windowwidth;
extern int windowheight;
extern glm::vec3 initialDir;
extern glm::vec3 worldUp;

class Camera {
public:
	int state;
	int HP;
	int ammunition;
	int fuel;
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	float Yaw;
	float Pitch;
	float Roll;
	float FoV;
	float Velocity;
	float MouseSensitivity;
	Camera() {
		state = 0;
		HP = 100;
		ammunition = 5000;
		fuel = 100000;
		Position = glm::vec3(0.0f, 10.0f, 0.0f);
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Yaw = 0.0f;
		Pitch = 0.0f;
		Roll = 0.0f;
		FoV = 45.0f;
		Velocity = 0.0f;
		MouseSensitivity = 0.03f;
		UpdateCameraVectors();
	}
	void ProcessKeyBoard(Bullets& bullets,Camera& mycamera);
	void ProcessMouse();
	void UpdateCamera();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	void UpdateCameraVectors();
};

#endif