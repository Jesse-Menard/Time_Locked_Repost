#pragma once
#ifndef __SCENE_STATE__
#define __SCENE_STATE__
enum class SceneState
{
	NO_SCENE = -1,
	START,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	WIN,
	RESULTS,
	END,
	CREDITS,
	NUM_OF_SCENES,
	CONTROL_SCENE,
	LEVEL_SELECT
};
#endif /* defined (__SCENE_STATE__) */