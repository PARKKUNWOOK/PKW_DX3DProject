#include "Framework.h"
#include "EffectManager.h"

EffectManager::EffectManager()
{
    for (int i = 0; i < POOL_SIZE; i++)
    {
        Sprite* sprite = new Sprite(L"Resources/Textures/FX/explosion_4x4.png", 128, 128, 4, 4, true);
        sprite->SetSize(Float2(0.5f, 0.5f));
        FollowEffect effect;
        effect.sprite = sprite;
        effect.target = nullptr;
        effect.type = BulletEffectType::Green;
        greenBulletEffects.push_back(effect);
    }

    for (int i = 0; i < POOL_SIZE; i++)
    {
        Sprite* sprite = new Sprite(L"Resources/Textures/FX/Redexplosion_4x4.png", 128, 128, 4, 4, true);
        sprite->SetSize(Float2(0.5f, 0.5f));
        FollowEffect effect;
        effect.sprite = sprite;
        effect.target = nullptr;
        effect.type = BulletEffectType::Red;
        redBulletEffects.push_back(effect);
    }

    for (int i = 0; i < LIGHTNING_POOL_SIZE; i++)
    {
        Sprite* sprite = new Sprite(L"Resources/Textures/FX/Lightning_8x1.png", 222.0f, 68.0f, 1, 8, false);
        sprite->SetSize(Float2(10.0f, 10.0f));
        LightningEffect effect;
        effect.sprite = sprite;
        lightningEffects.push_back(effect);
    }

    explosionEffect = new Sprite(L"Resources/Textures/FX/explosion.png", 128.0f, 128.0f, 5, 3, true);
    explosionEffect->SetSize(Float2(4, 4));
}

EffectManager::~EffectManager()
{
    delete explosionEffect;

    for (int i = 0; i < greenBulletEffects.size(); i++)
        delete greenBulletEffects[i].sprite;

    for (int i = 0; i < redBulletEffects.size(); i++)
        delete redBulletEffects[i].sprite;

    for (LightningEffect& effect : lightningEffects)
        delete effect.sprite;
}

void EffectManager::Update()
{
    explosionEffect->Update();

    UpdateEffects(greenBulletEffects);
    UpdateEffects(redBulletEffects);

    for (LightningEffect& e : lightningEffects)
    {
        if (!e.active) continue;

        e.lifeTime += DELTA;
        e.sprite->Update();

        if (e.lifeTime > 1.0f)
        {
            e.sprite->Stop();
            e.active = false;
        }
    }
}

void EffectManager::UpdateEffects(vector<FollowEffect>& effects)
{
    for (int i = 0; i < effects.size(); i++)
    {
        FollowEffect& e = effects[i];

        if (!e.sprite->IsActive()) continue;

        if (e.target == nullptr || !e.target->IsActive())
        {
            e.sprite->Stop();
            e.target = nullptr;
            continue;
        }

        e.sprite->Play(e.target->GetGlobalPosition());
        e.sprite->Update();
    }
}

void EffectManager::Render()
{
    explosionEffect->Render();

    for (int i = 0; i < greenBulletEffects.size(); i++)
        greenBulletEffects[i].sprite->Render();

    for (int i = 0; i < redBulletEffects.size(); i++)
        redBulletEffects[i].sprite->Render();

    for (LightningEffect& e : lightningEffects)
        if (e.active) e.sprite->Render();
}

void EffectManager::PlayExplosion(const Vector3& position)
{
    explosionEffect->Play(position);
}

void EffectManager::PlayBulletEffect(Transform* target, BulletEffectType type)
{
    vector<FollowEffect>* pool = nullptr;

    switch (type)
    {
    case BulletEffectType::Green:
        pool = &greenBulletEffects;
        break;
    case BulletEffectType::Red:
        pool = &redBulletEffects;
        break;
    }

    for (int i = 0; i < pool->size(); i++)
    {
        FollowEffect& e = (*pool)[i];

        if (!e.sprite->IsActive())
        {
            e.target = target;
            e.sprite->Play(target->GetGlobalPosition());
            return;
        }
    }

    FollowEffect& first = (*pool)[0];
    first.target = target;
    first.sprite->Play(target->GetGlobalPosition());
}

void EffectManager::PlayLightningEffect(Vector3& start, Vector3& end)
{
    for (LightningEffect& e : lightningEffects)
    {
        if (!e.active)
        {
            e.start = start;
            e.end = end;
            e.lifeTime = 0.0f;
            e.active = true;

            Vector3 dir = end - start;
            Vector3 mid = (start + end) * 0.5f;
            float angle = atan2(dir.y, dir.x);
            float length = dir.Magnitude();

            e.sprite->SetSize(Float2(length, 1.0f));
            e.sprite->Play(mid);
            //e.sprite->SetRotation(angle);

            break;
        }
    }
}
