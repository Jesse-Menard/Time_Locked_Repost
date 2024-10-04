#pragma once
#ifndef __LEVEL_3_BOSS__
#define __LEVEL_3_BOSS__

#include "Enemy.h"
#include "BossAbilities.h"
class Level3Boss : public Enemy
{
public:
	Level3Boss();
	~Level3Boss() override;

	void Draw() override;
	void Update() override;
	void Clean() override;
	void Death2();
	void Revive();

	void DrawHitBox() const;
	[[nodiscard]] SDL_FRect* GetHitBox() const;
	[[nodiscard]] bool GetShielding() const;
	[[nodiscard]] bool GetIsMagnet() const;
	[[nodiscard]] float GetMagnetStrength() const;
	[[nodiscard]] bool GetPhaseTransition() const;
	[[nodiscard]] bool GetPhaseTwo() const;
	[[nodiscard]] bool GetAccelerateMap() const;

	void SetShielding(bool state);
	void SetIsMagnet(bool state);

private:
	int m_lastFrame;
	bool m_isShielding;
	bool m_castingShielding;
	bool m_isMagnet;
	bool m_castingMagnet;
	bool m_isShooting;
	bool m_castingShooting;
	float m_magnetStrength;

	bool m_isPhaseTwo;
	bool m_phaseTransition;
	bool m_accelerateMap;

	BossAbilities m_ability;

	void ChooseAttack();
	void SpecialAttack();
	void Attack() override;
	void Shield();
	void Magnet();
	void Shoot() override;

	SDL_FRect* m_hitBox;
	void SetHitBox(float x_offset, float y_offset, float w, float h);
	void UpdateHitBox();
};

#endif // Defeined __LEVEL_3_BOSS__