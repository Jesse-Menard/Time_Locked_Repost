#pragma once
#ifndef __WIN_SCENE__
#define __WIN_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Button.h"
#include "Background.h"
#include "Player.h"

class WinScene final : public Scene
{
public:
	WinScene();
	~WinScene() override;

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
	void GUI_Function();

private:
	Background* m_pBackground;

	Label* m_pWinLabel{};

	Button* m_pRestartButton{};
	Button* m_pExitButton{};
	Button* m_pMainMenuButton{};
};

#endif /* defined (__DEFEAT_SCENE__) */

