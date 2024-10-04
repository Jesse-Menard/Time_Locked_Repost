#include "Level2Boss.h"

#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"

Level2Boss::Level2Boss() :Enemy("Level2Boss", 40, 2.0f), m_hitBox(new SDL_FRect)
{
	// set frame Width/Height
	SetWidth(32 * GetScale());
	SetHeight(64 * GetScale());

	SetSpeed(1.5f);
	SetHitDelay(20);
	SetWeaponType(WeaponType::SPIKE_BALL);

	TextureManager::Instance().Load("../Assets/textures/shield.png", "shield");
}

Level2Boss::~Level2Boss() = default;

void Level2Boss::Draw()
{
	Enemy::Draw();

	static bool add = true;
	static int opacity = 0;

	add ? opacity++ : opacity--;

	if(opacity == 45 || opacity == 0)
		add = !add;

	if (m_isShielding)
		TextureManager::Instance().Draw("shield", GetTransform()->position, 0, 70 + opacity, true);
}

void Level2Boss::Update()
{
	Enemy::Update();
	if (GetHealth() > 0)
	{
		ChooseAttack();

		if (!GetIsAttacking() && !GetIsCasting())
			Move();
	}
	else
		Death();
	UpdateHitBox();
}

void Level2Boss::Clean()
{
	delete m_hitBox;
	m_hitBox = nullptr;
}

void Level2Boss::DrawHitBox()
{
	Util::DrawRect(glm::vec2(m_hitBox->x, m_hitBox->y), static_cast<int>(m_hitBox->w), static_cast<int>(m_hitBox->h), { 1, 0, 0, 1 });
}

SDL_FRect* Level2Boss::GetHitBox() const
{
	return m_hitBox;
}

bool Level2Boss::GetShielding() const
{
	return m_isShielding;
}

void Level2Boss::SetShielding(const bool state)
{
	m_isShielding = state;
}

void Level2Boss::ChooseAttack()
{
	static int ctr = 240; // 4 second initial delay
	if (!m_isShielding)
		ctr--;

	if (ctr <= 0 && !GetIsAttacking())
	{
		SpecialAttack();
		ctr = rand() % 240 + 180; // random delay between 4 - 7 seconds
	}
	else if (!GetIsCasting())
	{
		Attack();
	}
	Cast();
	Shield();
}

void Level2Boss::SpecialAttack()
{
	SetIsCasting(true);
}

void Level2Boss::Attack()
{
	bool last_attack_state = GetIsAttacking();


	if (GetTransform()->position.x - GetTargetPosition().x <= 0 && GetTransform()->position.x - GetTargetPosition().x >= -145 &&
		GetTransform()->position.y - GetTargetPosition().y <= 25 && GetTransform()->position.y - GetTargetPosition().y >= -50)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(1, true); // RIGHT
		}
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 145 && GetTransform()->position.x - GetTargetPosition().x >= 0 &&
		GetTransform()->position.y - GetTargetPosition().y <= 25 && GetTransform()->position.y - GetTargetPosition().y >= -50)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(3, true); // LEFT
		}
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 80 && GetTransform()->position.x - GetTargetPosition().x >= -110 &&
		GetTransform()->position.y - GetTargetPosition().y <= 0 && GetTransform()->position.y - GetTargetPosition().y >= -75 - static_cast<float>(GetHeight()) / 3)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(2, true); // DOWN
		}
	}

	if (GetTransform()->position.x - GetTargetPosition().x <= 80 && GetTransform()->position.x - GetTargetPosition().x >= -110 &&
		GetTransform()->position.y - GetTargetPosition().y <= 80 && GetTransform()->position.y - GetTargetPosition().y >= 0)
	{
		if (!GetIsAttacking())
		{
			SetIsAttacking(0, true); // UP
		}
	}

	if (GetAnimation("attack_oversized_up").current_frame == GetAnimation("attack_oversized_up").frames.size() - 1)
	{
		GetAnimation("attack_oversized_up").current_frame = 0;
		SetIsAttacking(0, false);
	}
	if (GetAnimation("attack_oversized_right").current_frame == GetAnimation("attack_oversized_right").frames.size() - 1)
	{
		GetAnimation("attack_oversized_right").current_frame = 0;
		SetIsAttacking(1, false);
	}
	if (GetAnimation("attack_oversized_down").current_frame == GetAnimation("attack_oversized_down").frames.size() - 1)
	{
		GetAnimation("attack_oversized_down").current_frame = 0;
		SetIsAttacking(2, false);
	}
	if (GetAnimation("attack_oversized_left").current_frame == GetAnimation("attack_oversized_left").frames.size() - 1)
	{
		GetAnimation("attack_oversized_left").current_frame = 0;
		SetIsAttacking(3, false);
	}

	if (GetIsAttacking(0)) // UP
	{
		SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_UP);
	}
	if (GetIsAttacking(1)) // RIGHT
	{
		SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_RIGHT);
	}
	if (GetIsAttacking(2)) // DOWN
	{
		SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_DOWN);
	}
	if (GetIsAttacking(3)) // LEFT
	{
		SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_LEFT);
	}

	// VV UGLY VV

	static int last_upFrame;
	static int last_rightFrame;
	static int last_downFrame;
	static int last_leftFrame;

	if ((GetAnimation("attack_oversized_up").current_frame == 1 || GetAnimation("attack_oversized_up").current_frame == 6) && GetAnimation("attack_oversized_up").current_frame != last_upFrame ||
		(GetAnimation("attack_oversized_right").current_frame == 1 || GetAnimation("attack_oversized_right").current_frame == 6) && GetAnimation("attack_oversized_right").current_frame != last_rightFrame ||
		(GetAnimation("attack_oversized_down").current_frame == 1 || GetAnimation("attack_oversized_down").current_frame == 6) && GetAnimation("attack_oversized_down").current_frame != last_downFrame ||
		(GetAnimation("attack_oversized_left").current_frame == 1 || GetAnimation("attack_oversized_left").current_frame == 6)  && GetAnimation("attack_oversized_left").current_frame != last_leftFrame)
	{
		SoundManager::Instance().PlaySound("sword", 0, -1);
	}

	last_upFrame = GetAnimation("attack_oversized_up").current_frame;
	last_rightFrame = GetAnimation("attack_oversized_right").current_frame;
	last_downFrame = GetAnimation("attack_oversized_down").current_frame;
	last_leftFrame = GetAnimation("attack_oversized_left").current_frame;
}

void Level2Boss::Shield()
{
	if (GetAnimation("cast_down").current_frame == 3 && GetAnimation("cast_down").current_frame != m_lastFrame)
	{
		m_isShielding = true;
	}
	m_lastFrame = GetAnimation("cast_down").current_frame;

	static int ctr = 0;
	if (m_isShielding)
		if (++ctr % 300 == 0) // 5 second duration
			m_isShielding = false;
}

void Level2Boss::SetHitBox(const float x_offset, const float y_offset, const float w, const float h)
{
	m_hitBox->x = GetTransform()->position.x + x_offset;
	m_hitBox->y = GetTransform()->position.y + y_offset;
	m_hitBox->w = w;
	m_hitBox->h = h;
}

void Level2Boss::UpdateHitBox()
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

	if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_UP)
	{
		// UP
		if (upFrame == 0)
		{
			SetHitBox(-67, 19, 60, 11);
		}
		else if (upFrame == 1)
		{
			SetHitBox(-67, 18, 60, 31);
		}
		else if (upFrame == 2)
		{
			SetHitBox(-87, -17, 76, 63);
		}
		else if (upFrame == 3)
		{
			SetHitBox(-87, -73, 162, 78);
		}
		else if (upFrame == 4)
		{
			SetHitBox(-14, -69, 139, 83);
		}
		else if (upFrame == 5)
		{
			SetHitBox(7, 3, 116, 11);
		}
		else if (upFrame == 6)
		{
			SetHitBox(28, -39, 83, 56);
		}
		else if (upFrame == 7)
		{
			SetHitBox(-45, -64, 163, 78);
		}
		else if (upFrame == 8)
		{
			SetHitBox(-107, -5, 100, 28);
		}
		else if (upFrame == 9)
		{
			SetHitBox(-68, 19, 60, 11);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_RIGHT)
	{
		// RIGHT
		 if (rightFrame == 0)
		{
			SetHitBox(-32, 0, 64, 40); // Body hit box
		}
		else if (rightFrame == 1)
		{
			SetHitBox(-44, 6, 73, 34);
		}
		else if (rightFrame == 2)
		{
			SetHitBox(-63, 2, 116, 32);
		}
		else if (rightFrame == 3)
		{
			SetHitBox(20, -23, 142, 52);
		}
		else if (rightFrame == 4)
		{
			SetHitBox(20, -23, 140, 34);
		}
		else if (rightFrame == 5)
		{
			SetHitBox(-32, 0, 64, 40); // Body hit box
		}
		else if (rightFrame == 6)
		{
			SetHitBox(29, -48, 61, 66);
		}
		else if (rightFrame == 7)
		{
			SetHitBox(16, 1, 137, 52);
		}
		else if (rightFrame == 8)
		{
			SetHitBox(-87, 11, 115, 33);
		}
		else if (rightFrame == 9)
		{
			SetHitBox(-32, 0, 64, 40); // Body hit box
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_DOWN)
	{
		// DOWN
		if (downFrame == 0)
		{
			SetHitBox(-67, 24, 77, 11); // Body hit box
		}
		else if (downFrame == 1)
		{
			SetHitBox(-64, -26, 63, 66);
		}
		else if (downFrame == 2)
		{
			SetHitBox(-81, -21, 100, 63);
		}
		else if (downFrame == 3)
		{
			SetHitBox(-86, 16, 163, 82);
		}
		else if (downFrame == 4)
		{
			SetHitBox(-12, 12, 138, 82);
		}
		else if (downFrame == 5)
		{
			SetHitBox(34, -40, 70, 70); // Body hit box
		}
		else if (downFrame == 6)
		{
			SetHitBox(31, 19, 87, 11);
		}
		else if (downFrame == 7)
		{
			SetHitBox(-3, 9, 136, 52);
		}
		else if (downFrame == 8)
		{
			SetHitBox(-52, 24, 157, 45);
		}
		else if (downFrame == 9)
		{
			SetHitBox(-68, 22, 86, 23); // Body hit box
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_LEFT)
	{
		// LEFT
		if (leftFrame == 0)
		{
			SetHitBox(-32, 0, 64, 40); // Body hit box
		}
		else if (leftFrame == 1)
		{
			SetHitBox(-44, 6, 73, 34);
		}
		else if (leftFrame == 2)
		{
			SetHitBox(-46, -1, 107, 32);
		}
		else if (leftFrame == 3)
		{
			SetHitBox(-163, -24, 140, 52);
		}
		else if (leftFrame == 4)
		{
			SetHitBox(-160, -25, 140, 38);
		}
		else if (leftFrame == 5)
		{
			SetHitBox(-32, 0, 64, 40); // Body hit box
		}
		else if (leftFrame == 6)
		{
			SetHitBox(-91, -49, 60, 62);
		}
		else if (leftFrame == 7)
		{
			SetHitBox(-154, 0, 138, 51);
		}
		else if (leftFrame == 8)
		{
			SetHitBox(-15, 10, 102, 28);
		}
		else if (leftFrame == 9)
		{
			SetHitBox(-32, 0, 64, 40); // Body hit box
		}
	}
	else
	{
		SetHitBox(0, 0, 0, 0); // Body hit box
	}
}
