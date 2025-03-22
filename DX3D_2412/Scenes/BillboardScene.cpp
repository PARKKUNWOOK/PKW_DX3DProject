#include "Framework.h"
#include "BillboardScene.h"

BillboardScene::BillboardScene()
{
	terrain = new Terrain();
   /* trees.reserve(SIZE);
    FOR(SIZE)
    {
        Float2 size;
        size.x = GameMath::Random(3.0f, 10.0f);
        size.y = GameMath::Random(3.0f, 10.0f);
    
        Quad* tree = new Quad(size);
        tree->GetMaterial()->SetDiffuseMap(L"Resources/Textures/Landscape/Tree.png");
        Vector3 pos;
        pos.x = GameMath::Random(0.0f, terrain->GetSize().x);
        pos.z = GameMath::Random(0.0f, terrain->GetSize().y);
        pos.y = terrain->GetHeight(pos) + size.y * 0.5f;
        tree->SetLocalPosition(pos);        
    
        trees.push_back(tree);
    }*/  

    material = new Material(L"Geometry/Billboard.hlsl");
    material->SetDiffuseMap(L"Resources/Textures/Landscape/Tree.png");

    geometryShader = Shader::AddGS(L"Geometry/Billboard.hlsl");

    vertices.resize(SIZE);

    FOR(SIZE)
    {
        vertices[i].uv.x = GameMath::Random(3.0f, 10.0f);
        vertices[i].uv.y = GameMath::Random(3.0f, 10.0f);

        vertices[i].pos.x = GameMath::Random(0.0f, terrain->GetSize().x);
        vertices[i].pos.z = GameMath::Random(0.0f, terrain->GetSize().y);
        vertices[i].pos.y = terrain->GetHeight(vertices[i].pos) + vertices[i].uv.y * 0.5f;
    }

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexUV), SIZE);
}

BillboardScene::~BillboardScene()
{
    delete terrain;

    //for (Quad* tree : trees)
    //    delete tree;
    delete material;
    delete vertexBuffer;
}

void BillboardScene::Update()
{
    //for (Quad* tree : trees)
    //{
    //    //tree->SetLocalRotation(CAM->GetLocalRotation());
    //    //tree->SetLocalRotation({ 0, CAM->GetLocalRotation().y, 0 });
    //    Vector3 dir = tree->GetLocalPosition() - CAM->GetLocalPosition();
    //    tree->SetLocalRotation({ 0, atan2(dir.x, dir.z), 0 });
    //
    //    tree->UpdateWorld();
    //}
}

void BillboardScene::PreRender()
{
}

void BillboardScene::Render()
{
    terrain->Render();

    Environment::Get()->SetAlphaBlend(true);
    //Environment::Get()->SetAlphaToCoverage();
    Environment::Get()->SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
    //Environment::Get()->SetDepthEnable(false);

    //for (Quad* tree : trees)
    //    tree->Render();
    vertexBuffer->Set(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
    material->Set();
    geometryShader->Set();

    DC->Draw(SIZE, 0);

    Environment::Get()->SetAlphaBlend(false);
    Environment::Get()->SetDepthWriteMask(D3D11_DEPTH_WRITE_MASK_ALL);

    DC->GSSetShader(nullptr, nullptr, 0);
}

void BillboardScene::PostRender()
{
}

void BillboardScene::GUIRender()
{
}
