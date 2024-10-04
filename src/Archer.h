#pragma once
#ifndef __ARCHER__
#define __ARCHER__

#include "Enemy.h"
class Archer : public Enemy
{
public:
	Archer();
	~Archer() override;

	void Draw() override;
	void Update() override;
	void Clean() override;

	void Attack() override;
	void Shoot() override;
	void FleeQuery();

	[[nodiscard]] glm::vec2 GetBulletDirection() const;
private:
	float m_fleeRadius;
	float m_attackRadius;
	int m_lastFrame[4];
	glm::vec2 m_bulletDirection;
};

#endif /* defined (__ARCHER__)*/