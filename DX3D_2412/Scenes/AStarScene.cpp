#include "Framework.h"
#include "AStarScene.h"

AStarScene::AStarScene()
{
	terrain = new Terrain();
	ghost = new Ghost();
	ghost->SetTerrain(terrain);
	aStar = new AStar();
	aStar->SetNode(terrain);
	ghost->SetAStar(aStar);

	particle = new ParticleSystem("Resources/FX/Explosion.fx");

	Audio::Get()->Add("Pick", "Resources/Sounds/Pick.wav");
	Audio::Get()->Add("BGM", "Resources/Sounds/naruto_bgm.mp3", true);

	Audio::Get()->Play("BGM");
}

AStarScene::~AStarScene()
{
	delete terrain;
	delete ghost;
	delete aStar;
	delete particle;
}

void AStarScene::Update()
{
	ghost->Update();
	aStar->Update();
	particle->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	particle->Render();

	terrain->Render();
	ghost->Render();
	aStar->Render();	
}

void AStarScene::PostRender()
{
}

void AStarScene::GUIRender()
{
	ghost->Edit();
	particle->Edit();
}
