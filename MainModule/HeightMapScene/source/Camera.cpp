#include "Camera.h"

/**
*   This class defines the camera and the player. The game is in first person
*	which in effect makes the camera the player as well. Camera is locked on pitch.
*	If you press 'O' your camera position will become the plane's position.
*
*  @name Camera.cpp
*  @author(s) Elvis Arifagic
*/

/**
*   Constructor for camera object. This is the one acts as the default for any object.
*	Roll is not needed for this game.
*
*   @param     heightdata	   - Vector with integers between 0-1081 that make up the map.
*   @param     startPosition  - Where the camera is initially located in the world space.
*   @param     startUp        - Direction of the world's up direction (+y is up).
*   @param     startYaw       - Giving the camera a non-turned view to start.
*   @param     startPitch     - How much the camera looks up and or down when starting.
*   @param     startMoveSpeed - Movement speed of the camera
*   @param     startTurnSpeed - Turn rate speed for the camera.
*
*	@see	   update()
*/

Camera::Camera(std::vector<std::vector<float>> heightdata, glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition; // initialize standard constructor way with passed user params.
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f); // mostly abstracting lab03 code.

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	heightVector = heightdata;

	update();

	std::vector<GLuint> indices =
	{
		 0, 1, 2,
		 1, 3, 2,
	};


	std::vector<GLfloat> vertices =
	{
		//x        y       z          u     v       
		0.0f,    0.0f,    2.0f,      0.0f,  0.0f,	  // 0
		2.0f,    0.0f,    2.0f,      1.0f,  0.0f,	  // 1
		0.0f,    0.0f,    0.0f,      0.0f,  1.0f,	  // 2
		2.0f,    0.0f,    0.0f,      1.0f,  1.0f     // 3
	};

	miniMapCameraVAO = std::make_shared<VertexArray>();
	miniMapCameraVAO->bind();

	miniMapCameraVBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(GLfloat));
	miniMapCameraVBO->bind();

	miniMapCameraVBLayout = std::make_unique<VertexBufferLayout>();
	miniMapCameraVBLayout->Push<float>(3);
	miniMapCameraVBLayout->Push<float>(2);

	miniMapCameraVAO->addBuffer(*miniMapCameraVBO, *miniMapCameraVBLayout);
	miniMapCameraIBO = std::make_shared<IndexBuffer>(&indices[0], indices.size());

	miniMapPacmanMat = std::make_shared<Material>();
	miniMapPacmanMat->getTexture("assets/textures/pacman_minimap.png");
	miniMapPacmanMat->loadTextureA();

	static const char* vShader = "assets/shaders/texture.vert";
	static const char* fShader = "assets/shaders/texture.frag";

	shader = std::make_shared<Shader>();
	shader->createShaderFromFile(vShader, fShader);

	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();
}

/**
*   Destructor for camera object. Cameras are used as smart pointers.
*
*/
Camera::~Camera() {}

/**
*   Users key control for movement of the camera.
*
*   @param     keys      - Pointer to relevant the ascii key inputs.
*   @param     deltaTime - Calculated delta time to have uniformity in frame-rate.
*/
void Camera::keyControls(bool* keys, GLfloat deltaTime) {

	GLfloat speed = moveSpeed * deltaTime;
	if (keys[GLFW_KEY_W]) 
	{
		position += front * speed;
	}

	if (keys[GLFW_KEY_S]) 
	{
		position -= front * speed;
	}

	if (keys[GLFW_KEY_A]) 
	{
		position -= right * speed;
	}

	if (keys[GLFW_KEY_D]) 
	{
		position += right * speed;
	}
}


/**
*   Makes the camera able to be moved dynamically in the world space.
*	The pitch is locked at 89 degrees such that you can't see behind yourself.
*
*   @param     changeX - Amount of mouse movement in X direction.
*   @param     changeY - Amount of mouse movement in y direction.
*   @see       update()
*/
void Camera::mouseControl(GLfloat changeX, GLfloat changeY) 
{
	changeX *= turnSpeed;
	changeY *= turnSpeed;

	yaw += changeX;
	pitch += changeY;

	if (pitch > 89.0f) 
	{
		pitch = 89.0f;
	} // lock the up-and down scroll of the mouse.

	if (pitch < -89.0f) 
	{
		pitch = -89.0f;
	}
	update();
}


/**
*   Retrieves the class data member for the position of the camera.
*
*	@return glm::vec3 - vector with the position
*/
glm::vec3 Camera::getCameraPosition()
{
	return position;
}

/**
*   Retrieves the direction the camera is looking.
*	Utilizing glm to get correct value.
*
*   @return glm::vec3 - vector containing "front" which is where the camera points.
*/
glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

/**
*   Utilizes glm library to give the correct view matrix for the view space transformation.
*
*   @return glm::mat4 - returns view matrix for view space.
*/
glm::mat4 Camera::calculateViewMatrix() 
{
	//can place an object to lookAt where "position + front" is. Third-person view
	return glm::lookAt(position, position + front, up);
}

/**
*   Calculates the view matrix for the 3d projection of the map.
*
*   @return glm::mat4 - returns the view matrix.
*/
glm::mat4 Camera::calculateMinimapView()
{
	
	glm::vec3 camPos(28, 73, 36); //For 2D minimap view
	return glm::lookAt(camPos, camPos + glm::vec3(0, -100, 0), glm::vec3(0, -1, -1));
}

/**
*   Update function which in real time updates values making the camera move.
*/
void Camera::update() 
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch)); // if you want to fly, use this instead of 0.0f
	//front.y = 0.0f;
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}