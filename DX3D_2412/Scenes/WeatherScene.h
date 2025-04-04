#pragma once

class WeatherScene : public Scene
{
public:
	WeatherScene();
	~WeatherScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	Collider* collider;
	Sprite* sprite;
	//ParticleSystem* particleSystem;
};