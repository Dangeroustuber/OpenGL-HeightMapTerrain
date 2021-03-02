#pragma once

#include <vector>
#include <string>
#include "stb_image.h"
#include <cstring>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"

class HeightMap {
private:
	
	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

	int height, width, bytes;
	GLfloat texCoord1;
	GLfloat texCoord2;

    std::vector<std::vector<float>> heightData;
	std::vector<glm::vec3> treePositions;
	std::vector<glm::vec3> fishPositions;
	std::vector<glm::vec3> deerPositions;

	std::unique_ptr<Material> heightMat;

	std::unique_ptr<Shader> shader;

	std::shared_ptr<VertexArray>		heightMapVAO;
	std::shared_ptr<VertexBuffer>		heightMapVBO;
	std::shared_ptr<VertexBufferLayout> heightMapVBLayout;
	std::shared_ptr<IndexBuffer>		heightMapIBO;

	std::shared_ptr<Renderer> heightMapRenderer;
	
public:

	HeightMap();

	void getHeightDataFromImage(const std::string& fileName);

	void createHeightMapfromData();

	std::vector<glm::vec3> getTreepos() { return treePositions; }
	std::vector<glm::vec3> getFishpos() { return fishPositions; }
	std::vector<glm::vec3> getDeerpos() { return deerPositions; }


	std::vector<std::vector<float>> getHeightData() { return heightData; }

	int getWidth() { return width; }
	void drawHeightMap(glm::mat4 model, glm::mat4 projection, std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader);	
	void drawMinimap(glm::mat4 model, glm::mat4 projection, std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader);
	int getHeight() { return height; }

};

