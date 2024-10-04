#include "Level3Boss.h"

#include "Renderer.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"

Level3Boss::Level3Boss() : Enemy("Mommy1", 40, 2.0f), m_isShielding(false), m_isMagnet(false), m_magnetStrength(0.75f), m_hitBox(new SDL_FRect)
{


	// set frame Width/Height
	SetWidth(32 * GetScale());
	SetHeight(64 * GetScale());

	SetSpeed(1.5f);
	SetHitDelay(20);
	SetWeaponType(WeaponType::FINAL_BOSS);


	TextureManager::Instance().Load("../Assets/textures/shield.png", "shield");
}

Level3Boss::~Level3Boss() = default;

void Level3Boss::Draw()
{
	if (m_isMagnet)
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("magnet"), GetTransform()->position, 0.25f, 0, 255, true);

	Enemy::Draw();

	static bool add = true;
	static int opacity = 0;

	add ? opacity++ : opacity--;

	if(opacity == 45 || opacity == 0)
		add = !add;

	if (m_isShielding)
		TextureManager::Instance().Draw("shield", GetTransform()->position, 0, 70 + opacity, true);
}

void Level3Boss::Update()
{
	Enemy::Update();
	if(m_phaseTransition)
		Revive();
	else
	{
		if (GetHealth() > 0)
		{
			ChooseAttack();

			if (!GetIsAttacking() && !GetIsCasting())
				Move();
		}
		else
			Death2();
	}
	UpdateHitBox();
}

void Level3Boss::Clean()
{
	delete m_hitBox;
	m_hitBox = nullptr;
}

void Level3Boss::Death2()
{
	SetAnimationState(PlayerAnimationState::PLAYER_DEATH);

	SetIsAttackPrepped(false);
	SetIsAttacking(false);
	m_isMagnet = false;

	if (GetAnimation("death").current_frame == 2 && !GetDeathSoundToggle())
	{
		SetDeathSoundToggle(true);
		SoundManager::Instance().PlaySound("enemy_death", 0, -1);
	}
	if (GetAnimation("death").current_frame == GetAnimation("death").frames.size() - 1)
	{
		if (m_isPhaseTwo)
			SetDeleteMe(true);
		else
		{
			SetMaxHealth(50);
			SetDeathSoundToggle(false);
			SetAnimationState(PlayerAnimationState::PLAYER_REVIVE);
			GetAnimation("death").current_frame = 0;
			m_phaseTransition = true;
		}
	}
}

void Level3Boss::Revive()
{
	SetAnimationState(PlayerAnimationState::PLAYER_REVIVE);

	static int ctr = 0;
	static bool wait = false;
	if(GetAnimation("revive").current_frame == 1 && !GetDeathSoundToggle())
	{
		SoundManager::Instance().SetMusicVolume(4);
		SoundManager::Instance().SetSoundVolume(32);

		SetDeathSoundToggle(true);
		SoundManager::Instance().PlaySound("bass");
		m_accelerateMap = true;
	}
	if(GetAnimation("revive").current_frame == 2)
	{
		SetDeathSoundToggle(false);
		wait = true;
	}
	//TextureManager::Instance().Draw()
	if(wait)
	{
		if (++ctr % 18 == 0 && GetHealth() < GetMaxHealth())
		{
			ctr = 0;
			SetHealth(GetHealth() + 1);
		}
	}

	if (GetAnimation("revive").current_frame == GetAnimation("revive").frames.size() - 1)
	{
		SoundManager::Instance().PlayMusic("phase2");
		SoundManager::Instance().SetMusicVolume(16);
		SoundManager::Instance().SetSoundVolume(24);
		SetHealth(GetMaxHealth());
		m_phaseTransition = false;
		m_accelerateMap = false;
		m_isPhaseTwo = true;
		m_keyName = "Mommy2";
		TextureManager::Instance().LoadSpriteSheet(
			"../Assets/sprites/enemy.txt",
			"../Assets/sprites/" + m_keyName + ".png",
			m_keyName + "_spritesheet");


		m_magnetStrength *= 1.75f;
	}
}

void Level3Boss::DrawHitBox() const
{
	Util::DrawRect(glm::vec2(m_hitBox->x, m_hitBox->y), static_cast<int>(m_hitBox->w), static_cast<int>(m_hitBox->h), { 1, 0, 0, 1 });
}

SDL_FRect* Level3Boss::GetHitBox() const
{
	return m_hitBox;
}

bool Level3Boss::GetShielding() const
{
	return m_isShielding;
}

bool Level3Boss::GetPhaseTwo() const
{
	return m_isPhaseTwo;
}

bool Level3Boss::GetAccelerateMap() const
{
	return m_accelerateMap;
}

void Level3Boss::SetShielding(const bool state)
{
	m_isShielding = state;
}

bool Level3Boss::GetIsMagnet() const
{
	return m_isMagnet;
}

void Level3Boss::SetIsMagnet(const bool state)
{
	m_isMagnet = state;
}

float Level3Boss::GetMagnetStrength() const
{
	return m_magnetStrength;
}

bool Level3Boss::GetPhaseTransition() const
{
	return m_phaseTransition;
}

void Level3Boss::ChooseAttack()
{
	if (GetAnimation("cast_down").current_frame == GetAnimation("cast_down").frames.size() - 1)
	{
		m_castingShielding = false;
		m_castingMagnet = false;
		m_castingShooting = false;
	}

	static int magnetCtr = 300; // 5 second initial delay
	static int shieldCtr = 300; // 5 second initial delay
	static int shootCtr = 180; // 4 second initial delay

	if (!m_isMagnet && !m_castingShooting && !m_castingShielding)
		magnetCtr--;
	if(m_isPhaseTwo)
	{
		if (!m_isShielding && !m_castingShooting && !m_castingMagnet)
			shieldCtr--;
		if (!m_castingMagnet && !m_castingShielding)
			shootCtr--;
	}

	if (magnetCtr <= 0 && !GetIsAttacking() && !m_castingShielding && !m_castingShooting)
	{
		m_castingMagnet = true;
		m_ability = BossAbilities::MAGNET;
		SpecialAttack();
		magnetCtr = rand() % 120 + 240; // random delay between 4 - 6 seconds
	}
	else if (shieldCtr <= 0 && !GetIsAttacking() && !m_castingMagnet && !m_castingShooting)
	{
		m_castingShielding = true;
		m_ability = BossAbilities::SHIELD;
		SpecialAttack();
		shieldCtr = rand() % 120 + 360; // random delay between 6 - 8 seconds
	}
	else if (shootCtr <= 0 && !GetIsAttacking() && !m_castingMagnet && !m_castingShielding)
	{
		m_castingShooting = true;
		m_ability = BossAbilities::SHOOT;
		SpecialAttack();
		shootCtr = 270; // every 4.5 seconds
	}
	else if (!GetIsCasting())
	{
		Attack();
	}

	Cast();
	Magnet();
	Shield();
	Shoot();

	m_lastFrame = GetAnimation("cast_down").current_frame;
}

void Level3Boss::SpecialAttack()
{
	SetIsCasting(true);
}

void Level3Boss::Attack()
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

	if (GetAnimation("attack_oversized_up").current_frame == 2 && GetAnimation("attack_oversized_up").current_frame != last_upFrame ||
		GetAnimation("attack_oversized_right").current_frame == 2 && GetAnimation("attack_oversized_right").current_frame != last_rightFrame ||
		GetAnimation("attack_oversized_down").current_frame == 2 && GetAnimation("attack_oversized_down").current_frame != last_downFrame ||
		GetAnimation("attack_oversized_left").current_frame == 2 && GetAnimation("attack_oversized_left").current_frame != last_leftFrame)
	{
		SoundManager::Instance().PlaySound("Gsword", 0, -1);
	}

	last_upFrame = GetAnimation("attack_oversized_up").current_frame;
	last_rightFrame = GetAnimation("attack_oversized_right").current_frame;
	last_downFrame = GetAnimation("attack_oversized_down").current_frame;
	last_leftFrame = GetAnimation("attack_oversized_left").current_frame;
}

void Level3Boss::Shield()
{
	if (GetAnimation("cast_down").current_frame == 3 && GetAnimation("cast_down").current_frame != m_lastFrame && m_ability == BossAbilities::SHIELD)
	{
		m_isShielding = true;
	}

	static int ctr = 0;
	if (m_isShielding)
	{
		if (++ctr % 240 == 0) // 4 second duration
		{
			ctr = 1;
			m_isShielding = false;
		}
	}
}

void Level3Boss::Magnet()
{
	if (GetAnimation("cast_down").current_frame == 3 && GetAnimation("cast_down").current_frame != m_lastFrame && m_ability == BossAbilities::MAGNET)
	{
		m_isMagnet = true;
	}

	static int ctr = 0;
	if (m_isMagnet)
	{
		if (++ctr % 600 == 0) // 10 second duration
		{
			ctr = 1;
			m_isMagnet = false;
		}
	}
}

void Level3Boss::Shoot()
{
	m_spawnBullet = false;
	if (GetAnimation("cast_down").current_frame == 3 && GetAnimation("cast_down").current_frame != m_lastFrame && m_ability == BossAbilities::SHOOT)
	{
		m_spawnBullet = true;
	}
}

void Level3Boss::SetHitBox(const float x_offset, const float y_offset, const float w, const float h)
{
	m_hitBox->x = GetTransform()->position.x + x_offset;
	m_hitBox->y = GetTransform()->position.y + y_offset;
	m_hitBox->w = w;
	m_hitBox->h = h;
}

void Level3Boss::UpdateHitBox()
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
		else if (upFrame == 1 || upFrame == 2)
		{
			SetHitBox(-67, 18, 60, 31);
		}
		else if (upFrame == 3)
		{
			SetHitBox(-87, -17, 76, 63);
		}
		else if (upFrame == 4)
		{
			SetHitBox(-87, -73, 162, 78);
		}
		else if (upFrame == 5)
		{
			SetHitBox(-14, -69, 139, 83);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_RIGHT)
	{
		// RIGHT
		 if (rightFrame == 0)
		{
			SetHitBox(12, 25, 17, 15); // Body hit box
		}
		else if (rightFrame == 1 || rightFrame == 2)
		{
			SetHitBox(-44, 6, 68, 34);
		}
		else if (rightFrame == 3)
		{
			SetHitBox(-63, 2, 98, 28);
		}
		else if (rightFrame == 4)
		{
			SetHitBox(20, -23, 142, 52);
		}
		else if (rightFrame == 5)
		{
			SetHitBox(20, -23, 140, 22);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_DOWN)
	{
		// DOWN
		if (downFrame == 0)
		{
			SetHitBox(-67, 24, 77, 11); // Body hit box
		}
		else if (downFrame == 1 || downFrame == 2)
		{
			SetHitBox(-64, -26, 63, 66);
		}
		else if (downFrame == 3)
		{
			SetHitBox(-81, -21, 100, 63);
		}
		else if (downFrame == 4)
		{
			SetHitBox(-86, 16, 163, 82);
		}
		else if (downFrame == 5)
		{
			SetHitBox(-12, 12, 138, 82);
		}
	}
	else if (GetAnimationState() == PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_LEFT)
	{
		// LEFT
		if (leftFrame == 0)
		{
			SetHitBox(-32, 0, 64, 40); // Body hit box
		}
		else if (leftFrame == 1 || leftFrame == 2)
		{
			SetHitBox(-13, 7, 57, 26);
		}
		else if (leftFrame == 3)
		{
			SetHitBox(-38, 2, 99, 28);
		}
		else if (leftFrame == 4)
		{
			SetHitBox(-163, -24, 140, 52);
		}
		else if (leftFrame == 5)
		{
			SetHitBox(-160, -25, 140, 25);
		}
	}
	else
	{
		SetHitBox(0, 20, 0, 0); // Body hit box
	}
}
