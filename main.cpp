//FoUnbaker3D - main.cpp: the main file of program, the primary logic is written here.
//Copyright (C) 2024, APAMk2
#include "fo3d.h"
#include "tinygltf/tiny_gltf.h"
#include <iostream>
#include <filesystem>
#include <vector>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

using namespace std;

void writeMesh(tinygltf::Model& model, Bone_t& currBone)
{
    size_t indicesLen = currBone.mesh.indices.size();
    size_t verticesLen = currBone.mesh.vertices.size();

    if (verticesLen > 0 && verticesLen == indicesLen)
    {
        tinygltf::Mesh mesh;
        tinygltf::Primitive primitive;
        tinygltf::Buffer buffer;
        tinygltf::BufferView indicesView;
        tinygltf::BufferView verticesView;
        tinygltf::BufferView normalsView;
        tinygltf::BufferView texView;
        tinygltf::BufferView tangentView;
        tinygltf::BufferView weightView;
        tinygltf::Accessor indicesAccessor;
        tinygltf::Accessor vertexAccessor;
        tinygltf::Accessor normalsAccessor;
        tinygltf::Accessor texAccessor;
        tinygltf::Accessor tangentAccessor;
        tinygltf::Accessor weightAccessor;
        tinygltf::Node node;

        // This is the raw data buffer. 
        for (size_t i = 0; i < indicesLen; i++)
        {
            //cout << currBone.mesh.indices[i] << endl;
            uint8_t array[2] = { currBone.mesh.indices[i] & 0xff, currBone.mesh.indices[i] >> 8 };
            buffer.data.push_back(array[0]);
            buffer.data.push_back(array[1]);
        }

        for (size_t i = 0; i < verticesLen; i++)
        {
            for (size_t l = 0; l < 3; l++)
            {
                //cout << currBone.mesh.vertices[i].position[l] << " ";
                float2bytes currConv;
                currConv.value = currBone.mesh.vertices[i].position[l];

                for (size_t j = 0; j < 4; j++)
                {
                    buffer.data.push_back(currConv.bytes[j]);
                }
            }
            //cout << endl;
        }

        for (size_t i = 0; i < verticesLen; i++)
        {
            for (size_t l = 0; l < 3; l++)
            {
                float2bytes currConv;
                currConv.value = currBone.mesh.vertices[i].normal[l];

                for (size_t j = 0; j < 4; j++)
                {
                    buffer.data.push_back(currConv.bytes[j]);
                }
            }
        }

        for (size_t i = 0; i < verticesLen; i++)
        {
            for (size_t l = 0; l < 2; l++)
            {
                float2bytes currConv;
                currConv.value = currBone.mesh.vertices[i].texCoord[l];

                for (size_t j = 0; j < 4; j++)
                {
                    buffer.data.push_back(currConv.bytes[j]);
                }
            }
        }

        for (size_t i = 0; i < verticesLen; i++)
        {
            for (size_t l = 0; l < BONES_PER_VERTEX; l++)
            {
                float2bytes currConv;
                currConv.value = currBone.mesh.vertices[i].tangent[l];

                for (size_t j = 0; j < 4; j++)
                {
                    buffer.data.push_back(currConv.bytes[j]);
                }
            }
        }

        for (size_t i = 0; i < verticesLen; i++)
        {
            for (size_t l = 0; l < BONES_PER_VERTEX; l++)
            {
                float2bytes currConv;
                currConv.value = currBone.mesh.vertices[i].blendWeights[l];

                for (size_t j = 0; j < 4; j++)
                {
                    buffer.data.push_back(currConv.bytes[j]);
                }
            }
        }

        uint32_t buffersSize = model.buffers.size();
        uint32_t bufferViewsSize = model.bufferViews.size();

        // "The indices of the vertices (ELEMENT_ARRAY_BUFFER) take up 6 bytes in the
        // start of the buffer.
        indicesView.buffer = buffersSize;
        indicesView.byteOffset = 0;
        indicesView.byteLength = indicesLen * 2;
        indicesView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

        // The vertices take up 36 bytes (3 vertices * 3 floating points * 4 bytes)
        // at position 8 in the buffer and are of type ARRAY_BUFFER
        verticesView.buffer = buffersSize;
        verticesView.byteOffset = indicesLen * 2;
        verticesView.byteLength = verticesLen * 3 * 4;
        verticesView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

        normalsView.buffer = buffersSize;
        normalsView.byteOffset = (indicesLen * 2) + (verticesLen * 3 * 4);
        normalsView.byteLength = verticesLen * 3 * 4;
        normalsView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

        texView.buffer = buffersSize;
        texView.byteOffset = (indicesLen * 2) + (verticesLen * 6 * 4);
        texView.byteLength = verticesLen * 2 * 4;
        texView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

        tangentView.buffer = buffersSize;
        tangentView.byteOffset = (indicesLen * 2) + (verticesLen * 8 * 4);
        tangentView.byteLength = verticesLen * 4 * 4;
        tangentView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

        weightView.buffer = buffersSize;
        weightView.byteOffset = (indicesLen * 2) + (verticesLen * 12 * 4);
        weightView.byteLength = verticesLen * 4 * 4;
        weightView.target = TINYGLTF_TARGET_ARRAY_BUFFER;

        // Describe the layout of bufferView1, the indices of the vertices
        indicesAccessor.bufferView = bufferViewsSize;
        indicesAccessor.byteOffset = 0;
        indicesAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
        indicesAccessor.count = indicesLen;
        indicesAccessor.type = TINYGLTF_TYPE_SCALAR;
        indicesAccessor.maxValues.push_back(2);
        indicesAccessor.minValues.push_back(0);

        // Describe the layout of bufferView2, the vertices themself
        vertexAccessor.bufferView = bufferViewsSize + 1;
        vertexAccessor.byteOffset = 0;
        vertexAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        vertexAccessor.count = verticesLen;
        vertexAccessor.type = TINYGLTF_TYPE_VEC3;
        vertexAccessor.maxValues = { 1.0, 1.0, 0.0 };
        vertexAccessor.minValues = { 0.0, 0.0, 0.0 };

        normalsAccessor.bufferView = bufferViewsSize + 2;
        normalsAccessor.byteOffset = 0;
        normalsAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        normalsAccessor.count = verticesLen;
        normalsAccessor.type = TINYGLTF_TYPE_VEC3;

        texAccessor.bufferView = bufferViewsSize + 3;
        texAccessor.byteOffset = 0;
        texAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        texAccessor.count = verticesLen;
        texAccessor.type = TINYGLTF_TYPE_VEC2;

        tangentAccessor.bufferView = bufferViewsSize + 4;
        tangentAccessor.byteOffset = 0;
        tangentAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        tangentAccessor.count = verticesLen;
        tangentAccessor.type = TINYGLTF_TYPE_VEC4;

        weightAccessor.bufferView = bufferViewsSize + 5;
        weightAccessor.byteOffset = 0;
        weightAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        weightAccessor.count = verticesLen;
        weightAccessor.type = TINYGLTF_TYPE_VEC4;

        // Build the mesh primitive and add it to the mesh
        primitive.indices = bufferViewsSize;                 // The index of the accessor for the vertex indices
        primitive.attributes["POSITION"] = bufferViewsSize + 1;  // The index of the accessor for positions
        primitive.attributes["NORMAL"] = bufferViewsSize + 2;
        primitive.attributes["TEXCOORD_0"] = bufferViewsSize + 3;
        primitive.attributes["TANGENT"] = bufferViewsSize + 4;
        primitive.attributes["WEIGHTS_0"] = bufferViewsSize + 5;
        primitive.material = 0;
        primitive.mode = TINYGLTF_MODE_TRIANGLES;
        mesh.primitives.push_back(primitive);

        model.meshes.push_back(mesh);
        cout << "Pushed back a mesh" << endl;
        model.buffers.push_back(buffer);
        model.bufferViews.push_back(indicesView);
        model.bufferViews.push_back(verticesView);
        model.bufferViews.push_back(normalsView);
        model.bufferViews.push_back(texView);
        model.bufferViews.push_back(tangentView);
        model.bufferViews.push_back(weightView);
        model.accessors.push_back(indicesAccessor);
        model.accessors.push_back(vertexAccessor);
        model.accessors.push_back(normalsAccessor);
        model.accessors.push_back(texAccessor);
        model.accessors.push_back(tangentAccessor);
        model.accessors.push_back(weightAccessor);

        node.mesh = buffersSize;
        model.nodes.push_back(node);
    }

    for (size_t i = 0, len = currBone.childBones.size(); i < len; i++)
    {
        writeMesh(model, currBone.childBones[i]);
    }
}

void writeModel(file_t& file, filesystem::path filename)
{
    tinygltf::Model m;
    tinygltf::Scene scene;
    
    tinygltf::Asset asset;

    writeMesh(m, file.skeleton);
    scene.nodes.push_back(0); // Default scene

    // Define the asset. The version is required
    asset.version = "2.0";
    asset.generator = "tinygltf";

    // Now all that remains is to tie back all the loose objects into the
    // our single model.
    m.scenes.push_back(scene);
    m.asset = asset;

    // Create a simple material
    tinygltf::Material mat;
    mat.pbrMetallicRoughness.baseColorFactor = { 1.0f, 0.9f, 0.9f, 1.0f };
    mat.doubleSided = true;
    m.materials.push_back(mat);

    // Save it to a file
    tinygltf::TinyGLTF gltf;
    gltf.WriteGltfSceneToFile(&m, filename.stem().string() + ".gltf", true, true, true, false);
}

int main(int argc, char* argv[]) 
{
    if(argc < 2)
    {
        cout << "HELP:\nTo use this tool drag&drop needed Fonline Art file to program, or type file name as arg in console.\n\nABOUT:\nThe format has no name, but used in FOnline engine.\nThe author of format is Anton 'Cvet' Tsvetinsky.\nThe author of FoUnbaker is APAMk2. 2024.";
        return 0;
    }
    filesystem::path filename = argv[1];
    ByteReader* reader = new ByteReader;
    reader->Reset(filename.string(), ByteReader::BigEndian);
    file_t file;
    file.read(reader);
    writeModel(file, filename);

    return 0;
}