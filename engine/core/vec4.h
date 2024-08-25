#pragma once

class vec4 {
public:
	float x, y, z, w;

	vec4() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	vec4(const float x, const float y, const float z, const float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	vec4(vec4 const& vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = vec.w;
	}

	vec4(vec3 const& vec, float var) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = var;
	}

	//Methods and operators
	vec4& operator=(vec4 const& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		this->w = rhs.w;
		return *this;
	}

	vec4 operator-() {
		return vec4{ -x, -y, -z, -w };
	}

	vec4 operator+(vec4 const& rhs) {
		return { (x + rhs.x), (y + rhs.y), (z + rhs.z), (w + rhs.w)};
	}

	vec4& operator+=(vec4 const& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;
		return *this;
	}

	vec4 operator-(vec4 const& rhs) {
		return { (x - rhs.x), (y - rhs.y), (z - rhs.z), (w - rhs.w) };
	}

	vec4& operator-=(vec4 const& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;
		return *this;
	}

	vec4& operator*=(float const scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;
		return *this;
	}

	vec4 operator*(float const scalar) {
		return { (x * scalar), (y * scalar), (z * scalar), (w * scalar) };
	}

	bool operator==(vec4 const& rhs) {
		if (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w) {
			return true;
		}
		return false;
	}

	bool operator!=(vec4 const& rhs) {
		if (this->x != rhs.x || this->y != rhs.y || this->z != rhs.z || this->w != rhs.w) {
			return true;
		}
		return false;
	}

	float& operator[](unsigned int const i) {  // Range [0, 3]
		switch (i) {
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
		case 3:
			return this->w;
		}
	}

};

//Functions
inline float dot(vec4 const& a, vec4 const& b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

inline float length(vec4 const& v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2));
}

inline vec4 normalize(vec4 const& v) {
	return vec4(v.x / length(v), v.y / length(v), v.z / length(v), v.w / length(v));
}
