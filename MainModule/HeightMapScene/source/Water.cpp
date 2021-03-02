#include "Water.h"
#include <math.h> 

/**
*  Water class that in an adaptation of previous FLOOR class. This class works well in this scenario because of it's dynamic size.
*  Just pass in the size in integer arguments and it will generate the appropriate quad.
*
*  @name Map.cpp
*  @author(s) Elvis Arifagic
*/

Water::Water()
{
	waterPos = glm::vec3(0.0f, -60.0f, 0.0f);
	generateWater(1081, 1081);

	waterMat = std::make_unique<Material>();
	waterMat->getTexture("assets/textures/water.png"); // IMPORTNAT FORMAT OF ANY TEX MUST BE 32RBA TOO MANY HOuRS LOST ON THIS
	waterMat->loadTextureA();
}

/**
*   Destructor for map object
*/
Water::~Water(){}


void Water::generateWater(int x, int y)
{
	float tilesX = static_cast<float>(x);
	float tilesY = static_cast<float>(y);

	//Indices for the floor vertices
	std::vector<GLuint> indices =
	{
		0, 1, 2,
		1, 3, 2
	};

	//Vertices for the floor - Position - UV texCoord - Normals
	std::vector<GLfloat> vertices =
	{
		// x        y       z          u     v        nx     ny    nz
		 0.0f,    0.0f,    tilesY,    0.0f,  0.0f,	 0.0f, -1.0f, 0.0f, // 0
		 tilesX,  0.0f,    tilesY,    25.0f, 0.0f,	 0.0f, -1.0f, 0.0f, // 1
		 0.0f,    0.0f,    0.0f,      0.0f,  25.0f,	 0.0f, -1.0f, 0.0f, // 2
		 tilesX,  0.0f,    0.0f,      25.0f, 25.0f,  0.0f, -1.0f, 0.0f  // 3
	};

	//Calculate the normals of the object. 
	shader->calculateAverageNormals(indices, 6, vertices, 32, 8, 5);

	waterVAO = std::make_shared<VertexArray>();
	waterVAO->bind();

	waterVBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(GLfloat));
	waterVBO->bind();

	waterVBLayout = std::make_unique<VertexBufferLayout>();
	waterVBLayout->Push<float>(3);
	waterVBLayout->Push<float>(2);
	waterVBLayout->Push<float>(3);

	waterVAO->addBuffer(*waterVBO, *waterVBLayout);
	waterIBO = std::make_shared<IndexBuffer>(&indices[0], indices.size());
}

/**
*   Draw the map object and use the camera, texture, model, projection, shader and position from the Game class. 
*
*   @param  model        - Sends the model matrix from the Game class
*   @param  projection   - Sends the projection matrix from the Game class
*   @param  camera		 - Sends the camera object from the Game class
*   @param  shader		 - Sends the shader object from the Game class
* 
*	@see useShader(), getModelLocation(), getProjectionLocation(), getViewLocation(),useTexture(), drawElements()
* 
*/
void Water::draw(glm::mat4 model, glm::mat4 projection, glm::vec3 position, std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader)
{
	shader->useShader();

	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();
	
	model = glm::translate(model, position);
	waterMat->useTexture();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	waterRenderer->drawElements(waterVAO, waterIBO);
}