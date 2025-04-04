#pragma once

class ChainGun : public Weapon
{
public:
    ChainGun(Transform* transform);
    ~ChainGun();

    void HandleInput() override;

private:
    float fireInterval = 0.0f;
    float fireTimer = 0.0f;

    float minigunStartInterval = 2.0f;
    float minigunMinInterval = 0.05f;
    float minigunDecreaseRate = 7.0f;
    float minigunElapsed = 0.0f;
};