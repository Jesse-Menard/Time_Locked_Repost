#include "Bullet.h"

#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"

Bullet::Bullet() : m_duration(-1), m_isPlayerOwner(true)
{
	GetTransform()->position = { 9999.0f, 9999.0f };
}

/*In PlayScene, test-for all bullets not playerOwned, check collisions, Player health -1, have a hit-stun counter for Player (Can't be hit while != 0)*/
Bullet::Bullet(glm::vec2 position, glm::vec2 direction, WeaponType type, bool playerOwned, bool seeking, float seekingStrength) :
	m_isPlayerOwner(playerOwned), m_direction(direction), m_isSeeking(seeking), m_turnRate(seekingStrength), m_particleCtr(0)
{
	m_directionAngle = Util::SignedAngle(glm::vec2(1, 0), m_direction);

	Init(position, type);
}

Bullet::~Bullet() = default;

void Bullet::Init(const glm::vec2 position, const WeaponType type)
{
	m_weaponType = type;
	if (m_weaponType == WeaponType::SPEAR)
	{
		m_renderAngle = Util::SignedAngle(glm::vec2(1.0f, 0.0f), m_direction);
		m_duration = 11;
		m_speed = 4.0f;
		if (m_isPlayerOwner)
		{
			m_duration = 15;
			m_speed = 5.0f;
		}
		m_textureName = "spear";
		if (m_direction == glm::vec2(0, -1)) // UP
			GetTransform()->position = position + glm::vec2(9, -27);
		if (m_direction == glm::vec2(1, 0)) // RIGHT
			GetTransform()->position = position + glm::vec2(27, 13);
		if (m_direction == glm::vec2(0, 1)) // DOWN
			GetTransform()->position = position + glm::vec2(-6, 29);
		if (m_direction == glm::vec2(-1, 0)) // LEFT
			GetTransform()->position = position + glm::vec2(-27, 13);

		SetWidth(44);
		SetHeight(20);
		if (m_direction.x == 0.0f) // If direction is vertical, flip W / H
		{
			SetWidth(20);
			SetHeight(44);
		}

		TextureManager::Instance().Load("../Assets/textures/spearEnergy.png", m_textureName);
		SoundManager::Instance().PlaySound("spear");
	}
	if (m_weaponType == WeaponType::SPIKE_BALL)
	{
		m_renderAngle = Util::SignedAngle(glm::vec2(1.0f, 0.0f), m_direction);
		GetTransform()->position = position;
		m_duration = 800;

		m_isSeeking = true;
		// Randomness // * 10 ) * 0.1 is to increase precision because % likes ints
		m_speed = 2.3f + static_cast<float>(rand() % 5) * 0.1f; // range 0 - 0.5
		m_turnRate = 1.0f + static_cast<float>(rand() % 10) * 0.1f; // range between 2 - 3
		m_textureName = "spike_ball";

		SetWidth(32);
		SetHeight(32);

		TextureManager::Instance().Load("../Assets/textures/spikeBall.png", m_textureName);
		SoundManager::Instance().PlaySound("throw");
	}
	if(m_weaponType == WeaponType::ARROW)
	{
		m_renderAngle = Util::SignedAngle(glm::vec2(1.0f, 0.0f), m_direction);
		GetTransform()->position = position;
		m_duration = 400;
		m_speed = 4.25f;
		m_textureName = "arrow";

		if(m_renderAngle < -45 && m_renderAngle > -135 || m_renderAngle < 135 && m_renderAngle > 45)
		{
			SetWidth(6);
			SetHeight(35);
		}
		else
		{
			SetWidth(35);
			SetHeight(6);
		}

		TextureManager::Instance().Load("../Assets/textures/arrow.png", m_textureName);
		SoundManager::Instance().PlaySound("arrow");
	}
	if(m_weaponType == WeaponType::FINAL_BOSS)
	{
		m_renderAngle = Util::SignedAngle(glm::vec2(1.0f, 0.0f), m_direction);
		GetTransform()->position = position;
		m_duration = 800;
		m_speed = 7.75f;
		m_textureName = "spike_ball";

		SetWidth(32);
		SetHeight(32);

		TextureManager::Instance().Load("../Assets/textures/spikeBall.png", m_textureName);
		SoundManager::Instance().PlaySound("throw");
	}
	if(m_weaponType == WeaponType::LASER)
	{
		m_renderAngle = Util::SignedAngle(glm::vec2(1.0f, 0.0f), m_direction);
		GetTransform()->position = position;
		m_duration = 150;
		m_speed = 4.5f;
		m_textureName = "laser";

		if(m_renderAngle < -45 && m_renderAngle > -135 || m_renderAngle < 135 && m_renderAngle > 45)
		{
			SetWidth(5);
			SetHeight(32);
		}
		else
		{
			SetWidth(32);
			SetHeight(5);
		}

		TextureManager::Instance().Load("../Assets/textures/laser.png", m_textureName);
		SoundManager::Instance().PlaySound("laser");
	}
	if(m_weaponType == WeaponType::MAGIC_MISSILE)
	{
		m_renderAngle = Util::SignedAngle(glm::vec2(1.0f, 0.0f), m_direction);
		GetTransform()->position = position;
		m_duration = 300;
		m_speed = 4.5f;

		m_isSeeking = true;
		m_turnRate = 0.5f;
		m_textureName = "magic_missile";

		SetWidth(20);
		SetHeight(20);

		TextureManager::Instance().Load("../Assets/textures/magicMissile.png", m_textureName);
		SoundManager::Instance().PlaySound("select");
	}

	setIsCentered(true);
}

void Bullet::Draw()
{
	if (GetDuration() > 0)
	{
		if(m_weaponType == WeaponType::SPEAR)
			TextureManager::Instance().Draw(m_textureName, GetTransform()->position, m_renderAngle, 100 + 10 * m_duration, true);
		
		if(m_weaponType == WeaponType::SPIKE_BALL || m_weaponType == WeaponType::MAGIC_MISSILE || m_weaponType == WeaponType::FINAL_BOSS)
			TextureManager::Instance().Draw(m_textureName, GetTransform()->position, m_renderAngle += 7.5f, 255, true);

		if(m_weaponType == WeaponType::ARROW || m_weaponType == WeaponType::LASER)
			TextureManager::Instance().Draw(m_textureName, GetTransform()->position, m_renderAngle, 255, true);
	}
}

void Bullet::Update()
{
	if (GetDuration() > 0)
	{
		if(m_weaponType == WeaponType::FINAL_BOSS) // BOUNCY
		{
			if(GetTransform()->position.x - GetWidth()/2 <= 0 ||
			   GetTransform()->position.x + GetWidth()/2 >= Config::SCREEN_WIDTH)
			{
				m_direction.x = -m_direction.x;
			}
			if(GetTransform()->position.y - GetHeight()/2 <= 0 ||
				GetTransform()->position.y + GetHeight()/2 >= Config::SCREEN_HEIGHT)
			{
				m_direction.y = -m_direction.y;
			}
		}

		Move();
		m_particleCtr++;
		m_duration--;
	}
}

void Bullet::Clean()
{
}

int Bullet::GetDuration() const
{
	return m_duration;
}

bool Bullet::GetIsPlayerOwned() const
{
	return m_isPlayerOwner;
}

WeaponType Bullet::GetWeaponType() const
{
	return m_weaponType;
}

int Bullet::GetParticleCtr() const
{
	return m_particleCtr;
}

bool Bullet::isSeeking() const
{
	return m_isSeeking;
}

glm::vec2 Bullet::GetTargetDirection() const
{
	return m_targetNormal;
}

glm::vec2 Bullet::GetTargetPosition() const
{
	return m_targetPosition;
}

void Bullet::SetTargetDirection(const glm::vec2 targetPosition)
{
	m_targetNormal = Util::Normalize(targetPosition - GetTransform()->position);
}

void Bullet::SetTargetPosition(const glm::vec2 targetPosition)
{
	m_targetPosition = targetPosition;
}

void Bullet::Seek()
{
	SetTargetDirection(GetTargetPosition());

	const float target_rotation = Util::SignedAngle(m_direction, m_targetNormal);

	if (abs(target_rotation) > m_turnRate)
	{
		if (target_rotation > 0.0f)
		{
			m_directionAngle += m_turnRate;
		}
		if (target_rotation < 0.0f)
		{
			m_directionAngle -= m_turnRate;
		}
	}

	const auto x = cos(m_directionAngle * Util::Deg2Rad);
	const auto y = sin(m_directionAngle * Util::Deg2Rad);
	m_direction = glm::vec2(x, y);
}

void Bullet::Move()
{
	if(m_isSeeking)
	{
		Seek();
	}
	GetTransform()->position += m_direction * m_speed;
}
