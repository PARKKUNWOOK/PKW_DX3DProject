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

	float minAttackDistance = 9.0f;  // �ּ� ���� �Ÿ�
	float maxAttackDistance = 12.0f;  // �ִ� ���� �Ÿ�
	float attackCooldown = 1.5f;     // ���� ��Ÿ�� (��)

	Vector3 direction = player->GetLocalPosition() - GetLocalPosition();
	direction.y = 0.0f;
	float distance = direction.Magnitude();

	if (distance < minAttackDistance || distance > maxAttackDistance)
	{
		isThrowAttacking = false;
		return;
	}

	if (!isThrowAttacking) // ���� ���� �ƴ� ���� ����
	{
		isThrowAttacking = true;
		throwAttackFrameCount = static_cast<int>(attackCooldown / DELTA);

		Audio::Get()->Add("ThrowerAttackVoice", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyAttackVoice.ogg");
		Audio::Get()->Play("ThrowerAttackVoice", 3.0f);
		Audio::Get()->Add("ThrowerAttack", "Resources/Sounds/HellfireDescentSound/ThrowerEnemyAttack.ogg");
		Audio::Get()->Play("ThrowerAttack", 3.0f);
		// �� �ִϸ��̼��� "Throw"�� ����
		model->PlayClip(1);

		//���Ͱ� �ٶ󺸴� ����
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

	float desiredDistance = 9.0f; // ���� �����ϰ� ���� �Ÿ�
	float minDistance = 9.0f;     // �ʹ� ����� ���
	float maxDistance = 12.0f;     // �ʹ� �� ���
	float moveSpeed = speed * DELTA;

	Vector3 direction = target->GetLocalPosition() - GetLocalPosition();
	direction.y = 0.0f;
	float distance = direction.Magnitude();

	//���� �����ϰ� ���� �Ÿ��� ��� ���ڸ����� ����
	if (distance >= minDistance && distance <= maxDistance)
	{
		float targetAngle = atan2(direction.x, direction.z);
		SetLocalRotation(Vector3(0.0f, targetAngle, 0.0f));

		Attack(dynamic_cast<Player*>(target));
		return;
	}

	// �÷��̾�� �ʹ� ������ ���� �����ϰ� ���� �Ÿ���ŭ �ݴ�������� �̵�
	if (distance < minDistance)
	{
		isThrowAttacking = false;
		direction = -direction;
		direction.Normalize();
		float moveAmount = min(moveSpeed, desiredDistance - distance);
		SetLocalPosition(GetLocalPosition() + direction * moveAmount);
	}
	// �÷��̾�� �ʹ� �־����� ���� �����ϰ� ���� �Ÿ���ŭ �÷��̾�� ����
	else if (distance > maxDistance)
	{
		isThrowAttacking = false;
		direction.Normalize();
		float moveAmount = min(moveSpeed, distance - desiredDistance);
		SetLocalPosition(GetLocalPosition() + direction * moveAmount);
	}

	// �÷��̾� ������ �ٶ󺸵��� ȸ��(�̵�)
	float targetAngle = atan2(direction.x, direction.z);
	Vector3 currentRotation = GetLocalRotation();
	float newAngle = GameMath::Lerp(currentRotation.y, targetAngle, 0.1f);
	SetLocalRotation(Vector3(0.0f, newAngle, 0.0f));
}
