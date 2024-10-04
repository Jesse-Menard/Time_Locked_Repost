#include "Player.h"

#include "SoundManager.h"
#include "TextureManager.h"

Player::Player() : m_currentAnimationState(PlayerAnimationState::PLAYER_WALK_UP), m_hitDelay(30), m_hitDelayCtr(0), m_health(140), m_maxHealth(140)
{
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/player.txt",
		"../Assets/sprites/new_player.png", 
		"player_spritesheet");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("player_spritesheet"));
	
	// set frame width
	SetWidth(32);

	// set frame height
	SetHeight(64);
	
	GetTransform()->position = glm::vec2(400.0f, 300.0f);
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	setIsCentered(true);
	SetType(GameObjectType::PLAYER);
	SetIsGod(false);
	

	m_HpBar = new HPBar(GetTransform()->position, GetHealth(), glm::vec4(0, 1, 0, 1));

	BuildAnimations();
}

Player::~Player()
= default;

void Player::Draw()
{
	//TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("walk_right"), GetTransform()->position, 0.75f, 0, 255, true);
	// draw the player according to animation state
	

	switch(m_currentAnimationState)
	{
	case PlayerAnimationState::PLAYER_WALK_UP:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("walk_up"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_WALK_LEFT:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("walk_left"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_WALK_DOWN:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("walk_down"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_WALK_RIGHT:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("walk_right"), GetTransform()->position, 0.5f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_UP:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("attack_up"), GetTransform()->position, 0.9f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_LEFT:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("attack_left"), GetTransform()->position, 0.9f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_DOWN:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("attack_down"), GetTransform()->position, 0.9f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_ATTACK_RIGHT:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("attack_right"), GetTransform()->position, 0.9f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_DEATH:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("death"), GetTransform()->position, 0.1f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_IDLE_UP:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("idle_up"), GetTransform()->position, 0.025f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_IDLE_LEFT:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("idle_left"), GetTransform()->position, 0.025f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_IDLE_DOWN:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("idle_down"), GetTransform()->position, 0.025f, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_IDLE_RIGHT:
		TextureManager::Instance().PlayAnimation("player_spritesheet", GetAnimation("idle_right"), GetTransform()->position, 0.025f, 0, 255, true);
		break;
	default:
		break;
	}

	m_HpBar->Draw();
}

void Player::Update()
{
	m_HpBar->Update(GetTransform()->position, GetHealth(), GetHealth() > 0 ? true : false);
	m_hitDelayCtr--;
}

void Player::Clean() {}

int Player::GetHitDelay() const
{
	return m_hitDelay;
}

int Player::GetHitDelayCtr() const
{
	return m_hitDelayCtr;
}

PlayerAnimationState Player::GetAnimationState()
{
	return m_currentAnimationState;
}

int Player::GetHealth()
{
	return m_health;
}

int Player::GetMaxHealth() const
{
	return m_maxHealth;
}

bool Player::GetIsAttacking()
{
	return m_isAttacking;
}

bool Player::GetIsGod()
{
	return m_godMode;
}

void Player::SetIsGod(bool god)
{
	m_godMode = god;
}

void Player::SetIsAttacking(bool state)
{
	m_isAttacking = state;
}

void Player::TakeDamage(int damage)
{
	if (!GetIsGod())
	{
		SoundManager::Instance().PlaySound("damage");
		m_health -= damage;
	}	
}

void Player::SetHealth(int health)
{
	m_health = health;
}

void Player::SetAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::ResetHitDelayCtr()
{
	m_hitDelayCtr = m_hitDelay;
}

void Player::BuildAnimations()
{
	auto walk_up = Animation();

	walk_up.name = "walk_up";
	walk_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up1"));
	walk_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up2"));
	walk_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up3"));
	walk_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up4"));
	walk_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up5"));
	walk_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up6"));
	walk_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up7"));
	walk_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up8"));
	walk_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up9"));

	SetAnimation(walk_up);

	auto walk_left = Animation();

	walk_left.name = "walk_left";
	walk_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left1"));
	walk_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left2"));
	walk_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left3"));
	walk_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left4"));
	walk_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left5"));
	walk_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left6"));
	walk_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left7"));
	walk_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left8"));
	walk_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left9"));

	SetAnimation(walk_left);

	auto walk_down = Animation();

	walk_down.name = "walk_down";
	walk_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down1"));
	walk_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down2"));
	walk_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down3"));
	walk_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down4"));
	walk_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down5"));
	walk_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down6"));
	walk_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down7"));
	walk_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down8"));
	walk_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down9"));

	SetAnimation(walk_down);

	auto walk_right = Animation();

	walk_right.name = "walk_right";
	walk_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right1"));
	walk_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right2"));
	walk_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right3"));
	walk_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right4"));
	walk_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right5"));
	walk_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right6"));
	walk_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right7"));
	walk_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right8"));
	walk_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right9"));

	SetAnimation(walk_right);

	auto attack_up = Animation();

	attack_up.name = "attack_up";
	attack_up.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_up1"));
	attack_up.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_up2"));
	attack_up.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_up3"));
	attack_up.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_up4"));
	attack_up.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_up5"));
	attack_up.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_up6"));
	attack_up.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_up7"));
	attack_up.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_up8"));

	SetAnimation(attack_up);

	auto attack_left = Animation();

	attack_left.name = "attack_left";
	attack_left.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_left1"));
	attack_left.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_left2"));
	attack_left.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_left3"));
	attack_left.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_left4"));
	attack_left.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_left5"));
	attack_left.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_left6"));
	attack_left.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_left7"));
	attack_left.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_left8"));

	SetAnimation(attack_left);

	auto attack_down = Animation();

	attack_down.name = "attack_down";
	attack_down.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_down1"));
	attack_down.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_down2"));
	attack_down.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_down3"));
	attack_down.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_down4"));
	attack_down.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_down5"));
	attack_down.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_down6"));
	attack_down.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_down7"));
	attack_down.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_down8"));

	SetAnimation(attack_down);

	auto attack_right = Animation();

	attack_right.name = "attack_right";
	attack_right.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_right1"));
	attack_right.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_right2"));
	attack_right.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_right3"));
	attack_right.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_right4"));
	attack_right.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_right5"));
	attack_right.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_right6"));
	attack_right.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_right7"));
	attack_right.frames.push_back(GetSpriteSheet()->GetFrame("player_attack_right8"));

	SetAnimation(attack_right);

	auto death = Animation();

	death.name = "death";
	death.frames.push_back(GetSpriteSheet()->GetFrame("death1"));
	death.frames.push_back(GetSpriteSheet()->GetFrame("death2"));
	death.frames.push_back(GetSpriteSheet()->GetFrame("death3"));
	death.frames.push_back(GetSpriteSheet()->GetFrame("death4"));
	death.frames.push_back(GetSpriteSheet()->GetFrame("death5"));
	death.frames.push_back(GetSpriteSheet()->GetFrame("death6"));
	death.frames.push_back(GetSpriteSheet()->GetFrame("death6"));

	SetAnimation(death);

	auto idle_up = Animation();

	idle_up.name = "idle_up";
	idle_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up1"));
	idle_up.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_up9"));

	SetAnimation(idle_up);

	auto idle_left = Animation();

	idle_left.name = "idle_left";
	idle_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left1"));
	idle_left.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_left9"));

	SetAnimation(idle_left);

	auto idle_down = Animation();

	idle_down.name = "idle_down";
	idle_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down1"));
	idle_down.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_down9"));

	SetAnimation(idle_down);

	auto idle_right = Animation();

	idle_right.name = "idle_right";
	idle_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right1"));
	idle_right.frames.push_back(GetSpriteSheet()->GetFrame("player_walk_right9"));

	SetAnimation(idle_right);



	//auto run_animation = Animation();
	//
	//run_animation.name = "run";
	//run_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-run-0"));
	//run_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-run-1"));
	//run_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-run-2"));
	//run_animation.frames.push_back(GetSpriteSheet()->GetFrame("megaman-run-3"));
	//
	//SetAnimation(run_animation);
}
