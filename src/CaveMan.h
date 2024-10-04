#pragma once
#ifndef __CAVEMAN__
#define __CAVEMAN__

#include "Enemy.h"

class CaveMan : public Enemy
{
public:
	CaveMan();
	~CaveMan() override;

	void Draw() override;
	void Update() override;
	void Clean() override;

	void Attack() override;
	void Shoot() override;

	[[nodiscard]] glm::vec2 GetBulletDirection() const;
	glm::vec2 GetClosestNode();

	void SetNodes();
private:
	int m_lastFrame[4];
	glm::vec2 m_bulletDirection;
	glm::vec2 m_targetNodes[4];
};

#endif /* defined (__CAVEMAN__)*/