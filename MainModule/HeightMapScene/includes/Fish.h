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


class Fish { // these fish are instance drawn! How cool is that?!
private:

	std::unique_ptr<Model> fishModel;
	std::unique_ptr<Material> fishMat;
	std::unique_ptr<Shader> fishShader;

	std::shared_ptr<VertexArray> instancedVAO;
	std::shared_ptr<VertexBuffer> instancedVBO;
	std::shared_ptr<VertexBufferLayout> instancedVBLayout;

	std::vector<glm::vec3> fishPositions;

	int numberOfFish;

	glm::vec3 velocity;
	glm::vec3 position;

	glm::mat4 projection;
	glm::mat4 model;

	glm::mat4* modelMatrices; // Raw because of shader things.

	GLuint uniformView;
	GLuint uniformProjection;

public:
	Fish(std::vector<glm::vec3> fishPos);
	Fish();
	~Fish();

	void loadFishModel();
	void drawFish();

	glm::vec3 getPosition() { return position; }
	glm::vec3 getVelocity() { return velocity; }

	int calculateRandomNumber(int min, int max);
};