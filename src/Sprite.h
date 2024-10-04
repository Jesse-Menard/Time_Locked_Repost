#pragma once
#ifndef __SPRITE__
#define __SPRITE__

#include "DisplayObject.h"
#include "Animation.h"
#include <unordered_map>
#include "SpriteSheet.h"

class Sprite : public DisplayObject
{
public:
	Sprite();
	virtual ~Sprite();
	
	// Life Cycle Functions
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Clean() = 0;

	// getters
	SpriteSheet* GetSpriteSheet();
	Animation& GetAnimation(const std::string& name);
	
	// setters
	void SetSpriteSheet(SpriteSheet* sprite_sheet);
	void SetAnimation(const Animation& animation);

	void BuildAnimation(std::string animation_name, int frame_start, int frame_end, int scale, int repeat_frame = 0, int repeat_amount = 0, const bool repeat_last = false);
private:
	// private utility functions
	bool CheckIfAnimationExists(const std::string& id);

	SpriteSheet* m_pSpriteSheet;

	std::unordered_map<std::string, Animation> m_pAnimations;
};

#endif /* defined (__SPRITE__) */