#include "Game.h"


/**
*  The main class of the engine. This class takes in one of every object needed to construct the game.
*  All the functions get ran in the game loop. Aside from objects it constructs shaders and textures.
*
*  @name Game.cpp
*  @author Elvis Arifagic
*/


/**
*   Constructor game object. This works essentially as "game engine startup" mechanism.
*/
Game::Game()
	:projection(0),skyboxProjection(0), deltaTime(0), lastTime(0),
	time(0), now(0), uniformModel(0), uniformView(0), uniformProjection(0),model(1.0f)
{}

/**
*   Destructor for game object, everything is initialized as smart pointers.
*/
Game::~Game(){}

/**
*   retrieves/makes/compiles all the shaders that will be used in the entire game.
*
*   @see - createShaderFromFile()
*/
void Game::generateShaders()
{
	static const char* vShader = "assets/shaders/lights.vert";
	static const char* fShader = "assets/shaders/lights.frag";

	shader = std::make_shared<Shader>();
	shader->createShaderFromFile(vShader, fShader);
	
	static const char* vheightmapShader = "assets/shaders/heightmap.vert";
	static const char* fheightmapShader = "assets/shaders/heightmap.frag";

	heightmapshader = std::make_shared<Shader>();
	heightmapshader->createShaderFromFile(vheightmapShader, fheightmapShader);

	static const char* vfishShader = "assets/shaders/fish.vert";
	static const char* FfishShader = "assets/shaders/fish.frag";

	fishShader = std::make_shared<Shader>();
	fishShader->createShaderFromFile(vfishShader, FfishShader);

	static const char* vtreeShader = "assets/shaders/pinetree.vert";
	static const char* ftreeShader = "assets/shaders/pinetree.frag";

	treeShader = std::make_shared<Shader>();
	treeShader->createShaderFromFile(vtreeShader, ftreeShader);

	static const char* vminiMapShader = "assets/shaders/minimap.vert";
	static const char* fminiMapShader = "assets/shaders/minimap.frag";

	minimapShader = std::make_unique<Shader>();
	minimapShader->createShaderFromFile(vminiMapShader, fminiMapShader);
	
	static const char* vscreenShader = "assets/shaders/screenShader.vert";
	static const char* fscreenShader = "assets/shaders/screenShader.frag";

	screenShader = std::make_unique<Shader>();
	screenShader->createShaderFromFile(vscreenShader, fscreenShader);
	
	static const char* vdeerShader = "assets/shaders/deer.vert";
	static const char* fdeerShader = "assets/shaders/deer.frag";

	deerShader = std::make_unique<Shader>();
	deerShader->createShaderFromFile(vdeerShader, fdeerShader);
}

/**
*   Load/create all the lights that will be in the game
*/
void Game::generateLights()
{
	/* --- The Directional Lights in the scene --- */

	mapLight = std::make_shared<DirectionalLight>(
		1.0f, 1.0f, 1.0f,	 // Color
		0.09f, 1.1f,			 // Ambient, diffuse 
		5.0f, -25.0f, 6.0f); // Direction

	/* --- The Point Lights in the scene --- */

	pointLights[0] = PointLight(
		1.0f, 0.1f, 0.1f,    // Color
		2.5f, 0.8f, 		 // Ambient, diffuse 
		287.0f, -76.0f, 5.0f,  // Position 
		0.1f, 0.1f, 0.1f); 	 // Constant, linear, exponent

	pointLights[1] = PointLight(
		0.9f, 0.0f, 1.0f,    // Color
		2.5f, 0.8f, 		 // Ambient, diffuse 
		287.0f, -71.0f, 123.0f,  // Position 
		0.1f, 0.1f, 0.1f); 	 // Constant, linear, exponent

	pointLights[2] = PointLight(
		0.7f, 0.4f, 0.1f,    // Color
		2.5f, 0.8f, 		 // Ambient, diffuse 
		206.0f, -45.0f, 164.0f,  // Position 
		0.1f, 0.1f, 0.1f); 	 // Constant, linear, exponent

	/* --- The Flash light in the scene --- */

	spotLights[0] = SpotLight(
		1.0f, 1.0f, 1.0f,	 // Color
		1.0f, 2.0f, 		 // Ambient, diffuse 
		0.0f, 0.0f, 0.0f,	 // Position 
		0.0f, 0.0f, 0.0f,	 // Direction
		1.0f, 0.0f, 0.0f,	 // Constant, linear, exponent
		5.0f);				 // Edge
}

/**
*   Generate the entire game. All objects, all needed functions.
*
*   @see - generateShaders(), generateLights(), getTreepos(),
*   getFishpos(), getHeightDataFromImage(),
*   createHeightMapfromData(), getHeightData(), generateMinimap(),
*   generateMinimapMVP()
*/
void Game::generateGame(std::shared_ptr<GLWindow>& mainWindow)
{
	float offset = mainWindow->getBufferWidth() - mainWindow->getBufferHeight() + 40.0f;
	generateShaders();
	generateLights();

	skyboxFaces.push_back("assets/textures/posx.jpg");
	skyboxFaces.push_back("assets/textures/negx.jpg");
	skyboxFaces.push_back("assets/textures/posy.jpg");
	skyboxFaces.push_back("assets/textures/negy.jpg"); // HAVE TO BE IN CORRECT ORDER, posx-negx-posy-negy-posz-negz.
	skyboxFaces.push_back("assets/textures/posz.jpg");
	skyboxFaces.push_back("assets/textures/negz.jpg");

	heightmap = std::make_unique<HeightMap>();
	heightmap->getHeightDataFromImage("assets/textures/Randsf_HeightMap.png");
	std::vector<glm::vec3> treeposition = heightmap->getTreepos(); // important order of operations in this, diffrence can cause nullptr.
	std::vector<glm::vec3> fishposition = heightmap->getFishpos();
	std::vector<glm::vec3> deerposition = heightmap->getDeerpos();
	heightmap->createHeightMapfromData();
	heightVector = heightmap->getHeightData();

	skybox = std::make_shared<Skybox>(skyboxFaces);
	tree = std::make_shared<Tree>(treeposition);
	water = std::make_shared<Water>();
	fish = std::make_shared<Fish>(fishposition);

	moose = std::make_shared<Moose>();
	eagle = std::make_shared<Eagle>();
	plane = std::make_shared<Plane>();
	deer = std::make_shared<Deer>(deerposition);
	
	camera = std::make_shared<Camera>(heightVector, glm::vec3(287, -72.f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -270.0f, 0.0f, 75.0f, 0.03f);
	projection = glm::perspective(glm::radians(45.0f), ((GLfloat)mainWindow->getBufferWidth() / mainWindow->getBufferHeight()), 0.1f, 1200.0f);
	
	projectionMinimap = glm::ortho(0.0f, 1200.0f, -1200.0f, 800.0f, 0.1f, 2000.0f); // ortho for true 2d instead of assignemnt 2 solution

	generateMinimap(mainWindow);
	generateMinimapMVP();
}

/*
*   Generate the minimap view. All objects, all needed functions.
*   in turn this cleans up main.cpp greatly.
*
*   @param mainWindow - to retrieve bufferwidth set by user.
*/
void Game::generateMinimap(std::shared_ptr<GLWindow>& mainWindow) {
	float minimapVertices[] = // vertex attributes for a quad that fills the entire screen.
	{
		//x	 y		 u		v			   Switch coordinates to cover entire screen or only top right
	   0.40f, 1.0f,		0.0f, 1.0f, // 0		-1.0f,  1.0f,  0.0f, 1.0f,		0.40f, 1.0f,	0.0f, 1.0f, 
	   0.40f, 0.25f,	0.0f, 0.0f, // 1		-1.0f, -1.0f,  0.0f, 0.0f,		0.40f, 0.25f,	0.0f, 0.0f,	
	   1.0f,  0.25f,	1.0f, 0.0f, // 2		 1.0f, -1.0f,  1.0f, 0.0f,		1.0f,  0.25f,	1.0f, 0.0f,	

	   0.40f, 1.0f,		0.0f, 1.0f, // 3		-1.0f,  1.0f,  0.0f, 1.0f,		0.40f, 1.0f,	0.0f, 1.0f,	
	   1.0f,  0.25f,	1.0f, 0.0f, // 4		 1.0f, -1.0f,  1.0f, 0.0f,		1.0f,  0.25f,	1.0f, 0.0f,	
	   1.0f,  1.0f,		1.0f, 1.0f  // 5		 1.0f,  1.0f,  1.0f, 1.0f		1.0f,  1.0f,	1.0f, 1.0f 	
	};

	minimapVAO = std::make_shared<VertexArray>();
	minimapVAO->bind();

	minimapVBO = std::make_shared<VertexBuffer>(&minimapVertices, sizeof(minimapVertices));
	minimapVBO->bind();

	minimapVBLayout = std::make_shared<VertexBufferLayout>();
	minimapVBLayout->Push<float>(2);
	minimapVBLayout->Push<float>(2);

	minimapVAO->addBuffer(*minimapVBO, *minimapVBLayout);

	framebuffer = std::make_unique<FrameBuffer>();
	framebuffer->generateFB();	//Frame buffer
	framebuffer->generateTBO(mainWindow->getBufferWidth(), mainWindow->getBufferHeight()); // dynamic size
	framebuffer->generateRBO(mainWindow->getBufferWidth(), mainWindow->getBufferHeight());
	framebuffer->checkStatus();
	framebuffer->unbind();
}

/**
*   Generate the MVP for the minimap. All uniforms needed are set here.
*   Used in generateGame function.
*
*   @see - Shader class
*/
void Game::generateMinimapMVP() {
	uniformModel = minimapShader->getModelLocation();
	uniformProjection = minimapShader->getProjectionLocation();
	uniformView = minimapShader->getViewLocation();
	uniformMinimapTexture = screenShader->getMinimapTextureLocation();
}

/**
*   Updating the minimap as long as the window is open.
*
*   @see  bind(), enableDepth, clear(), useShader(), updateMinimapMVP(), updateTime(),
*		  updateLights(), drawMinimap(), getCameraPosition(), drawMinimapPacman(), unbind(),
*		  move(), disableDepth(), bindTBO(), drawArrays()
*
*/
void Game::updateMinimap() {
	framebuffer->bind();

	renderer->enableDepth(); // enable depth testing (is disabled for rendering view quad)
	renderer->clear(0.1f, 0.1f, 0.1f, 1.0f);

	minimapShader->useShader();

	heightmap->drawMinimap(model, projectionMinimap, camera, minimapShader);

	shader->useShader();
	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMinimap));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateMinimapView()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	updateTime();

	framebuffer->unbind();
	renderer->disableDepth();	 // disable depth test so view quad isn't discarded due to depth test.
	screenShader->useShader();
	framebuffer->bindTBO();
	renderer->drawArrays(minimapVAO);
}


/**	
*	This function can be used for collision detection and is used for smooth gameplay.	
*/
void Game::updateTime()
{
	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	time += deltaTime;
}

/**
*   Updating the game as long as the window is open.
*	Function handles all event updating.
*
*   @param mainWindow - Current open window.
*   @see   useShader(), updateMVP(), updateTime(), keyControls(), mouseControl(), updateLights(),
*		   retrieveKeys(), toggleFlashLight(), clear(), enableDepth(), draw(),
*		   closeWindow(), getViewLocation(), getProjectionLocation(), calculateViewMatrix(),
*		   setDirectionalLight(), setSpotLights(), updateMinimap(), getPosition();
* 
*/
void Game::updateGame(std::shared_ptr<GLWindow>& mainWindow)
{
	renderer->clear(0.1f, 0.1f, 0.1f, 1.0f);
	renderer->enableDepth();
	updateTime(); 	
	camera->keyControls(mainWindow->retrieveKeys(), deltaTime);
	camera->mouseControl(mainWindow->getChangeX(), mainWindow->getChangeY());
	glm::vec3 planePosCamera = plane->getPosition();
	
	shader->useShader();
	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shader->setDirectionalLight(mapLight);
	shader->setPointLights(pointLights, 3);
	shader->setSpotLights(spotLights, 1);
	lowerLight = camera->getCameraPosition();
	lowerLight.y -= 0.1f; // Offsets the flashlight
	spotLights[0].setFlash(lowerLight, camera->getCameraDirection());

	// Toggle the Flash light on and off with the F key.
	if (mainWindow->retrieveKeys()[GLFW_KEY_F]){
		spotLights[0].toggleFlashLight();

		// Set it to false to only catch the button pressed, and not while it's pressed.
		mainWindow->retrieveKeys()[GLFW_KEY_F] = false;
	}

	if (mainWindow->retrieveKeys()[GLFW_KEY_O]) { // to follow plane, hold O!
		camera->changePosition(planePosCamera);
	}	

	glm::vec3 moosePos = moose->getPosition();
	glm::vec3 eaglePos = eagle->getPosition();
	glm::vec3 planePos = plane->getPosition();
	glm::vec3 waterPosition = water->getWaterPos();

	skybox->drawSkyBox(skyboxViewMatrix, projection, camera); // IMPORTANT TO DRAW SKYBOX FIRST OR ELSE IT WON'T WORK!

	/************************************************************************************************/
	heightmapshader->useShader();
	uniformModel = shader->getModelLocation();
	uniformProjection = shader->getProjectionLocation();
	uniformView = shader->getViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	heightmapshader->setDirectionalLight(mapLight);
	heightmapshader->setPointLights(pointLights, 3);
	shader->setSpotLights(spotLights, 1);
	heightmap->drawHeightMap(model, projection, camera, heightmapshader);
	
	/***********************************************************************************************/
	
	fishShader->useShader();
	uniformProjection = fishShader->getProjectionLocation();
	uniformView = fishShader->getViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

	fishShader->setDirectionalLight(mapLight);
	fishShader->setPointLights(pointLights, 3);
	fishShader->setSpotLights(spotLights, 1);
	fish->drawFish(); // <-------- no model, important!

	/***********************************************************************************************/	
	
	treeShader->useShader();
	uniformProjection = treeShader->getProjectionLocation();
	uniformView = treeShader->getViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

	treeShader->setDirectionalLight(mapLight);
	treeShader->setPointLights(pointLights, 3);
	treeShader->setSpotLights(spotLights, 1);
	tree->drawTree(); // <-------- no model, important!

	/***********************************************************************************************/

	deerShader->useShader();
	uniformProjection = deerShader->getProjectionLocation();
	uniformView = deerShader->getViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

	deerShader->setDirectionalLight(mapLight);
	deerShader->setPointLights(pointLights, 3);
	deerShader->setSpotLights(spotLights, 1);
	deer->drawDeer(); // <-------- no model, important!
	
	/***********************************************************************************************/
	
	water->draw(model,projection, waterPosition,camera,shader);
	moose->drawMoose(camera, shader, moosePos, model, projection);
	moose->moveMoose(deltaTime);
	eagle->drawEagle(camera, shader, eaglePos, model, projection);
	plane->drawPlane(camera, shader, planePos, model, projection);
	plane->movePlane(deltaTime);

	updateMinimap();
	glUseProgram(0);
}

