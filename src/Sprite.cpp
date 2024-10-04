#include "Sprite.h"

#include <iostream>

Sprite::Sprite()
{
}

Sprite::~Sprite()
= default;

SpriteSheet* Sprite::GetSpriteSheet()
{
	return m_pSpriteSheet;
}

Animation& Sprite::GetAnimation(const std::string& name)
{
	return m_pAnimations[name];
}

void Sprite::SetSpriteSheet(SpriteSheet* sprite_sheet)
{
	m_pSpriteSheet = sprite_sheet;
}

void Sprite::SetAnimation(const Animation& animation)
{
	if(!CheckIfAnimationExists(animation.name))
	{
		m_pAnimations[animation.name] = animation;
	}
}

void Sprite::BuildAnimation(const std::string animation_name, const int frame_start, const int frame_end, const int scale, const int repeat_frame, const int repeat_amount, const bool repeat_last)
{
	auto animation = Animation();

	animation.name = animation_name;
	for (int i = frame_start; i < frame_end + 1; ++i)
	{
		if ( i == repeat_frame)
		{
			for (int j = 0; j < repeat_amount; ++j)
			{
				animation.frames.push_back(GetSpriteSheet()->GetFrame(animation_name + std::to_string(i)));
			}
		}
		if(i == frame_end && repeat_last)
			animation.frames.push_back(GetSpriteSheet()->GetFrame(animation_name + std::to_string(i)));

		animation.frames.push_back(GetSpriteSheet()->GetFrame(animation_name + std::to_string(i)));
	}

	// Modifying to accomodate for scale
	for (unsigned i = 0; i < animation.frames.size(); i++)
	{
		animation.frames[i].x *= scale;
		animation.frames[i].y *= scale;
		animation.frames[i].w *= scale;
		animation.frames[i].h *= scale;
	}

	SetAnimation(animation);
}

bool Sprite::CheckIfAnimationExists(const std::string& id)
{
	return m_pAnimations.find(id) != m_pAnimations.end();
}
