#include "Framework.h"
#include "CubeMapEditorScene.h"

CubeMapEditorScene::CubeMapEditorScene()
{
	mapEditor = new CubeMapEditor();
}

CubeMapEditorScene::~CubeMapEditorScene()
{
	delete mapEditor;
}

void CubeMapEditorScene::Update()
{
	mapEditor->Update();
}

void CubeMapEditorScene::PreRender()
{
	
}

void CubeMapEditorScene::Render()
{
	mapEditor->Render();
}

void CubeMapEditorScene::PostRender()
{
}

void CubeMapEditorScene::GUIRender()
{
	mapEditor->Edit();	
}
