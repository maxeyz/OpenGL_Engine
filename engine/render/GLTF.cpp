#include "config.h"
#include "render/GLTF.h"
#include "core/stb_image.h"
#include <algorithm>
#include <iostream>
#include <experimental/filesystem>

using namespace fx;
namespace fs = std::experimental::filesystem;

GLTF::GLTF() {
    // Empty
}

GLTF::~GLTF() {
    // Empty
}

int GLTF::GetSlotNumber(std::string attrib) {
    if (attrib == "POSITION") { return 0; }
    if (attrib == "COLOR_0") { return 1; }
    if (attrib == "TEXCOORD_0") { return 2; }
    if (attrib == "NORMAL") { return 3; }
    if (attrib == "TANGENT") { return 4; } 
    else {
        std::cout << "Missing attribute slot" << std::endl;
        return -1;
    }
}

GLTF GLTF::LoadGLTF(const char* path, bool flipUV) {
    // Attempt to read the .gltf file
    GLTF model;
    try { 
        gltf::LoadFromText(path);
    }
    catch (const std::exception& e) {
        std::cout << "Error: GLTF file could not be read." << std::endl;
        return model;
    }
    gltf::Document doc = gltf::LoadFromText(path);

    // TEXTURES
    auto LoadTexture = [&doc, &path, &model](int index, fx::gltf::Texture const& texture, bool flipUV) {
        TextureResource t;
        fx::gltf::Image const& image = doc.images[texture.source];
        std::string name = image.uri;

        std::string filePath = path;
        for (int i = strlen(path) - 1; i > 0; i--) {
            if (filePath[i] == '/') {
                break;
            }
            else {
                filePath.pop_back();
            }
        }
        // Concatenate a string to the directory path of the texture.
        std::string conc = filePath + name;
        const char* texturePath = conc.c_str();

        t.LoadFromFile(texturePath, flipUV);
        std::cout << "Loaded: " << texturePath << std::endl;

        model.textures.push_back(std::make_shared<TextureResource>(t));
    };
    
    for (auto const& material : doc.materials) {
        int textureIndex = material.pbrMetallicRoughness.baseColorTexture.index;
        int normalIndex = material.normalTexture.index;
        if (textureIndex != -1) {
            LoadTexture(textureIndex, doc.textures[textureIndex], flipUV);
        }
        if (normalIndex != -1) {
            LoadTexture(textureIndex, doc.textures[normalIndex], flipUV);
            model.hasNormals = true;
        }
    }

    // MESHES
    // Generate the buffers
    model.buffers.resize(doc.bufferViews.size());
    glGenBuffers(doc.bufferViews.size(), model.buffers.data());

    // Load the data from the bufferViews into our buffers
    for (unsigned i = 0; i < doc.bufferViews.size(); i++) {
        auto const& bufferView = doc.bufferViews[i];
        
        GLenum target = (GLenum)bufferView.target == 0 ? GL_ELEMENT_ARRAY_BUFFER : (GLenum)bufferView.target;
        glBindBuffer(target, model.buffers[i]);
        glBufferData(target, bufferView.byteLength, (byte*)(doc.buffers[bufferView.buffer].data.data()) + bufferView.byteOffset, GL_STATIC_DRAW);
    }

    // Setup vertex attributes for every primitive in every mesh
    for (auto const& mesh : doc.meshes) {
        MeshResource meshResource;
        TextureResource textureResource;
        for (auto const& primitive : mesh.primitives) {
            if (doc.images[primitive.material].IsEmbeddedResource()) { //Embedded textures.
                std::vector<uint8_t> data;
                doc.images[primitive.material].MaterializeData(data);
                textureResource.LoadFromMemory(data);
            }
            MeshResource::Primitive prim;
            glGenVertexArrays(1, &prim.VAO);
            glBindVertexArray(prim.VAO);
            for (auto const& attribute : primitive.attributes) {
                auto const& accessor = doc.accessors[attribute.second];

                MeshResource::Attribute attrib;
                attrib.byteOffset = accessor.byteOffset;
                attrib.stride = doc.bufferViews[accessor.bufferView].byteStride;
                attrib.normalized = accessor.normalized;
                attrib.slot = GetSlotNumber(attribute.first);
                attrib.type = (GLenum)accessor.componentType;

                if (accessor.type == gltf::Accessor::Type::Scalar) {
                    attrib.components = 1;
                }
                else if (
                    accessor.type == gltf::Accessor::Type::Vec2 ||
                    accessor.type == gltf::Accessor::Type::Vec3 ||
                    accessor.type == gltf::Accessor::Type::Vec4) {
                    attrib.components = (GLint)accessor.type;
                }

                if (attribute.first == "TANGENT") {
                    model.hasTangents = true;
                }

                GLenum target = (GLenum)doc.bufferViews[accessor.bufferView].target == 0 ? GL_ARRAY_BUFFER : (GLenum)doc.bufferViews[accessor.bufferView].target;
                glBindBuffer(target, model.buffers[accessor.bufferView]);
                glEnableVertexArrayAttrib(prim.VAO, attrib.slot);
                glVertexAttribPointer(attrib.slot, attrib.components, attrib.type, attrib.normalized, attrib.stride, (void*)(intptr_t)attrib.byteOffset);
            }
            // Setup EBO
            auto const& accessor = doc.accessors[primitive.indices];

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.buffers[accessor.bufferView]);
            prim.nrIndices = accessor.count;
            prim.byteOffset = accessor.byteOffset;
            prim.indexType = (GLenum)accessor.componentType;

            glBindVertexArray(0);
            meshResource.primitives.push_back(std::move(prim));
        }
        model.meshes.push_back(std::move(std::make_shared<MeshResource>(meshResource)));
    }

    return model;
}