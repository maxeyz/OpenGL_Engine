#include "config.h"
#include "render/GraphicNode.h"
#include "render/InputManager.h"

void GraphicNode::OrbitAnimation() {
    float radius = 1.0f;
    float speed = 0.8;
    static float time = 0.0f;
    time += 0.0002f;

    m_transform.m[3].z = sin(time * speed) * radius;
    m_transform.m[3].x = cos(time * speed) * radius;
}

void GraphicNode::Draw(Display::Window* window, std::shared_ptr<ShaderResource> shader, GLuint method, bool hasPrimitives) {

    // Deferred Rendering
    if (shader) {
        shader->SetMat4Uniform("model", m_transform);
    }
    // Forward Rendering
    else if (m_shader) {
        m_shader->SetMat4Uniform("model", m_transform);
    }
    else {
        std::cerr << "Missing shader for rendering." << std::endl;
        return;
    }

    // GLTF models
    if (hasPrimitives) {
        for (auto const& primitive : m_mesh->m_primitives) {
            glBindVertexArray(primitive.m_VAO);
            m_mesh->Render(primitive);
        }
    }
    else {
        glBindVertexArray(m_mesh->m_VAO);
        if (method == GL_QUADS || method == GL_TRIANGLES || method == GL_POLYGON) {
            m_mesh->Render(method);
        }
        else {
            std::cout << "Primitive not added yet." << std::endl;
        }
    }
}