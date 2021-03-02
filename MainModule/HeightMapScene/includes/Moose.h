#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <random>
#include <chrono>

#include "GLWindow.h"
#include "Camera.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"

class Moose {
private:

		
	std::unique_ptr<Model> MooseModel;
	std::unique_ptr<Material> MooseMat;
	std::unique_ptr<Shader> MooseShader;
	std::unique_ptr<Material> MooseMaterial;
	
	glm::vec3 velocity;
	glm::vec3 position;

	glm::mat4 projection;
	glm::mat4 model;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

public:

	Moose();
	~Moose();

	void loadMooseModel();
	void drawMoose(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader, glm::vec3 position, glm::mat4 model, glm::mat4 projection);

	void moveMoose(float dt);

	glm::vec3 getPosition() { return position; }
	glm::vec3 getVelocity() { return velocity; }


};
