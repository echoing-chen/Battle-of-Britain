#include "NPC.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "bullet.hpp"

void Bullets::openfire(glm::vec3 position, glm::vec3 front,Camera& mycamera) {
	Bullet bullet;
	bullet.position1 = glm::vec4(position + 8.0f * front, 0);
	bullet.position2 = glm::vec4(position - 2.0f * front, 1);
	bullet.front = front;
	bullet.life = 3.0f;
	bullet.speed = 300;
	if (glm::distance(position, mycamera.Position) <= 1000) {
		visible_bullets.push_back(bullet);
	}
	else {
		invisible_bullets.push_back(bullet);
	}
}

void Bullets::managebullet(Camera& mycamera,NPCs& npcs) {
	invisible_bullets.erase(
		std::remove_if(invisible_bullets.begin(), invisible_bullets.end(),
			[this, &mycamera, &npcs](const Bullet& bullet) { return (bullet.life < 0) || checkbulletcollision(bullet, mycamera, npcs); }),
		invisible_bullets.end()
	);
	for (Bullet& bullet : invisible_bullets) {
		bullet.life -= deltatime;
		bullet.position1 += glm::vec4(bullet.front * deltatime * bullet.speed, 0);
		bullet.position2 += glm::vec4(bullet.front * deltatime * bullet.speed, 0);
	}
	visible_bullets.erase(
		std::remove_if(visible_bullets.begin(), visible_bullets.end(),
			[this, &mycamera, &npcs](const Bullet& bullet) { return (bullet.life < 0) || checkbulletcollision(bullet, mycamera, npcs); }),
		visible_bullets.end()
	);
	for (Bullet& bullet : visible_bullets) {
		bullet.life -= deltatime;
		bullet.position1 += glm::vec4(bullet.front * deltatime * bullet.speed, 0);
		bullet.position2 += glm::vec4(bullet.front * deltatime * bullet.speed, 1);
	}
}

void Bullets::renderbullets(Shader& bulletshader) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(bulletVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bulletVBO);
	for (size_t i = 0; i < ((visible_bullets.size()<10000)?visible_bullets.size():10000); i++) {
		glBufferSubData(GL_ARRAY_BUFFER, 2 * i * sizeof(glm::vec4), sizeof(glm::vec4), &visible_bullets[i].position1);
		glBufferSubData(GL_ARRAY_BUFFER, (2 * i + 1) * sizeof(glm::vec4), sizeof(glm::vec4), &visible_bullets[i].position2);
	}
	bulletshader.use();
	glDrawArrays(GL_LINES, 0, 2 * visible_bullets.size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
}
bool Bullets::checkbulletcollision(Bullet bullet,Camera& mycamera,NPCs& npcs) {
	for (Npc& ally : npcs.allies) {
		if (glm::distance(glm::vec3(bullet.position1), ally.position) < 6.0f) {
			ally.state = 0;
			return true;
		}
	}
	for (Npc& bf109 : npcs.bf109s) {
		if (glm::distance(glm::vec3(bullet.position1), bf109.position) < 6.0f) {
			bf109.state = 0;
			return true;
		}
	}
	for (Npc& bomber : npcs.bombers) {
		if (glm::distance(glm::vec3(bullet.position1), bomber.position) < 9.0f) {
			bomber.state = 0;
			return true;
		}
	}
	if (glm::distance(glm::vec3(bullet.position1), mycamera.Position) < 6.0f) {
		mycamera.HP -=4;
		return true;
	}
	return false;
}