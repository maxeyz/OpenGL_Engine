#include "config.h"
#include "core/util.h"
#include "render/Camera.h"
#include "render/window.h"

mat4 Camera::ProjectionMatrix(GLfloat fov, GLfloat aspect, GLfloat n, GLfloat f) {
	float rad = fov / 2;
	float h = cos(rad) / sin(rad);
	float w = h * aspect;
	float d = f / 2 * (n - f);

	mat4 projectionMatrix = {
		vec4{          w,		         0,	            0,				 0},
		vec4{	       0,                h,	            0,				 0},
		vec4{	       0,			     0,				d,			 d * n},
		vec4{	       0,				 0,			   -1,				 0},
	};

	return inverse(projectionMatrix);
}

mat4 Camera::OrbitScene(float speed) {
	float radius = 0.1f;

	// Use for continuous rotation for LookAt().
	m_cameraPos.x = sin(glfwGetTime() * speed) * radius;
	m_cameraPos.z = cos(glfwGetTime() * speed) * radius;

	m_projMatrix = ProjectionMatrix(1.8, 1024.0f / 768.0f, 0.4f, 15.0f);
	m_viewMatrix = LookAt(vec3(m_cameraPos.x, 0.05f, m_cameraPos.z), vec3(0.0f, 0.0f, 0.0f));

	return m_projMatrix * m_viewMatrix;
}

mat4 Camera::LookAt(vec3 camera, vec3 target) {
	m_cameraDirection = normalize(camera - target);
	m_cameraRight = normalize(cross(m_upVec, m_cameraDirection));
	m_cameraUp = cross(m_cameraDirection, m_cameraRight);

	mat4 rotationMatrix = {
		vec4{	 m_cameraRight.x,	  m_cameraRight.y,	   m_cameraRight.z,		0},
		vec4{		   m_upVec.x,			m_upVec.y,			 m_upVec.z,		0}, // Check
		vec4{m_cameraDirection.x, m_cameraDirection.y, m_cameraDirection.z,		0},
		vec4{					   0,						0,						 0,		1},
	};

	mat4 translationMatrix = {
		vec4{1, 0, 0, -camera.x},
		vec4{0, 1, 0, -camera.y},
		vec4{0, 0, 1, -camera.z},
		vec4{0, 0, 0,		  1},
	};

	return inverse(rotationMatrix) * inverse(translationMatrix);
}