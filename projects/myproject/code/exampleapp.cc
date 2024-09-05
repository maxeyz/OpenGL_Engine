//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "core/util.h"
#include "core/mat4.h"
#include "core/vec4.h"
#include "core/vec3.h"
#include "render/MeshResource.h"
#include "render/Camera.h"
#include "render/grid.h"
#include "render/TextureResource.h"
#include "render/ShaderResource.h"
#include "render/GraphicNode.h"
#include "render/InputManager.h"
#include "render/LightingResource.h"
#include "render/GLTF.h"
#include <cstring>

using namespace Display;
using namespace Render;
using namespace Input2;
using json = nlohmann::json;
using namespace fx;

// Init objects position and/or render multiple in a grid formation.
// Have not decided yet where to place this function. Probably will make a util header for these kind of things once I create better folder structure.
// Maybe add functionality to specify the area where the models will be randomly spawned in.
std::vector<mat4> InitObjectPositions(vec3 pos = { 0,0,0 }, int rows = 1, int columns = 1, vec3 posOffset = { 0,0,0 }) {
	std::vector<mat4> objectPositions;

	// Calculate object positions in a grid layout
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < columns; j++) {
			float posX = pos.x + j * posOffset.x;
			float posZ = pos.z + i * posOffset.z;

			mat4 obj = mat4(
				vec4(1.0f, 0.0f, 0.0f, 0.0f),
				vec4(0.0f, 1.0f, 0.0f, 0.0f),
				vec4(0.0f, 0.0f, 1.0f, 0.0f),
				vec4(posX, 0.0f, posZ, 1.0f)
			);

			objectPositions.push_back(obj);
		}
	}

	return objectPositions;
}

namespace Example
{

	//------------------------------------------------------------------------------
	/**
	*/
	ExampleApp::ExampleApp()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ExampleApp::~ExampleApp()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool ExampleApp::Open()
	{
		App::Open();
		this->window = new Display::Window;
		window->SetKeyPressFunction([this](int32 key, int32, int32, int32)
			{
				if (key == GLFW_KEY_ESCAPE)
					this->window->Close();
			});

		GLfloat buf[] =
		{
			-0.5f,	-0.5f,	-1,			// pos 0
			1,		0,		0,		1,	// color 0
			0,		0.5f,	-1,			// pos 1
			0,		1,		0,		1,	// color 0
			0.5f,	-0.5f,	-1,			// pos 2
			0,		0,		1,		1	// color 0
		};

		if (this->window->Open())
		{
			// set clear color
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void ExampleApp::Close()
	{
		if (this->window->IsOpen())
			this->window->Close();

		Core::App::Close();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void ExampleApp::Run() {
		//-------------GLOBAL OPENGL STATE-------------
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		//-------------VARIABLES-------------
		int SCR_WIDTH = 1024;
		int SCR_HEIGHT = 768;
		bool moveLightAround = true;
		const int NR_OF_POINT_LIGHTS = 4;
		const int NR_OF_SWORDS = 2;
		const char* cubePath = "gltf/Cube/glTF/Cube.gltf";
		const char* avocadoPath = "gltf/Avocado/glTF/Avocado.gltf";
		const char* damagedHelmetPath = "gltf/DamagedHelmet/glTF/DamagedHelmet.gltf";
		const char* flightHelmetPath = "gltf/FlightHelmet/glTF/FlightHelmet.gltf";
		const char* tangentMirrorPath = "gltf/NormalTangentMirrorTest/glTF/NormalTangentMirrorTest.gltf";

		//-------------OBJECTS-------------
		GraphicNode cubeNode0, cubeNode1, cubeNode2, cubeNode3;
		GraphicNode swordNode0, swordNode1;
		GraphicNode gltfModel;
		GraphicNode quad, sphere;
		Camera camera;
		Grid grid;
		InputManager* inputManager = InputManager::Instance();

		// Object vectors
		std::vector<GraphicNode> pointLightCubes;
		std::vector<GraphicNode> swordNodeVector;

		// Push to their vectors
		pointLightCubes.push_back(cubeNode0);
		pointLightCubes.push_back(cubeNode1);
		pointLightCubes.push_back(cubeNode2);
		pointLightCubes.push_back(cubeNode3);

		swordNodeVector.push_back(swordNode0);
		swordNodeVector.push_back(swordNode1);

		//-------------MESHES-------------
		// Group 1 - OpenGL
		for (int i = 0; i < 4; i++) {
			pointLightCubes[i].m_mesh = std::make_shared<MeshResource>(MeshResource::CreateCubeMesh(0.1, 0.1, 0.1));
		}
		quad.m_mesh = std::make_shared<MeshResource>(MeshResource::CreateQuadMesh());
		sphere.m_mesh = std::make_shared<MeshResource>(MeshResource::CreateSphereMesh());

		// Group 2 - OBJ
		swordNode0.m_mesh->ReadOBJFile("obj/sting_sword/sting_sword.obj");
		swordNode0.m_mesh = std::make_shared<MeshResource>(swordNode0.m_mesh->CreateOBJMesh());
		swordNode1.m_mesh->ReadOBJFile("obj/sting_sword/sting_sword.obj");
		swordNode1.m_mesh = std::make_shared<MeshResource>(swordNode1.m_mesh->CreateOBJMesh());

		// Group 3 - GLTF
		//GLTF avocado = GLTF::LoadGLTF(avocadoPath);
		//GLTF cube = GLTF::LoadGLTF(cubePath);
		//GLTF damagedHelmet = GLTF::LoadGLTF(damagedHelmetPath);
		GLTF flightHelmet = GLTF::LoadGLTF(flightHelmetPath, false);
		//GLTF tangentTest = GLTF::LoadGLTF(tangentMirrorPath, false);

		// Simply swap model from any of the Group 3 to change model.
		gltfModel.m_model = flightHelmet;

		//-------------TEXTURES-------------
		// Group 1 - OpenGL
		for (int i = 0; i < 4; i++) { // Will not be used in the end
			pointLightCubes[i].m_texture->LoadFromFile("texture/glass.jpg");
		}

		// Group 2 - OBJ
		for (int i = 0; i < NR_OF_SWORDS; i++) {
			swordNodeVector[i].m_texture->LoadFromFile("obj/sting_sword/Sting_Base_Color.png");
		}

		//-------------SHADERS-------------
		std::shared_ptr<ShaderResource> shaderGeometryPass = std::make_shared<ShaderResource>();
		std::shared_ptr<ShaderResource> shaderDirectionalLight = std::make_shared<ShaderResource>();
		std::shared_ptr<ShaderResource> shaderPointLight = std::make_shared<ShaderResource>();
		std::shared_ptr<ShaderResource> shaderLightSphere = std::make_shared<ShaderResource>();

		shaderGeometryPass->LoadShaders("shader/vGeometryPass.txt", "shader/fGeometryPass.txt");
		shaderDirectionalLight->LoadShaders("shader/vDirectionalLight.txt", "shader/fDirectionalLight.txt");
		shaderPointLight->LoadShaders("shader/vPointLights.txt", "shader/fPointLights.txt");
		shaderLightSphere->LoadShaders("shader/vLightSphere.txt", "shader/fLightSphere.txt");

		for (int i = 0; i < 4; i++) {
			pointLightCubes[i].m_shader->LoadShaders("shader/cubeLightSource_vs.txt", "shader/cubeLightSource_fs.txt");
		}

		quad.m_shader = shaderDirectionalLight;
		sphere.m_shader = shaderLightSphere;

		//-------------MATERIALS-------------
		// Load the BlinnPhong shader. If already loaded reference to it instead.
		BlinnPhongMaterial bpLargeSword(swordNode0.m_shader, 16.0f, vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f));
		BlinnPhongMaterial bpSmallSword(swordNode1.m_shader, 16.0f, vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f));
		BlinnPhongMaterial bpMaterial(gltfModel.m_shader, 32.0f, vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), true);

		//-------------LIGHTS-------------
		LightingResource directionalLight(vec3(-0.2f, -1.0f, -0.3f), vec3(1.0f, 1.0f, 1.0f), 0.35f);

		// Pre-deferred rendering tests
		//LightingResource pointLight0(vec3(1.0f, 0.5f, 0.0f), vec3(1.0f, 0.0f, 1.0f), 0.012f);
		//LightingResource pointLight1(vec3(1.0f, 0.5f, -1.0f), vec3(1.0f, 0.0f, 0.0f), 0.015f);
		//LightingResource pointLight2(vec3(-1.0f, 0.5f, 1.0f), vec3(0.0f, 1.0f, 0.0f), 0.02f);
		//LightingResource pointLight3(vec3(1.0f, 0.5f, 1.0f), vec3(0.0f, 0.0f, 1.0f), 0.03f);

		//Light vectors
		std::vector<LightingResource> pointLightVector;

		// Pre-deferred rendering tests
		//pointLightVector.push_back(pointLight0);
		//pointLightVector.push_back(pointLight1);
		//pointLightVector.push_back(pointLight2);
		//pointLightVector.push_back(pointLight3);

		//-------------POINT LIGHT INFO-------------
		srand(13);
		for (unsigned int i = 0; i < NR_OF_POINT_LIGHTS; i++)
		{
			LightingResource pointLight;
			// calculate slightly random offsets
			float x = static_cast<float>((rand() % 100) / 100.0);
			float y = static_cast<float>(((rand() % 100) / 100.0) * 0.5);
			float z = static_cast<float>((rand() % 100) / 100.0);
			pointLight.m_pos = vec3(0, 1, 0);
			// calculate random color between 0.5 and 1
			float r = static_cast<float>(((rand() % 100) / 200.0f) + 0.5);
			float g = static_cast<float>(((rand() % 100) / 200.0f) + 0.5);
			float b = static_cast<float>(((rand() % 100) / 200.0f) + 0.5);
			pointLight.m_color = vec3(r, g, b);

			pointLightVector.push_back(pointLight);
		}

		//-------------INPUT HANDLING-------------
		window->SetKeyPressFunction([&gltfModel, &moveLightAround](int32 key, int32 scancode, int32 action, int32 mod) {
			InputManager* inputManager = InputManager::Instance();
			inputManager->HandleKeyEvent(key, action);
			});
		window->SetMousePressFunction([](int32 key, int32 action, int32 what) {
			InputManager* inputManager = InputManager::Instance();
			inputManager->HandleMouseEvent(key, action);
			});
		window->SetMouseMoveFunction([&gltfModel](float64 x, float64 y) {
			InputManager* inputManager = InputManager::Instance();

			if (inputManager->mouse.held[LEFT_CLICK]) {
				float rotSpeed = 0.005;
				float rad = 0.0f;

				inputManager->mouse.xPrevPos = inputManager->mouse.xCurrentPos;
				inputManager->mouse.yPrevPos = inputManager->mouse.yCurrentPos;
				inputManager->mouse.xCurrentPos = x;
				inputManager->mouse.yCurrentPos = y;

				// Up
				if (inputManager->mouse.yCurrentPos < inputManager->mouse.yPrevPos) {
					rad += rotSpeed;
					gltfModel.m_transform = gltfModel.m_transform * rotationx(rad);
				}
				// Down
				if (inputManager->mouse.yCurrentPos > inputManager->mouse.yPrevPos) {
					rad -= rotSpeed;
					gltfModel.m_transform = gltfModel.m_transform * rotationx(rad);
				}
				// Left
				if (inputManager->mouse.xCurrentPos < inputManager->mouse.xPrevPos) {
					rad += rotSpeed;
					gltfModel.m_transform = gltfModel.m_transform * rotationy(rad);
				}
				// Right
				if (inputManager->mouse.xCurrentPos > inputManager->mouse.xPrevPos) {
					rad -= rotSpeed;
					gltfModel.m_transform = gltfModel.m_transform * rotationy(rad);
				}
			}
			});

		//-------------PLACE IN SCENE-------------
		//pointLightCubes[0].m_transform.m[3] = vec4(pointLight0.pos, 1.0f);
		//pointLightCubes[1].transform.m[3] = vec4(pointLight1.pos, 1.0f);
		//pointLightCubes[2].transform.m[3] = vec4(pointLight2.pos, 1.0f);
		//pointLightCubes[3].transform.m[3] = vec4(pointLight3.pos, 1.0f);
		swordNode0.m_transform.m[3] = vec4(-0.7f, 0.0f, 0.0f, 1.0f);
		swordNode1.m_transform.m[3] = vec4(1.0f, 0.0f, 0.0f, 1.5f);
		//gltfModel.transform.m[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		// All positions of the objects in Deferred Rendering
		std::vector<mat4> objectPositions = InitObjectPositions(vec3{ -1, 0, -1, }, 4, 5, vec3{ 0.5, 0, 0.5 });

		//-------------SETUP G-BUFFER-------------
		unsigned int gBuffer;
		unsigned int gPosition, gNormal, gAlbedoSpec;
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		// Position color buffer
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

		// Normal color buffer
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

		// Color + specular color buffer
		glGenTextures(1, &gAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

		// Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		// Depth buffer
		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		// Check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//-------------SHADER CONFIGURATION-------------
		shaderDirectionalLight->Run();
		shaderDirectionalLight->SetIntUniform("gPosition", 0);
		shaderDirectionalLight->SetIntUniform("gNormal", 1);
		shaderDirectionalLight->SetIntUniform("gAlbedoSpec", 2);

		shaderPointLight->Run();
		shaderPointLight->SetIntUniform("gPosition", 0);
		shaderPointLight->SetIntUniform("gNormal", 1);
		shaderPointLight->SetIntUniform("gAlbedoSpec", 2);

		//-------------RENDER LOOP-------------
		while (this->window->IsOpen())
		{
			inputManager->BeginFrame();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();

			// Process input
			{
				GLfloat movementSpeed = 0.002f;
				// W-key
				if (inputManager->keyboard.held[UP]) {
					gltfModel.m_transform.m[3].z -= movementSpeed;
				}
				// S-key
				if (inputManager->keyboard.held[DOWN]) {
					gltfModel.m_transform.m[3].z += movementSpeed;
				}
				// A-key
				if (inputManager->keyboard.held[LEFT]) {
					gltfModel.m_transform.m[3].x -= movementSpeed;
				}
				// D-key
				if (inputManager->keyboard.held[RIGHT]) {
					gltfModel.m_transform.m[3].x += movementSpeed;
				}
				// Q-key
				if (inputManager->keyboard.held[Q]) {
					gltfModel.m_transform.m[3].y -= movementSpeed;
				}
				// E-key
				if (inputManager->keyboard.held[E]) {
					gltfModel.m_transform.m[3].y += movementSpeed;
				}
				// Space-key
				if (inputManager->keyboard.pressed[SPACE]) {
					moveLightAround = !moveLightAround;
				}
				// Esc-key
				if (inputManager->keyboard.pressed[ESCAPE]) {
					this->window->Close();
				}
			}

			// 1. GEOMETRY PASS
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			camera.OrbitScene(0); // To do: Too tightly coupled logic. Decouple and make a camera.Update() pipeline that takes care of everything.
			shaderGeometryPass->Run();
			shaderGeometryPass->SetMat4Uniform("projection", camera.GetCameraProjMatrix());
			shaderGeometryPass->SetMat4Uniform("view", camera.GetCameraViewMatrix());

			for (unsigned int i = 0; i < objectPositions.size(); i++) {
				GLint textureIndex = 0;
				GLint normalIndex = 1;
				for (auto const& mesh : gltfModel.m_model.m_meshes) {
					gltfModel.m_mesh = mesh;

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gltfModel.m_model.m_textures[textureIndex]->m_textureID);
					shaderGeometryPass->SetSampler2DUniform("texture_diffuse1", 0);

					if (gltfModel.m_model.m_hasNormals) {
						glActiveTexture(GL_TEXTURE0 + 1);
						glBindTexture(GL_TEXTURE_2D, gltfModel.m_model.m_textures[normalIndex]->m_textureID);
						shaderGeometryPass->SetSampler2DUniform("texture_specular1", 1);
						if (gltfModel.m_model.m_hasTangents) {
							shaderGeometryPass->SetIntUniform("hasTangents", true);
						}
					}

					gltfModel.m_transform = objectPositions[i];
					gltfModel.Draw(this->window, shaderGeometryPass, GL_TRIANGLES, true);
					textureIndex += 2;
					normalIndex += 2;
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// 2.0. LIGHT PASS - Directional Light
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shaderDirectionalLight->Run();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPosition);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNormal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

			// Send light relevant uniforms
			shaderDirectionalLight->SetVec3Uniform("light.direction", directionalLight.m_pos);
			shaderDirectionalLight->SetVec3Uniform("light.color", directionalLight.m_color);
			shaderDirectionalLight->SetFloatUniform("light.intensity", directionalLight.m_intensity);
			shaderDirectionalLight->SetVec3Uniform("viewPos", camera.GetCameraPos());

			shaderDirectionalLight->SetFloatUniform("material.shininess", bpMaterial.m_shininess);
			shaderDirectionalLight->SetVec3Uniform("material.ambient", bpMaterial.m_ambient);
			shaderDirectionalLight->SetVec3Uniform("material.diffuse", bpMaterial.m_diffuse);
			shaderDirectionalLight->SetVec3Uniform("material.specular", bpMaterial.m_specular);

			//// 2.5. LIGHT PASS - Point Lights
			// ERROR: Overrides direct light
			// SOLUTION: blend mode?
			shaderPointLight->Run();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPosition);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNormal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

			// Send light relevant uniforms
			const float constant = 1.0f;
			const float linear = 0.7f;
			const float quadratic = 1.8f;
			for (unsigned int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
				std::string number = std::to_string(i);
				shaderPointLight->SetVec3Uniform("lights[" + number + "].pos", pointLightVector[i].m_pos);
				shaderPointLight->SetVec3Uniform("lights[" + number + "].color", pointLightVector[i].m_color);
				shaderPointLight->SetFloatUniform("lights[" + number + "].intensity", 0.03);
				shaderPointLight->SetFloatUniform("lights[" + number + "].constant", constant);
				shaderPointLight->SetFloatUniform("lights[" + number + "].linear", linear);
				shaderPointLight->SetFloatUniform("lights[" + number + "].quadratic", quadratic);
				shaderPointLight->SetVec3Uniform("viewPos", camera.GetCameraPos());

				shaderPointLight->SetFloatUniform("material.shininess", bpMaterial.m_shininess);
				shaderPointLight->SetVec3Uniform("material.ambient", bpMaterial.m_ambient);
				shaderPointLight->SetVec3Uniform("material.diffuse", bpMaterial.m_diffuse);
				shaderPointLight->SetVec3Uniform("material.specular", bpMaterial.m_specular);
			}

			// Render quad for full-screen light pass
			quad.Draw(this->window);

			// Copy content of geometry's depth buffer to default framebuffer's depth buffer
			// ERROR: Incorrect depth
			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// 3. Render light spheres on top of scene
			shaderLightSphere->Run();
			shaderLightSphere->SetMat4Uniform("projection", camera.GetCameraProjMatrix());
			shaderLightSphere->SetMat4Uniform("view", camera.GetCameraViewMatrix());
			for (unsigned int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
				sphere.m_transform.m[3] = vec4(pointLightVector[i].m_pos, 1.0);
				shaderLightSphere->SetVec3Uniform("lightColor", pointLightVector[i].m_color);
				sphere.Draw(this->window);
			}



			// GLTF & Normalmap tests (to be removed)
			//// Camera & UI
			//mat4 projViewMatrix = camera.OrbitScene();
			//grid.Draw(&projViewMatrix[0][0]);

			//// Large sword
			//bpLargeSword.Apply(swordNode0.shader, swordNode0.texture, 0, directionalLight, pointLightVector, camera);
			//swordNode0.Draw(this->window, GL_QUADS);

			//// Small sword
			//bpSmallSword.Apply(swordNode1.shader, swordNode1.texture, 0, directionalLight, pointLightVector, camera);
			//swordNode1.Draw(this->window, GL_QUADS);

			//// GLTF Test
			//GLint textureIndex = 0;
			//GLint normalIndex = 1;
			//for (auto const& mesh : gltfModel.model.meshes) {
			//	gltfModel.mesh = mesh;
			//	bpMaterial.Apply(gltfModel.shader, gltfModel.model.textures[textureIndex], 0, directionalLight, pointLightVector, camera);
			//	if (gltfModel.model.hasNormals) {
			//		bpMaterial.Apply(gltfModel.shader, gltfModel.model.textures[normalIndex], 1, directionalLight, pointLightVector, camera);
			//	}
			//	if (gltfModel.model.hasTangents) {
			//		gltfModel.shader->SetIntUniform("hasTangents", true);
			//	}
			//	gltfModel.Draw(this->window);
			//	textureIndex += 2;
			//	normalIndex += 2;
			//}

			//// Point Lights
			////The point light which orbits around the scene. Play/pause by pressing SPACE
			if (moveLightAround) {
				for (auto& light : pointLightVector) {
					light.OrbitAnimation();
				}
			}

			gltfModel.OrbitAnimation();

			//// Pre-deferred rendering tests
			//// Draw cubes representing Point Lights
			//for (int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
			//	pointLightCubes[i].shader->Run();
			//	glActiveTexture(GL_TEXTURE0);
			//	glBindTexture(GL_TEXTURE_2D, pointLightCubes[i].texture->textureID);
			//	pointLightCubes[i].shader->SetMat4Uniform("view", camera.m_viewMatrix);
			//	pointLightCubes[i].shader->SetMat4Uniform("projection", camera.m_projMatrix);
			//	pointLightCubes[i].Draw(this->window, GL_TRIANGLES);
			//}

			this->window->SwapBuffers();

#ifdef CI_TEST
			// if we're running CI, we want to return and exit the application after one frame
			// break the loop and hopefully exit gracefully
			break;
#endif
		}
	}
}