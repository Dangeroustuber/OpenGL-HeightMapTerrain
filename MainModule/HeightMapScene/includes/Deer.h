#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <random>
#include <chrono>

#include "GLWindow.h"
#include "Camera.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"

class Deer {
private:
	
	std::unique_ptr<Model> deerModel;
	std::unique_ptr<Material> deerMat;
	std::unique_ptr<Shader> deerShader;

	std::shared_ptr<VertexArray> instancedVAO;
	std::shared_ptr<VertexBuffer> instancedVBO;
	std::shared_ptr<VertexBufferLayout> instancedVBLayout;

	std::vector<glm::vec3> deerPositions;

	int amountOfDeer;

	glm::vec3 velocity;
	glm::vec3 position;

	glm::mat4 projection;
	glm::mat4 model;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

	glm::mat4* modelMatrices; // Raw because of shader things.

public:
	Deer();
	Deer(std::vector<glm::vec3> deerPos);
	~Deer();

	void loadDeerModel();
	void drawDeer();

	glm::vec3 getPosition() { return position; }
	glm::vec3 getVelocity() { return velocity; }

};
