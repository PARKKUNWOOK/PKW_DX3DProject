#include "Framework.h"
#include "WeatherScene.h"

WeatherScene::WeatherScene()
{
	//collider = new BoxCollider(Vector3(50, 50, 1));	
	collider = new SphereCollider(50);	
    
	//particle = new Sprite(L"Resources/Textures/FX/Lightning_8x1.png", 50, 10, 1, 8);
    //particle = new Spark(L"Resources/Textures/FX/star.png", true);
    //particle = new Rain();
    //particle = new Snow();
    particle = new ParticleSystem("Resources/FX/Explosion.fx");
}

WeatherScene::~WeatherScene()
{
	delete collider;
	delete particle;
}

void WeatherScene::Update()
{
    if (KEY->Down(VK_LBUTTON))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);
        RaycastHit hit;

        if (collider->IsRayCollision(ray, &hit))
        {
            particle->Play(hit.point);

            Vector3 rot;
            rot.x = atan2(hit.normal.x, hit.normal.z);
            particle->SetLocalRotation(rot);
        }
    }

    particle->Update();
}

void WeatherScene::PreRender()
{
}

void WeatherScene::Render()
{
	collider->Render();
    particle->Render();
}

void WeatherScene::PostRender()
{
}

void WeatherScene::GUIRender()
{
    //particle->GUIRender();
}
