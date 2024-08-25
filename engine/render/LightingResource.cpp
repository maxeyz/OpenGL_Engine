#include "config.h"
#include "render/LightingResource.h"

LightingResource::LightingResource() {
	// Empty
}

LightingResource::LightingResource(vec3 pos, vec3 color, GLfloat intensity) {
	this->pos = pos;
	this->color = color;
	this->intensity = intensity;
}
