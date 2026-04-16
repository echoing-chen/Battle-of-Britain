#ifndef BULLET_HPP
#define BULLET_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class NPCs;
class Shader;
class Camera;
extern float deltatime;

struct Bullet {
	glm::vec4 position1;
	glm::vec4 position2;
	glm::vec3 front;
	float life;
	float speed;
};

class Bullets {
public:
	std::vector<Bullet> invisible_bullets;
	std::vector<Bullet> visible_bullets;
	GLuint bulletVAO;
	GLuint bulletVBO;
	Bullets() {
		invisible_bullets = {};
		visible_bullets = {};
		glGenVertexArrays(1, &bulletVAO);
		glGenBuffers(1, &bulletVBO);
		glBindVertexArray(bulletVAO);
		glBindBuffer(GL_ARRAY_BUFFER, bulletVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 2 * 10000, NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void openfire(glm::vec3 position, glm::vec3 front,Camera& mycamera);
	void managebullet(Camera& mycamera,NPCs& npcs);
	void renderbullets(Shader& bulletshader);
	bool checkbulletcollision(Bullet bullet,Camera& mycamera,NPCs& npcs);
};

#endif