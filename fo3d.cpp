#include "fo3d.h"

void Mesh_t::read(ByteReader* reader)
{
    uint32_t len = 0;
    reader->Read(&len, sizeof(len));
    std::cout << "VertsLen: " << std::to_string(len) << " Cursor: " + std::to_string(reader->cursor()) << std::endl;
    vertices.resize(len);
    reader->Read(&vertices[0], len * sizeof(vertices[ 0 ]));

    reader->Read(&len, sizeof(len));
    std::cout << "IndicesLen: " << std::to_string(len) << " Cursor: " + std::to_string(reader->cursor()) << std::endl;
    indices.resize(len);
    reader->Read(&indices[0], len * sizeof(indices[0]));

    reader->Read(&len, sizeof(len));
    std::cout << "DiffTexLen: " << std::to_string(len) << " Cursor: " + std::to_string(reader->cursor()) << std::endl;
    diffuseTexture.resize(len);
    reader->Read(&diffuseTexture[0], len);
    std::cout << "diffuseTexture: " << diffuseTexture << " Cursor: " + std::to_string(reader->cursor()) << std::endl;

    reader->Read(&len, sizeof(len));
    std::cout << "SkinBoneNameHashesLen: " << std::to_string(len) << " Cursor: " + std::to_string(reader->cursor()) << std::endl;
    skinBoneNameHashes.resize(len);
    reader->Read(&skinBoneNameHashes[0], len * sizeof(skinBoneNameHashes[0]));

    reader->Read(&len, sizeof(len));
    std::cout << "SkinBoneOffsetslen: " << std::to_string(len) << " Cursor: " + std::to_string(reader->cursor()) << std::endl;
    skinBoneOffsets.resize(len);
    reader->Read(&skinBoneOffsets[0], len * sizeof(skinBoneOffsets[ 0 ]));
    //skinBones.resize(SkinBoneOffsets.size())
}

void Bone_t::read(ByteReader* reader)
{
    reader->Read(&nameHash, sizeof(nameHash));
    reader->Read(&transformationMatrix, sizeof(transformationMatrix));
    reader->Read(&globalTransformationMatrix, sizeof(globalTransformationMatrix));
    bool hasMesh = reader->u8() == 1;
    std::cout << "hasMesh: " << ( hasMesh ? "True" : "False" ) << " Cursor: " + std::to_string(reader->cursor()) << std::endl;
    if (hasMesh)
    {
        Mesh_t currMesh;
        currMesh.read(reader);
        mesh = currMesh;
    }
    uint32_t len;
    reader->Read(&len, sizeof(len));
    std::cout << "Childlen: " << std::to_string(len) << " Cursor: " + std::to_string(reader->cursor()) << std::endl;
    for (size_t i = 0; i < len; i++)
    {
        if (reader->Eof()) break;
        Bone_t currBone;
        currBone.read(reader);
        childBones.push_back( currBone );
    }
}

void file_t::read(ByteReader* reader)
{
    Bone_t root;
    root.read(reader);
    skeleton = root;
}