#pragma once
#include <iostream>
#include <GL/glew.h>
#include "render/window.h"
#include "core/app.h"
#include "render/ShaderResource.h"

class TextureResource {
public:
	unsigned int m_textureID;
	int m_width;
	int m_height;
	int m_nrChannels;

	TextureResource() {};
	~TextureResource() {};
	void LoadFromFile(const char* filename, bool flip = true);
	void LoadFromMemory(std::vector<uint8_t> buffer);
	void SetTextureUniform(std::shared_ptr<ShaderResource> shader, const char* uniform, GLuint unit);
};