#include "Skull.h"

Skull::Skull() : Enemy("skull", 3)
{
	SetWidth(32);
	SetHeight(32);

	SetSpeed(1.75f);
	SetHitDelay(20);
}

Skull::~Skull()
= default;

void Skull::Draw()
{
	Enemy::Draw();
}

void Skull::Update()
{
	Enemy::Update();
	if (GetHealth() > 0)
		Move();
	else
		Death();
}

void Skull::Clean()
{
	Enemy::Clean();
}

void Skull::Attack()
{
	Enemy::Attack();
}
