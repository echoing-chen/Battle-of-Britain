#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "stb_image.h"
#include "model.hpp"
#include "shader.hpp"

void renderscene(Model& scene,Shader& modelshader) {
	modelshader.use();
	glm::mat4 model = glm::mat4(1.0f)*glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f));
	modelshader.setMat4("model", model);
	scene.Draw(modelshader);
}