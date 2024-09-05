#pragma once
#include "core/vec3.h"
#include "core/mat4.h"
#include <GL/glew.h>
#include "render/ShaderResource.h"

class Camera {
public:
	Camera() : m_upVec(0.0f, 1.0f, 0.0f) {};
	~Camera() {};

	mat4 GetCameraViewMatrix() { return m_viewMatrix; }
	mat4 GetCameraProjMatrix() { return m_projMatrix; }
	vec3 GetCameraPos() { return m_cameraPos; }

	void SetCameraPos(vec3 pos) { m_cameraPos = pos; }

	mat4 LookAt(vec3 camera, vec3 target);
	mat4 OrbitScene(float speed = 0.7f);
	mat4 ProjectionMatrix(GLfloat fov, GLfloat aspect, GLfloat n, GLfloat f);

private:
	vec3 m_cameraPos;
	vec3 m_cameraTarget;
	vec3 m_cameraDirection;
	vec3 m_upVec;
	vec3 m_cameraUp;
	vec3 m_cameraRight;

	mat4 m_viewMatrix;
	mat4 m_projMatrix;
};