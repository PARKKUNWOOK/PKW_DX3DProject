#pragma once

class MapEditorScene : public Scene
{
public:
	MapEditorScene();
	~MapEditorScene();
	
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void ByteAddress();

private:
	TerrainEditor* terrainEditor;
};
