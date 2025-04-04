#pragma once

class HeavyCannon : public Weapon
{
public:
    HeavyCannon(Transform* transform);
    ~HeavyCannon();

    void HandleInput() override;
};