#pragma once
#ifndef __RES_SCENE__
#define __RES_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Button.h"
#include "Background.h"
#include "Player.h"

class ResScene final : public Scene
{
public:
	ResScene();
	~ResScene() override;

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
	void GUI_Function();

	int m_timer;

private:
	Background* m_pBackground;

	Label* m_results{};
	Label* m_kills{};
	Label* m_totalKills{};
	Label* m_TimePlayed{};
	Label* m_level{};
	Label* m_Damage{};
	Label* m_TotalDamage{};

	Button* m_pRestartButton{};
	Button* m_pExitButton{};
	Button* m_pNextLevel{};
	Button* m_pMainMenuButton{};
};

#endif /* defined (__DEFEAT_SCENE__) */

