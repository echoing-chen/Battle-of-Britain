#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Shader;
class Camera;
class Model;
class Bullets;
extern float deltatime;
extern glm::vec3 initialDir;
extern glm::vec3 worldUp;


struct Npc {
	glm::vec3 position;
	float yaw;
	float pitch;
	float roll;
	glm::vec3 front;
	glm::mat4 rotation;
	glm::mat4 scale;
	float velocity;
	int state;  //state=-1ËÀÍö state=0ÖÐµ¯ state=1Õý³£
};

struct Particle {
	glm::vec3 position;
	glm::vec3 color;
	float alpha;
	float lifetime;
};

class NPCs {
public:
	int allyf;
	int bf109f;
	std::vector<Npc> allies;
	std::vector<Npc> bf109s;
	std::vector<Npc> bombers;
	GLuint alliesVBO;
	GLuint bf109sVBO;
	GLuint bombersVBO;
	GLuint particleVAO;
	GLuint particleVBO;
	std::vector<Particle> particles;
	NPCs() {
		allyf = 0;
		bf109f = 0;
		allies = {};
		bf109s = {};
		bombers = {};
		particles = {};
	}
	void initnpcs(Model ally_model, Model bf109_model, Model bomber_model);
	void stage1();
	void stage1AI(Bullets& bullets, Camera& mycamera);
	void renderstage1npc(Shader& npcsshader,Model& ally_model,Model& bf109_model);
	void updatestage1npc(Camera& mycamera);
	void stage2();
	void stage2AI(Bullets& bullets, Camera& mycamera);
	void renderstage2npc(Shader& npcsshader, Model& ally_model, Model& bomber_model);
	void updatestage2npc(Camera& mycamera);
	void createParticle(glm::vec3& position, Camera& mycamera);
	void updateParticles();
	void renderParticles(Shader& particleshader);
};