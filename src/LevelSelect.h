#pragma once
#ifndef __LEVEL_SELECT__
#define __LEVEL_SELECT__
#include "Scene.h"
#include "Background.h"
#include "Button.h"
#include "ControlScene.h"
#include "Label.h"

class LevelSelect final : public Scene
{
public:
	LevelSelect();
	~LevelSelect() override;

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
	void GUI_Function();

private:
	Background* m_pBackground;

	Label* m_pLevelSelectLabel{};

	Button* m_pLevelOneButton{};
	Button* m_pLevelTwoButton{};
	Button* m_pLevelThreeButton{};
	Button* m_pBackButton{};

};
#endif