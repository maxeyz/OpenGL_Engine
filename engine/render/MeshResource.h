#pragma once
#include <GL/glew.h>
#include "core/util.h"
#include "core/mat4.h"
#include "core/vec4.h"
#include "core/vec3.h"
#include "core/vec2.h"
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include "render/TextureResource.h"
#include "render/ShaderResource.h"
#include "render/Materials.h"
#include "render/GLTF.h"

class MeshResource {
public:
	struct Primitive {
		GLuint VAO;
		GLuint nrIndices;
		GLuint byteOffset;
		GLenum indexType;
		Material material;
	};

	struct Attribute {
		GLuint slot;
		GLint components;
		GLenum type = GL_NONE;
		GLsizei stride;
		GLsizei byteOffset;
		GLboolean normalized = GL_FALSE;
	};

	struct Vertex {
		vec3 position;
		vec3 color;
		vec2 texCoord;
		vec3 normal;

		Vertex(vec3 position, vec3 color, vec2 texCoord, vec3 normal) {
			this->position = position;
			this->color = color;
			this->texCoord = texCoord;
			this->normal = normal;
		}
	};

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	
	std::vector<Primitive> primitives;

	MeshResource();
	MeshResource(std::vector<Vertex> vertexVec, const unsigned& nrOfVertices, std::vector<GLuint> indexVec, const unsigned& nrOfIndices);
	MeshResource(std::vector<Vertex> vertexVec, std::vector<GLuint> indexVec, std::vector<TextureResource> textures);
	~MeshResource();

	// Quad
	void DrawQuad(); //old
	static MeshResource CreateQuadMesh();
	void TransformQuad(GLuint program);

	// Cube
	static MeshResource CreateCubeMesh(float width, float height, float depth);

	// Sphere
	static MeshResource CreateSphereMesh();

	// Utility
	void Render(int mode);
	void Render(Primitive prim);
	static mat4 Rotate(GLuint program); //old
	mat4 Place(GLuint program, float x, float y, float z); //old

	// OBJ Loader
	bool ReadOBJFile(const char* path);
	MeshResource CreateOBJMesh();

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<TextureResource> textures;

	void InitVertexData(std::vector<Vertex> vertexVec, const unsigned& nrOfVertices, std::vector<GLuint> indexVec, const unsigned& nrOfIndices) {
		for (size_t i = 0; i < nrOfVertices; i++) {
			this->vertices.push_back(vertexVec[i]);
		}

		for (size_t i = 0; i < nrOfIndices; i++) {
			this->indices.push_back(indexVec[i]);
		}
	}

	void InitVAO() {
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		// Generate VBO, bind and send data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

		// Generate EBO, bind and send data
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

		// Set VertexAttribPointers and enable
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		// Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(2);

		// Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		// Unbind VAO
		glBindVertexArray(0);
	}
};
