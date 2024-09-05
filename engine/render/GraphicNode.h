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
	mat4 m_transform;
	GLTF m_model;
	std::shared_ptr<MeshResource> m_mesh;
	std::shared_ptr<TextureResource> m_texture;
	std::shared_ptr<ShaderResource> m_shader;

	GraphicNode() :
		m_mesh(std::make_shared<MeshResource>()),
		m_texture(std::make_shared<TextureResource>()),
		m_shader(std::make_shared<ShaderResource>()) {}
	~GraphicNode() {};

	void GraphicNode::Draw(Display::Window* window, std::shared_ptr<ShaderResource> m_shader = nullptr, GLuint method = GL_TRIANGLES, bool hasPrimitives = false);

	void OrbitAnimation();
};