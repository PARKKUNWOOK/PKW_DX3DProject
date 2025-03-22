#pragma once

class OutlineScene : public Scene
{
public:
	OutlineScene();
	~OutlineScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	TestModel* testModel;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Quad* quad;

	FloatValueBuffer* valueBuffer;
};