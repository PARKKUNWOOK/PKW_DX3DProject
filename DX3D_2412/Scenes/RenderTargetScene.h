#pragma once

class RenderTargetScene : public Scene
{
public:
	RenderTargetScene();
	~RenderTargetScene();
	
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	TestModel* testModel;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	vector<Quad*> quads;
};