#pragma once
#ifndef __PAUSE_SCENE__
#define __PAUSE_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"

class PauseScene final : public Scene
{
public:
	PauseScene();
	~PauseScene() override;

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;

private:
	Label* m_label{};

	Button* m_pResumeButton{};
	Button* m_pControlsButton{};

	bool m_controlsToggle;
};

#endif // __PAUSE_SCENE__