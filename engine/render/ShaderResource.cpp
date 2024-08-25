#include "config.h"
#include "render/ShaderResource.h"

ShaderResource::ShaderResource() {
    // Empty
}

ShaderResource::~ShaderResource() {
    // Empty
}

void ShaderResource::LoadShaders(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Read files
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure error)
    {
        std::cout << "ERROR: Problem reading shader files.\n" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Load vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR: Vertex shader failed to compile.\n" << infoLog << std::endl;
    };

    // Load fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR: Fragment shader failed to compile.\n" << infoLog << std::endl;
    };

    // Link shaders
    this->program = glCreateProgram();
    glAttachShader(this->program, vertex);
    glAttachShader(this->program, fragment);
    glLinkProgram(this->program);

    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR: Failed linking shaders.\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void ShaderResource::SendAllLightUniformsToShader(ShaderResource shader, LightingResource directionalLight, std::vector<LightingResource> pointLight) {
    // Directional light uniforms
	shader.SetVec3Uniform("directionalLight.direction", directionalLight.pos); // Direction of light, not pos of light source.
    shader.SetVec3Uniform("directionalLight.color", directionalLight.color);
    shader.SetFloatUniform("directionalLight.intensity", directionalLight.intensity);

	// Point light uniforms
	for (int i = 0; i < pointLight.size(); i++) {
		std::string number = std::to_string(i);
        shader.SetVec3Uniform("pointLight[" + number + "].pos", pointLight[i].pos);
        shader.SetVec3Uniform("pointLight[" + number + "].color", pointLight[i].color);
        shader.SetFloatUniform("pointLight[" + number + "].intensity", pointLight[i].intensity);
        shader.SetFloatUniform("pointLight[" + number + "].constant", 1.0f);
        shader.SetFloatUniform("pointLight[" + number + "].linear", 0.7f);
        shader.SetFloatUniform("pointLight[" + number + "].quadratic", 1.8f);
	}
}

void ShaderResource::Run() {
    glUseProgram(this->program);
}

void ShaderResource::SetMat4Uniform(const std::string& name, mat4 mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderResource::SetVec4Uniform(const std::string& name, vec4 vec) const {
    glUniform4f(glGetUniformLocation(this->program, name.c_str()), vec[0], vec[1], vec[2], vec[3]);
}

void ShaderResource::SetVec3Uniform(const std::string& name, vec3 vec) const {
    glUniform3f(glGetUniformLocation(this->program, name.c_str()), vec[0], vec[1], vec[2]);
}

void ShaderResource::SetFloatUniform(const std::string& name, GLfloat val) const {
    glUniform1f(glGetUniformLocation(this->program, name.c_str()), val);
}

void ShaderResource::SetIntUniform(const std::string& name, GLfloat val) const {
    glUniform1i(glGetUniformLocation(this->program, name.c_str()), val);
}

void ShaderResource::SetSampler2DUniform(const std::string& name, GLint textureNr) const {
    glUniform1i(glGetUniformLocation(this->program, name.c_str()), textureNr);
}