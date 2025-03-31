#include "Framework.h"

ThrowerEnemy::ThrowerEnemy()
{
	tag = "Throw";

	model = new ModelAnimator("ThrowerEnemy");
	model->SetShader(L"Model/Model.hlsl");
	model->ReadClip("Walking");
	model->ReadClip("Throw");
	model->ReadClip("Death");
	model->CreateTexture();

	model->GetClip(2)->SetEvent(bind(&ThrowerEnemy::Death, this), 0.8f);

	model->SetParent(this);
	model->SetLocalPosition(0.0f, -1.0f, 0.0f);
	model->SetLocalScale(0.01f, 0.01f, 0.01f);
	model->SetLocalRotation(0, XM_PI, 0);

	model->PlayClip(0);

	speed = 1.0f;
	//maxHp = curHp = 3;
	maxHp = curHp = 20;
}

ThrowerEnemy::~ThrowerEnemy()
{
}

void ThrowerEnemy::Render()
{
	Enemy::Render();
}

void ThrowerEnemy::Attack(Player* player)
{
	if (!IsActive() || player->IsGameOver()) return;

	float minAttackDistance = 9.0f;  // 최소 공격 거리
	float maxAttackDistance = 12.0f;  // 최대 공격 거리
	float attackCooldown = 1.5f;     // 공격 쿨타임 (초)

	Vector3 direction = player->GetLocalPosition() - GetLocalPosition();
	direction.y = 0.0f;
	float distance = direction.Magnitude();

	if (distance < minAttackDistance || distance > maxAttackDistance)
	{
		isThrowAttacking = false;
		return;
	}

	if (!isThrowAttacking) // 공격 중이 아닐 때만 실행
	{
		isThrowAttacking = true;
		throwAttackFrameCount = static_cast<int>(attackCooldown / DELTA);

		Audio::Get()->Add("ThrowerAttackVoice", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyAttackVoice.ogg");
		Audio::Get()->Play("ThrowerAttackVoice", 3.0f);
		Audio::Get()->Add("ThrowerAttack", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyAttack.ogg");
		Audio::Get()->Play("ThrowerAttack", 3.0f);
		// 모델 애니메이션을 "Throw"로 변경
		model->PlayClip(1);

		//몬스터가 바라보는 방향
		Vector3 fireDirection = Vector3(sin(GetLocalRotation().y), 0.0f, cos(GetLocalRotation().y));
		fireDirection.Normalize();

		Vector3 firePosition = GetLocalPosition() + fireDirection * 1.0f;
		BulletManager::Get()->FireEnemyBullet(firePosition, fireDirection);
	}
}

void ThrowerEnemy::AttackAction()
{
	if (isThrowAttacking)
	{
		throwAttackFrameCount--;

		if (throwAttackFrameCount <= 0)
		{
			isThrowAttacking = false;

			if (!isDying)
			{
				model->PlayClip(0);
			}
		}
	}
	else
	{
		FollowTarget();
	}

	for (Bullet* bullet : BulletManager::Get()->GetBullets())
	{
		if (!bullet->IsActive()) continue;

		if (bullet->IsPlayerBullet() && bullet->EnemyCollisionCheck(this))
		{
			TakeDamage(bullet->GetDamage());
			bullet->SetActive(false);
			if (curHp == 0)
			{
				Audio::Get()->Add("ThrowerDeath", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyDeath.ogg");
				Audio::Get()->Play("ThrowerDeath", 3.0f);
			}
			else
			{
				Audio::Get()->Add("ThrowerDamage", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyDamage.ogg");
				Audio::Get()->Play("ThrowerDamage", 3.0f);
			}
			return;
		}
	}
}

void ThrowerEnemy::FollowTarget()
{
	if (target == nullptr || isDying) return;

	float desiredDistance = 9.0f; // 적이 유지하고 싶은 거리
	float minDistance = 9.0f;     // 너무 가까운 경우
	float maxDistance = 12.0f;     // 너무 먼 경우
	float moveSpeed = speed * DELTA;

	Vector3 direction = target->GetLocalPosition() - GetLocalPosition();
	direction.y = 0.0f;
	float distance = direction.Magnitude();

	//적이 유지하고 싶은 거리일 경우 제자리에서 공격
	if (distance >= minDistance && distance <= maxDistance)
	{
		float targetAngle = atan2(direction.x, direction.z);
		SetLocalRotation(Vector3(0.0f, targetAngle, 0.0f));

		Attack(dynamic_cast<Player*>(target));
		return;
	}

	// 플레이어와 너무 가까우면 적이 유지하고 싶은 거리만큼 반대방향으로 이동
	if (distance < minDistance)
	{
		isThrowAttacking = false;
		direction = -direction;
		direction.Normalize();
		float moveAmount = min(moveSpeed, desiredDistance - distance);
		SetLocalPosition(GetLocalPosition() + direction * moveAmount);
	}
	// 플레이어와 너무 멀어지면 적이 유지하고 싶은 거리만큼 플레이어에게 접근
	else if (distance > maxDistance)
	{
		isThrowAttacking = false;
		direction.Normalize();
		float moveAmount = min(moveSpeed, distance - desiredDistance);
		SetLocalPosition(GetLocalPosition() + direction * moveAmount);
	}

	// 플레이어 방향을 바라보도록 회전(이동)
	float targetAngle = atan2(direction.x, direction.z);
	Vector3 currentRotation = GetLocalRotation();
	float newAngle = GameMath::Lerp(currentRotation.y, targetAngle, 0.1f);
	SetLocalRotation(Vector3(0.0f, newAngle, 0.0f));
}
