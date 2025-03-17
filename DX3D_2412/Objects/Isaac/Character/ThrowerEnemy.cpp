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
	maxHp = curHp = 1;

	bullets = new PoolingManager<Bullet>();
	bullets->Create(10);
}

ThrowerEnemy::~ThrowerEnemy()
{
	delete bullets;
}

void ThrowerEnemy::Render()
{
	Enemy::Render();
	bullets->Render();
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

		// �� �ִϸ��̼��� "Throw"�� ����
		model->PlayClip(1);

		// �Ѿ� �߻�
		Bullet* bullet = bullets->Pop();
		if (!bullet) return;
		
		//���Ͱ� �ٶ󺸴� ����
		Vector3 fireDirection = Vector3(sin(GetLocalRotation().y), 0.0f, cos(GetLocalRotation().y));
		fireDirection.Normalize();

		Vector3 firePosition = GetLocalPosition() + fireDirection * 1.0f;
		bullet->Fire(firePosition, fireDirection, false);
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
			else
			{
				TakeDamage(1);
			}
		}
	}
	else
	{
		FollowTarget();
	}

	for (Bullet* bullet : Player::Get()->GetBullets()->GetAllActive())
	{
		if (bullet->ThrowerEnemyCollisionCheck(this))
		{
			bullet->SetActive(false);
			TakeDamage(1);
			return;
		}
	}

	bullets->Update();
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
