#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Player.h"
#include "Button.h"
#include "SDL_image.h"
#include "Background.h"
#include "ControlScene.h"
#include "LevelSelect.h"


class StartScene final : public Scene
{
public:
	StartScene();
	~StartScene() override;

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
	void GUI_Function();
	
private:

	Background* m_pBackground;

	Label* m_pStartLabel{};
	Label* m_pInstructionsLabel{};

	Player* m_pPlayer{};

	Button* m_pStartButton{};
	Button* m_pLevelSelectButton{};
	Button* m_pControlSelectButton{};
	Button* m_pExitButton{};
};

#endif /* defined (__START_SCENE__) */