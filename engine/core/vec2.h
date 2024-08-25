#pragma once
#include <cmath>

class vec2 {
public:
	float x, y;

	vec2() {
		x = 0;
		y = 0;
	}
	vec2(const float x, const float y) {
		this->x = x;
		this->y = y;
	}
	vec2(vec2 const& vec) {
		x = vec.x;
		y = vec.y;
	}
};