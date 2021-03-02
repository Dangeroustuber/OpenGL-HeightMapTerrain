#include "Eagle.h"

/**
*   Eagle class for static object in the scene. This model overlooks the water hunting for fish!
*	That's why i've chosen it to be static.
*	
*  @name Eagle.cpp
*  @author(s) Elvis Arifagic
*/

/**
*   Default constructor that sets the position and loads the eagle model.
*/
Eagle::Eagle() {
	loadEagleModel();
	position = glm::vec3(273.0f, -22.0f, 165.0f); // positioned on the side of a hill.
}

/**
*   Default destructor, all smart pointers.
*/
Eagle::~Eagle() {}

/**
*   Uses core library functionality to load eagle model.
*/
void Eagle::loadEagleModel() {
	eagleModel = std::make_unique<Model>();
	eagleModel->loadModel("assets/models/Eagle02.obj");
}

/**
*   Draws the eagle in the correct world space. OBJ had to be scaled and rotated.
*
*   @param camera - for getting matrices.
*   @param shader - which shader should the eagle use.
*   @param position - used for translation.
*   @param model - for moving object.
*   @param projection - for showing object correctly in world.
*/
void Eagle::drawEagle(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader, glm::vec3 position, glm::mat4 model, glm::mat4 projection) {

	shader->useShader();

	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();
	uniformModel = shader->getModelLocation();


	model = glm::translate(model, position);

	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	eagleModel->renderElements();
}
