#include "config.h"
#include "render/LightingResource.h"

void LightingResource::OrbitAnimation() {
	float radius = 1.0f;
	float speed = 7.0;
	static float time = 0.0f;
	time += 0.0002f;

	m_pos.z = sin(time * speed) * radius;
	m_pos.x = cos(time * speed) * radius;
}
