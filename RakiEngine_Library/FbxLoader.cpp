﻿#include "FbxLoader.h"
#include <iostream>

const std::string FbxLoader::baseDir = "Resources/";

const std::string FbxLoader::defaultTexName = "white1x1.png";

using namespace DirectX;

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize()
{
    assert(fbxmgr == nullptr);

    fbxmgr = FbxManager::Create();

    FbxIOSettings* ios = FbxIOSettings::Create(fbxmgr, IOSROOT);
    fbxmgr->SetIOSettings(ios);

    fbxImporter = FbxImporter::Create(fbxmgr, "");

}

void FbxLoader::Finalize()
{
    fbxImporter->Destroy();
    fbxmgr->Destroy();
}

fbxModel* FbxLoader::LoadFBXFile(string filename)
{
    const string dirPass = baseDir + filename + "/";

    folderPass = filename + "/";

    const string modelname = filename + ".fbx";

    const string fullpath = dirPass + modelname;

    std::cout << "Loading :" << fullpath << std::endl;

    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxmgr->GetIOSettings())) {
        assert(0);
    }

    FbxScene* fbxscene = FbxScene::Create(fbxmgr, "fbxScene");

    fbxImporter->Import(fbxscene);

    fbxModel* model = new fbxModel();
    model->name = filename;

    int nodecount = fbxscene->GetNodeCount();

    model->nodes.reserve(nodecount);

    ParseNodeRecursive(model, fbxscene->GetRootNode());

    model->CreateBuffers();

    fbxscene->Destroy();

    return model;
}

void FbxLoader::ParseNodeRecursive(fbxModel* model, FbxNode* fbxnode,Node *parent)
{
    string name = fbxnode->GetName();

    model->nodes.emplace_back();
    Node& node = model->nodes.back();

    node.name = fbxnode->GetName();

    FbxDouble3 rotate = fbxnode->LclRotation.Get();
    FbxDouble3 scale = fbxnode->LclScaling.Get();
    FbxDouble3 trans = fbxnode->LclTranslation.Get();

    node.rotation = { (float)rotate[0],(float)rotate[1],(float)rotate[2],0.0f };
    node.scaling = { (float)scale[0],(float)scale[1],(float)scale[2],0.0f };
    node.translation = { (float)trans[0],(float)trans[1],(float)trans[2],1.0f };

    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

    XMMATRIX matscale, matrot, mattrans;
    matscale = XMMatrixScalingFromVector(node.scaling);
    matrot = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    mattrans = XMMatrixTranslationFromVector(node.translation);

    node.transform = XMMatrixIdentity();
    node.transform *= matscale;
    node.transform *= matrot;
    node.transform *= mattrans;
    node.globalTransform = node.transform;

    if (parent) {
        node.parent = parent;

        node.globalTransform *= parent->globalTransform;
    }
    

    FbxNodeAttribute* fbxNodeAttribute = fbxnode->GetNodeAttribute();
    if (fbxNodeAttribute) {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
            model->meshNode = &node;
            ParseMesh(model, fbxnode);
        }
    }

    std::cout << fbxnode->GetChildCount() << std::endl;
    //子ノードに対し再帰呼び出し
    for (int i = 0; i < fbxnode->GetChildCount(); i++) {
        ParseNodeRecursive(model, fbxnode->GetChild(i),&node);
    }

}

void FbxLoader::ParseMesh(fbxModel* model, FbxNode* fbxnode)
{
    FbxMesh* fbxmesh = fbxnode->GetMesh();

    ParseMeshVertices(model, fbxmesh);

    ParseMeshFaces(model, fbxmesh);

    ParseMaterial(model, fbxnode);

}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
    size_t pos1;

    pos1 = path.rfind('\\');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    pos1 = path.rfind('/');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    return path;

}

void FbxLoader::ParseMeshVertices(fbxModel* model, FbxMesh* mesh)
{
    auto& vertices = model->vertices;

    const int ctrlPointCount = mesh->GetControlPointsCount();

    fbxVertex v{};
    model->vertices.resize(ctrlPointCount, v);

    FbxVector4 *pCoord = mesh->GetControlPoints();

    for (int i = 0; i < ctrlPointCount; i++) {
        fbxVertex& vertex = vertices[i];
        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];
    }
}

void FbxLoader::ParseMeshFaces(fbxModel* model, FbxMesh* mesh)
{
    auto& vertices = model->vertices;
    auto& indices = model->indices;

    assert(indices.size() == 0);

    const int polygonCount = mesh->GetPolygonCount();
    const int texUVCount = mesh->GetTextureUVCount();

    FbxStringList uvNames;
    mesh->GetUVSetNames(uvNames);

    for (int i = 0; i < polygonCount; i++) {
        const int polygonsize = mesh->GetPolygonSize(i);
        assert(polygonsize <= 4);

        for (int j = 0; j < polygonsize; j++) {
            int index = mesh->GetPolygonVertex(i, j);
            assert(index >= 0);

            fbxVertex& v = vertices[index];
            FbxVector4 normal;
            if (mesh->GetPolygonVertexNormal(i, j, normal)) {
                v.normal.x = (float)normal[0];
                v.normal.y = (float)normal[1];
                v.normal.z = (float)normal[2];
            }

            if (texUVCount > 0) {
                FbxVector2 uvs;
                bool lUnmappedUV;

                if (mesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV)) {
                    v.uv.x = (float)uvs[0];
                    v.uv.y = (float)uvs[1];
                }
            }

            if (j < 3) {
                indices.push_back(index);
            }
            else {
                int index2 = indices[indices.size() - 1];
                int index3 = index;
                int index0 = indices[indices.size() - 3];
                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index0);
            }
        }
    }

}

void FbxLoader::ParseMaterial(fbxModel* model, FbxNode* node)
{
    const int materialCount = node->GetMaterialCount();
    if (materialCount > 0) {

        FbxSurfaceMaterial* material = node->GetMaterial(0);

        bool textureLoaded = false;

        if (material) {
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
                FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                model->material.ambient.x = (float)ambient.Get()[0];
                model->material.ambient.y = (float)ambient.Get()[1];
                model->material.ambient.z = (float)ambient.Get()[2];

                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
                model->material.diffuse.x = (float)diffuse.Get()[0];
                model->material.diffuse.y = (float)diffuse.Get()[1];
                model->material.diffuse.z = (float)diffuse.Get()[2];
            }

            const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (diffuseProperty.IsValid()) {
                const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                if (texture) {
                    const char* filepath = texture->GetFileName();

                    string path_str = filepath;
                    string name = ExtractFileName(path_str);

                    model->material.texNumber = TexManager::LoadTexture(baseDir + folderPass + name);

                    textureLoaded = true;
                }
            }
        }

        if (!textureLoaded) {
            model->material.texNumber = TexManager::LoadTexture(baseDir + defaultTexName);
        }
    }

}
