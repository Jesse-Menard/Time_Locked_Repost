#include "Level1Boss.h"

#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"

Level1Boss::Level1Boss():Enemy("Level1Boss", 40, 2.0f), m_hitBox(new SDL_FRect)
{
	//SetScale(2.0f);
	// set frame Width/Height
	SetWidth(32*GetScale());
	SetHeight(64*GetScale());

	SetSpeed(1.5f);
	SetHitDelay(20);
	SetWeaponType(WeaponType::SPIKE_BALL);
}

Level1Boss::~Level1Boss() = default;

void Level1Boss::Draw()
{
	Enemy::Draw();
}

void Level1Boss::Update()
{
	Enemy::Update();
	if (GetHealth() > 0)
	{
		ChooseAttack();
		//SetNodes();
		//SetTargetPosition(GetClosestNode());

		if (!GetIsAttacking() && !GetIsCasting())
			Move();
	}
	else
		Death();
	UpdateHitBox();
}

void Level1Boss::Clean()
{
	delete m_hitBox;
	m_hitBox = nullptr;
}

void Level1Boss::DrawHitBox()
{
	Util::DrawRect(glm::vec2(m_hitBox->x, m_hitBox->y), static_cast<int>(m_hitBox->w), static_cast<int>(m_hitBox->h), {1, 0, 0, 1});
}

SDL_FRect* Level1Boss::GetHitBox() const
{
	return m_hitBox;
}

void Level1Boss::ChooseAttack()
{
	static int ctr = 1;

	if(ctr-- == 0)
	{
		SpecialAttack();
		ctr = rand() % 360 + 480; // random delay between 8 - 14 seconds
	}
	else
	{
		Attack();
	}
	Cast();
	Shoot();
}

void Level1Boss::SpecialAttack()
{
	SetIsCasting(true);
}

void Level1Boss::Attack()
{
	if (GetTransform()->position.x - GetTargetPosition().x <= 32 && GetTransform()->position.x - GetTargetPosition().x >= -32 &&
		GetTransform()->position.y - GetTargetPosition().y <= 60 && GetTransform()->position.y - GetTargetPosition().y >= 0)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(0, true); // UP
			//m_bulletDirection = glm::vec2(0, -1);
		}
	}
	else if (GetAnimation("attack_oversized_up").current_frame == GetAnimation("attack_oversized_up").frames.size() - 1)
	{
		GetAnimation("attack_oversized_up").current_frame = 0;
		SetIsAttacking(0, false);
		SetIsAttackPrepped(false);
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 0 && GetTransform()->position.x - GetTargetPosition().x >= -95 &&
		GetTransform()->position.y - GetTargetPosition().y <= 30 && GetTransform()->position.y - GetTargetPosition().y >= -70)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(1, true); // RIGHT
			//m_bulletDirection = glm::vec2(1, 0);
		}
	}
	else if (GetAnimation("attack_oversized_right").current_frame == GetAnimation("attack_oversized_right").frames.size() - 1)
	{
		GetAnimation("attack_oversized_right").current_frame = 0;
		SetIsAttacking(1, false);
		SetIsAttackPrepped(false);
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 32 && GetTransform()->position.x - GetTargetPosition().x >= -32 &&
		GetTransform()->position.y - GetTargetPosition().y <= 0 && GetTransform()->position.y - GetTargetPosition().y >= -75 - static_cast<float>(GetHeight()) / 3)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(2, true); // DOWN
			//m_bulletDirection = glm::vec2(0, 1);
		}
	}
	else if (GetAnimation("attack_oversized_down").current_frame == GetAnimation("attack_oversized_down").frames.size() - 1)
	{
		GetAnimation("attack_oversized_down").current_frame = 0;
		SetIsAttacking(2, false);
		SetIsAttackPrepped(false);
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 95 && GetTransform()->position.x - GetTargetPosition().x >= 0 &&
		GetTransform()->position.y - GetTargetPosition().y <= 30 && GetTransform()->position.y - GetTargetPosition().y >= -70)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(3, true); // LEFT
			//m_bulletDirection = glm::vec2(-1, 0);
		}
	}
	else if (GetAnimation("attack_oversized_left").current_frame == GetAnimation("attack_oversized_left").frames.size() - 1)
	{
		GetAnimation("attack_oversized_left").current_frame = 0;
		SetIsAttacking(3, false);
		SetIsAttackPrepped(false);
	}


	if (GetIsAttacking(0)) // UP
	{
		if (!GetIsAttackPrepped())
		{
			SetAnimationState(PlayerAnimationState::PLAYER_PREP_ATTACK_OVERSIZED_UP);
			if (GetAnimation("prep_oversized_up").current_frame == GetAnimation("prep_oversized_up").frames.size() - 1)
			{
				GetAnimation("prep_oversized_up").current_frame = 0;
				SetIsAttackPrepped(true);
			}
		}
		else
			SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_UP);
	}
	if (GetIsAttacking(1)) // RIGHT
	{
		if (!GetIsAttackPrepped())
		{
			SetAnimationState(PlayerAnimationState::PLAYER_PREP_ATTACK_OVERSIZED_RIGHT);
			if (GetAnimation("prep_oversized_right").current_frame == GetAnimation("prep_oversized_right").frames.size() - 1)
			{
				GetAnimation("prep_oversized_right").current_frame = 0;
				SetIsAttackPrepped(true);
			}
		}
		else
			SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_RIGHT);
	}
	if (GetIsAttacking(2)) // DOWN
	{
		if (!GetIsAttackPrepped())
		{
			SetAnimationState(PlayerAnimationState::PLAYER_PREP_ATTACK_OVERSIZED_DOWN);
			if (GetAnimation("prep_oversized_down").current_frame == GetAnimation("prep_oversized_down").frames.size() - 1)
			{
				GetAnimation("prep_oversized_down").current_frame = 0;
				SetIsAttackPrepped(true);
			}
		}
		else
		{
			SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_DOWN);
		}
	}
	if (GetIsAttacking(3)) // LEFT
	{
		if (!GetIsAttackPrepped())
		{
			SetAnimationState(PlayerAnimationState::PLAYER_PREP_ATTACK_OVERSIZED_LEFT);
			if (GetAnimation("prep_oversized_left").current_frame == GetAnimation("prep_oversized_left").frames.size() - 1)
			{
				GetAnimation("prep_oversized_left").current_frame = 0;
				SetIsAttackPrepped(true);
			}
		}
		else
			SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_LEFT);
	}

	static int last_upFrame;
	static int last_rightFrame;
	static int last_downFrame;
	static int last_leftFrame;

	if(GetAnimation("attack_oversized_up").current_frame == 1 && GetAnimation("attack_oversized_up").current_frame != last_upFrame ||
		GetAnimation("attack_oversized_right").current_frame == 1 && GetAnimation("attack_oversized_right").current_frame != last_rightFrame ||
		GetAnimation("attack_oversized_down").current_frame == 1 && GetAnimation("attack_oversized_down").current_frame != last_downFrame ||
		GetAnimation("attack_oversized_left").current_frame == 1 && GetAnimation("attack_oversized_left").current_frame != last_leftFrame)
	{
		SoundManager::Instance().PlaySound("spear", 0, -1);
	}

	last_upFrame = GetAnimation("attack_oversized_up").current_frame;
	last_rightFrame = GetAnimation("attack_oversized_right").current_frame;
	last_downFrame = GetAnimation("attack_oversized_down").current_frame;
	last_leftFrame = GetAnimation("attack_oversized_left").current_frame;
}

void Level1Boss::Shoot()
{
	m_spawnBullet = false;
	if (GetAnimation("cast_down").current_frame == 5 && GetAnimation("cast_down").current_frame != m_lastFrame)
	{
		m_spawnBullet = true;
	}
	m_lastFrame = GetAnimation("cast_down").current_frame;
}

void Level1Boss::SetHitBox(const float x_offset, const float y_offset, const float w, const float h)
{
	m_hitBox->x = GetTransform()->position.x + x_offset;
	m_hitBox->y = GetTransform()->position.y + y_offset;
	m_hitBox->w = w;
	m_hitBox->h = h;
}

void Level1Boss::UpdateHitBox()
{
	const int upFrame = GetAnimation("attack_oversized_up").current_frame;
	const int rightFrame = GetAnimation("attack_oversized_right").current_frame;
	const int downFrame = GetAnimation("attack_oversized_down").current_frame;
	const int leftFrame = GetAnimation("attack_oversized_left").current_frame;

	//	std::cout << "UFrame: " << upFrame << std::endl;
	//	std::cout << "RFrame: " << rightFrame << std::endl;
	//	std::cout << "DFrame: " << downFrame << std::endl;
	//	std::cout << "LFrame: " << leftFrame << std::endl;
	//	std::cout << "state?: " << std::to_string((int)GetAnimationState()) << std::endl << std::endl;


	if(GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_UP)
	{
		// UP
		if (upFrame == 0 || upFrame == 1 || upFrame == 2)
		{
			SetHitBox(-50, 0, 24, 42);
		}
		else if (upFrame == 3)
		{
			SetHitBox(-40, -42, 84, 70);
		}
		else if (upFrame == 4)
		{
			SetHitBox(2, -42, 106, 66);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_RIGHT)
	{
		// RIGHT
		if (rightFrame == 0 || rightFrame == 1 || rightFrame == 2)
		{
			SetHitBox(-9, 22, 98, 40);
		}
		else if (rightFrame == 3)
		{
			SetHitBox(33, -12, 68, 68);
		}
		else if (rightFrame == 4)
		{
			SetHitBox(68, -26, 27, 54);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_DOWN)
	{
		// DOWN
		if (downFrame == 0 || downFrame == 1 || downFrame == 2)
		{
			SetHitBox(-52, 34, 31, 30);
		}
		else if (downFrame == 3)
		{
			SetHitBox(-36, 28, 126, 70);
		}
		else if (downFrame == 4)
		{
			SetHitBox(58, 2, 42, 78);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_LEFT)
	{
		// LEFT
		if (leftFrame == 0 || leftFrame == 1 || leftFrame == 2)
		{
			SetHitBox(-91, 21, 99, 41);
		}
		else if (leftFrame == 3)
		{
			SetHitBox(-103, -13, 68, 69);
		}
		else if (leftFrame == 4)
		{
			SetHitBox(-97, -26, 26, 54);
		}
	}
	else
	{
		SetHitBox(-32, 0, 64, 40); // Body hit box
	}
}
