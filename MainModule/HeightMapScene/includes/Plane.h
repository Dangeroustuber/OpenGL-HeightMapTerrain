#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <random>
#include <chrono>

#include "GLWindow.h"
#include "Camera.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"

class Plane {
private:

	std::unique_ptr<Model> planeModel;
	std::unique_ptr<Material> planeMat;
	std::unique_ptr<Shader> planeShader;

	glm::vec3 velocity;
	glm::vec3 position;

	glm::mat4 projection;
	glm::mat4 model;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

public:
	
	 Plane();
	~Plane();

	void loadPlaneModel();
	void drawPlane(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader, glm::vec3 position, glm::mat4 model, glm::mat4 projection);
	void movePlane(float dt);

	glm::vec3 getPosition() { return position; }
	glm::vec3 getVelocity() { return velocity; }
};