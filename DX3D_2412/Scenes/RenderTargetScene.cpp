#include "Framework.h"
#include "RenderTargetScene.h"

RenderTargetScene::RenderTargetScene()
{
	testModel = new TestModel();

    renderTarget = new RenderTarget();
    depthStencil = new DepthStencil();

    Texture* texture = Texture::Add(L"Target", renderTarget->GetSRV());
    Quad* quad = new Quad(CENTER); 
    quad->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
    quad->GetMaterial()->SetDiffuseMap(texture);
    quad->SetLocalPosition(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0);
    quad->UpdateWorld();

    quads.push_back(quad);

    quad = new Quad(CENTER);
    quad->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
    quad->GetMaterial()->SetDiffuseMap(texture);
    quad->SetLocalPosition(SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 4, 0);
    quad->UpdateWorld();

    quads.push_back(quad);
}

RenderTargetScene::~RenderTargetScene()
{
	delete testModel;

    delete renderTarget;
    delete depthStencil;

    for (Quad* quad : quads)
        delete quad;
}

void RenderTargetScene::Update()
{
	testModel->Update();
}

void RenderTargetScene::PreRender()
{
    renderTarget->Set(depthStencil);

    testModel->Render();
}

void RenderTargetScene::Render()
{
	//testModel->Render();

    
}

void RenderTargetScene::PostRender()
{
    for (Quad* quad : quads)
        quad->Render();
}

void RenderTargetScene::GUIRender()
{
	//testModel->Edit();
}
