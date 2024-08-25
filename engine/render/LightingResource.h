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
	vec3 pos;
	vec3 color;
	GLfloat intensity;

	LightingResource();
	LightingResource(vec3 pos, vec3 color, GLfloat intensity);

	void OrbitAnimation() {
		float radius = 1.0f;
		float speed = 0.8;
		static float time = 0.0f;
		time += 0.0002f;
		
		this->pos.z = sin(time * speed) * radius;
		this->pos.x = cos(time * speed) * radius;
	}
};