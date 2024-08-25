#pragma once
#include <cmath>

class vec3 {
public:
	float x, y, z;

	//Constructors
	vec3() {
		x = 0;
		y = 0;
		z = 0;
	}
	vec3(const float x, const float y, const float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	vec3(vec3 const& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	//Methods and operators
	vec3& operator=(vec3 const& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		return *this;
	}

	vec3 operator-() {
		float f1 = -this->x;
		float f2 = -this->y;
		float f3 = -this->z;
		return vec3(f1,f2,f3);
	}

	vec3 operator+(vec3 const& rhs) {
		float f1 = this->x + rhs.x;
		float f2 = this->y + rhs.y;
		float f3 = this->z + rhs.z;
		return vec3(f1,f2,f3);
	}

	vec3& operator+=(vec3 const& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}

	vec3 operator-(vec3 const& rhs) {
		float f1 = this->x - rhs.x;
		float f2 = this->y - rhs.y;
		float f3 = this->z - rhs.z;
		return vec3(f1,f2,f3);
	}

	vec3& operator-=(vec3 const& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

	vec3& operator*=(float const scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}

	vec3 operator*(float const scalar) {
		return { (x * scalar), (y * scalar), (z * scalar) };
	}

	bool operator==(vec3 const& rhs) {
		if (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z) {
			return true;
		}
		return false;
	}

	bool operator!=(vec3 const& rhs) {
		if (this->x != rhs.x || this->y == rhs.y || this->z == rhs.z) {
			return true;
		}
		return false;
	}

	float& operator[](unsigned int const i) {  // Range [0, 2]
		switch (i) {
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
		}
	}
};

//Functions
inline float dot(vec3 const& a, vec3 const& b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline float length(vec3 const& v) {
	const float len = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
	return len;
}

inline vec3 cross(vec3 const& a, vec3 const& b) {
	return vec3{ (a.y * b.z) - (a.z * b.y), - (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x)};
}

inline vec3 normalize(vec3 const& v) {
	float f1 = v.x / length(v);
	float f2 = v.y / length(v);
	float f3 = v.z / length(v);
	return vec3(f1, f2 ,f3);
}
