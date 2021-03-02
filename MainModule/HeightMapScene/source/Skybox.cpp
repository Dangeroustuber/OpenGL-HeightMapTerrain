#include "Skybox.h"


/**
*   Class that creates a cube around the camera.
*   This is done through specifying in correct order which images you want to plaster
*   on which side of the cube.
*   Skybox handels it's own cubemap texutre creation and binding.
*
*   @name Skybox.cpp
*   @author Elvis Arifagic.
*/


/*
*  Constructor that sets the planes velocity in +z to start and places it on the edge of the map to begin with.
*/
Skybox::Skybox() {}

/*
*  Construtor that creates the entirety of the skybox. It starts with loading the image and then using the cubemap 2d function.
*  From there it defines the vertices and indices and creates itself. There is no need for tex coords or normals.
*
*  @param faceLocations - vector holding the pictures used for skybox.
*/
Skybox::Skybox(std::vector<std::string> faceLocations) {

	static const char* skyboxV = "assets/shaders/skybox.vert";
	static const char* skyboxF = "assets/shaders/skybox.frag";

	skyboxShader = std::make_shared<Shader>();
	skyboxShader->createShaderFromFile(skyboxV, skyboxF);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (size_t i = 0; i < 6; i++) {
		unsigned char* texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitdepth, 0);
		if (!texData) {
			std::cout << "Could not retrieve the facelocation(s) ---> " << faceLocations[i].c_str();
			return;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	std::vector<GLfloat> verticePlaceHolder = {
		-40.0f, 40.0f, -40.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-40.0f, -40.0f, -40.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		40.0f, 40.0f, -40.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		40.0f, -40.0f, -40.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-40.0f, 40.0f, 40.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		40.0f, 40.0f, 40.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-40.0f, -40.0f, 40.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		40.f, -40.0f, 40.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	vertices.insert(end(vertices), verticePlaceHolder.begin(), verticePlaceHolder.end());

	std::vector<GLuint> indicesPlaceHolder = {
		
		0, 1, 2,// front
		2, 1, 3,
		
		2, 3, 5,// right
		5, 3, 7,
		
		5, 7, 4,// back
		4, 7, 6,
		
		4, 6, 0,// left
		0, 6, 1,
		
		4, 0, 5,// top
		5, 0, 2,
		
		1, 6, 3,// bottom
		3, 6, 7
	};

	
	indices.insert(end(indices), indicesPlaceHolder.begin(), indicesPlaceHolder.end());

	skyboxVAO = std::make_shared<VertexArray>();
	skyboxVAO->bind();

	skyboxVBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(GLfloat));
	skyboxVBO->bind();

	skyboxVBLayout = std::make_unique<VertexBufferLayout>();
	skyboxVBLayout->Push<float>(3);
	skyboxVBLayout->Push<float>(2);
	skyboxVBLayout->Push<float>(3);

	skyboxVAO->addBuffer(*skyboxVBO, *skyboxVBLayout);
	skyboxIBO = std::make_shared<IndexBuffer>(&indices[0], indices.size());
}

/*
*  draw call that disables the depth mask for the duration of the function. Also uses neat trick to perserve everything but rotation operation.
*  The conversion from glm::mat3->glm::mat4 discards the last column and row of 1's. Because matrices are calculated from right to left.
*  This function also handles texture binding, this is not done through renderModule's material class.
*
*  @param viewMatrix - Getting the cameras viewMatrix.
*  @param projectionMatrix - getting projection location from shader.
*  @param camera - which camera to get view matrix from, main game camera.
*/
void Skybox::drawSkyBox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, std::shared_ptr<Camera> camera) {
	
	glDepthMask(GL_FALSE);

	skyboxShader->useShader();

	uniformProjection = skyboxShader->getProjectionLocation();
	uniformView = skyboxShader->getViewLocation();

	viewMatrix = camera->calculateViewMatrix();

	viewMatrix = glm::mat4(glm::mat3(viewMatrix)); // no rotation, just want 3x3 data

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	skyboxRenderer->drawElements(skyboxVAO, skyboxIBO);
	glDepthMask(GL_TRUE);
}

/*
*  Default skybox deconstructor, everything is a smart pointer.
*/
Skybox::~Skybox() {}
