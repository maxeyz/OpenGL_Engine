#include "config.h"
#include "render/ObjLoader.h"
#include <cstring>

bool ObjLoader::loadOBJ(const char* path, std::vector <vec3>& outVertices, std::vector <vec3>& outUvs, std::vector <vec3>& outNormals) {
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<vec3> tempVertices;
	std::vector<vec3> tempUvs;
	std::vector<vec3> tempNormals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		std::cout << "Impossible to open the file." << std::endl;
		return false;
	}
	while (true) {
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break;
		}
		if (strcmp(lineHeader, "v") == 0) {
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			vec3 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3 normal;
			fscanf(file, "%f, %f, %f\n", &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(
				file,
				"%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2] );

			if (matches != 9) {
				std::cout << "ERROR: File can't be parsed." << std::endl;
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}

		// Vertex handling
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			unsigned int vertexIndex = vertexIndices[i];
			vec3 vertex = tempVertices[vertexIndex - 1];
			outVertices.push_back(vertex);
		}

		// UV handling
		for (unsigned int i = 0; i < uvIndices.size(); i++) {
			unsigned int uvIndex = uvIndices[i];
			vec3 uv = tempUvs[uvIndex - 1];
			outVertices.push_back(uv);
		}

		// Normals handling
		for (unsigned int i = 0; i < normalIndices.size(); i++) {
			unsigned int normalIndex = normalIndices[i];
			vec3 normals = tempVertices[normalIndex - 1];
			outVertices.push_back(normals);
		}


	}
}

void ObjLoader::RunObj() {
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(vec3), &this->vertices[0], GL_STATIC_DRAW);
}