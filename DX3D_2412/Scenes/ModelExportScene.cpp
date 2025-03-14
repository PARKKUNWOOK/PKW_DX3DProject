#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
    string name = "Monster1";
    string extension = "fbx";
    string file = "Resources/Models/FBX/" + name + "." + extension;

    //exporter = new ModelExporter(name, file);
    //exporter->ExportMaterial();
    //exporter->ExportMesh();
    //delete exporter;

    string clipName = "Idle";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";    
    
    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
    
    clipName = "Run";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";
    
    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
    
    clipName = "Attack";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";
    
    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;

    clipName = "Death";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
}

ModelExportScene::~ModelExportScene()
{
}

void ModelExportScene::Update()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PostRender()
{
}

void ModelExportScene::GUIRender()
{
}
