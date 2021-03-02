#include "Moose.h"


/**
*   The moose is a model that moves freely throughout the generated heightmap.
*	Textures work for this model!.
*
*   @name Moose.cpp
*   @author Elvis Arifagic.
*/


/*
*  Default constructor that sets the velocity and position of the moose.
*
*/
Moose::Moose() { 
	velocity = glm::vec3(0.0f, 0.0f, 1.0f);
	position = glm::vec3(565.0f, -42.0f, 270.0f); // positioned in cliffside
	loadMooseModel();
}

/*
*  Default destructor for moose, all objects are smart pointers.
*/
Moose::~Moose() {}

/*
*  Simple loader function for the moose model. LoadModel also pastes the texture.
*/
void Moose::loadMooseModel() {
	MooseModel = std::make_unique<Model>();
	MooseModel->loadModel("assets/models/12959_Moose_Female_v1_l3.obj");
}

/*
*  Draw call for the moose. Model was not in desirable size, location or scale. Thus those have been adjusted.
*
*  @param camera - for retrieving view matrix.
*  @param shader - Which shader to use on the moose
*  @param position - To update the mooses position in game time
*  @param model - For translating to world space.
*  @param projection - Making object visible to camera.
*
*/
void Moose::drawMoose(std::shared_ptr<Camera> camera,std::shared_ptr<Shader> shader, glm::vec3 position, glm::mat4 model, glm::mat4 projection) {

	shader->useShader();

	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();
	uniformModel = shader->getModelLocation();

	model = glm::translate(model, position);

	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	MooseModel->renderElements();
}

/*
*  Simple function that just moves the mooses position vector alongs it's given velocity value.
*
*  @param dt - holding delta time given in game loop.
*/
void Moose::moveMoose(float dt) {
	float delta = dt;
	position += velocity * dt;
}
