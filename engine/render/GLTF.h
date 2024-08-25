#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "window.h"
#include "render/MeshResource.h"
#include "render/TextureResource.h"
#include "render/ShaderResource.h"
#include "render/Camera.h"
#include "fx/gltf.h"
#include "nlohmann/json.hpp"

class MeshResource;
using json = nlohmann::json;

class GLTF {
public:
	std::vector<std::shared_ptr<MeshResource>> meshes;
	std::vector<std::shared_ptr<TextureResource>> textures;
	std::vector<GLuint> buffers;
	bool hasNormals = false;
	bool hasTangents = false;

	GLTF();
	~GLTF();

	static GLTF LoadGLTF(const char* path, bool flipUV = true);
	static int GetSlotNumber(std::string attrib);
};