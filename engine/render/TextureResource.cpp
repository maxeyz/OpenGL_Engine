#include "config.h"
#include "render/TextureResource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "core/stb_image.h"

TextureResource::TextureResource() {
	// Empty
}

TextureResource::~TextureResource() {
	//glDeleteTextures(1, &this->textureID);
}

void TextureResource::LoadFromFile(const char* filename, bool flip) {
	unsigned char* data = stbi_load(filename, &this->width, &this->height, &this->nrChannels, 0);

	if (data) {
		GLenum format;
		if (this->nrChannels == 1) {
			format = GL_RED;
		}
		else if (this->nrChannels == 3) {
			format = GL_RGB;
		}
		else if (this->nrChannels == 4) {
			format = GL_RGBA;
		}
		else {
			throw std::invalid_argument("Automatic Texture type recognisition failed.");
		}
		// Stbi will load the UVs flipped, so just flip them again so they can be read from OpenGL.
		stbi_set_flip_vertically_on_load(flip);


		glGenTextures(1, &this->textureID);
		glBindTexture(GL_TEXTURE_2D, this->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Texture Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		// Texture Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Mipmaps
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Failed to load texture." << std::endl;
	}

	stbi_image_free(data);
}

void TextureResource::LoadFromMemory(std::vector<uint8_t> buffer) {
    int length, x, y, comp;

	unsigned char* data = stbi_load_from_memory(&buffer[0], length, &x, &y, &comp, 0);

	if (data) {
		GLenum format;
		if (this->nrChannels == 1) {
			format = GL_RED;
		}
		else if (this->nrChannels == 3) {
			format = GL_RGB;
		}
		else if (this->nrChannels == 4) {
			format = GL_RGBA;
		}
		else {
			throw std::invalid_argument("Automatic Texture type recognisition failed.");
		}
		glGenTextures(1, &this->textureID);
		glBindTexture(GL_TEXTURE_2D, this->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Texture Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		// Texture Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Mipmaps
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Failed to load texture." << std::endl;
	}

	stbi_image_free(data);
}

// unused
void TextureResource::SetTextureUniform(std::shared_ptr<ShaderResource> shader, const char* uniform, GLuint unit) {
	GLuint location = glGetUniformLocation(shader->program, uniform);
	shader->Run();
	glUniform1i(location, unit);
}