#include "Enemy.h"

#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

Enemy::Enemy(const std::string key_name, int health, const float scale) : m_keyName(key_name), m_scale(scale), m_speed(0.0f), m_health(health), m_maxHealth(health), m_deleteMe(false), m_toggleDeathSound(false), m_isSeeking(true), m_isCircling(false)
{
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/enemy.txt",
		"../Assets/sprites/" + m_keyName + ".png",
		m_keyName + "_spritesheet");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet(m_keyName+"_spritesheet"));

	GetTransform()->position = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	setIsCentered(true);
	SetIsAttacking(false);

	SetAnimationState(PlayerAnimationState::PLAYER_WALK_DOWN);
	SetType(GameObjectType::AGENT);

	SetIsAttacking(false);

	m_HPBar = new HPBar(GetTransform()->position, GetHealth());

	int rand_num = rand();

	if (rand_num % 3 == 0) // top
		GetTransform()->position = glm::vec2(static_cast<float>(rand() % Config::SCREEN_WIDTH), static_cast<float>(rand() % (Config::SCREEN_HEIGHT / 2 - 128)));
	else if (rand_num % 3 == 1) // bottom
		GetTransform()->position = glm::vec2(static_cast<float>(rand() % Config::SCREEN_WIDTH), Config::SCREEN_HEIGHT / 2 + 128.0f + static_cast<float>(rand() % (Config::SCREEN_HEIGHT / 2 - 128)));
	else if (rand_num % 3 == 2) // sides
	{
		if(rand() % 2 == 0) // Left
			GetTransform()->position = glm::vec2(static_cast<float>(rand() % (Config::SCREEN_WIDTH / 2 - 128)), static_cast<float>(rand() % Config::SCREEN_HEIGHT));
		else
			GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2 + 128.0f + static_cast<float>(rand() % (Config::SCREEN_WIDTH / 2 - 128)), static_cast<float>(rand() % Config::SCREEN_HEIGHT));
	}
	else
		std::cout << "BAD RAND ENEMY SPAWN";

	BuildAnimations();
}

Enemy::~Enemy()
= default;

void Enemy::Draw()
{
	switch (GetAnimationState())
	{
	case PlayerAnimationState::PLAYER_WALK_UP:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("walk_up"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_WALK_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("walk_left"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_WALK_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("walk_down"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_WALK_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("walk_right"), GetTransform()->position, 0.5f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_ATTACK_UP:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("attack_up"), GetTransform()->position, 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("attack_left"), GetTransform()->position, 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("attack_down"), GetTransform()->position, 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("attack_right"), GetTransform()->position, 0.75f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_PREP_ATTACK_UP:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_up"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_PREP_ATTACK_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_left"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_PREP_ATTACK_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_down"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_PREP_ATTACK_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_right"), GetTransform()->position, 0.5f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_BOW_ATTACK_UP:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("bow_attack_up"), GetTransform()->position, 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_BOW_ATTACK_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("bow_attack_left"), GetTransform()->position, 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_BOW_ATTACK_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("bow_attack_down"), GetTransform()->position, 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_BOW_ATTACK_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("bow_attack_right"), GetTransform()->position, 0.75f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_PREP_BOW_ATTACK_UP:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_bow_attack_up"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_PREP_BOW_ATTACK_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_bow_attack_left"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_PREP_BOW_ATTACK_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_bow_attack_down"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_PREP_BOW_ATTACK_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_bow_attack_right"), GetTransform()->position, 0.5f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_SUMMONER_ATTACK_OVERSIZED_UP:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("summoner_oversized_up"), GetTransform()->position, 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_SUMMONER_ATTACK_OVERSIZED_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("summoner_oversized_left"), GetTransform()->position, 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_SUMMONER_ATTACK_OVERSIZED_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("summoner_oversized_down"), GetTransform()->position, 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_SUMMONER_ATTACK_OVERSIZED_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("summoner_oversized_right"), GetTransform()->position, 0.75f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_UP:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("boomerang_oversized_up"), GetTransform()->position, 1.2f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("boomerang_oversized_left"), GetTransform()->position, 1.2f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("boomerang_oversized_down"), GetTransform()->position, 1.2f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_BOOMERANG_ATTACK_OVERSIZED_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("boomerang_oversized_right"), GetTransform()->position, 1.2f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_UP:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("attack_oversized_up"), GetTransform()->position, m_keyName == "Mommy1" || m_keyName == "Mommy2" ? 0.4f : m_keyName == "Level1Boss" ? 0.6f : 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("attack_oversized_left"), GetTransform()->position, m_keyName == "Mommy1" || m_keyName == "Mommy2" ? 0.4f : m_keyName == "Level1Boss" ? 0.6f : 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("attack_oversized_down"), GetTransform()->position, m_keyName == "Mommy1" || m_keyName == "Mommy2" ? 0.4f : m_keyName == "Level1Boss" ? 0.6f : 0.75f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_OVERSIZED_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("attack_oversized_right"), GetTransform()->position, m_keyName == "Mommy1" || m_keyName == "Mommy2" ? 0.4f : m_keyName == "Level1Boss" ? 0.6f : 0.75f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_PREP_ATTACK_OVERSIZED_UP:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_oversized_up"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_PREP_ATTACK_OVERSIZED_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("prep_oversized_left"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_PREP_ATTACK_OVERSIZED_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName + "_spritesheet", GetAnimation("prep_oversized_down"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_PREP_ATTACK_OVERSIZED_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("prep_oversized_right"), GetTransform()->position, 0.5f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_CAST_UP:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("cast_up"), GetTransform()->position, 0.35f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_CAST_LEFT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("cast_left"), GetTransform()->position, 0.35f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_CAST_DOWN:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("cast_down"), GetTransform()->position, 0.35f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_CAST_RIGHT:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("cast_right"), GetTransform()->position, 0.35f, 0, 255, true);
		break;

	case PlayerAnimationState::PLAYER_DEATH:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("death"), GetTransform()->position, 0.15f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_REVIVE:
		TextureManager::Instance().PlayAnimation(m_keyName+"_spritesheet", GetAnimation("revive"), GetTransform()->position, 0.02f, 0, 255, true);
		break;
	default:
		break;
	}

	if(m_keyName != "Level1Boss" && m_keyName != "Level2Boss" && m_keyName != "Mommy1" && m_keyName != "Mommy2")
		m_HPBar->Draw();
}

void Enemy::Update()
{
	if(GetTransform()->position.x > 2000.0f  + Config::SCREEN_WIDTH / 2 || GetTransform()->position.x < - 2000.0f + Config::SCREEN_WIDTH / 2 ||
		GetTransform()->position.y > 2000.0f + Config::SCREEN_HEIGHT / 2 || GetTransform()->position.y < -2000.0f + Config::SCREEN_HEIGHT / 2)
	{
		int rand_num = rand();

		if (rand_num % 3 == 0) // top
			GetTransform()->position = glm::vec2(static_cast<float>(rand() % Config::SCREEN_WIDTH), static_cast<float>(rand() % (Config::SCREEN_HEIGHT / 2 - 128)));
		else if (rand_num % 3 == 1) // bottom
			GetTransform()->position = glm::vec2(static_cast<float>(rand() % Config::SCREEN_WIDTH), Config::SCREEN_HEIGHT / 2 + 128.0f + static_cast<float>(rand() % (Config::SCREEN_HEIGHT / 2 - 128)));
		else if (rand_num % 3 == 2) // sides
		{
			if (rand() % 2 == 0) // Left
				GetTransform()->position = glm::vec2(static_cast<float>(rand() % (Config::SCREEN_WIDTH / 2 - 128)), static_cast<float>(rand() % Config::SCREEN_HEIGHT));
			else
				GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2 + 128.0f + static_cast<float>(rand() % (Config::SCREEN_WIDTH / 2 - 128)), static_cast<float>(rand() % Config::SCREEN_HEIGHT));
		}
		else
			std::cout << "BAD RAND ENEMY SPAWN";
	}
	m_HPBar->Update(GetTransform()->position, GetHealth(), GetHealth() > 0 ? true : false);
	m_spawnBullet = false; // To prevent spamming when death or other unique conditions 
	m_summon = false;
	m_hitDelayCtr--;
}

void Enemy::Clean()
{

}

float Enemy::GetScale() const
{
	return m_scale;
}

float Enemy::GetSpeed() const
{
	return m_speed;
}

int Enemy::GetHealth() const
{
	return m_health;
}

int Enemy::GetMaxHealth() const
{
	return m_maxHealth;
}

bool Enemy::GetDeleteMe() const
{
	return m_deleteMe;
}

glm::vec2 Enemy::GetTargetDirection() const
{
	return m_targetNormal;
}

glm::vec2 Enemy::GetTargetPosition() const
{
	return m_targetPosition;
}

int Enemy::GetHitDelayCtr() const
{
	return m_hitDelayCtr;
}

int Enemy::GetHitDelayMax() const
{
	return m_hitDelay;
}

PlayerAnimationState Enemy::GetAnimationState() const
{
	return m_currentAnimationState;
}

bool Enemy::GetIsAttacking(const int index) const
{
	return m_isAttacking[index];
}

bool Enemy::GetIsAttacking() const
{
	if (m_isAttacking[0] || m_isAttacking[1] || m_isAttacking[2] || m_isAttacking[3])
	{
		return true;
	}
	return false;
}

bool Enemy::GetIsAttackPrepped() const
{
	return m_isAttackPrepped;
}

bool Enemy::GetIsCasting() const
{
	return m_isCasting;
}

WeaponType Enemy::GetWeaponType() const
{
	return m_weaponType;
}

bool Enemy::GetSeek() const
{
	return m_isSeeking;
}

bool Enemy::GetCircling() const
{
	return m_isCircling;
}

bool Enemy::GetDeathSoundToggle() const
{
	return m_toggleDeathSound;
}

void Enemy::SetScale(const float scale)
{
	m_scale = scale;
}

void Enemy::SetAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Enemy::SetIsAttacking(const int index, const bool state)
{
	m_isAttacking[index] = state;
}

void Enemy::SetIsAttacking(const bool state)
{
	m_isAttacking[0] = state;
	m_isAttacking[1] = state;
	m_isAttacking[2] = state;
	m_isAttacking[3] = state;
}

void Enemy::SetIsAttackPrepped(const bool state)
{
	m_isAttackPrepped = state;
}

void Enemy::SetIsCasting(const bool state)
{
	m_isCasting = state;
}

void Enemy::SetWeaponType(const WeaponType type)
{
	m_weaponType = type;
}

void Enemy::SetSeek(const bool state) // If not seeking, flee
{
	m_isSeeking = state;
}

void Enemy::SetCircling(const bool state)
{
	m_isCircling = state;
}

void Enemy::SetDeathSoundToggle(const bool state)
{
	m_toggleDeathSound = state;
}

void Enemy::SetTargetDirection(const glm::vec2 targetPosition)
{
	m_targetNormal = Util::Normalize(targetPosition - GetTransform()->position);
}

void Enemy::SetTargetPosition(const glm::vec2 targetPosition)
{
	m_targetPosition = targetPosition;
}

void Enemy::SetSpeed(const float speed)
{
	m_speed = speed;
}

void Enemy::SetHealth(const int health)
{
	m_health = health;
}

void Enemy::SetHitDelay(const int frames)
{
	m_hitDelay = frames;
}

void Enemy::SetDeleteMe(const bool deleteMe)
{
	m_deleteMe = deleteMe;
}

void Enemy::Seek()
{
	SetTargetDirection(GetTargetPosition());
	LookWhereYoureGoing();
}

void Enemy::LookWhereYoureGoing()
{
	if(GetTargetDirection().y > 0.707f)
	{
		SetAnimationState(PlayerAnimationState::PLAYER_WALK_DOWN);
	}
	else if (GetTargetDirection().y < -0.707f)
	{
		SetAnimationState(PlayerAnimationState::PLAYER_WALK_UP);
	}
	else if (GetTargetDirection().x > 0.0f)
	{
		SetAnimationState(PlayerAnimationState::PLAYER_WALK_RIGHT);
	}
	else if (GetTargetDirection().x < 0.0f)
	{
		SetAnimationState(PlayerAnimationState::PLAYER_WALK_LEFT);
	}
	else
	{
		std::cout << "NO ANIMATION DIRECTION DETECTED\n";
	}
}

void Enemy::Move()
{
	Seek();

	if(!m_isCircling)
	{
		if (m_isSeeking)
			GetTransform()->position += (GetTargetDirection() * GetSpeed());
		else
			GetTransform()->position -= (GetTargetDirection() * GetSpeed());
	}
	else
	{
		glm::vec2 new_direction;
		new_direction.x = -GetTargetDirection().y;
		new_direction.y = GetTargetDirection().x;
		//	std::cout << "Old: (" << GetTargetDirection().x << ", " << GetTargetDirection().y << ") New: (" << new_direction.x << ", " << new_direction.y << ")" << std::endl;
		GetTransform()->position += new_direction * GetSpeed();
	}
}

void Enemy::TakeDamage(const int dmgVal)
{
	if (m_hitDelayCtr <= 0)
	{
		SoundManager::Instance().PlaySound("damage");
		SetHealth(GetHealth() - dmgVal);
		m_hitDelayCtr = m_hitDelay;
	}
}

void Enemy::Cast()
{
	//	std::cout << "Cast status: " << m_isCasting << std::endl;
	if (m_isCasting)
	{
		SetAnimationState(PlayerAnimationState::PLAYER_CAST_DOWN);
		if (GetAnimation("cast_down").current_frame == GetAnimation("cast_down").frames.size() - 1)
		{
			GetAnimation("cast_down").current_frame = 0;
			m_isCasting = false;
		}
	}
}

void Enemy::Death()
{
	SetAnimationState(PlayerAnimationState::PLAYER_DEATH);

	if (GetAnimation("death").current_frame == 1 && !m_toggleDeathSound)
	{
		m_toggleDeathSound = true;
		SoundManager::Instance().PlaySound("enemy_death", 0, -1);
	}
	if (GetAnimation("death").current_frame == GetAnimation("death").frames.size() - 1)
	{
		SetDeleteMe(true);
	}
}

void Enemy::SetMaxHealth(const int value)
{
	m_maxHealth = value;
}

void Enemy::BuildAnimations()
{
	// Animation overloads
	if(m_keyName == "Mommy1" || m_keyName == "Mommy2")
	{
		BuildAnimation("death", 1, 6, static_cast<int>(m_scale), 6, 4);

		// Oversized Attacking (sprite sheet)
		BuildAnimation("attack_oversized_up", 2, 6, static_cast<int>(m_scale), 3, 1, true);
		BuildAnimation("attack_oversized_left", 2, 6, static_cast<int>(m_scale), 3, 1, true);
		BuildAnimation("attack_oversized_down", 2, 6, static_cast<int>(m_scale),3, 1, true);
		BuildAnimation("attack_oversized_right", 2, 6, static_cast<int>(m_scale),3, 1, true);
	}
	if(m_keyName == "Level1Boss")
	{
		// Oversized Attacking (sprite sheet)
		BuildAnimation("attack_oversized_up", 2, 6, static_cast<int>(m_scale), 3, 1, true);
		BuildAnimation("attack_oversized_left", 2, 6, static_cast<int>(m_scale), 3, 1, true);
		BuildAnimation("attack_oversized_down", 2, 6, static_cast<int>(m_scale), 3, 1, true);
		BuildAnimation("attack_oversized_right", 2, 6, static_cast<int>(m_scale), 3, 1, true);
	}
	if (m_keyName == "Level2Boss") 
	{
		BuildAnimation("death", 1, 10, static_cast<int>(m_scale));

		// Oversized Attacking (sprite sheet)
		BuildAnimation("attack_oversized_up", 2, 11, static_cast<int>(m_scale));
		BuildAnimation("attack_oversized_left", 2, 11, static_cast<int>(m_scale));
		BuildAnimation("attack_oversized_down", 2, 11, static_cast<int>(m_scale));
		BuildAnimation("attack_oversized_right", 2, 11, static_cast<int>(m_scale));
	}
	if(m_keyName == "crossbow_enemy")
	{
		// Normal Attack
		BuildAnimation("attack_up", 4, 8, static_cast<int>(m_scale), 6, 8);
		BuildAnimation("attack_left", 4, 8, static_cast<int>(m_scale), 6, 8);
		BuildAnimation("attack_down", 4, 8, static_cast<int>(m_scale), 6, 8);
		BuildAnimation("attack_right", 4, 8, static_cast<int>(m_scale), 6, 8);
	}

	// Casting
	BuildAnimation("cast_up", 1, 7, static_cast<int>(m_scale));
	BuildAnimation("cast_left", 1, 7, static_cast<int>(m_scale));
	BuildAnimation("cast_down", 1, 7, static_cast<int>(m_scale));
	BuildAnimation("cast_right", 1, 7, static_cast<int>(m_scale));

	// Walking
	{
		BuildAnimation("walk_up", 1, 9, static_cast<int>(m_scale));
		BuildAnimation("walk_left", 1, 9, static_cast<int>(m_scale));
		BuildAnimation("walk_down", 1, 9, static_cast<int>(m_scale));
		BuildAnimation("walk_right", 1, 9, static_cast<int>(m_scale));
	}

	// Attacking
	{
		// Normal Attack
		BuildAnimation("attack_up", 4, 8, static_cast<int>(m_scale), 4, 2);
		BuildAnimation("attack_left", 4, 8, static_cast<int>(m_scale), 4, 2);
		BuildAnimation("attack_down", 4, 8, static_cast<int>(m_scale), 4, 2);
		BuildAnimation("attack_right", 4, 8, static_cast<int>(m_scale), 4, 2);

		// Normal Attack prep
		BuildAnimation("prep_up", 1, 3, static_cast<int>(m_scale));
		BuildAnimation("prep_left", 1, 3, static_cast<int>(m_scale));
		BuildAnimation("prep_down", 1, 3, static_cast<int>(m_scale));
		BuildAnimation("prep_right", 1, 3, static_cast<int>(m_scale));

		// Bow Attack
		BuildAnimation("bow_attack_up", 3, 13, static_cast<int>(m_scale), 9, 2);
		BuildAnimation("bow_attack_left", 3, 13, static_cast<int>(m_scale), 9, 2);
		BuildAnimation("bow_attack_down", 3, 13, static_cast<int>(m_scale), 9, 2);
		BuildAnimation("bow_attack_right", 3, 13, static_cast<int>(m_scale), 9, 2);

		// Bow Attack prep
		BuildAnimation("prep_bow_attack_up", 1, 3, static_cast<int>(m_scale));
		BuildAnimation("prep_bow_attack_left", 1, 3, static_cast<int>(m_scale));
		BuildAnimation("prep_bow_attack_down", 1, 3, static_cast<int>(m_scale));
		BuildAnimation("prep_bow_attack_right", 1, 3, static_cast<int>(m_scale));

		// Oversized Attacking (sprite sheet)
		BuildAnimation("attack_oversized_up", 4, 6, static_cast<int>(m_scale), 4, 2);
		BuildAnimation("attack_oversized_left", 4, 6, static_cast<int>(m_scale), 4, 2);
		BuildAnimation("attack_oversized_down", 4, 6, static_cast<int>(m_scale), 4, 2);
		BuildAnimation("attack_oversized_right", 4, 6, static_cast<int>(m_scale), 4, 2);
		
		// Oversized Summoner Attacking (sprite sheet)
		BuildAnimation("summoner_oversized_up", 1, 8, static_cast<int>(m_scale));
		BuildAnimation("summoner_oversized_left", 1,8, static_cast<int>(m_scale));
		BuildAnimation("summoner_oversized_down", 1, 8, static_cast<int>(m_scale));
		BuildAnimation("summoner_oversized_right", 1, 8, static_cast<int>(m_scale));

		// Oversized Boomerang Attacking (sprite sheet)
		BuildAnimation("boomerang_oversized_up",  1, 12, static_cast<int>(m_scale));
		BuildAnimation("boomerang_oversized_left", 1,12, static_cast<int>(m_scale));
		BuildAnimation("boomerang_oversized_down", 1, 12, static_cast<int>(m_scale));
		BuildAnimation("boomerang_oversized_right", 1, 12, static_cast<int>(m_scale));

		// Oversized Attack prep (sprite sheet)
		BuildAnimation("prep_oversized_up", 1, 3, static_cast<int>(m_scale));
		BuildAnimation("prep_oversized_left", 1, 3, static_cast<int>(m_scale));
		BuildAnimation("prep_oversized_down", 1, 3, static_cast<int>(m_scale));
		BuildAnimation("prep_oversized_right", 1, 3, static_cast<int>(m_scale));
	}

	// Death
	BuildAnimation("death", 1, 6, static_cast<int>(m_scale));

	// Revive ????
	BuildAnimation("revive", 1, 8, static_cast<int>(m_scale));

	BuildAnimation("magnet", 1, 6, static_cast<int>(m_scale));
}
