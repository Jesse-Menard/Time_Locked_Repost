#pragma once
#ifndef __BOOMERANG_ENEMY__
#define __BOOMERANG_ENEMY__

#include "Enemy.h"

class BoomerangEnemy : public Enemy
{
public:
	BoomerangEnemy();
	~BoomerangEnemy() override;

	void Draw() override;
	void Update() override;
	void Clean() override;

	void Attack() override;

	glm::vec2 GetClosestNode();
	void SetNodes();

	void DrawHitBox();
	SDL_FRect* GetHitBox() const;
private:
	int m_lastFrame[4];
	glm::vec2 m_bulletDirection;
	glm::vec2 m_targetNodes[4];

	SDL_FRect* m_hitBox;
	void SetHitBox(float x_offset, float y_offset, float w, float h);
	void UpdateHitBox();
};

#endif /* defined (__BOOMERANG_ENEMY__)*/