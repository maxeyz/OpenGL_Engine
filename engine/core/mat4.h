#pragma once

class mat4 {
public:
	vec4 m[4];
	 
	// Constructors
	mat4() {
		this->m[0].x = 1;
		this->m[1].y = 1;
		this->m[2].z = 1;
		this->m[3].w = 1;
	}

	mat4(vec4 const& r0, vec4 const& r1, vec4 const& r2, vec4 const& r3) {
		this->m[0] = r0;
		this->m[1] = r1;
		this->m[2] = r2;
		this->m[3] = r3;
	}

	mat4(mat4 const& m) {
		for (int i = 0; i <= 3; i++) {
			this->m[i] = m.m[i];
		}
	}

	// Methods and operators
	mat4& operator=(mat4 const& rhs) {
		for (int i = 0; i <= 3; i++) {
			this->m[i] = rhs.m[i];
		}
		return *this;
	}

	mat4 operator*(mat4 const& rhs) const { //Matrix * Matrix
		mat4 obj;

		for (int i = 0; i <= 3; i++) {
			obj.m[i].x = this->m[0].x * rhs.m[i].x + this->m[1].x * rhs.m[i].y + this->m[2].x * rhs.m[i].z + this->m[3].x * rhs.m[i].w;
			obj.m[i].y = this->m[0].y * rhs.m[i].x + this->m[1].y * rhs.m[i].y + this->m[2].y * rhs.m[i].z + this->m[3].y * rhs.m[i].w;
			obj.m[i].z = this->m[0].z * rhs.m[i].x + this->m[1].z * rhs.m[i].y + this->m[2].z * rhs.m[i].z + this->m[3].z * rhs.m[i].w;
			obj.m[i].w = this->m[0].w * rhs.m[i].x + this->m[1].w * rhs.m[i].y + this->m[2].w * rhs.m[i].z + this->m[3].w * rhs.m[i].w;
		}
		return obj;
	}

	vec4 operator*(vec4 const& rhs) const { //Matrix * Vector
		vec4 obj;

		obj = {
			this->m[0].x * rhs.x + this->m[1].x * rhs.y + this->m[2].x * rhs.z + this->m[3].x * rhs.w,
			this->m[0].y * rhs.x + this->m[1].y * rhs.y + this->m[2].y * rhs.z + this->m[3].y * rhs.w,
			this->m[0].z * rhs.x + this->m[1].z * rhs.y + this->m[2].z * rhs.z + this->m[3].z * rhs.w,
			this->m[0].w * rhs.x + this->m[1].w * rhs.y + this->m[2].w * rhs.z + this->m[3].w * rhs.w,
		};

		return obj;
	}

	bool operator==(mat4 const& rhs) {
		if (this->m[0] == rhs.m[0] && this->m[1] == rhs.m[1] && this->m[2] == rhs.m[2] && this->m[3] == rhs.m[3]) {
			return true;
		}
		return false;
	}

	bool operator!=(mat4 const& rhs) {
		if (this->m[0] != rhs.m[0] || this->m[1] != rhs.m[1] || this->m[2] != rhs.m[2] || this->m[3] != rhs.m[3]) {
			return true;
		}
		return false;
	}

	vec4& operator[](unsigned int const i) { // Range [0, 3]
		switch (i) {
		case 0:
			return this->m[0];
		case 1:
			return this->m[1];
		case 2:
			return this->m[2];
		case 3:
			return this->m[3];
		}
	}
};

//Functions
inline float determinant(mat4 const& m) {
	float det = 0;

	float x0 = m.m[0].x;
	float y0 = m.m[0].y;
	float z0 = m.m[0].z;
	float w0 = m.m[0].w;

	float x1 = m.m[1].x;
	float y1 = m.m[1].y;
	float z1 = m.m[1].z;
	float w1 = m.m[1].w;

	float x2 = m.m[2].x;
	float y2 = m.m[2].y;
	float z2 = m.m[2].z;
	float w2 = m.m[2].w;

	float x3 = m.m[3].x;
	float y3 = m.m[3].y;
	float z3 = m.m[3].z;
	float w3 = m.m[3].w;

	det = y1 * (z2 * w3 - z3 * w2) - y2 * (z1 * w3 - w1 * z3) + y3 * (z1 * w2 - z2 * w1) - y0 * (z2 * w3 - z3 * w2)
		- y2 * (z0 * w3 - w0 * z3) + y3 * (z0 * w2 - z2 * w0) - y0 * (z1 * w3 - z3 * w1) - y1 * (z0 * w3 - w0 * z3)
		+ y3 * (z0 * w1 - z1 * w0) - y0 * (z1 * w2 - z2 * w1) - y1 * (z0 * w2 - z2 * w0) + y2 * (z0 * w1 - z1 * w0);

	return det;
}

inline mat4 inverse(mat4 const& m) { // WIP :: Returns identity upon failure
	mat4 temp;
	float inv[16], det;
	int i;
	
	inv[0] = m.m[1].y * m.m[2].z * m.m[3].w - // x0
		m.m[1].y * m.m[2].w * m.m[3].z -
		m.m[2].y * m.m[1].z * m.m[3].w +
		m.m[2].y * m.m[1].w * m.m[3].z +
		m.m[3].y * m.m[1].z * m.m[2].w -
		m.m[3].y * m.m[1].w * m.m[2].z;

	inv[4] = -m.m[1].x * m.m[2].z * m.m[3].w + // x1
		m.m[1].x * m.m[2].w * m.m[3].z +
		m.m[2].x * m.m[1].z * m.m[3].w -
		m.m[2].x * m.m[1].w * m.m[3].z -
		m.m[3].x * m.m[1].z * m.m[2].w +
		m.m[3].x * m.m[1].w * m.m[2].z;

	inv[8] = m.m[1].x * m.m[2].y * m.m[3].w - // x2
		m.m[1].x * m.m[2].w * m.m[3].y -
		m.m[2].x * m.m[1].y * m.m[3].w +
		m.m[2].x * m.m[1].w * m.m[3].y +
		m.m[3].x * m.m[1].y * m.m[2].w -
		m.m[3].x * m.m[1].w * m.m[2].y;

	inv[12] = -m.m[1].x * m.m[2].y * m.m[3].z + // x3
		m.m[1].x * m.m[2].z * m.m[3].y +
		m.m[2].x * m.m[1].y * m.m[3].z -
		m.m[2].x * m.m[1].z * m.m[3].y -
		m.m[3].x * m.m[1].y * m.m[2].z +
		m.m[3].x * m.m[1].z * m.m[2].y;

	inv[1] = -m.m[0].y * m.m[2].z * m.m[3].w + // y0
		m.m[0].y * m.m[2].w * m.m[3].z +
		m.m[2].y * m.m[0].z * m.m[3].w -
		m.m[2].y * m.m[0].w * m.m[3].z -
		m.m[3].y * m.m[0].z * m.m[2].w +
		m.m[3].y * m.m[0].w * m.m[2].z;

	inv[5] = m.m[0].x * m.m[2].z * m.m[3].w - // y1
		m.m[0].x * m.m[2].w * m.m[3].z -
		m.m[2].x * m.m[0].z * m.m[3].w +
		m.m[2].x * m.m[0].w * m.m[3].z +
		m.m[3].x * m.m[0].z * m.m[2].w -
		m.m[3].x * m.m[0].w * m.m[2].z;

	inv[9] = -m.m[0].x * m.m[2].y * m.m[3].w + // y2
		m.m[0].x * m.m[2].w * m.m[3].y +
		m.m[2].x * m.m[0].y * m.m[3].w -
		m.m[2].x * m.m[0].w * m.m[3].y -
		m.m[3].x * m.m[0].y * m.m[2].w +
		m.m[3].x * m.m[0].w * m.m[2].y;

	inv[13] = m.m[0].x * m.m[2].y * m.m[3].z - // y3
		m.m[0].x * m.m[2].z * m.m[3].y -
		m.m[2].x * m.m[0].y * m.m[3].z +
		m.m[2].x * m.m[2].z * m.m[3].y +
		m.m[3].x * m.m[0].y * m.m[2].z -
		m.m[3].x * m.m[0].z * m.m[2].y;

	inv[2] = m.m[0].y * m.m[1].z * m.m[3].w - // z0
		m.m[0].y * m.m[1].w * m.m[3].z -
		m.m[1].y * m.m[0].z * m.m[3].w +
		m.m[1].y * m.m[0].w * m.m[3].z +
		m.m[3].y * m.m[0].z * m.m[1].w -
		m.m[3].y * m.m[0].w * m.m[1].z;

	inv[6] = -m.m[0].x * m.m[1].z * m.m[3].w + // z1
		m.m[0].y * m.m[1].w * m.m[3].z +
		m.m[1].x * m.m[0].z * m.m[3].w -
		m.m[1].x * m.m[0].w * m.m[3].z -
		m.m[3].x * m.m[0].z * m.m[1].w +
		m.m[3].x * m.m[0].w * m.m[1].z;

	inv[10] = m.m[0].x * m.m[1].y * m.m[3].w - // z2
		m.m[0].x * m.m[1].w * m.m[3].y -
		m.m[1].x * m.m[0].y * m.m[3].w +
		m.m[1].x * m.m[0].w * m.m[3].y +
		m.m[3].x * m.m[0].y * m.m[1].w -
		m.m[3].x * m.m[0].w * m.m[1].y;

	inv[14] = -m.m[0].x * m.m[1].y * m.m[3].z + // z3
		m.m[0].x * m.m[1].z * m.m[3].y +
		m.m[1].x * m.m[0].y * m.m[3].z -
		m.m[1].x * m.m[0].z * m.m[3].y -
		m.m[3].x * m.m[0].y * m.m[1].z +
		m.m[3].x * m.m[0].z * m.m[1].y;

	inv[3] = -m.m[0].y * m.m[1].z * m.m[2].w + // w0
		m.m[0].y * m.m[1].w * m.m[2].z +
		m.m[1].y * m.m[0].z * m.m[2].w -
		m.m[1].y * m.m[0].w * m.m[2].z -
		m.m[2].y * m.m[0].z * m.m[1].w +
		m.m[2].y * m.m[0].w * m.m[1].z;

	inv[7] = m.m[0].x * m.m[1].z * m.m[2].w - // w1
		m.m[0].x * m.m[1].w * m.m[2].z -
		m.m[1].x * m.m[2].z * m.m[2].w +
		m.m[1].x * m.m[0].w * m.m[2].z +
		m.m[2].x * m.m[0].z * m.m[1].w -
		m.m[2].x * m.m[0].w * m.m[1].z;

	inv[11] = -m.m[0].x * m.m[1].y * m.m[2].w + // w2
		m.m[0].x * m.m[1].w * m.m[2].y +
		m.m[1].x * m.m[0].y * m.m[2].w -
		m.m[1].x * m.m[0].w * m.m[2].y -
		m.m[2].x * m.m[0].y * m.m[1].w +
		m.m[2].x * m.m[0].w * m.m[1].y;

	inv[15] = m.m[0].x * m.m[1].y * m.m[2].z - // w3
		m.m[0].x * m.m[1].z * m.m[2].y -
		m.m[1].x * m.m[0].y * m.m[2].z +
		m.m[1].x * m.m[0].z * m.m[2].y +
		m.m[2].x * m.m[0].y * m.m[1].z -
		m.m[2].x * m.m[0].z * m.m[1].y;

	det = m.m[0].x * inv[0] + m.m[0].y * inv[4] + m.m[0].z * inv[8] + m.m[0].w * inv[12];

	if (det == 0)
		return mat4();

	det = 1.0 / det;

	temp.m[0].x = inv[0] * det;
	temp.m[0].y = inv[1] * det;
	temp.m[0].z = inv[2] * det;
	temp.m[0].w = inv[3] * det;

	temp.m[1].x = inv[4] * det;
	temp.m[1].y = inv[5] * det;
	temp.m[1].z = inv[6] * det;
	temp.m[1].w = inv[7] * det;

	temp.m[2].x = inv[8] * det;
	temp.m[2].y = inv[9] * det;
	temp.m[2].z = inv[10] * det;
	temp.m[2].w = inv[11] * det;

	temp.m[3].x = inv[12] * det;
	temp.m[3].y = inv[13] * det;
	temp.m[3].z = inv[14] * det;
	temp.m[3].w = inv[15] * det;
		
	return temp;
}

inline mat4 transpose(mat4 const& m) {
	mat4 obj = {
		vec4 (m.m[0].x, m.m[1].x, m.m[2].x, m.m[3].x),
		vec4 (m.m[0].y, m.m[1].y, m.m[2].y, m.m[3].y),
		vec4 (m.m[0].z, m.m[1].z, m.m[2].z, m.m[3].z),
		vec4 (m.m[0].w, m.m[1].w, m.m[2].w, m.m[3].w)
	};

	return obj;
}

inline mat4 rotationx(float const rad) {
	mat4 matrix;
	const float c = cos(-rad);
	const float s = sin(-rad);

	matrix = {
		{1, 0, 0, 0},
		{0, c, -s, 0},
		{0, s, c, 0},
		{0, 0, 0, 1},
	};
	return matrix;

}

inline mat4 rotationy(float const rad) {
	mat4 matrix;

	const float c = cos(-rad);
	const float s = sin(-rad);

	matrix = {
		{c, 0, s, 0},
		{0, 1, 0, 0},
		{-s, 0, c, 0},
		{0, 0, 0, 1},
	};
	return matrix;

}

inline mat4 rotationz(float const rad) {
	mat4 matrix;

	const float c = cos(-rad);
	const float s = sin(-rad);

	matrix = {
		{c, -s, 0, 0},
		{s, c, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1},
	};

	return matrix;
}

inline mat4 rotationaxis(vec3 const& v, float const rad) {
	mat4 matrix;

	const float c = cos(-rad);
	const float s = sin(-rad);
	const float vx2 = pow(v.x, 2);
	const float vy2 = pow(v.y, 2);
	const float vz2 = pow(v.z, 2);

	matrix = {
		{ c + vx2 * (1 - c), v.x * v.y * (1 - c) - v.z * s, v.x * v.z * (1 - c) + v.y * s, 0 },
		{ v.y * v.x * (1 - c) + v.z * s, c + vy2 * (1 - c), v.y * v.z * (1 - c) - v.x * s, 0 },
		{ v.z * v.x * (1 - c) - v.y * s, v.z * v.y * (1 - c) + v.x * s, c + vz2 * (1 - c), 0 },
		{ 0, 0, 0, 1}
	};
	
	return matrix;
}