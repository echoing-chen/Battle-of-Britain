#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "Bullet.hpp"
#include "NPC.hpp"
#include <random>

std::mt19937 engine(time(nullptr));
std::uniform_int_distribution<int> dist(-30, 30);


void NPCs::initnpcs(Model ally_model, Model bf109_model, Model bomber_model) {
	glGenBuffers(1, &alliesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, alliesVBO);
	glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	for (int i = 0; i < ally_model.meshes.size(); i++) {
		glBindVertexArray(ally_model.meshes[i].VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
	}
	glGenBuffers(1, &bf109sVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bf109sVBO);
	glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	for (int i = 0; i < bf109_model.meshes.size(); i++){
		glBindVertexArray(bf109_model.meshes[i].VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
	}
	glGenBuffers(1, &bombersVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bombersVBO);
	glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	for (int i = 0; i < bomber_model.meshes.size(); i++) {
		glBindVertexArray(bomber_model.meshes[i].VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenVertexArrays(1, &particleVAO);
	glGenBuffers(1, &particleVBO);
	glBindVertexArray(particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferData(GL_ARRAY_BUFFER, 10000 * sizeof(Particle), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, alpha));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void NPCs::stage1() {
	allies.clear();
	bf109s.clear();
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 20; j++) {
			Npc ally;
			ally.position = glm::vec3(-400 - 60 * j, 100 + 50 * i, 400.0f+20*j);
			ally.yaw = 0.0f;
			ally.pitch = 0.0f;
			ally.roll = 0.0f;
			ally.front = glm::vec3(0.0f, 0.0f, -1.0f);
			ally.rotation = glm::mat4(1.0f);
			ally.scale = glm::scale(glm::mat4(1.0f),glm::vec3(0.05));
			ally.velocity = 30.0f;
			ally.state = 1;
			allies.push_back(ally);
			Npc bf109;
			bf109.position = glm::vec3(300 - 60 * j, 300 + 50 * i, -1200.0f-10*i);
			bf109.yaw = 180.0f;
			bf109.pitch = 0.0f;
			bf109.roll = 0.0f;
			bf109.front = glm::vec3(0.0f, 0.0f, 1.0f);
			bf109.rotation = glm::mat4(1.0f);
			bf109.scale = glm::mat4(1.0f);
			bf109.velocity = 30.0f;
			bf109.state = 1;
			bf109s.push_back(bf109);
		}
	}
}

void NPCs::stage1AI(Bullets& bullets,Camera& mycamera) {
	for (Npc& ally : allies) {
		if (ally.state==1){
			Npc target;
			target.position = glm::vec3(1000000, 1000000, 1000000);
			for (Npc& bf109 : bf109s) {
				if ((bf109.state == 1) && (glm::distance(target.position, ally.position) > glm::distance(bf109.position, ally.position))) {
					target = bf109;
				}
			}
			allyf--;
			if ((allyf<=0)&&(distance(target.position,ally.position)<=600.0f)&&glm::length(glm::cross(target.position - ally.position, ally.front)) <= 30) {
					bullets.openfire(ally.position, ally.front, mycamera);
					allyf = 1.0f / deltatime;
			}
			ally.yaw = fmod(ally.yaw, 360.0f);
			if (ally.yaw > 180) {
				ally.yaw -= 360;
			}
			else if (ally.yaw <= -180) {
				ally.yaw += 360;
			}
			glm::vec3 direction = glm::normalize(target.position - ally.position);
			glm::vec2 directionxz = glm::normalize(glm::vec2(direction.x, direction.z));
			float targetyaw;
			if (directionxz.x <= 0 && directionxz.y <= 0) {
				targetyaw = glm::degrees(glm::asin(-directionxz.x));
			}
			else if (directionxz.x <= 0 && directionxz.y > 0) {
				targetyaw = glm::degrees(glm::acos(-directionxz.y));
			}
			else if (directionxz.x > 0 && directionxz.y <= 0) {
				targetyaw = glm::degrees(glm::asin(-directionxz.x));
			}
			else {
				targetyaw = -180.0f - glm::degrees(glm::asin(-directionxz.x));
			}

			float targetpitch = glm::degrees(glm::asin(direction.y));
			float x = targetyaw - ally.yaw;
			if (x < -180) {
				x += 360;
			}
			else if (x > 180) {
				x -= 360;
			}
			ally.roll = glm::clamp(-x, -75.0f, 75.0f);
			ally.pitch += (targetpitch - ally.pitch) * deltatime;
		}
	}
	for (Npc& bf109 : bf109s) {
		if (bf109.state==1){
			Npc target;
			target.position = glm::vec3(mycamera.Position.x, mycamera.Position.y, mycamera.Position.z);
			for (Npc& ally : allies) {
				if ((ally.state == 1) && (glm::distance(target.position, bf109.position) > glm::distance(ally.position, bf109.position))) {
					target = ally;
				}
			}
			bf109f--;
			if ((bf109f <= 0)&& (distance(target.position, bf109.position) <= 600.0f) && glm::length(glm::cross(target.position - bf109.position, bf109.front)) <= 30) {
					bullets.openfire(bf109.position, bf109.front, mycamera);
					bf109f = 1.0f / deltatime;
			}
			bf109.yaw = fmod(bf109.yaw, 360.0f);
			if (bf109.yaw > 180) {
				bf109.yaw -= 360;
			}
			else if (bf109.yaw <= -180) {
				bf109.yaw += 360;
			}
			glm::vec3 direction = glm::normalize(target.position - bf109.position);
			glm::vec2 directionxz = glm::normalize(glm::vec2(direction.x, direction.z));
			float targetyaw;
			if (directionxz.x <= 0 && directionxz.y <= 0) {
				targetyaw = glm::degrees(glm::asin(-directionxz.x));
			}
			else if (directionxz.x <= 0 && directionxz.y > 0) {
				targetyaw = glm::degrees(glm::acos(-directionxz.y));
			}
			else if (directionxz.x > 0 && directionxz.y <= 0) {
				targetyaw = glm::degrees(glm::asin(-directionxz.x));
			}
			else {
				targetyaw = -180.0f - glm::degrees(glm::asin(-directionxz.x));
			}

			float targetpitch = glm::degrees(glm::asin(direction.y));
			float x = targetyaw - bf109.yaw;
			if (x < -180) {
				x += 360;
			}
			else if (x > 180) {
				x -= 360;
			}
			
			bf109.roll = glm::clamp(-x, -75.0f, 75.0f);
			bf109.pitch += (targetpitch - bf109.pitch) * deltatime;
		}
	}
}

void NPCs::renderstage1npc(Shader& npcsshader,Model& ally_model,Model& bf109_model) {
	vector<glm::mat4> alliesmodel;
	for (Npc& ally:allies) {
		alliesmodel.push_back( glm::translate(glm::mat4(1.0f), ally.position) * ally.rotation * ally.scale);
	}
	glBindBuffer(GL_ARRAY_BUFFER, alliesVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, allies.size() * sizeof(glm::mat4), alliesmodel.data());
	npcsshader.use();
	npcsshader.setInt("texture_diffusel", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ally_model.textures_loaded[0].id);
	for (int i = 0; i < ally_model.meshes.size(); i++) {
		glBindVertexArray(ally_model.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(ally_model.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, allies.size());
		glBindVertexArray(0);
	}

	vector<glm::mat4> bf109smodel;
	for (Npc& bf109:bf109s) {
		bf109smodel.push_back(glm::translate(glm::mat4(1.0f), bf109.position) * bf109.rotation * bf109.scale);
	}
	glBindBuffer(GL_ARRAY_BUFFER, bf109sVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bf109s.size() * sizeof(glm::mat4), bf109smodel.data());
	npcsshader.use();
	npcsshader.setInt("texture_diffusel", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bf109_model.textures_loaded[0].id);
	for (int i = 0; i < bf109_model.meshes.size(); i++) {
		glBindVertexArray(bf109_model.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(bf109_model.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, bf109s.size());
		glBindVertexArray(0);
	}
}

void NPCs::updatestage1npc(Camera& mycamera) {
	if (allies.size() == 0) {
		mycamera.state = -3;
		return;
	}
	else if (bf109s.size() == 0) {
		mycamera.state = 2;
		return;
	}
	allies.erase(
		std::remove_if(allies.begin(), allies.end(),
			[](const Npc ally) { return ally.state < 0; }),
		allies.end()
	);
	bf109s.erase(
		std::remove_if(bf109s.begin(), bf109s.end(),
			[](const Npc bf109) { return bf109.state < 0; }),
		bf109s.end()
	);
	for (Npc& ally : allies) {
		if (ally.state == 0) {
			createParticle(ally.position,mycamera);
			ally.position -= 6 * deltatime;
			ally.front -= glm::vec3(0.0f, 1.0f, 0.0f) * deltatime;
			if (ally.position.y < 0) {
				ally.state = -1;
			}
		}
		else {
			ally.yaw -= ally.roll * deltatime * 0.2f;
			glm::mat4 Yawrotation = glm::rotate(glm::mat4(1.0f), glm::radians(ally.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec3 Pitchaxis = glm::normalize(glm::vec3(Yawrotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
			glm::mat4 Pitchrotation = glm::rotate(glm::mat4(1.0f), glm::radians(ally.pitch), Pitchaxis);
			ally.front = glm::normalize(glm::vec3(Pitchrotation * Yawrotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
			glm::mat4 Rollrotation = glm::rotate(glm::mat4(1.0f), glm::radians(-ally.roll), ally.front);
			ally.rotation = Rollrotation * Pitchrotation * Yawrotation;
		}
		ally.position += ally.velocity * deltatime * ally.front;
	}
	for (Npc& bf109 : bf109s) {
		if (bf109.state == 0) {
			createParticle(bf109.position,mycamera);
			bf109.front -= glm::vec3(0.0f, 1.0f, 0.0f)*deltatime;
			if (bf109.position.y < 0) {
				bf109.state = -1;
			}
		}
		else {
			bf109.yaw -= bf109.roll * deltatime * 0.2f;
			glm::mat4 Yawrotation = glm::rotate(glm::mat4(1.0f), glm::radians(bf109.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec3 Pitchaxis = glm::normalize(glm::vec3(Yawrotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
			glm::mat4 Pitchrotation = glm::rotate(glm::mat4(1.0f), glm::radians(bf109.pitch), Pitchaxis);
			bf109.front = glm::normalize(glm::vec3(Pitchrotation * Yawrotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
			glm::mat4 Rollrotation = glm::rotate(glm::mat4(1.0f), glm::radians(-bf109.roll), bf109.front);
			bf109.rotation = Rollrotation * Pitchrotation * Yawrotation*glm::rotate(glm::mat4(1.0f),glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
		}
		bf109.position += bf109.velocity * deltatime * bf109.front;
	}
}

void NPCs::stage2() {
	allies.clear();
	bf109s.clear();
	bombers.clear();
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 10; j++) {
			Npc ally;
			ally.position = glm::vec3(150 - 30 * j, 50 + 20 * i, 400.0f);
			ally.yaw = 0.0f;
			ally.pitch = 0.0f;
			ally.roll = 0.0f;
			ally.front = glm::vec3(0.0f, 0.0f, -1.0f);
			ally.rotation = glm::mat4(1.0f);
			ally.scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05));
			ally.velocity = 30.0f;
			ally.state = 1;
			allies.push_back(ally);
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 20; j++) {
			Npc bomber;
			bomber.position = glm::vec3(800 - 50 * j, (float)(200 + 30 * i), -1500.0f);
			bomber.yaw = 180.0f;
			bomber.pitch = 0.0f;
			bomber.roll = 0.0f;
			bomber.front = glm::vec3(0.0f, 0.0f, 1.0f);
			bomber.rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
			bomber.scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.02));
			bomber.velocity = 5.0f;
			bomber.state = 1;
			bombers.push_back(bomber);
		}
	}
}

void NPCs::stage2AI(Bullets& bullets, Camera& mycamera) {
	for (Npc& ally : allies) {
		Npc target;
		target.position = glm::vec3(1000000, 1000000, 1000000);
		for (Npc& bomber : bombers) {
			if ((bomber.state == 1) && (glm::distance(target.position, ally.position) > glm::distance(bomber.position, ally.position))) {
				target = bomber;
			}
		}
		allyf--;
		if ((allyf<=0)&& (distance(target.position, ally.position) <= 600.0f) && glm::length(glm::cross(target.position - ally.position, ally.front)) <= 30) {
			bullets.openfire(ally.position, ally.front, mycamera);
			allyf = 1.0f / deltatime;
		}
		ally.yaw = fmod(ally.yaw, 360.0f);
		if (ally.yaw > 180) {
			ally.yaw -= 360;
		}
		else if (ally.yaw <= -180) {
			ally.yaw += 360;
		}
		glm::vec3 direction = glm::normalize(target.position - ally.position);
		glm::vec2 directionxz = glm::normalize(glm::vec2(direction.x, direction.z));
		float targetyaw;
		if (directionxz.x <= 0 && directionxz.y <= 0) {
			targetyaw = glm::degrees(glm::asin(-directionxz.x));
		}
		else if (directionxz.x <= 0 && directionxz.y > 0) {
			targetyaw = glm::degrees(glm::acos(-directionxz.y));
		}
		else if (directionxz.x > 0 && directionxz.y <= 0) {
			targetyaw = glm::degrees(glm::asin(-directionxz.x));
		}
		else {
			targetyaw = -180.0f - glm::degrees(glm::asin(-directionxz.x));
		}

		float targetpitch = glm::degrees(glm::asin(direction.y));
		float x = targetyaw - ally.yaw;
		if (x < -180) {
			x += 360;
		}
		else if (x > 180) {
			x -= 360;
		}
		ally.roll = glm::clamp(-x, -75.0f, 75.0f);
		ally.pitch += (targetpitch - ally.pitch) * deltatime;
	}
}

void NPCs::renderstage2npc(Shader& npcsshader, Model& ally_model, Model& bomber_model) {
	vector<glm::mat4> alliesmodel;
	for (Npc& ally : allies) {
		alliesmodel.push_back(glm::translate(glm::mat4(1.0f), ally.position) * ally.rotation * ally.scale);
	}
	glBindBuffer(GL_ARRAY_BUFFER, alliesVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, allies.size() * sizeof(glm::mat4), alliesmodel.data());
	npcsshader.use();
	npcsshader.setInt("texture_diffusel", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ally_model.textures_loaded[0].id);
	for (int i = 0; i < ally_model.meshes.size(); i++) {
		glBindVertexArray(ally_model.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(ally_model.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, allies.size());
		glBindVertexArray(0);
	}

	vector<glm::mat4> bombersmodel;
	for (Npc& bomber:bombers) {
		bombersmodel.push_back(glm::translate(glm::mat4(1.0f), bomber.position) * bomber.rotation * bomber.scale);
	}
	glBindBuffer(GL_ARRAY_BUFFER, bombersVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bombers.size() * sizeof(glm::mat4), bombersmodel.data());
	npcsshader.use();
	npcsshader.setInt("texture_diffusel", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bomber_model.textures_loaded[0].id);
	for (int i = 0; i < bomber_model.meshes.size(); i++) {
		glBindVertexArray(bomber_model.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(bomber_model.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, bombers.size());
		glBindVertexArray(0);
	}
}
void NPCs::updatestage2npc(Camera& mycamera) {
	if (allies.size() == 0) {
		mycamera.state = -3;
		return;
	}
	else if (bombers.size() == 0) {
		mycamera.state = 2;
		return;
	}
	allies.erase(
		std::remove_if(allies.begin(), allies.end(),
			[](const Npc ally) { return ally.state < 0; }),
		allies.end()
	);
	bombers.erase(
		std::remove_if(bombers.begin(), bombers.end(),
			[](const Npc bomber) { return bomber.state < 0; }),
		bombers.end()
	);
	for (Npc& ally : allies) {
		if (ally.state == 0) {
			createParticle(ally.position, mycamera);
			ally.front -= glm::vec3(0.0f, 1.0f, 0.0f) * deltatime;
			if (ally.position.y < 0) {
				ally.state = -1;
			}
		}
		else {
			ally.yaw -= ally.roll * deltatime * 0.2f;
			glm::mat4 Yawrotation = glm::rotate(glm::mat4(1.0f), glm::radians(ally.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec3 Pitchaxis = glm::normalize(glm::vec3(Yawrotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
			glm::mat4 Pitchrotation = glm::rotate(glm::mat4(1.0f), glm::radians(ally.pitch), Pitchaxis);
			ally.front = glm::normalize(glm::vec3(Pitchrotation * Yawrotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
			glm::mat4 Rollrotation = glm::rotate(glm::mat4(1.0f), glm::radians(-ally.roll), ally.front);
			ally.rotation = Rollrotation * Pitchrotation * Yawrotation;
		}
		ally.position += ally.velocity * deltatime * ally.front;
	}
	for (Npc& bomber : bombers) {
		if (bomber.position.z >= 1500) {
			mycamera.state = -3;
			return;
		}
		if (bomber.state == 0) {
			createParticle(bomber.position, mycamera);
			bomber.front -= glm::vec3(0.0f, 1.0f, 0.0f) * deltatime;
			bomber.position -= 6 * deltatime;
			if (bomber.position.y < 0) {
				bomber.state = -1;
			}
		}
		bomber.position += bomber.velocity * deltatime * bomber.front;
	}
}






void NPCs::createParticle(glm::vec3& position,Camera& mycamera) {
	if (glm::distance(position, mycamera.Position) <= 1000.0f) {
		Particle particle;
		particle.position.x = position.x + (float)dist(engine)/10;
		particle.position.y = position.y + (float)dist(engine)/10;
		particle.position.z = position.z + (float)dist(engine)/10;
		particle.lifetime = 2.0f;
		particle.color = glm::vec3(0.0f);
		particle.alpha = 1.0f;
		particles.push_back(particle);
	}
}
void NPCs::updateParticles() {
	for (int i = 0; i < particles.size();) {
		particles[i].lifetime -= deltatime;
		float lifeRatio = 1.0f - (particles[i].lifetime / 2.0f);
		particles[i].color = glm::vec3(lifeRatio * 0.6f);
		particles[i].alpha = glm::clamp(particles[i].lifetime * 0.8f, 0.0f, 1.0f);
		if (particles[i].lifetime <= 0.0f) {
			particles.erase(particles.begin() + i);
		}
		else {
			i++;
		}
	}
}
void NPCs::renderParticles(Shader& particleshader) {
	glBindVertexArray(particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	particleshader.use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(Particle), particles.data());
	glDrawArrays(GL_POINTS, 0, particles.size());
	glDisable(GL_BLEND);
	glBindBuffer(GL_ARRAY_BUFFER,particleVBO);
	glBindVertexArray(0);
}