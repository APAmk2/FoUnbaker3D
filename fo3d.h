#pragma once
#include "ByteReader.hpp"
#include <string>
#include <vector>

#define BONES_PER_VERTEX ( 4 )

struct Matrix {
    float a1, a2, a3, a4;
    float b1, b2, b3, b4;
    float c1, c2, c3, c4;
    float d1, d2, d3, d4;
};

struct Vertex3D
{
    float position[3];
    float normal[3];
    float texCoord[2];
    float texCoordBase[2];
    float tangent[3];
    float bitangent[3];
    float blendWeights[BONES_PER_VERTEX];
    float blendIndices[BONES_PER_VERTEX];
};

class Mesh_t
{
public:
    std::vector<Vertex3D> vertices;
    std::vector<uint32_t> indices;
    std::string diffuseTexture;
    std::vector<uint32_t> skinBoneNameHashes;
    std::vector<Matrix> skinBoneOffsets;

    void read(ByteReader* reader);
};

class Bone_t
{
public:
	uint32_t nameHash;
    Matrix transformationMatrix;
    Matrix globalTransformationMatrix;
    Mesh_t mesh;
    std::vector<Bone_t> childBones;

    void read(ByteReader* reader);
};

class file_t
{
public:
    Bone_t skeleton;

    void read(ByteReader* reader);
};

union float2bytes
{
    float value;
    uint8_t bytes[sizeof(float)];
};