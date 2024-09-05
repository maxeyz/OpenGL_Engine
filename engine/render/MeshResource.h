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
		GLuint m_VAO;
		GLuint m_nrIndices;
		GLuint m_byteOffset;
		GLenum m_indexType;
		Material m_material;
	};

	struct Attribute {
		GLuint m_slot;
		GLint m_components;
		GLenum m_type = GL_NONE;
		GLsizei m_stride;
		GLsizei m_byteOffset;
		GLboolean m_normalized = GL_FALSE;
	};

	struct Vertex {
		vec3 m_position;
		vec3 m_color;
		vec2 m_texCoord;
		vec3 m_normal;

		Vertex(vec3 pos, vec3 col, vec2 textCoord, vec3 normal) {
			m_position = pos;
			m_color = col;
			m_texCoord = textCoord;
			m_normal = normal;
		}
	};

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	
	std::vector<Primitive> m_primitives;

	MeshResource() {};
	MeshResource(std::vector<Vertex> vertexVec, const unsigned& nrOfVertices, std::vector<GLuint> indexVec, const unsigned& nrOfIndices);
	MeshResource(std::vector<Vertex> vertexVec, std::vector<GLuint> indexVec, std::vector<TextureResource> textures);
	~MeshResource() {};

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
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<TextureResource> m_textures;

	void InitVertexData(std::vector<Vertex> vertexVec, const unsigned& nrOfVertices, std::vector<GLuint> indexVec, const unsigned& nrOfIndices) {
		for (size_t i = 0; i < nrOfVertices; i++) {
			this->m_vertices.push_back(vertexVec[i]);
		}

		for (size_t i = 0; i < nrOfIndices; i++) {
			this->m_indices.push_back(indexVec[i]);
		}
	}

	void InitVAO() {
		glCreateVertexArrays(1, &this->m_VAO);
		glBindVertexArray(this->m_VAO);

		// Generate VBO, bind and send data
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
		glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(Vertex), this->m_vertices.data(), GL_STATIC_DRAW);

		// Generate EBO, bind and send data
		glGenBuffers(1, &this->m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(GLuint), this->m_indices.data(), GL_STATIC_DRAW);

		// Set VertexAttribPointers and enable
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_position));
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_color));
		glEnableVertexAttribArray(1);

		// Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoord));
		glEnableVertexAttribArray(2);

		// Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
		glEnableVertexAttribArray(3);

		// Unbind VAO
		glBindVertexArray(0);
	}
};
