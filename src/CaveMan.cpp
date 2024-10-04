#include "CaveMan.h"

#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

CaveMan::CaveMan():Enemy("caveman", 9)
{
	// Set frame Width/Height
	SetWidth(32);
	SetHeight(64);

	SetSpeed(1.0f);
	SetHitDelay(20);
	SetWeaponType(WeaponType::SPEAR);
}

CaveMan::~CaveMan()
= default;

void CaveMan::Draw()
{
	Enemy::Draw();
}

void CaveMan::Update()
{
	Enemy::Update();
	if (GetHealth() > 0)
	{
		Attack();
		SetNodes();
		SetTargetPosition(GetClosestNode());

		if (!GetIsAttacking())
			Move();
	}
	else
  		Death();
}

void CaveMan::Clean()
{
	
}

void CaveMan::Attack()
{
	if (GetTransform()->position.x - GetTargetPosition().x <= 17 - 9 && GetTransform()->position.x - GetTargetPosition().x >= -17 - 9 &&
		GetTransform()->position.y - GetTargetPosition().y <= 100 && GetTransform()->position.y - GetTargetPosition().y >= 0)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(0, true); // UP
			m_bulletDirection = glm::vec2(0, -1);
		}
	}
	else if (GetAnimation("attack_up").current_frame == GetAnimation("attack_up").frames.size() - 1)
	{
		GetAnimation("attack_up").current_frame = 0;
		SetIsAttacking(0, false);
		SetIsAttackPrepped(false);
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 0 && GetTransform()->position.x - GetTargetPosition().x >= -100 &&
		GetTransform()->position.y - GetTargetPosition().y <= 25 - 13 && GetTransform()->position.y - GetTargetPosition().y >= -25 - 13)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(1, true); // RIGHT
			m_bulletDirection = glm::vec2(1, 0);
		}
	}
	else if (GetAnimation("attack_right").current_frame == GetAnimation("attack_right").frames.size() - 1)
	{
		GetAnimation("attack_right").current_frame = 0;
		SetIsAttacking( 1, false);
		SetIsAttackPrepped(false);
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 17 + 6 && GetTransform()->position.x - GetTargetPosition().x >= -17 + 6 &&
		GetTransform()->position.y - GetTargetPosition().y <= 0 && GetTransform()->position.y - GetTargetPosition().y >= -100)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(2, true); // DOWN
			m_bulletDirection = glm::vec2(0, 1);
		}
	}
	else if (GetAnimation("attack_down").current_frame == GetAnimation("attack_down").frames.size() - 1)
	{
		GetAnimation("attack_down").current_frame = 0;
		SetIsAttacking(2, false);
		SetIsAttackPrepped(false);
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 100 && GetTransform()->position.x - GetTargetPosition().x >= 0 &&
		GetTransform()->position.y - GetTargetPosition().y <= 25 - 13 && GetTransform()->position.y - GetTargetPosition().y >= -25 - 13)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(3, true); // LEFT
			m_bulletDirection = glm::vec2(-1, 0);
		}
	}
	else if (GetAnimation("attack_left").current_frame == GetAnimation("attack_left").frames.size() - 1)
	{
		GetAnimation("attack_left").current_frame = 0;
		SetIsAttacking(3, false);
		SetIsAttackPrepped(false);
	}


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
	if(GetIsAttacking(2)) // DOWN
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
	if(GetIsAttacking(3)) // LEFT
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

	Shoot();
}

void CaveMan::Shoot()
{
	m_spawnBullet = false;
	if (GetAnimation("attack_up").current_frame == 3 && GetAnimation("attack_up").current_frame != m_lastFrame[0])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[0] = GetAnimation("attack_up").current_frame;

	if (GetAnimation("attack_down").current_frame == 3 && GetAnimation("attack_down").current_frame != m_lastFrame[1])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[1] = GetAnimation("attack_down").current_frame;

	if (GetAnimation("attack_left").current_frame == 3 && GetAnimation("attack_left").current_frame != m_lastFrame[2])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[2] = GetAnimation("attack_left").current_frame;

	if (GetAnimation("attack_right").current_frame == 3 && GetAnimation("attack_right").current_frame != m_lastFrame[3])
	{
		m_spawnBullet = true;
	}
	m_lastFrame[3] = GetAnimation("attack_right").current_frame;
}

glm::vec2 CaveMan::GetBulletDirection() const
{
	return m_bulletDirection;
}

glm::vec2 CaveMan::GetClosestNode()
{
	glm::vec2 closest = m_targetNodes[0];
	for (int i = 1; i < 4; i++)
	{
		if (Util::SquaredDistance(GetTransform()->position, closest) > Util::SquaredDistance(GetTransform()->position, m_targetNodes[i]))
		{
			closest = m_targetNodes[i];
		}
	}

	return closest;
}

void CaveMan::SetNodes()
{
	// Up
	m_targetNodes[0] = GetTargetPosition() + glm::vec2(0, -64);
	// Right
	m_targetNodes[1] = GetTargetPosition() + glm::vec2(64, 0);
	// Down
	m_targetNodes[2] = GetTargetPosition() + glm::vec2(0, 64);
	// Left
	m_targetNodes[3] = GetTargetPosition() + glm::vec2(-64, 0);
}