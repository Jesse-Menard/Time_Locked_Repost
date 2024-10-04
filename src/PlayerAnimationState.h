#pragma once
#ifndef __PLAYER_ANIMATION_STATE__
#define __PLAYER_ANIMATION_STATE__

enum class PlayerAnimationState
{
	PLAYER_WALK_UP,
	PLAYER_WALK_LEFT,
	PLAYER_WALK_RIGHT,
	PLAYER_WALK_DOWN,
	PLAYER_ATTACK_UP,
	PLAYER_ATTACK_LEFT,
	PLAYER_ATTACK_RIGHT,
	PLAYER_ATTACK_DOWN,
	PLAYER_PREP_ATTACK_UP,
	PLAYER_PREP_ATTACK_LEFT,
	PLAYER_PREP_ATTACK_RIGHT,
	PLAYER_PREP_ATTACK_DOWN,
	PLAYER_BOW_ATTACK_UP,
	PLAYER_BOW_ATTACK_LEFT,
	PLAYER_BOW_ATTACK_RIGHT,
	PLAYER_BOW_ATTACK_DOWN,
	PLAYER_PREP_BOW_ATTACK_UP,
	PLAYER_PREP_BOW_ATTACK_LEFT,
	PLAYER_PREP_BOW_ATTACK_RIGHT,
	PLAYER_PREP_BOW_ATTACK_DOWN,
	PLAYER_ATTACK_OVERSIZED_UP,
	PLAYER_ATTACK_OVERSIZED_LEFT,
	PLAYER_ATTACK_OVERSIZED_RIGHT,
	PLAYER_ATTACK_OVERSIZED_DOWN,
	PLAYER_SUMMONER_ATTACK_OVERSIZED_UP,
	PLAYER_SUMMONER_ATTACK_OVERSIZED_LEFT,
	PLAYER_SUMMONER_ATTACK_OVERSIZED_RIGHT,
	PLAYER_SUMMONER_ATTACK_OVERSIZED_DOWN,
	PLAYER_BOOMERANG_ATTACK_OVERSIZED_UP,
	PLAYER_BOOMERANG_ATTACK_OVERSIZED_LEFT,
	PLAYER_BOOMERANG_ATTACK_OVERSIZED_RIGHT,
	PLAYER_BOOMERANG_ATTACK_OVERSIZED_DOWN,
	PLAYER_PREP_ATTACK_OVERSIZED_UP,
	PLAYER_PREP_ATTACK_OVERSIZED_LEFT,
	PLAYER_PREP_ATTACK_OVERSIZED_RIGHT,
	PLAYER_PREP_ATTACK_OVERSIZED_DOWN,
	PLAYER_CAST_UP,
	PLAYER_CAST_LEFT,
	PLAYER_CAST_RIGHT,
	PLAYER_CAST_DOWN,
	PLAYER_IDLE_UP,
	PLAYER_IDLE_LEFT,
	PLAYER_IDLE_DOWN,
	PLAYER_IDLE_RIGHT,
	PLAYER_DEATH,
	PLAYER_REVIVE,
	NUM_OF_ANIMATION_STATES
};

#endif /* defined (__PLAYER_ANIMATION_STATE__)*/