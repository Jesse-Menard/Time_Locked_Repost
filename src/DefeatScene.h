#pragma once
#ifndef __DEFEAT_SCENE__
#define __DEFEAT_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Button.h"
#include "Background.h"

class EndScene final : public Scene
{
public:
	EndScene();
	~EndScene() override;

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;

private:
	Background* m_pBackground;

	Label* m_label{};

	Button* m_pRestartButton{};
};

#endif /* defined (__DEFEAT_SCENE__) */