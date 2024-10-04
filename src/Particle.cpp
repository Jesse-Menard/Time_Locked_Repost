#include "Particle.h"

#include "TextureManager.h"

Particle::Particle(glm::vec2 position, int duration) : m_duration(duration), m_startingDuration(duration)
{
	GetTransform()->position = position;
	TextureManager::Instance().Load("../Assets/textures/magicMissileParticle.png", "particle1");
}

void Particle::Draw()
{
	if(m_duration > 0)
		TextureManager::Instance().Draw("particle1", GetTransform()->position, 0, (255 / m_startingDuration) * m_duration, true);
}

void Particle::Update()
{
	m_duration--;
}

void Particle::Clean()
{

}
