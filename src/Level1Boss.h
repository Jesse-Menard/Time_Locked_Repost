#pragma once
#ifndef __LEVEL_1_BOSS__
#define __LEVEL_1_BOSS__

#include "Enemy.h"
class Level1Boss : public Enemy
{
public:
	Level1Boss();
	~Level1Boss() override;

	void Draw() override;
	void Update() override;
	void Clean() override;

	void DrawHitBox();
	SDL_FRect* GetHitBox() const;
private:
	int m_lastFrame;

	void ChooseAttack();
	void SpecialAttack();
	void Attack() override;
	void Shoot() override;

	SDL_FRect* m_hitBox;
	void SetHitBox(float x_offset, float y_offset, float w, float h);
	void UpdateHitBox();
};

#endif // Defined __LEVEL_1_BOSS__