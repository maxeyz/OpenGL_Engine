#include "config.h"
#include "render/GraphicNode.h"
#include "render/InputManager.h"

GraphicNode::GraphicNode() {
	MeshResource mesh;
	TextureResource texture;
	ShaderResource shader;

	this->mesh = std::make_shared<MeshResource>(mesh);
	this->texture = std::make_shared<TextureResource>(texture);
	this->shader = std::make_shared<ShaderResource>(shader);
}

GraphicNode::~GraphicNode() {
	// Empty
}

void GraphicNode::OrbitAnimation() {
	float radius = 1.0f;
	float speed = 0.8;
	static float time = 0.0f;
	time += 0.0002f;
	
	this->transform.m[3].z = sin(time * speed) * radius;
	this->transform.m[3].x = cos(time * speed) * radius;
}

void GraphicNode::Draw(Display::Window* window, GLuint method) {
	// Bind
	glBindVertexArray(this->mesh->VAO);

	// Transform
	this->shader->SetMat4Uniform("model", this->transform);
	
	// Render
	if (method == GL_QUADS) {
		this->mesh->Render(method);
	}
	else if (method == GL_TRIANGLES) {
		this->mesh->Render(method);
	}
	else if (method == GL_POLYGON) {
		this->mesh->Render(method);
	}
	else {
		std::cout << "Primitive not added yet!." << std::endl;
	}
}

void GraphicNode::Draw(Display::Window* window, GLuint method, std::shared_ptr<ShaderResource> shader) {
	// Bind
	glBindVertexArray(this->mesh->VAO);

	// Transform
	shader->SetMat4Uniform("model", this->transform);

	// Render
	if (method == GL_QUADS) {
		this->mesh->Render(method);
	}
	else if (method == GL_TRIANGLES) {
		this->mesh->Render(method);
	}
	else if (method == GL_POLYGON) {
		this->mesh->Render(method);
	}
	else {
		std::cout << "Primitive not added yet!." << std::endl;
	}
}

void GraphicNode::Draw(Display::Window* window) {
	for (auto const& primitive : this->mesh->primitives) {
		// Bind
		glBindVertexArray(primitive.VAO);

		// Transform
		this->shader->SetMat4Uniform("model", this->transform);

		// Render
		this->mesh->Render(primitive);
	}
}

void GraphicNode::Draw(Display::Window* window, std::shared_ptr<ShaderResource> shader) {
	for (auto const& primitive : this->mesh->primitives) {
		// Bind
		glBindVertexArray(primitive.VAO);

		// Transform
		shader->SetMat4Uniform("model", this->transform);

		// Render
		this->mesh->Render(primitive);
	}
}