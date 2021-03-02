#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "LevelLoader.h"
#include "GLWindow.h"
#include "Material.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"

class Water
{
private:

	GLfloat startX;
	GLfloat endX;
	GLfloat startZ;
	GLfloat endZ;

	int tilesX;
	int tilesZ;

	glm::vec3 waterPos;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

	std::unique_ptr<Shader> shader;
	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;

	std::shared_ptr<VertexArray>		waterVAO;
	std::unique_ptr<VertexBuffer>		waterVBO;
	std::unique_ptr<VertexBufferLayout> waterVBLayout;
	std::shared_ptr<IndexBuffer>		waterIBO;

	std::shared_ptr<Renderer> waterRenderer;

	std::unique_ptr <Material> waterMat;
public:
	Water();
	~Water();

	void generateWater(int x, int y);

	void draw(glm::mat4 model, glm::mat4 projection, glm::vec3 position, std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader);
	void drawMinimap(glm::mat4 model, glm::mat4 projection, std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader);

	glm::vec3 getWaterPos() { return waterPos; }
};

