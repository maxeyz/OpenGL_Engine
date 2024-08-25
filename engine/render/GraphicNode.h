#pragma once
#include "window.h"
#include "render/MeshResource.h"
#include "render/TextureResource.h"
#include "render/ShaderResource.h"
#include "render/LightingResource.h"
//#include "render/Materials.h"
#include "render/GLTF.h"

class GraphicNode {
public:
	std::shared_ptr<MeshResource> mesh;
	std::shared_ptr<TextureResource> texture;
	std::shared_ptr<ShaderResource> shader;
	mat4 transform;
	GLTF model;

	GraphicNode();
	~GraphicNode();

	void Draw(Display::Window* window, GLuint method);
	void Draw(Display::Window* window, GLuint method, std::shared_ptr<ShaderResource> shader);

	// GLTF exclusive
	void Draw(Display::Window* window);
	void Draw(Display::Window* window, std::shared_ptr<ShaderResource> shader);

	void OrbitAnimation();
};