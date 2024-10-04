#include "CrossBowEnemy.h"

#include <iostream>

#include "Util.h"

CrossBowEnemy::CrossBowEnemy() :Enemy("crossbow_enemy", 6), m_fleeRadius(100), m_attackRadius(400)
{
	// Set frame Width/Height
	SetWidth(32);
	SetHeight(64);

	SetSpeed(1.25f);
	SetHitDelay(20);

	SetWeaponType(WeaponType::LASER);
	SetAnimationState(PlayerAnimationState::PLAYER_WALK_DOWN);

	SetTargetPosition({ 9999, 9999 });
}

CrossBowEnemy::~CrossBowEnemy()
= default;

void CrossBowEnemy::Draw()
{
	Enemy::Draw();
}

void CrossBowEnemy::Update()
{
	Enemy::Update();
	if (GetHealth() > 0)
	{
		FleeQuery();
		Attack();

		if (!GetIsAttacking())
			Move();
	}
	else
		Death();
}

void CrossBowEnemy::Clean()
{
	Enemy::Clean();
}

void CrossBowEnemy::Attack()
{
	// If seeking and not too far:
	if (GetSeek() && Util::SquaredDistance(GetTransform()->position, GetTargetPosition()) <= m_attackRadius * m_attackRadius)
	{
		if (GetTargetDirection().x > 0.707f) // RIGHT
		{
			if (!GetIsAttacking())
			{
				SetIsAttacking(1, true); // RIGHT
			}
		}
		else if (GetAnimation("attack_right").current_frame == GetAnimation("attack_right").frames.size() - 1)
		{
			GetAnimation("attack_right").current_frame = 0;
			SetIsAttacking(1, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking is false\n";
		}

		if (GetTargetDirection().x < -0.707f) // LEFT
		{
			if (!GetIsAttacking())
			{
				SetIsAttacking(3, true); // LEFT
			}
		}
		else if (GetAnimation("attack_left").current_frame == GetAnimation("attack_left").frames.size() - 1)
		{
			GetAnimation("attack_left").current_frame = 0;
			SetIsAttacking(3, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking is false\n";
		}

		if (GetTargetDirection().y > 0.707f) // DOWN
		{
			if (!GetIsAttacking())
			{
				SetIsAttacking(2, true); // DOWN
			}
		}
		else if (GetAnimation("attack_down").current_frame == GetAnimation("attack_down").frames.size() - 1)
		{
			GetAnimation("attack_down").current_frame = 0;
			SetIsAttacking(2, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking down is false\n";
		}

		if (GetTargetDirection().y < -0.707f) // UP
		{
			if (!GetIsAttacking())
			{
				SetIsAttacking(0, true); // UP
			}
		}
		else if (GetAnimation("attack_up").current_frame == GetAnimation("attack_up").frames.size() - 1)
		{
			GetAnimation("attack_up").current_frame = 0;
			SetIsAttacking(0, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking is false\n";
		}
	}
	else
	{
		if (GetAnimation("attack_down").current_frame == GetAnimation("attack_down").frames.size() - 1)
		{
			GetAnimation("attack_down").current_frame = 0;
			SetIsAttacking(2, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking down is false\n";
		}

		if (GetAnimation("attack_up").current_frame == GetAnimation("attack_up").frames.size() - 1)
		{
			GetAnimation("attack_up").current_frame = 0;
			SetIsAttacking(0, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking is false\n";
		}

		if (GetAnimation("attack_right").current_frame == GetAnimation("attack_right").frames.size() - 1)
		{
			GetAnimation("attack_right").current_frame = 0;
			SetIsAttacking(1, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking is false\n";
		}

		if (GetAnimation("attack_left").current_frame == GetAnimation("attack_left").frames.size() - 1)
		{
			GetAnimation("attack_left").current_frame = 0;
			SetIsAttacking(3, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking is false\n";
		}
	}

	// Prepping
	{
		if (GetIsAttacking(0)) // UP
			{
			if (!GetIsAttackPrepped())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_PREP_ATTACK_UP);
				if (GetAnimation("prep_up").current_frame == GetAnimation("prep_up").frames.size() - 1)
				{
					GetAnimation("prep_up").current_frame = 0;
					SetIsAttackPrepped(true);
				}
			}
			else
				SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_UP);
			}
		if (GetIsAttacking(1)) // RIGHT
			{
			if (!GetIsAttackPrepped())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_PREP_ATTACK_RIGHT);
				if (GetAnimation("prep_right").current_frame == GetAnimation("prep_right").frames.size() - 1)
				{
					GetAnimation("prep_right").current_frame = 0;
					SetIsAttackPrepped(true);
				}
			}
			else
				SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_RIGHT);
			}
		if (GetIsAttacking(2)) // DOWN
			{
			if (!GetIsAttackPrepped())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_PREP_ATTACK_DOWN);
				if (GetAnimation("prep_down").current_frame == GetAnimation("prep_down").frames.size() - 1)
				{
					GetAnimation("prep_down").current_frame = 0;
					SetIsAttackPrepped(true);
				}
			}
			else
			{
				SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_DOWN);
			}
			}
		if (GetIsAttacking(3)) // LEFT
			{
			if (!GetIsAttackPrepped())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_PREP_ATTACK_LEFT);
				if (GetAnimation("prep_left").current_frame == GetAnimation("prep_left").frames.size() - 1)
				{
					GetAnimation("prep_left").current_frame = 0;
					SetIsAttackPrepped(true);
				}
			}
			else
				SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_LEFT);
			}
	}

	Shoot();
}

void CrossBowEnemy::Shoot()
{
	SetTargetDirection(GetTargetPosition());
	m_spawnBullet = false;
	if (GetAnimation("attack_up").current_frame == 11 && GetAnimation("attack_up").current_frame != m_lastFrame[0])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[0] = GetAnimation("attack_up").current_frame;

	if (GetAnimation("attack_down").current_frame == 11 && GetAnimation("attack_down").current_frame != m_lastFrame[1])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[1] = GetAnimation("attack_down").current_frame;

	if (GetAnimation("attack_left").current_frame == 11 && GetAnimation("attack_left").current_frame != m_lastFrame[2])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[2] = GetAnimation("attack_left").current_frame;

	if (GetAnimation("attack_right").current_frame == 11 && GetAnimation("attack_right").current_frame != m_lastFrame[3])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[3] = GetAnimation("attack_right").current_frame;
}

void CrossBowEnemy::FleeQuery()
{
	if (Util::SquaredDistance(GetTransform()->position, GetTargetPosition()) <= m_fleeRadius * m_fleeRadius)
		SetSeek(false);
	else
		SetSeek(true);
}

glm::vec2 CrossBowEnemy::GetBulletDirection() const
{
	return m_bulletDirection;
}
