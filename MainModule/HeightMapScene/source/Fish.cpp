#include "Fish.h"

/**
*   Fish class that creates 10000 fish spread around in the water zones without going below or above the water.
*   You can always add more fish by just decreasing the space between them. That increases the amount you can have before
*   you reach heightmap data that can't be read.
* 
*   @name Fish.cpp
*   @author Elvis Arifagic.
*/



/**
*   Fish constructor that sets amount of 10000 instanced fish.
*/
Fish::Fish(std::vector<glm::vec3> fishPos) {
	numberOfFish = 10000; // wow!
	fishPositions = fishPos;
	loadFishModel();
}

/**
*   Default fish constructor, must have.
*/
Fish::Fish() {}

/**
*   VERY IMPORTANT! frees the large memory held by the integer number "numberofFish".
*/
Fish::~Fish() {
	free(modelMatrices); // notice free instead of delete.
}

/**
* This function creates the fish model, loads it's texture. Then it procceds to give every instance of fish model a different location.
* Fish get the data for positions only relating to the water quad height which makes for a nice effect.
* The model also has a small space between each one, if it didn't have this they would all spawn at the edge.
* 
* @see loadModel, bind(), addBufferDivisor, unbind().
*/
void Fish::loadFishModel() {
	int counter = 0;
	int spaceBetweenFish = 10.0f;
	fishModel = std::make_unique<Model>();
	fishModel->loadModel("assets/models/fish.obj");
	modelMatrices = new glm::mat4[numberOfFish];

	for (int i = 0; i < numberOfFish; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, fishPositions[static_cast<std::vector<glm::vec3, std::allocator<glm::vec3>>::size_type>(i) + counter]); // casting to wider type.
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // Model is too large, scaling down.
		modelMatrices[i] = model;
		counter += spaceBetweenFish; // this works as you would expect, going from fish to fish adding space.
	}

	instancedVAO = std::make_shared<VertexArray>();
	instancedVAO = fishModel->getVertexArray();
	instancedVAO->bind();

	instancedVBO = std::make_shared<VertexBuffer>(&modelMatrices[0], numberOfFish * sizeof(glm::mat4));
	instancedVBO->bind();

	instancedVAO->addBufferDivisor();

	instancedVAO->unbind();
}

/*
*  Draw call for rendering the instaces of Fish.
*/
void Fish::drawFish() {
	fishModel->renderInstanced(numberOfFish);
}

/*
* Used for previous method of giving fish random spawn positions. Might be handy for something else.
*
* @param min - minimum number range.
* @param max - maximum number range.
*/
int Fish::calculateRandomNumber(int min, int max) {
		unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 engine(seed); // mersenne twister engine for good PRNG.
		std::uniform_int_distribution<> dist(min, max);

		auto randomNum = dist(engine);
		return randomNum;
}
