#include "config.h"
#include "core/util.h"
#include "render/Camera.h"
#include "render/window.h"

Camera::Camera() {
	this->upVec = vec3{ 0.0f, 1.0f, 0.0f };
}

Camera::~Camera() {
	// Empty
}

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

mat4 Camera::OrbitScene() {
	float radius = 0.1f;
	float speed = 0.5f;

	// Use for continuous rotation for LookAt().
	this->cameraPos.x = sin(glfwGetTime() * speed) * radius;
	this->cameraPos.z = cos(glfwGetTime() * speed) * radius;

	this->projMatrix = this->ProjectionMatrix(1.8, 1024.0f / 768.0f, 0.4f, 15.0f);
	this->viewMatrix = this->LookAt(vec3(cameraPos.x, 0.05f, cameraPos.z), vec3(0.0f, 0.0f, 0.0f));

	return projMatrix * viewMatrix;
}

mat4 Camera::LookAt(vec3 camera, vec3 target) {
	this->cameraDirection = normalize(camera - target);
	this->cameraRight = normalize(cross(this->upVec, this->cameraDirection));
	this->cameraUp = cross(this->cameraDirection, this->cameraRight);

	mat4 rotationMatrix = {
		vec4{	 this->cameraRight.x,	  this->cameraRight.y,	   this->cameraRight.z,		0},
		vec4{		   this->upVec.x,			this->upVec.y,			 this->upVec.z,		0}, // Check
		vec4{this->cameraDirection.x, this->cameraDirection.y, this->cameraDirection.z,		0},
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