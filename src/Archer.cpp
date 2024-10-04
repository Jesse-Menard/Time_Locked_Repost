#include "Archer.h"

#include <iostream>

#include "Util.h"

Archer::Archer() :Enemy("archer+quiver", 6), m_fleeRadius(100), m_attackRadius(400)
{
	// Set frame Width/Height
	SetWidth(32);
	SetHeight(64);

	SetSpeed(1.25f);
	SetHitDelay(20);
	SetWeaponType(WeaponType::ARROW);
}

Archer::~Archer()
= default;

void Archer::Draw()
{
	Enemy::Draw();
}

void Archer::Update()
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

void Archer::Clean()
{
	Enemy::Clean();
}

void Archer::Attack()
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
		else if (GetAnimation("bow_attack_right").current_frame == GetAnimation("bow_attack_right").frames.size() - 1)
		{
			GetAnimation("bow_attack_right").current_frame = 0;
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
		else if (GetAnimation("bow_attack_left").current_frame == GetAnimation("bow_attack_left").frames.size() - 1)
		{
			GetAnimation("bow_attack_left").current_frame = 0;
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
		else if (GetAnimation("bow_attack_down").current_frame == GetAnimation("bow_attack_down").frames.size() - 1)
		{
			GetAnimation("bow_attack_down").current_frame = 0;
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
		else if (GetAnimation("bow_attack_up").current_frame == GetAnimation("bow_attack_up").frames.size() - 1)
		{
			GetAnimation("bow_attack_up").current_frame = 0;
			SetIsAttacking(0, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking is false\n";
		}
	}
	else
	{
		if (GetAnimation("bow_attack_down").current_frame == GetAnimation("bow_attack_down").frames.size() - 1)
		{
			GetAnimation("bow_attack_down").current_frame = 0;
			SetIsAttacking(2, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking down is false\n";
		}

		if (GetAnimation("bow_attack_up").current_frame == GetAnimation("bow_attack_up").frames.size() - 1)
		{
			GetAnimation("bow_attack_up").current_frame = 0;
			SetIsAttacking(0, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking is false\n";
		}

		if (GetAnimation("bow_attack_right").current_frame == GetAnimation("bow_attack_right").frames.size() - 1)
		{
			GetAnimation("bow_attack_right").current_frame = 0;
			SetIsAttacking(1, false);
			SetIsAttackPrepped(false);
			// std::cout << "Attacking is false\n";
		}

		if (GetAnimation("bow_attack_left").current_frame == GetAnimation("bow_attack_left").frames.size() - 1)
		{
			GetAnimation("bow_attack_left").current_frame = 0;
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
				SetAnimationState(PlayerAnimationState::PLAYER_PREP_BOW_ATTACK_UP);
				if (GetAnimation("prep_bow_attack_up").current_frame == GetAnimation("prep_bow_attack_up").frames.size() - 1)
				{
					GetAnimation("prep_bow_attack_up").current_frame = 0;
					SetIsAttackPrepped(true);
				}
			}
			else
				SetAnimationState(PlayerAnimationState::PLAYER_BOW_ATTACK_UP);
			}
		if (GetIsAttacking(1)) // RIGHT
			{
			if (!GetIsAttackPrepped())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_PREP_BOW_ATTACK_RIGHT);
				if (GetAnimation("prep_bow_attack_right").current_frame == GetAnimation("prep_bow_attack_right").frames.size() - 1)
				{
					GetAnimation("prep_bow_attack_right").current_frame = 0;
					SetIsAttackPrepped(true);
				}
			}
			else
				SetAnimationState(PlayerAnimationState::PLAYER_BOW_ATTACK_RIGHT);
			}
		if (GetIsAttacking(2)) // DOWN
			{
			if (!GetIsAttackPrepped())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_PREP_BOW_ATTACK_DOWN);
				if (GetAnimation("prep_bow_attack_down").current_frame == GetAnimation("prep_bow_attack_down").frames.size() - 1)
				{
					GetAnimation("prep_bow_attack_down").current_frame = 0;
					SetIsAttackPrepped(true);
				}
			}
			else
			{
				SetAnimationState(PlayerAnimationState::PLAYER_BOW_ATTACK_DOWN);
			}
			}
		if (GetIsAttacking(3)) // LEFT
			{
			if (!GetIsAttackPrepped())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_PREP_BOW_ATTACK_LEFT);
				if (GetAnimation("prep_bow_attack_left").current_frame == GetAnimation("prep_bow_attack_left").frames.size() - 1)
				{
					GetAnimation("prep_bow_attack_left").current_frame = 0;
					SetIsAttackPrepped(true);
				}
			}
			else
				SetAnimationState(PlayerAnimationState::PLAYER_BOW_ATTACK_LEFT);
			}
	}

	Shoot();
}

void Archer::Shoot()
{
	SetTargetDirection(GetTargetPosition());
	m_spawnBullet = false;
	if (GetAnimation("bow_attack_up").current_frame == 9 && GetAnimation("bow_attack_up").current_frame != m_lastFrame[0])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[0] = GetAnimation("bow_attack_up").current_frame;

	if (GetAnimation("bow_attack_down").current_frame == 9 && GetAnimation("bow_attack_down").current_frame != m_lastFrame[1])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[1] = GetAnimation("bow_attack_down").current_frame;

	if (GetAnimation("bow_attack_left").current_frame == 9 && GetAnimation("bow_attack_left").current_frame != m_lastFrame[2])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[2] = GetAnimation("bow_attack_left").current_frame;

	if (GetAnimation("bow_attack_right").current_frame == 9 && GetAnimation("bow_attack_right").current_frame != m_lastFrame[3])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[3] = GetAnimation("bow_attack_right").current_frame;
}

void Archer::FleeQuery()
{
	if (Util::SquaredDistance(GetTransform()->position, GetTargetPosition()) <= m_fleeRadius * m_fleeRadius)
		SetSeek(false);
	else
		SetSeek(true);
}

glm::vec2 Archer::GetBulletDirection() const
{
	return m_bulletDirection;
}
