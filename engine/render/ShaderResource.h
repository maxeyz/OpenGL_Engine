#pragma once
#ifndef SHADER_H
#define SHADER_H
#include "core/util.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include "core/mat4.h"
#include "core/vec4.h"
#include "render/LightingResource.h"

class LightingResource;

class ShaderResource {
public:
	GLuint program;

	ShaderResource();
	~ShaderResource();
	void LoadShaders(const char* vertexPath, const char* fragmentPath);
	void Run();
	static void SendAllLightUniformsToShader(ShaderResource shader, LightingResource directionalLight, std::vector<LightingResource> pointLight);

	//Uniform utility functions
	void SetMat4Uniform(const std::string &name, mat4 mat) const;
	void SetVec4Uniform(const std::string &name, vec4 vec) const;
	void SetVec3Uniform(const std::string& name, vec3 vec) const;
	void SetFloatUniform(const std::string& name, GLfloat val) const;
	void SetIntUniform(const std::string& name, GLfloat val) const;
	void SetSampler2DUniform(const std::string& name, GLint textureNr) const;
};

#endif