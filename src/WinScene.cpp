#include "WinScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

WinScene::WinScene()
{
	WinScene::Start();
}

WinScene::~WinScene()
= default;

void WinScene::Draw()
{
	DrawDisplayList();
}

void WinScene::Update()
{
	UpdateDisplayList();
}

void WinScene::Clean()
{
	RemoveAllChildren();
}

void WinScene::HandleEvents()
{
	EventManager::Instance().Update();

	// Button Events
	m_pRestartButton->Update();

	// Keyboard Events
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		SoundManager::Instance().StopMusic(0);
		Game::Instance().Quit();
	}
}

void WinScene::Start()
{
	const SDL_Color white = { 255, 255, 255, 255 };

	m_pBackground = new Background("../Assets/backgrounds/Victory2.png", "default");
	AddChild(m_pBackground);

	m_pWinLabel = new Label("Time UnLocked!", "Dock51", 68, white, glm::vec2(640.0f, 300.0f));
	m_pWinLabel->SetParent(this);
	AddChild(m_pWinLabel);

	m_pRestartButton = new Button("../Assets/textures/restartButton.png", "restartButton", GameObjectType::RESTART_BUTTON);
	m_pRestartButton->GetTransform()->position = glm::vec2(640.0f, 400.0f);
	m_pRestartButton->AddEventListener(Event::CLICK, [&]()-> void
		{
			m_pRestartButton->SetActive(false);
	Game::Instance().ChangeSceneState(SceneState::START);
		});

	m_pRestartButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
		{
			m_pRestartButton->SetAlpha(128);
		});

	m_pRestartButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
		{
			m_pRestartButton->SetAlpha(255);
		});

	AddChild(m_pRestartButton);

	SoundManager::Instance().Load("../Assets/audio/VictoryMusic.mp3", "win", SoundType::SOUND_MUSIC);
	SoundManager::Instance().PlayMusic("win", 0);

	ImGuiWindowFrame::Instance().SetDefaultGuiFunction();
}

void WinScene::GUI_Function()
{
}
