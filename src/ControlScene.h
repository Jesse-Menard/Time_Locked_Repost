#pragma once
#ifndef __CONTROL_SCENE__
#define __CONTROL_SCENE__
#include "Scene.h"
#include "Background.h"
#include "Button.h"
#include "Label.h"
#include "LevelSelect.h"

class ControlScene final : public Scene
{
public:
	ControlScene();
	~ControlScene() override;

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
	void GUI_Function();

private:

	Background* m_pBackground;

	Label* m_pControlSceneLabel{};
	Button* m_pBackButton{};

};
#endif
