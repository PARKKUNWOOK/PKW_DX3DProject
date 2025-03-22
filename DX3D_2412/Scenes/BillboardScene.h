#pragma once

class BillboardScene : public Scene
{
private:
	const UINT SIZE = 1000;

public:
	BillboardScene();
	~BillboardScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	Terrain* terrain;
	//vector<Quad*> trees;  
	Material* material;
	VertexBuffer* vertexBuffer;
	GeometryShader* geometryShader;

	vector<VertexUV> vertices;
};