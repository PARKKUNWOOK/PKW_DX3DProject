#include "Framework.h"
#include "WeatherScene.h"

WeatherScene::WeatherScene()
{
	collider = new BoxCollider(Vector3(50, 50, 1));	
	//collider = new SphereCollider(50);	
    
	//particle = new Sprite(L"Resources/Textures/FX/Lightning_8x1.png", 50, 10, 1, 8);
    //particle = new Spark(L"Resources/Textures/FX/star.png", true);
    //particle = new Rain();

    sprite = new Sprite(L"Resources/Textures/FX/explosion.png",
        128.0f, 128.0f, 5, 3, true);
    sprite->SetSize(Float2(10.0f, 10.0f));

    //sprite = new Sprite(L"Resources/Textures/FX/explosion_4x4.png",
    //    241.0f, 238.0f, 4, 4, false);
    //sprite->SetSize(Float2(10.0f, 10.0f));

    //sprite = new Sprite(L"Resources/Textures/FX/Redexplosion_4x4.png",
    //    241.0f, 238.0f, 4, 4, false);
    //sprite->SetSize(Float2(10.0f, 10.0f));

    //sprite = new Sprite(L"Resources/Textures/FX/Lightning_8x1.png",
    //    222.0f, 68.0f, 1, 8, false);
    //sprite->SetSize(Float2(10.0f, 10.0f));

    //sprite = new Sprite(L"Resources/Textures/FX/GreenLightning_8x1.png",
    //    222.0f, 68.0f, 1, 8, false);
    //sprite->SetSize(Float2(10.0f, 10.0f));

    //particleSystem = new ParticleSystem("Resources/FX/Explosion.fx");
}

WeatherScene::~WeatherScene()
{
	delete collider;
	delete sprite;
    //delete particleSystem;
}

void WeatherScene::Update()
{
    if (KEY->Down(VK_LBUTTON))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);
        RaycastHit hit;

        if (collider->IsRayCollision(ray, &hit))
        {
            sprite->Play(hit.point);

            Vector3 rot;
            rot.x = atan2(hit.normal.x, hit.normal.z);
            //particleSystem->SetLocalRotation(rot);
        }
    }

    sprite->Update();
    //particleSystem->Update();
}

void WeatherScene::PreRender()
{
}

void WeatherScene::Render()
{
	collider->Render();
    sprite->Render();
    //particleSystem->Render();
}

void WeatherScene::PostRender()
{
}

void WeatherScene::GUIRender()
{
    sprite->GUIRender();
}
