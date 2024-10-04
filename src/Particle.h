#pragma once
#ifndef __PARTICLE__
#define __PARTICLE__
#include "DisplayObject.h"

class Particle : public DisplayObject
{
public:
	Particle(glm::vec2 position, int duration);
	void Draw() override;
	void Update() override;
	void Clean() override;

	int m_duration;
private:
	int m_startingDuration;

};
#endif // Defined __PARTICLE__