#pragma once
#ifndef __ENEMY__
#define __ENEMY__

#include "PlayerAnimationState.h"
#include "Sprite.h"
#include "Bullet.h"
#include "HPBar.h"

class Enemy : public Sprite
{
public:
	// Constructor
	Enemy(std::string key_name, int health, float scale = 1.0f);

	// Destructor
	~Enemy() override;

	// Lifecycle functions
	void Draw() override;
	void Update() override;
	void Clean() override;

	// Getters
	[[nodiscard]] float GetScale() const;
	[[nodiscard]] float GetSpeed() const;
	[[nodiscard]] int GetHealth() const;
	[[nodiscard]] int GetMaxHealth() const;
	[[nodiscard]] bool GetDeleteMe() const;
	[[nodiscard]] glm::vec2 GetTargetDirection() const;
	[[nodiscard]] glm::vec2 GetTargetPosition() const;
	[[nodiscard]] int GetHitDelayCtr() const;
	[[nodiscard]] int GetHitDelayMax() const;
	[[nodiscard]] PlayerAnimationState GetAnimationState() const;
	[[nodiscard]] bool GetIsAttacking(int index) const;
	[[nodiscard]] bool GetIsAttacking() const;
	[[nodiscard]] bool GetIsAttackPrepped() const;
	[[nodiscard]] bool GetIsCasting() const;
	[[nodiscard]] WeaponType GetWeaponType() const;
	[[nodiscard]] bool GetSeek() const;
	[[nodiscard]] bool GetCircling() const;
	[[nodiscard]] bool GetDeathSoundToggle() const;

	// Setters
	void SetScale(float scale);
	void SetSpeed(float speed);
	void SetHealth(int health);
	void SetHitDelay(int frames);
	void SetDeleteMe(const bool deleteMe);
	void SetTargetDirection(glm::vec2 targetPosition);
	void SetTargetPosition(glm::vec2 targetPosition);
	void SetAnimationState(PlayerAnimationState new_state);
	void SetIsAttacking(bool state);
	void SetIsAttacking(int index, bool state);
	void SetIsAttackPrepped(bool state);
	void SetIsCasting(bool state);
	void SetWeaponType(const WeaponType type);
	void SetSeek(bool state); // If not seeking, flee
	void SetCircling(bool state);
	void SetDeathSoundToggle(const bool state);

	// Public functions
	void Seek();
	void LookWhereYoureGoing();
	void Move();
	void TakeDamage(const int dmgVal);
	virtual void Attack() {}
	virtual void Shoot() {}
	virtual void Summon() {}
	void Cast();
	virtual void Death();

	bool m_spawnBullet;
	bool m_summon;
protected:
	void SetMaxHealth(const int value);
	std::string m_keyName;
private:
	// private variables
	float m_scale;
	float m_speed;
	int m_health;
	int m_maxHealth;
	bool m_deleteMe;
	bool m_toggleDeathSound;
	int m_hitDelay;
	int m_hitDelayCtr;
	PlayerAnimationState m_currentAnimationState;

	// Attacking stuff
	bool m_isAttackPrepped;
	bool m_isAttacking[4];
	bool m_isCasting;
	WeaponType m_weaponType;

	// Where we want to go
	glm::vec2 m_targetNormal;
	glm::vec2 m_targetPosition;

	// Movement variables
	bool m_isSeeking;
	bool m_isCircling;

	// HP bar
	HPBar* m_HPBar{};

	void BuildAnimations();
};

#endif /* defined (__ENEMY__)*/