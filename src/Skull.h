#pragma once
#ifndef __SKULL__
#define __SKULL__

#include "Enemy.h"

class Skull : public Enemy
{
public:
	Skull();
	~Skull() override;

	void Draw() override;
	void Update() override;
	void Clean() override;

private:
	void Attack() override;
};

#endif // Define __SKULL__