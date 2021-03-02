#pragma once

#include <iostream>
#include <set>
#include <cmath>
#include <vector>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include "GLWindow.h"
#include "Water.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Skybox.h"
#include "Moose.h"

#include "Material.h"
#include "Renderer.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "HeightMap.h"
#include "Tree.h"
#include "Eagle.h"
#include "Plane.h"
#include "Fish.h"
#include "Deer.h"

class Game 
{

private:

	GLfloat deltaTime;
	GLfloat lastTime;
	GLfloat time;
	GLfloat now;

	int numberOfTrees;

	std::shared_ptr<Moose> moose;
	std::shared_ptr<Eagle> eagle;
	std::shared_ptr<Plane> plane;
	std::shared_ptr<Water> water;
	std::shared_ptr<Fish> fish;
	std::shared_ptr<Tree> tree;
	std::shared_ptr<Deer> deer;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<Skybox> skybox;
	std::vector<std::string> skyboxFaces;
	std::unique_ptr<HeightMap> heightmap;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> screenShader;
	std::shared_ptr<Shader> heightmapshader;
	std::shared_ptr<Shader> fishShader;
	std::shared_ptr<Shader> treeShader;
	std::shared_ptr<Shader> minimapShader;
	std::shared_ptr<Shader> deerShader;


	std::shared_ptr<Camera> camera;

	std::shared_ptr<GLWindow> mainWindow;
	
	std::shared_ptr<DirectionalLight> mapLight;
	std::shared_ptr<DirectionalLight> fishLight;
	PointLight pointLights[15];
	SpotLight spotLights[3];

	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;		
	GLuint uniformSkyBoxView;
	GLuint uniformSkyBoxProjection;
	GLuint uniformEyePosition;
	GLuint uniformAmbientIntensity;
	GLuint uniformDiffuseIntensity;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	GLuint uniformMinimapTexture;

	GLuint fishView;
	GLuint fishProjection;

	glm::mat4 model;
	glm::mat4 projection;
	glm::mat4 projectionMinimap;

	glm::mat4 fishProjectionMatrix;	
	glm::mat4 fishViewMatrix;

	glm::mat4 skyboxProjection;
	glm::mat4 skyboxViewMatrix;

	std::shared_ptr<VertexArray>		minimapVAO;
	std::shared_ptr<VertexBuffer>		minimapVBO;
	std::shared_ptr<VertexBufferLayout> minimapVBLayout;
	std::shared_ptr<IndexBuffer>		minimapIBO;
	std::unique_ptr<FrameBuffer>		framebuffer;

	std::vector<std::vector<float>> heightVector;
	glm::vec3 lowerLight;

public:

	Game();
	~Game();

	void generateGame(std::shared_ptr<GLWindow>& mainWindow);
	void updateGame(std::shared_ptr<GLWindow>& mainWindow);


	void generateMinimap(std::shared_ptr<GLWindow>& mainWindow);
	void generateMinimapMVP();
	void updateMinimap();


	void generateShaders();
	void generateLights();

	void generateMVP();

	void updateMVP();
	void updateTime();

};
