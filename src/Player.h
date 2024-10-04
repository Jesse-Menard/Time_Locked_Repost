#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "HPBar.h"
#include "PlayerAnimationState.h"
#include "Sprite.h"

class Player final : public Sprite
{
public:
	Player();
	~Player() override;

	// Life Cycle Methods
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	
	// Getters
	int GetHitDelay() const;
	int GetHitDelayCtr() const;
	PlayerAnimationState GetAnimationState();
	int GetHealth();
	int GetMaxHealth()const;
	bool GetIsAttacking();
	bool GetIsGod();


	// setters
	void SetAnimationState(PlayerAnimationState new_state);
	void TakeDamage(int damage);
	void SetHealth(int health);
	void SetIsAttacking(bool state);
	void SetIsGod(bool god);


	void ResetHitDelayCtr();
private:
	void BuildAnimations();
	int m_hitDelay;
	int m_hitDelayCtr;
	const int m_maxHealth;
	int m_health;
	bool m_isAttacking;
	bool m_godMode;

	PlayerAnimationState m_currentAnimationState;

	HPBar* m_HpBar;
};

#endif /* defined (__PLAYER__) */