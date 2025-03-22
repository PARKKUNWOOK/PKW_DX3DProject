#include "Framework.h"

TestModel::TestModel()
{
    sphere = new Sphere();
    sphere->SetTag("TestSphere");
    sphere->GetMaterial()->Load("Resources/Materials/FieldStone.mat");
    sphere->Load();

    model = new Model("Agatha");
    model->SetTag("TestModel");
    model->Load();

    animator = new ModelAnimator("Ghost");
    animator->SetTag("TestAnimator");
    animator->ReadClip("Ghost_animation", 3);
    animator->CreateTexture();
    animator->Load();

    skybox = new Skybox(L"Resources/Textures/Landscape/Snow_ENV.dds");
}

TestModel::~TestModel()
{
    delete sphere;
    delete model;
    delete animator;
    delete skybox;
}

void TestModel::Update()
{
    sphere->UpdateWorld();
    model->UpdateWorld();
    animator->Update();
}

void TestModel::Render()
{
    //skybox->Render();

    sphere->Render();
    model->Render();
    animator->Render();
}

void TestModel::Edit()
{
    sphere->Edit();
    model->Edit();
    animator->Edit();
}

void TestModel::SetPixelShader(wstring shaderFile)
{
    sphere->GetMaterial()->SetPixelShader(shaderFile);
    model->SetPixelShader(shaderFile);
    animator->SetPixelShader(shaderFile);
}
