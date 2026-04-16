#ifndef SKY_HPP
#define SKY_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "stb_image.h"



class Shader;
class Model;

struct Sky {
	GLuint textureID;
	std::vector<std::string> faces;
};
class Skies {
public:
	Sky sky1;
	Sky sky2;
    GLuint skyVAO;
    GLuint skyVBO;
	Skies() {
		sky1.faces = { "sky1/right.png","sky1/left.png","sky1/top.png","sky1/bottom.png","sky1/front.png","sky1/back.png" };
		sky1.textureID = loadSky(sky1.faces);
		sky2.faces = { "sky2/right.png","sky2/left.png","sky2/top.png","sky2/bottom.png","sky2/front.png","sky2/back.png" };
		sky2.textureID = loadSky(sky2.faces);
	}
    GLuint loadSky(std::vector<std::string> faces);
    void initsky();
    void rendersky1(Shader& skyshader,Camera& mycamera);
    void rendersky2(Shader& skyshader,Camera& mycamera);
};

#endif