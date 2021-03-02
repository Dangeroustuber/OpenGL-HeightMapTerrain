#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SpotLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"


class Shader {

private:

	int pointLightCount;
	int spotLightCount;

	/* -- The GLuint for all of the uniform variables -- */

	GLuint shaderID;
	GLuint uniformProjection;
	GLuint uniformView;
	GLuint uniformModel;
	GLuint uniformEyePosition;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;
	GLuint uniformMinimapTexture;
	GLuint uniformTexture;

	/* -- The structs are for the different types of light. The idea       --
	   -- here is simply to have the specific set of uniform variables     --
	   -- for each light, so that we don't end up with to many all in all. --
	*/

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[15];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[3];

	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

public:

	Shader();

	void createShaderFromFile(const char* vertexLocation, const char* fragmentLocation);

	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
	}

	void setVec2(const std::string& name, const glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation(shaderID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(shaderID, name.c_str()), x, y, z);
	}

	void setMat3(const std::string& name, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	std::string readFile(const char* fileLocation);

	/* -- The inline functions get uniforms from the shader -- */

	inline GLuint getViewLocation() { return uniformView; }
	inline GLuint getModelLocation() { return uniformModel; }
	inline GLuint getShininessLocation() { return uniformShininess; }
	inline GLuint getProjectionLocation() { return uniformProjection; }
	inline GLuint getEyePositionLocation() { return uniformEyePosition; }
	inline GLuint getMinimapTextureLocation() { return uniformMinimapTexture; };
	inline GLuint getSpecularIntensityLocation() { return uniformSpecularIntensity; }
	inline GLuint getDirectionLocation() { return uniformDirectionalLight.uniformDirection; }
	inline GLuint getAmbientColourLocation() { return uniformDirectionalLight.uniformColour; }
	inline GLuint getAmbientIntensityLocation() { return uniformDirectionalLight.uniformAmbientIntensity; }
	inline GLuint getDiffuseIntensityLocation() { return uniformDirectionalLight.uniformDiffuseIntensity; }

	/* -- Set the different lights during runtime -- */

	void setDirectionalLight(std::shared_ptr<DirectionalLight>& dLight);
	void setPointLights(PointLight* pLight, unsigned int lightCount);
	void setSpotLights(SpotLight* sLight, unsigned int lightCount);

	void useShader();
	void clearShader();

	void calculateAverageNormals(std::vector<GLuint> indices, unsigned int indiceCount, std::vector<GLfloat> vertices, unsigned int verticeCount,
		unsigned int vLength, unsigned int normalOffset);

	~Shader();

};