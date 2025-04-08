#pragma once

enum class BulletEffectType
{
	Green, Red
};

class EffectManager : public Singleton<EffectManager>
{
private:
	friend class Singleton;

	EffectManager();
	~EffectManager();

public:
	void Update();
	void Render();

	void PlayExplosion(const Vector3& position);
	void PlayBFGExplosion(const Vector3& position);

	void PlayBulletEffect(Transform* target, BulletEffectType type);

	void PlayLightningEffect(Vector3& start, Vector3& end);
	void PlayBFG9000LightningEffect(const Vector3& start, const Vector3& end);

private:
	struct FollowEffect
	{
		Sprite* sprite = nullptr;
		Transform* target = nullptr;
		BulletEffectType type;
	};

	struct LightningEffect
	{
		Sprite* sprite = nullptr;
		Vector3 start;
		Vector3 end;
		float lifeTime = 0.0f;
		bool active = false;
	};

	void UpdateEffects(vector<FollowEffect>& effects);

private:
	Sprite* explosionEffect = nullptr;
	Sprite* bfgExplosionEffect = nullptr;

	vector<FollowEffect> greenBulletEffects;
	vector<FollowEffect> redBulletEffects;
	vector<LightningEffect> lightningEffects;

	vector<LightningEffect> greenLightningHorizontalEffects;
	vector<LightningEffect> greenLightningVerticalEffects;

	const int POOL_SIZE = 100;
	const int LIGHTNING_POOL_SIZE = 50;
};