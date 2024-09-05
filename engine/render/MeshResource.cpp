#include "config.h"
#include "render/MeshResource.h"
#include <cstring>

MeshResource::MeshResource(std::vector<Vertex> vertexVec, const unsigned& nrOfVertices, std::vector<GLuint> indexVec, const unsigned& nrOfIndices) {
	InitVertexData(vertexVec, nrOfVertices, indexVec, nrOfIndices);
	InitVAO();
}

MeshResource::MeshResource(std::vector<Vertex> vertexVec, std::vector<GLuint> indexVec, std::vector<TextureResource> textures) {
	m_vertices = vertexVec;
	m_indices = indexVec;
	m_textures = textures;
	InitVAO();
}

MeshResource MeshResource::CreateQuadMesh() {
	std::vector<Vertex> vertexVec = {
		// Front
			Vertex(
				vec3(-1.0f,		      -1.0f,			   -1.0f),					// Bottom left index: 0
				vec3(0.0f,			  0.0f,			   0.0f),					// Color
				vec2(0.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(1.0f,		      -1.0f,			   -1.0f),					// Bottom right index: 1
				vec3(0.0f,			  0.0f,			   0.0f),					// Color
				vec2(1.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3( 1.0f,		       1.0f,			   -1.0f),					// Top right index: 2
				vec3(0.0f,			  0.0f,			   0.0f),					// Color
				vec2(1.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(-1.0f,		       1.0f,			   -1.0f),					// Top left index: 3
				vec3(0.0f,			  0.0f,			   0.0f),					// Color
				vec2(0.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),
	};

	std::vector<GLuint> indexVec = {
		0, 1, 2, // Right triangle
		2, 3, 0  // Left triangle
	};

	unsigned nrOfVertices = vertexVec.size();
	unsigned nrOfIndices = indexVec.size();

	MeshResource quad(vertexVec, nrOfVertices, indexVec, nrOfIndices);

	return quad;
}

void MeshResource::DrawQuad() { 
	// Render to screen
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void MeshResource::TransformQuad(GLuint program) {
	mat4 movement;
	mat4 rotation;
	mat4 conc;
	static float scale = 0.0f;
	static float delta = 0.00005f; // Movement speed
	static float rad = 0.0f;

	rad += 0.0003; // Rotation speed
	scale += delta;

	rotation = rotationz(rad);

	// Range of movement
	if ((scale >= 0.5f) || (scale <= -0.5f)) {
		delta *= -1.0f;
	}

	movement = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{scale, 0.0f, 0.0f, 1.0f },
	};

	conc = movement * rotation;

	GLuint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &conc[0][0]);
}

mat4 MeshResource::Rotate(GLuint program) {
	mat4 model;
	static float rad = 0.0f;

	rad += 0.0001; // Rotation speed

	model = rotationy(rad) * rotationx(rad);

	GLuint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	return model;
}

mat4 MeshResource::Place(GLuint program, float x, float y, float z) {
	mat4 model = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{	 x,	   y,    z, 1.0f },
	};

	GLuint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	return model;
}

bool MeshResource::ReadOBJFile(const char* path) {
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<vec3> tempVertexPos;
	std::vector<vec2> tempUvsPos;
	std::vector<vec3> tempNormalsPos;
	std::map<std::string, int> vertexMap;
	int indexCounter = 0;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		std::cout << "Impossible to open the file." << std::endl;
		return false;
	}
	while (1) {
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break;
		}
		if (strcmp(lineHeader, "v") == 0) {
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertexPos.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUvsPos.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNormalsPos.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];

			int matches = fscanf(file,
				"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2],
				&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

			if (matches != 12) {
				std::cout << "ERROR: File can't be parsed." << std::endl;
				return false;
			}
			// Re-use identical vertices
			for (int i = 0; i < 4; i++) {
				std::string vertex = std::to_string(vertexIndex[i]);
				std::string uv = std::to_string(uvIndex[i]);
				std::string normal = std::to_string(normalIndex[i]);
				std::string face = vertex + "/" + uv + "/" + normal;
				if (vertexMap.count(face)) {
					m_indices.push_back(vertexMap[face]);
				}
				else if (vertexMap.find(face) == vertexMap.end()) {
					vertexMap.insert(std::make_pair(vertex + "/" + uv + "/" + normal, indexCounter));

					vertexIndices.push_back(vertexIndex[i] - 1);
					uvIndices.push_back(uvIndex[i] - 1);
					normalIndices.push_back(normalIndex[i] - 1);

					m_indices.push_back(indexCounter++);
				}
			}
		}
	}
	//Vertex indexing
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		m_vertices.push_back(Vertex(tempVertexPos[vertexIndices[i]], vec3(1.0, 1.0f, 1.0f), tempUvsPos[uvIndices[i]], tempNormalsPos[normalIndices[i]]));
	}
}

MeshResource MeshResource::CreateOBJMesh() {
	// Use if the mesh is too large.
	float scale = 0.035;
	for (int i = 0; i < m_vertices.size(); i++) {
		m_vertices[i].m_position = m_vertices[i].m_position * scale;
	}

	unsigned nrOfVertices = m_vertices.size();
	unsigned nrOfIndices = m_indices.size();

	MeshResource mesh(m_vertices, nrOfVertices, m_indices, nrOfIndices);

	return mesh;
}

MeshResource MeshResource::CreateCubeMesh(float width, float height, float depth) {
	std::vector<Vertex> vertexVec = {
		// Front
			Vertex(
				vec3(-width,		   -height,			 -depth),				// Bottom left index: 0
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(0.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(width,		   -height,			 -depth),					// Bottom right index: 1
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(1.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)),										// Normal

			Vertex(
				vec3(width,		    height,			 -depth),					// Top right index: 2
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(1.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(-width,		    height,			 -depth),				// Top left index: 3
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(0.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

		// Back
			Vertex(
				vec3(width,		   -height,			 depth),					// Bottom left index: 4
				vec3(0.0f,			  1.0f,		      0.0f),					// Color
				vec2(0.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(-width,		   -height,			 depth),				// Bottom right index: 5
				vec3(0.0f,			  1.0f,			  0.0f),					// Color
				vec2(1.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),
			Vertex(
				vec3(-width,			height,			 depth),				// Top right index: 6
				vec3(0.0f,			  1.0f,			  0.0f),					// Color
				vec2(1.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(width,			height,			 depth),					// Top left index: 7
				vec3(0.0f,			  1.0f,			  0.0f),					// Color
				vec2(0.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

		// Bottom
			Vertex(
				vec3(-width,		    height,			 -depth),				// Bottom left index: 8
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(0.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),
			Vertex(
				vec3(width,		    height,			 -depth),					// Bottom right index: 9
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(1.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(width,			height,			  depth),					// Top right index: 10
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(1.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal	
			),

			Vertex(
				vec3(-width,			height,			  depth),				// Top left index: 11
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(0.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

		// Top
			Vertex(
				vec3(-width,		   -height,		      depth),				// Bottom left index: 12
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(0.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(width,		   -height,			  depth),					// Bottom right index: 13
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(1.0f,			  0.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(width,		   -height,			  -depth),					// Top right index: 14
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(1.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),

			Vertex(
				vec3(-width,		   -height,			  -depth),				// Top left index: 15
				vec3(1.0f,			  0.0f,			   0.0f),					// Color
				vec2(0.0f,			  1.0f),									// Texture cord
				vec3(0.0f, 0.0f, 0.0f)											// Normal
			),
	};
	std::vector<GLuint> indexVec = {
		// Front
		0, 1, 2,
		2, 3, 0,

		// Back
		4, 5, 6,
		6, 7, 4,

		// Left
		5, 0, 3,
		3, 6, 5,

		// Right
		1, 4, 7,
		7, 2, 1,

		// Bottom
		8, 9, 10,
		10, 11, 8,

		// Top
		12, 13, 14,
		14, 15, 12,
	};

	unsigned nrOfVertices = vertexVec.size();
	unsigned nrOfIndices = indexVec.size();

	MeshResource cube(vertexVec, nrOfVertices, indexVec, nrOfIndices);

	return cube;
}

void MeshResource::Render(int mode) {
	glDrawElements(mode, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void MeshResource::Render(Primitive prim) {
	glDrawElements(GL_TRIANGLES, prim.m_nrIndices, prim.m_indexType, (void*)prim.m_byteOffset);
}

MeshResource MeshResource::CreateSphereMesh() {
	std::vector<Vertex> vertexVec;
	unsigned int indexCount;

	std::vector<vec3> positions;
	std::vector<vec2> uv;
	std::vector<vec3> normals;

	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359f;
	for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	{
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(vec3(xPos, yPos, zPos));
			uv.push_back(vec2(xSegment, ySegment));
			normals.push_back(vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	indexCount = static_cast<unsigned int>(indices.size());

	std::vector<float> data;
	GLfloat scale = 0.015;
	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);

		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}

		vertexVec.push_back(Vertex
		(
			vec3(positions[i].x * scale, positions[i].y * scale, positions[i].z * scale),
			vec3(1.0f, 0.0f, 0.0f), 
			vec2(uv[i].x, uv[i].y), 
			vec3(normals[i].x, normals[i].y, normals[i].z))
		);
	}

	unsigned nrOfVertices = vertexVec.size();
	unsigned nrOfIndices = indices.size();

	MeshResource sphere(vertexVec, nrOfVertices, indices, nrOfIndices);

	return sphere;
}
