#include "Tree.h"

/**
*   Instanced tree class that makes 5000 trees in the scene.
*	These are given random VALID positions in the heightmap through the heightmap class
*   populating a vec3 with the positions, which the constructor recieves.
* 
*   @name Tree.cpp
*   @author Elvis Arifagic.
*/

/*
*  Constructor that sets the amount of trees in the scene and gives takes in heightmap data to use for positioning.
*
*  @param treePos - vector holding all legal tree positions within the heightmap data supplied.
*/
Tree::Tree(std::vector<glm::vec3> treePos) { // Trees have had random spawn positions since i first implemented them but i'm choosing to create 3 forest sites
	numberOfTrees = 3000;
	treePositions = treePos;
	
	//for (int i = 0; i < numberOfTrees; i++) { 
	//	if(i > 25) {
	//		position.x = calculateRandomNumber(39, 192);
	//		position.z = calculateRandomNumber(152, 747);
	//		position.y = calculateNegativeRandomNumber(-53, -45); // this code is actually nice and i want to keep it so im letting it stay.
	//		treePositions.push_back(position);
	//	}
	loadTreeModel();
}

/*
*  Default constructor for trees.
*/
Tree::Tree() {}

/*
*  Freeing the memory that has been allocated for the modelmatrices.
*/
Tree::~Tree() {
	free(modelMatrices);
}

/*
*  This function creates all the positions that the trees will have around on the heightmap.
*  Model was not as i wanted it so i utilize glm to form it. Same math formula is used in fish. 
*/
void Tree::loadTreeModel() {
	int counter = 0;
	int spaceBetweenTrees = 330.0f; // This is in part controlled by the amount of trees.
	treeModel = std::make_unique<Model>();
	treeModel->loadModel("assets/models/10447_Pine_Tree_v1_L3b.obj");
	modelMatrices = new glm::mat4[numberOfTrees];

	for (int i = 0; i < numberOfTrees; i++) {
		model = glm::mat4(1.0f); // have to reset this or everything gets broken
		model = glm::translate(model, treePositions[static_cast<std::vector<glm::vec3, std::allocator<glm::vec3>>::size_type>(i) + counter]); // wide cast.
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		modelMatrices[i] = model;
		counter += spaceBetweenTrees; // this works just fine. Dont need anything fancy.
	}

	instancedTreeVAO = std::make_shared<VertexArray>();
	instancedTreeVAO = treeModel->getVertexArray();
	instancedTreeVAO->bind();

	instancedTreeVBO = std::make_shared<VertexBuffer>(&modelMatrices[0], numberOfTrees * sizeof(glm::mat4));
	instancedTreeVBO->bind();

	instancedTreeVAO->addBufferDivisor();

	instancedTreeVAO->unbind();
}

/*
*  Draw call for instanced trees.
*/
void Tree::drawTree() {
	treeModel->renderInstanced(numberOfTrees);
}

/*
*   Calculates a random number between 0 and the argument value. Can be useful
*
*	@param min - lowest random value
*	@param max - highest random value.
*/
int Tree::calculateRandomNumber(int min, int max) {
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 engine(seed); // mersenne twister engine for good PRNG.
	std::uniform_int_distribution<> dist(min, max);

	auto randomNum = dist(engine);
	return randomNum;
}
