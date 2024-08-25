#pragma once
#include "core/vec3.h"
#include "core/mat4.h"
#include <GL/glew.h>
#include "render/ShaderResource.h"

class Camera {
public:
	vec3 cameraPos;
	vec3 cameraTarget;
	vec3 cameraDirection;
	vec3 upVec;
	vec3 cameraUp;
	vec3 cameraRight;

	mat4 viewMatrix;
	mat4 projMatrix;

	Camera();
	~Camera();
	mat4 LookAt(vec3 camera, vec3 target);
	mat4 OrbitScene();
	mat4 ProjectionMatrix(GLfloat fov, GLfloat aspect, GLfloat n, GLfloat f);
};