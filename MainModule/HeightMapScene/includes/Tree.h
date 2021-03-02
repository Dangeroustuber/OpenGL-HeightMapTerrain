#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <random>
#include <chrono>

#include "GLWindow.h"
#include "Camera.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"
#include "HeightMap.h"

class Tree {
private:

	std::unique_ptr<Model> treeModel;
	std::unique_ptr<Shader> treeShader;
	std::unique_ptr<Material> treeMaterial;

	std::shared_ptr<VertexArray> instancedTreeVAO;
	std::shared_ptr<VertexBuffer> instancedTreeVBO;
	std::shared_ptr<VertexBufferLayout> instancedTreeVBLayout;

	std::vector<std::vector<float>> height;
    
	glm::vec3 velocity;
	glm::vec3 position;

	std::vector<glm::vec3> treePositions;

	glm::mat4 projection;
	glm::mat4 model;

	GLuint uniformModel;
	GLuint uniformProjection;
	int numberOfTrees;
	glm::mat4* modelMatrices; // Raw because of shader and glm things.

public:

	Tree(std::vector<glm::vec3> treePos);
	Tree();
	~Tree();

	void loadTreeModel();
	void drawTree();

	void generateSpawnPositions();

	glm::vec3 randomSpawnPosition();

	int calculateRandomNumber(int min, int max);
};
