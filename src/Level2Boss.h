#pragma once
#ifndef __LEVEL_2_BOSS__
#define __LEVEL_2_BOSS__

#include "Enemy.h"
class Level2Boss : public Enemy
{
public:
	Level2Boss();
	~Level2Boss() override;

	void Draw() override;
	void Update() override;
	void Clean() override;

	void DrawHitBox();
	[[nodiscard]] SDL_FRect* GetHitBox() const;

	[[nodiscard]] bool GetShielding() const;
	void SetShielding(bool state);
private:
	int m_lastFrame;
	bool m_isShielding;

	void ChooseAttack();
	void SpecialAttack();
	void Attack() override;
	void Shield();

	SDL_FRect* m_hitBox;
	void SetHitBox(float x_offset, float y_offset, float w, float h);
	void UpdateHitBox();
};

#endif // Defined __LEVEL_2_BOSS__