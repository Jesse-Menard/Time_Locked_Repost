#pragma once
#ifndef __CREDITS_SCENE__
#define __CREDITS_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Button.h"
#include "Background.h"
#include "Player.h"

class CreditsScene final : public Scene
{
public:
	CreditsScene();
	~CreditsScene() override;

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
	Label* m_pJesseLabel{};
	Label* m_pSamLabel{};
	Label* m_pTaejaunLabel{};
	Label* m_pMigelLabel{};

	Button* m_pRestartButton{};
	Button* m_pExitButton{};
	Button* m_pMainMenuButton{};
};

#endif /* defined (__DEFEAT_SCENE__) */

