#include "config.h"
#include "render/Materials.h"

void Material::Apply(std::shared_ptr<ShaderResource> shader, std::shared_ptr<TextureResource> texture, GLint textureSlot,
	LightingResource directionalLight, std::vector<LightingResource> pointLight, Camera camera) {

}

BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<ShaderResource>& shader, float shininess, vec3 ambient,
	vec3 diffuse, vec3 specular, bool hasTangents) {
	static bool loaded = false;
	static bool loadedNormalMap = false;
	static std::shared_ptr<ShaderResource> bpShader;
	static std::shared_ptr<ShaderResource> bpNormalmapShader;

	if (!loaded && !hasTangents) {
		shader->LoadShaders("shader/vShader.txt", "shader/fShader.txt"); // No normalmapping
		bpShader = shader;
		loaded = true;
	}
	else if (loaded && !hasTangents) {
		shader = bpShader;
	}
	else if (!loadedNormalMap && hasTangents) {
		shader->LoadShaders("shader/vShaderNormalmapping.txt", "shader/fShaderNormalMapping.txt"); // With normalmapping
		bpNormalmapShader = shader;
		loadedNormalMap = true;
	}
	else if (loadedNormalMap && hasTangents) {
		shader = bpNormalmapShader;
	}

	m_shininess = shininess;
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}

void BlinnPhongMaterial::Apply(std::shared_ptr<ShaderResource> shader, std::shared_ptr<TextureResource> texture, GLint textureSlot,
	LightingResource directionalLight, std::vector<LightingResource> pointLight, Camera camera) {
	shader->Run();

	// Camera uniforms
	shader->SetMat4Uniform("view", camera.GetCameraViewMatrix());
	shader->SetMat4Uniform("projection", camera.GetCameraProjMatrix());
	shader->SetVec3Uniform("viewPos", camera.GetCameraPos());

	// Set texture
	switch (textureSlot) {
	case 0:
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->m_textureID);
		shader->SetSampler2DUniform("texture0", 0);
		break;
	case 1:
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, texture->m_textureID);
		shader->SetSampler2DUniform("normalMap", 1);
		break;
	}

	shader->SetFloatUniform("material.shininess", m_shininess);
	shader->SetVec3Uniform("material.ambient", m_ambient);
	shader->SetVec3Uniform("material.diffuse", m_diffuse);
	shader->SetVec3Uniform("material.specular", m_specular);

	// Directional light uniforms
	shader->SetVec3Uniform("directionalLight.direction", directionalLight.m_pos); // Direction of light, not pos of light source.
	shader->SetVec3Uniform("directionalLight.color", directionalLight.m_color);
	shader->SetFloatUniform("directionalLight.intensity", directionalLight.m_intensity);

	// Point light uniforms
	for (int i = 0; i < pointLight.size(); i++) {
		std::string number = std::to_string(i);
		shader->SetVec3Uniform("pointLight[" + number + "].pos", pointLight[i].m_pos);
		shader->SetVec3Uniform("pointLight[" + number + "].color", pointLight[i].m_color);
		shader->SetFloatUniform("pointLight[" + number + "].intensity", pointLight[i].m_intensity);
		shader->SetFloatUniform("pointLight[" + number + "].constant", 1.0f);
		shader->SetFloatUniform("pointLight[" + number + "].linear", 0.7f);
		shader->SetFloatUniform("pointLight[" + number + "].quadratic", 1.8f);
	}
}