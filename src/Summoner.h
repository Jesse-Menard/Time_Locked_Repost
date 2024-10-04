#pragma once
#ifndef __SUMMONER__
#define __SUMMONER__

#include "Enemy.h"

class Summoner : public Enemy
{
public:
	Summoner();
	~Summoner() override;

	void Draw() override;
	void Update() override;
	void Clean() override;

private:

	void FleeQuery();
	void CirclePlayer();
	void ChooseAttack();
	void SpecialAttack();
	void Attack() override;
	void EndAttack();
	void Shoot() override;
	void Summon() override;
	void BuildAnimations();

	float m_fleeRadius;
	float m_attackRadius;
	int m_lastFrame[4];
	int m_lastSummonFrame;
};

#endif // Define __SUMMONER__