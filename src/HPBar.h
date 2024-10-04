#pragma once
#ifndef __HP_BAR__
#define __HP_BAR__

#include "DisplayObject.h"
#include "Util.h"

class HPBar : DisplayObject
{
public:
	HPBar(const glm::vec2 position, const int health, glm::vec4 colour = { 1, 0, 0, 1 })
		: m_position(position), m_display(true), m_health(health), m_maxHealth(health), m_colour(colour), m_displayColour(colour) {}
	~HPBar() override = default;

	void Draw() override
	{
		// For hit effect
		if(m_colourCtr++ % 2 == 0)
		{
			if (m_displayColour.r <= m_colour.r + 0.1f && m_displayColour.r >= m_colour.r - 0.1f &&
			   m_displayColour.g <= m_colour.g + 0.1f && m_displayColour.g >= m_colour.g - 0.1f &&
			   m_displayColour.b <= m_colour.b + 0.1f && m_displayColour.b >= m_colour.b - 0.1f)
			{
				m_displayColour = m_colour;
			}
			else
			{
				if (m_colour == glm::vec4(1, 0, 0, 1))
					m_displayColour += glm::vec4(0.2, 0, 0, 0);
				else if (m_colour == glm::vec4(0, 1, 0, 1))
					m_displayColour += glm::vec4(-0.2, 0.2, 0, 0);
			}
		}


		if (m_display)
			Util::DrawFilledRect(m_position - glm::vec2(32, 32 + 2), 64 * m_health / m_maxHealth, 7, m_displayColour);
		Util::DrawRect(m_position - glm::vec2(32, 32 + 2), 64, 7, glm::vec4(0, 0, 0, 1));

	}
	void Update() override {}

	void Update(const glm::vec2 position, const int health, const bool alive)
	{
		if (m_health > health)
		{
			if (m_colour == glm::vec4(1, 0, 0, 1))
				m_displayColour = glm::vec4(0, 0, 0, 1);
			else if (m_colour == glm::vec4(0, 1, 0, 1))
				m_displayColour = glm::vec4(1, 0, 0, 1);

		}
		m_position = position;
		m_health = health;
		m_display = alive;
	}
	void Clean() override {}

private:
	glm::vec2 m_position;
	bool m_display;
	int m_health;
	int m_maxHealth;
	glm::vec4 m_colour;
	glm::vec4 m_displayColour;

	int m_colourCtr;
};
#endif // Defined __HP_BAR__