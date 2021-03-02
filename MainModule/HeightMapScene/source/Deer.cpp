#include "Deer.h"

/*
*  Deer class of 500 instanced deer.
*  For deer the logical solution was to just use the same IF statment which determines if trees get their position.
*  A deer make sense to give the same heightmap information to. The spacing of this is quite large as rendering many models more than 
*  500 for the map doesn't make alot of sense.
*  
*  @name Deer.cpp
*  @author Elvis Arifagic.
*/

/*
*  Default deer constructor
*/
Deer::Deer() {

}

/*
*  This construtor sets the amount of deer to be instanced and takes in the position which is gotten when the game is generated.
*  Function also handles loading of model which has a correct texture.
*
*  @param deerPos - vector with positions.
*/
Deer::Deer(std::vector<glm::vec3> deerPos) {
	deerPositions = deerPos;
	amountOfDeer = 200;
	loadDeerModel();
}

/*
* Manually freeing the allocated memory for the matrices.
*/
Deer::~Deer() {
	free(modelMatrices);
}

/*
*  Function that scales rotates and tranlsates the deer into position based on the perviously constructed deer position data.
*  Sets the space quite large as too many deer looks weird on the scene.
*/
void Deer::loadDeerModel() {
	int counter = 0;
	int spaceBetweenDeer = 5000.0f;
	deerModel = std::make_unique<Model>();
	deerModel->loadModel("assets/models/12961_White-Tailed_Deer_v1_l2.obj");
	modelMatrices = new glm::mat4[amountOfDeer];

	for (int i = 0; i < amountOfDeer; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, deerPositions[static_cast<std::vector<glm::vec3, std::allocator<glm::vec3>>::size_type>(i) + counter]);
		model = glm::rotate(model, -90.0f, glm::vec3(1.0f,0.0f,0.0f)); 
		//model = glm::rotate(model, -180.0f, glm::vec3(1.0f,0.0f,0.0f)); 
		//model = glm::rotate(model, 90.0f, glm::vec3(0.0f,1.0f,0.0f)); 
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f)); 		
		modelMatrices[i] = model;
		counter += spaceBetweenDeer; // this works as you would expect, going from fish to fish adding space.
	}

	instancedVAO = std::make_shared<VertexArray>();
	instancedVAO = deerModel->getVertexArray();
	instancedVAO->bind();

	instancedVBO = std::make_shared<VertexBuffer>(&modelMatrices[0], amountOfDeer * sizeof(glm::mat4));
	instancedVBO->bind();

	instancedVAO->addBufferDivisor();

	instancedVAO->unbind();
}

/*
*  Draw call of the amount of deer defined in constructor.
*/
void Deer::drawDeer() {
	deerModel->renderInstanced(amountOfDeer);
}

