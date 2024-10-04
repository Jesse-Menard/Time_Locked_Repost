#include "BoomerangEnemy.h"

#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

BoomerangEnemy::BoomerangEnemy():Enemy("boomerang_enemy", 9), m_hitBox(new SDL_FRect)
{
	// Set frame Width/Height
	SetWidth(32);
	SetHeight(64);

	SetSpeed(1.0f);
	SetHitDelay(20);
	SetWeaponType(WeaponType::BOOMERANG);
}

BoomerangEnemy::~BoomerangEnemy()
= default;

void BoomerangEnemy::Draw()
{
	Enemy::Draw();
}

void BoomerangEnemy::Update()
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
	UpdateHitBox();
}

void BoomerangEnemy::Clean()
{
	
}

void BoomerangEnemy::Attack()
{
	if (GetTransform()->position.x - GetTargetPosition().x <= 17 - 9 && GetTransform()->position.x - GetTargetPosition().x >= -17 - 9 &&
		GetTransform()->position.y - GetTargetPosition().y <= 75 && GetTransform()->position.y - GetTargetPosition().y >= 0)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(0, true); // UP
		}
	}


	if (GetTransform()->position.x - GetTargetPosition().x <= 0 && GetTransform()->position.x - GetTargetPosition().x >= -75 &&
		GetTransform()->position.y - GetTargetPosition().y <= 25 - 13 && GetTransform()->position.y - GetTargetPosition().y >= -25 - 13)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(1, true); // RIGHT
		}
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 17 + 6 && GetTransform()->position.x - GetTargetPosition().x >= -17 + 6 &&
		GetTransform()->position.y - GetTargetPosition().y <= 0 && GetTransform()->position.y - GetTargetPosition().y >= -75)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(2, true); // DOWN
		}
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 75 && GetTransform()->position.x - GetTargetPosition().x >= 0 &&
		GetTransform()->position.y - GetTargetPosition().y <= 25 - 13 && GetTransform()->position.y - GetTargetPosition().y >= -25 - 13)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(3, true); // LEFT
		}
	}




	if (GetAnimation("boomerang_oversized_up").current_frame == 1 && 1 != m_lastFrame[0])
	{
		SoundManager::Instance().PlaySound("throw");
	}
	m_lastFrame[0] = GetAnimation("boomerang_oversized_up").current_frame;
	
	if (GetAnimation("boomerang_oversized_down").current_frame == 1 && 1 != m_lastFrame[1])
	{
		SoundManager::Instance().PlaySound("throw");
	}
	m_lastFrame[1] = GetAnimation("boomerang_oversized_down").current_frame;
	
	if (GetAnimation("boomerang_oversized_left").current_frame == 1 && 1 != m_lastFrame[2])
	{
		SoundManager::Instance().PlaySound("throw");
	}
	m_lastFrame[2] = GetAnimation("boomerang_oversized_left").current_frame;
	
	if (GetAnimation("boomerang_oversized_right").current_frame == 1 && 1 != m_lastFrame[3])
	{
		SoundManager::Instance().PlaySound("throw");
	}
	m_lastFrame[3] = GetAnimation("boomerang_oversized_right").current_frame;


	if (GetAnimation("boomerang_oversized_up").current_frame == GetAnimation("boomerang_oversized_up").frames.size() - 1)
	{
		GetAnimation("boomerang_oversized_up").current_frame = 0;
		SetIsAttacking(0, false);
	}
	if (GetAnimation("boomerang_oversized_right").current_frame == GetAnimation("boomerang_oversized_right").frames.size() - 1)
	{
		GetAnimation("boomerang_oversized_right").current_frame = 0;
		SetIsAttacking(1, false);
	}
	if (GetAnimation("boomerang_oversized_down").current_frame == GetAnimation("boomerang_oversized_down").frames.size() - 1)
	{
		GetAnimation("boomerang_oversized_down").current_frame = 0;
		SetIsAttacking(2, false);
	}
	if (GetAnimation("boomerang_oversized_left").current_frame == GetAnimation("boomerang_oversized_left").frames.size() - 1)
	{
		GetAnimation("boomerang_oversized_left").current_frame = 0;
		SetIsAttacking(3, false);
	}
	
	if (GetIsAttacking(0)) // UP
	{
		SetAnimationState(PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_UP);
	}
	if (GetIsAttacking(1)) // RIGHT
	{
		SetAnimationState(PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_RIGHT);
	}
	if(GetIsAttacking(2)) // DOWN
	{
		SetAnimationState(PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_DOWN);
	}
	if(GetIsAttacking(3)) // LEFT
	{
		SetAnimationState(PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_LEFT);
	}
}

glm::vec2 BoomerangEnemy::GetClosestNode()
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

void BoomerangEnemy::SetNodes()
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

void BoomerangEnemy::DrawHitBox()
{
	Util::DrawRect(glm::vec2(m_hitBox->x, m_hitBox->y), static_cast<int>(m_hitBox->w), static_cast<int>(m_hitBox->h), { 1, 0, 0, 1 });
}

SDL_FRect* BoomerangEnemy::GetHitBox() const
{
	return m_hitBox;
}

void BoomerangEnemy::SetHitBox(const float x_offset, const float y_offset, const float w, const float h)
{
	m_hitBox->x = GetTransform()->position.x + x_offset;
	m_hitBox->y = GetTransform()->position.y + y_offset;
	m_hitBox->w = w;
	m_hitBox->h = h;
}

void BoomerangEnemy::UpdateHitBox()
{
	const int upFrame = GetAnimation("boomerang_oversized_up").current_frame;
	const int rightFrame = GetAnimation("boomerang_oversized_right").current_frame;
	const int downFrame = GetAnimation("boomerang_oversized_down").current_frame;
	const int leftFrame = GetAnimation("boomerang_oversized_left").current_frame;

	//	std::cout << "UFrame: " << upFrame << std::endl;
	//	std::cout << "RFrame: " << rightFrame << std::endl;
	//	std::cout << "DFrame: " << downFrame << std::endl;
	//	std::cout << "LFrame: " << leftFrame << std::endl;
	//	std::cout << "state?: " << std::to_string((int)GetAnimationState()) << std::endl << std::endl;


	if (GetAnimationState() == PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_UP)
	{
		// UP
		if (upFrame == 0 || upFrame == 11)
		{
			SetHitBox(17, -14, 19, 20);
		}
		else if (upFrame == 1 || upFrame == 10)
		{
			SetHitBox(11, -17, 16, 21);
		}
		else if (upFrame == 2 || upFrame == 9)
		{
			SetHitBox(11, -17, 16, 21);
		}
		else if (upFrame == 3 || upFrame == 8)
		{
			SetHitBox(-3, -35, 21, 17);
		}
		else if (upFrame == 4 || upFrame == 7)
		{
			SetHitBox(0, -56, 22, 21);
		}
		else if (upFrame == 5 || upFrame == 6)
		{
			SetHitBox(-5, -70, 30, 19);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_RIGHT)
	{
		// RIGHT
		if (rightFrame == 0 || rightFrame == 11)
		{
			SetHitBox(12, -17, 15, 21);
		}
		else if (rightFrame == 1 || rightFrame == 10)
		{
			SetHitBox(16, -15, 17, 21);
		}
		else if (rightFrame == 2 || rightFrame == 9)
		{
			SetHitBox(22, 5, 19, 13);
		}
		else if (rightFrame == 3 || rightFrame == 8)
		{
			SetHitBox(34, -3, 24, 21);
		}
		else if (rightFrame == 4 || rightFrame == 7)
		{
			SetHitBox(46, 4, 26, 17);
		}
		else if (rightFrame == 5 || rightFrame == 6)
		{
			SetHitBox(59, 1, 24, 23);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_DOWN)
	{
		// DOWN
		if (downFrame == 0 || downFrame == 11)
		{
			SetHitBox(15, -13, 16, 21);
		}
		else if (downFrame == 1 || downFrame == 10)
		{
			SetHitBox(15, -6, 16, 20);
		}
		else if (downFrame == 2 || downFrame == 9)
		{
			SetHitBox(-5, 15, 19, 15);
		}
		else if (downFrame == 3 || downFrame == 8)
		{
			SetHitBox(-2, 36, 25, 18);
		}
		else if (downFrame == 4 || downFrame == 7)
		{
			SetHitBox(-4, 54, 31, 15);
		}
		else if (downFrame == 5 || downFrame == 6)
		{
			SetHitBox(-1, 68, 20, 20);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_LEFT)
	{
		// LEFT
		if (leftFrame == 0 || leftFrame == 11)
		{
			SetHitBox(-28, -16, 15, 20);
		}
		else if (leftFrame == 1 || leftFrame == 10)
		{
			SetHitBox(-35, -14, 16, 21);
		}
		else if (leftFrame == 2 || leftFrame == 9)
		{
			SetHitBox(-44, 5, 17, 12);
		}
		else if (leftFrame == 3 || leftFrame == 8)
		{
			SetHitBox(-57, -3, 20, 21);
		}
		else if (leftFrame == 4 || leftFrame == 7)
		{
			SetHitBox(-72, 4, 24, 15);
		}
		else if (leftFrame == 5 || leftFrame == 6)
		{
			SetHitBox(-83, 2, 21, 22);
		}
	}
	else
	{
		//SetHitBox(-32, 0, 64, 40); // Body hit box
		SetHitBox(0, 0, 0, 0); // Body hit box
	}
}