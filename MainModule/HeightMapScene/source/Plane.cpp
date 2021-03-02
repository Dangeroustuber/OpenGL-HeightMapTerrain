#include "Plane.h"

/**
*   This class constructs a plane from a .OBJ and makes it go in square form.
*	The plane is given without texture.	
* 
*   @name Plane.cpp
*   @author Elvis Arifagic.
*/


/*
*  Constructor that sets the planes velocity in +z to start and places it on the edge of the map to begin with.
*/
Plane::Plane() {
	velocity = glm::vec3(0.0f, 0.0f, 30.0f); // plane starts moving +z direction.
	loadPlaneModel();
	position = glm::vec3(25.0f, 80.0f, 0.0f); // start pos of plane
}

/*
*  Default deconstructor.
*/
Plane::~Plane() {}

/*
*  Plane loading, plain texture gets added.
*/
void Plane::loadPlaneModel() {
	planeModel = std::make_unique<Model>();
	planeModel->loadModel("assets/models/Plane.obj");
}

/*
*  Draw call for plane that also handles it's rotation.
*
*  @param camera - for getting view matrix.
*  @param shader - Which shader to use on object.
*  @param position - updating position vector in game loop
*  @param model - used for translation.
*  @param projection - Putting object infront of camera.
*  
*/
void Plane::drawPlane(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader, glm::vec3 position, glm::mat4 model, glm::mat4 projection) {
	shader->useShader();

	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();
	uniformModel = shader->getModelLocation();

	model = glm::translate(model, position);

	float angle = 0.0f;
	if (velocity.x == 30.0f) { angle = 90.0f; };
	if (velocity.z == -30.0f){ angle = -180.0f;  }; // These turn the plane.
	if (velocity.x == -30.0f) { angle = 270; };

	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	planeModel->renderElements();
}

/*
*  This function makes the plane move in a square fashion using IF statements.
*  There are arbitrary positions that have been selected by me and those are turning points for the plane.
*  The draw call handles the updating of the rotation during runtime.
*
*  @param dt - Holding delta time given in main game loop.
*/
void Plane::movePlane(float dt) {
	
	float delta = dt;

	position += velocity * dt;

	if (velocity.x == 30 || velocity.x == -30) {
		if (position.x > 570.0f) {
			velocity = glm::vec3(0.0f, 0.0f, -30.0f); // this is a neat trick that creates a square path for the plane!
			position.x = 569.0f;
		}
		if (position.x < 0.0f) {
			velocity = glm::vec3(0.0f, 0.0f, 30.0f);
			position.x = 5.0f;
		}
	}

	if (velocity.z == 30 || velocity.z == -30) {
		if (position.z > 570.0f) {
			velocity = glm::vec3(30.0f, 0.0f, 0.0f);
			position.z = 569.0f;
		}
		if (position.z < 0.0f) {
			velocity = glm::vec3(-30.0f, 0.0f, 0.0f);
			position.z = 5.0f;
		}
	}
}

