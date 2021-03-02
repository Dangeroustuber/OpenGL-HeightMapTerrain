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


class Eagle {
private:


	std::unique_ptr<Model> eagleModel;
	std::unique_ptr<Material> eagleMat;
	std::unique_ptr<Shader> eagleShader;

	glm::vec3 velocity;
	glm::vec3 position;

	glm::mat4 projection;
	glm::mat4 model;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

public:

	Eagle();
	~Eagle();

	void loadEagleModel();
	void drawEagle(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader, glm::vec3 position, glm::mat4 model, glm::mat4 projection);

	glm::vec3 getPosition() { return position; }
	glm::vec3 getVelocity() { return velocity; }


};
