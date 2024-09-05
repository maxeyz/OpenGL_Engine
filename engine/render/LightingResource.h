#pragma once
#include <GL/glew.h>
#include "core/util.h"
#include <GLFW/glfw3.h>
#include "core/mat4.h"
#include "core/vec4.h"
#include "core/vec3.h"
#include <vector>
#include <algorithm>
#include "render/ShaderResource.h"

class LightingResource {
public:
	vec3 m_pos;
	vec3 m_color;
	GLfloat m_intensity;

	LightingResource() : m_pos() {};
	LightingResource(vec3 pos, vec3 color, GLfloat intensity) {
		m_pos = pos;
		m_color = color;
		m_intensity = intensity;
	}

	void OrbitAnimation();
};