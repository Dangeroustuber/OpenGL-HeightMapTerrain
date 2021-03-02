#include "HeightMap.h"

/**
*   Heightmap class that reads in the data from a garyscale image.
*   Data is then used to construct the geometry of the object.
*   Quite large with almost 700 000 vertices.
* 
*   @name HeightMap.cpp
*   @author Elvis Arifagic.
*/

/*
*  Construtor that creates a heightmap when called and loads a simple grass texture on top of it.
*/
HeightMap::HeightMap() {

    heightMat = std::make_unique<Material>();
    heightMat->getTexture("assets/textures/Grass.png");
    heightMat->loadTextureA();
}

/*
* 
*  Function takes in a filename and reads the data into a vector of vectors using a for loop.
*  Some complicated types so auto is used for readability.
*  This function also creates the positions for the fish and trees.
*
*  @param fileName - file containing data to be interpreted.
* 
*/
void HeightMap::getHeightDataFromImage(const std::string& fileName) {

    const auto imageData = stbi_load(fileName.c_str(), &width, &height, &bytes, 0);
    int counter = 0;
    int fishCounter = 0;
    if (imageData == nullptr) {
        std::cout << "Could not load image the image ---> " << fileName << "\n";
        return;
    }
   
    heightData.resize(height, std::vector<float>(width)); // Modify vector and fill with default state object.

    auto pixelPtr = &imageData[0];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            heightData[i][j] = float(*pixelPtr);
            pixelPtr += bytes;
        }
    }

    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            heightData[z][x] = (int)(imageData[counter * bytes]);
            counter++;
            if (heightData[z][x] > 10 && heightData[z][x] < 25) { // clamping height values with MIN and MAX
                fishPositions.push_back(glm::vec3(x, heightData[z][x]- 85.0f, z)); // offset to match water.
            }
            if (heightData[z][x] > 30 && heightData[z][x] < 160) {
                treePositions.push_back(glm::vec3(x, heightData[z][x] - 90.0f, z)); // offset to match instance position of trees realative to my HEIGHTMAP!
                deerPositions.push_back(glm::vec3(x, heightData[z][x] - 90.0f, z)); 
            }
        }
    }
    stbi_image_free(imageData); // Free the image in memory as it's no longer needed.
}

/*
*  Creation of the indices and vertices for the heightmap. Using mostly techniques from assignement 2 and some from 1.
*  UV have been chosen to be smaller considering the 1081*1081 size of the image.
*  Normals are calculated seperately.
*/
void HeightMap::createHeightMapfromData() {
    
    for (int z = 0; z < heightData.size(); z++) {
        for (int x = 0; x < heightData[0].size(); x++) { // go through the data and handle new line cases.

            if (z % 2 == 0) { // do for every other.
                if (x % 2 == 1) {
                    texCoord1 = 0.0f;
                    texCoord2 = 0.05f; // No need to use 1.0f because these datapoints are so small and plentiful.
                    
                } else {
                    texCoord1 = 0.0f;
                    texCoord2 = 0.0f;
                } 
            } else {
                if (x % 2 == 1) {
                    texCoord1 = 0.05f;
                    texCoord2 = 0.05f;
                } else {
                    texCoord1 = 0.05f;
                    texCoord2 = 0.0f;
                }
            }
            GLfloat X = x;
            GLfloat Y = heightData[z][x];
            GLfloat Z = z;
            std::vector<GLfloat> verticePlaceHolder = {
                    X,	Y,	Z,     texCoord1,  texCoord2, X,   -Y,  Z, // <--- Fix for normals, makes the normal point upwards as it should.
                 // x   y   z       u               v     1    -1   1
            };
            vertices.insert(end(vertices), verticePlaceHolder.begin(), verticePlaceHolder.end()); // Assignement 2 technique.
        }
    }
    for (unsigned int z = 0; z < height - 1; z++) { // no -1 causes program to crash.
        for (unsigned int x = 0; x < width - 1; x++) { // -1 also accounts for edge cases at newlines.
            std::vector<GLuint> indicesPlaceHolder = {
                x + width * z, 

                width * z + x + 1,

                x + (z + 1) * width,

                width * z + x + 1, 

                (z + 1) * width + x,

                (z + 1) * width + x + 1
            };
            indices.insert(end(indices), indicesPlaceHolder.begin(), indicesPlaceHolder.end());
        }
    }
        
    shader->calculateAverageNormals(indices, indices.size(), vertices, vertices.size(), 8, 5);
    
    heightMapVAO = std::make_shared<VertexArray>();
    heightMapVAO->bind();
    heightMapVBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(float)); // GLfloat
    heightMapVBO->bind();
    heightMapVBLayout = std::make_unique<VertexBufferLayout>();
    heightMapVBLayout->Push<float>(3);
    heightMapVBLayout->Push<float>(2);
    heightMapVBLayout->Push<float>(3);
    heightMapVAO->addBuffer(*heightMapVBO, *heightMapVBLayout);
    heightMapIBO = std::make_shared<IndexBuffer>(&indices[0], indices.size());
}

/*
*  Draw call for the heightmap. The postion is translated -90 such that the 0,0,0 position vector is at the top corner of the heightmap.
*  This translation affects the instance model positions.
*
*  @param model - To translate the object in world space
*  @param projection - For viewing the object with camera.
*  @param camera - for retrieving matrices.
*  @param shader - Which shader to use for object.
*
*/
void HeightMap::drawHeightMap(glm::mat4 model, glm::mat4 projection, std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader) {
    shader->useShader();

    uniformModel = shader->getModelLocation();
    uniformProjection = shader->getProjectionLocation();
    uniformView = shader->getViewLocation();

    //model = glm::mat4(1.0f);
    glm::vec3 heightMapPosition = { 0.0f,-90.0f,0.0f }; // move heightmap down so that 0,0,0 is corner of heightmap!.
    model = glm::translate(model, heightMapPosition);

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix())); // Notice bottom location.
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    
    heightMat->useTexture();
    heightMapRenderer->drawElements(heightMapVAO, heightMapIBO); 
}


/**
* Drawin the heightmap again but for the rendered quad in the corner.
*
* @param model - For moving object
* @param projection - getting view of minimap (orthographic)
* @param camera - used to retrieve matrices.
* @param shader - what shader to use for object.
*/
void HeightMap::drawMinimap(glm::mat4 model, glm::mat4 projection, std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& shader) {
    shader->useShader();

    uniformModel = shader->getModelLocation();
    uniformProjection = shader->getProjectionLocation();
    uniformView = shader->getViewLocation();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateMinimapView()));

    model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    heightMat->useTexture();
    heightMapRenderer->drawElements(heightMapVAO, heightMapIBO);
}
