#include "Summoner.h"

#include <iostream>

#include "Util.h"

Summoner::Summoner() : Enemy("summonerBlue", 5), m_fleeRadius(150), m_attackRadius(500)
{
	SetWidth(32);
	SetHeight(64);

	SetSpeed(0.85f);
	SetHitDelay(20);
	SetWeaponType(WeaponType::MAGIC_MISSILE);

	BuildAnimations();
}

Summoner::~Summoner()
= default;

void Summoner::Draw()
{
	Enemy::Draw();
}

void Summoner::Update()
{
	SetCircling(true);
	Enemy::Update();
	if (GetHealth() > 0)
	{
		FleeQuery();
		CirclePlayer();
		ChooseAttack();

		if (!GetIsAttacking() && !GetIsCasting())
		{
			Move();
		}
	}
	else
		Death();
}

void Summoner::Clean()
{
	Enemy::Clean();
}

void Summoner::FleeQuery()
{
	if (Util::SquaredDistance(GetTransform()->position, GetTargetPosition()) <= m_fleeRadius * m_fleeRadius)
		SetSeek(false);
	else
		SetSeek(true);
}

void Summoner::CirclePlayer()
{
	if (Util::SquaredDistance(GetTransform()->position, GetTargetPosition()) <= m_attackRadius * m_attackRadius && GetSeek())
		SetCircling(true);
	else
		SetCircling(false);
}

void Summoner::ChooseAttack()
{
	static int cast_ctr = 420;
	static int atk_ctr = 240;
	if (Util::SquaredDistance(GetTransform()->position, GetTargetPosition()) <= m_attackRadius * m_attackRadius && GetSeek())
	{
		if (cast_ctr-- <= 0 && !GetIsAttacking())
		{
			SpecialAttack();
			cast_ctr = rand() % 180 + 360; // random delay between 6 - 9 seconds
		}
		if (atk_ctr-- <= 0 && !GetIsCasting())
		{
			Attack();
			//if(!GetIsAttacking())
				atk_ctr = rand() % 120 + 240; // random delay between 4 - 6 seconds
		}
	}
	Cast();
	Shoot();
	Summon();
	EndAttack();
}

void Summoner::SpecialAttack()
{
	SetIsCasting(true);
}

void Summoner::Attack()
{
	// If seeking and not too far:
	{
		if (GetTargetDirection().x > 0.707f) // RIGHT
		{
			SetAnimationState(PlayerAnimationState::PLAYER_SUMMONER_ATTACK_OVERSIZED_RIGHT);
			SetIsAttacking(1, true); // RIGHT
		}

		if (GetTargetDirection().x < -0.707f) // LEFT
		{
			if (!GetIsAttacking())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_SUMMONER_ATTACK_OVERSIZED_LEFT);
				SetIsAttacking(3, true); // LEFT
			}
		}

		if (GetTargetDirection().y > 0.707f) // DOWN
		{
			if (!GetIsAttacking())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_SUMMONER_ATTACK_OVERSIZED_DOWN);
				SetIsAttacking(2, true); // DOWN
			}
		}

		if (GetTargetDirection().y < -0.707f) // UP
		{
			if (!GetIsAttacking())
			{
				SetAnimationState(PlayerAnimationState::PLAYER_SUMMONER_ATTACK_OVERSIZED_UP);
				SetIsAttacking(0, true); // UP
			}
		}
	}
}

void Summoner::EndAttack()
{
	if (GetAnimation("summoner_oversized_down").current_frame == GetAnimation("summoner_oversized_down").frames.size() - 1)
	{
		GetAnimation("summoner_oversized_down").current_frame = 0;
		SetIsAttacking(2, false);
	}
	
	if (GetAnimation("summoner_oversized_up").current_frame == GetAnimation("summoner_oversized_up").frames.size() - 1)
	{
		GetAnimation("summoner_oversized_up").current_frame = 0;
		SetIsAttacking(0, false);
	}
	
	if (GetAnimation("summoner_oversized_right").current_frame == GetAnimation("summoner_oversized_right").frames.size() - 1)
	{
		GetAnimation("summoner_oversized_right").current_frame = 0;
		SetIsAttacking(1, false);
	}
	
	if (GetAnimation("summoner_oversized_left").current_frame == GetAnimation("summoner_oversized_left").frames.size() - 1)
	{
		GetAnimation("summoner_oversized_left").current_frame = 0;
		SetIsAttacking(3, false);
	}
}

void Summoner::Shoot()
{
	SetTargetDirection(GetTargetPosition());
	m_spawnBullet = false;
	if (GetAnimation("summoner_oversized_up").current_frame == 5 && GetAnimation("summoner_oversized_up").current_frame != m_lastFrame[0])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[0] = GetAnimation("summoner_oversized_up").current_frame;

	if (GetAnimation("summoner_oversized_down").current_frame == 5 && GetAnimation("summoner_oversized_down").current_frame != m_lastFrame[1])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[1] = GetAnimation("summoner_oversized_down").current_frame;

	if (GetAnimation("summoner_oversized_left").current_frame == 5 && GetAnimation("summoner_oversized_left").current_frame != m_lastFrame[2])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[2] = GetAnimation("summoner_oversized_left").current_frame;

	if (GetAnimation("summoner_oversized_right").current_frame == 5 && GetAnimation("summoner_oversized_right").current_frame != m_lastFrame[3])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[3] = GetAnimation("summoner_oversized_right").current_frame;
}

void Summoner::Summon()
{
	m_summon = false;
	if (GetAnimation("cast_down").current_frame == 4 && GetAnimation("cast_down").current_frame != m_lastSummonFrame)
	{
		m_summon = true;
	}
	m_lastSummonFrame = GetAnimation("cast_down").current_frame;
}

void Summoner::BuildAnimations()
{
	// Normal Attack
	BuildAnimation("summoner_oversized_up", 1, 8, 1, 4, 2);
	BuildAnimation("summoner_oversized_left", 1, 8, 1, 4, 2);
	BuildAnimation("summoner_oversized_down", 1, 8, 1, 4, 2);
	BuildAnimation("summoner_oversized_right", 1, 8, 1, 4, 2);
}
