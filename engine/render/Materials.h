#pragma once
#include "GL/glew.h";
#include "render/ShaderResource.h"
#include "render/Camera.h"
#include "render/LightingResource.h"
#include "render/TextureResource.h"
#include <cstring>
#include <vector>


class Material {
public:
	GLfloat shininess;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	virtual void Apply(std::shared_ptr<ShaderResource> shader, std::shared_ptr<TextureResource> texture, GLint textureSlot,
	LightingResource directionalLight, std::vector<LightingResource> pointLight, Camera camera);

	Material();
};

class BlinnPhongMaterial : public Material {
public:
	BlinnPhongMaterial();
	BlinnPhongMaterial(std::shared_ptr<ShaderResource>& shader, float shininess, vec3 ambient, vec3 diffuse, vec3 specular, bool hasTangents = false);
	void Apply(std::shared_ptr<ShaderResource> shader, std::shared_ptr<TextureResource> texture, GLint textureSlot, 
	LightingResource directionalLight, std::vector<LightingResource> pointLight, Camera camera);
};