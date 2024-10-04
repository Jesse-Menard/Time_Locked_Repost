#include "Credits.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

CreditsScene::CreditsScene()
{
	CreditsScene::Start();
}

CreditsScene::~CreditsScene()
= default;

void CreditsScene::Draw()
{
	DrawDisplayList();
}

void CreditsScene::Update()
{
	UpdateDisplayList();
}

void CreditsScene::Clean()
{
	RemoveAllChildren();
}

void CreditsScene::HandleEvents()
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

void CreditsScene::Start()
{
	const SDL_Color blue = { 0, 255, 255, 255 };
	const SDL_Color purple = { 180, 0, 255, 255 };

	m_pJesseLabel = new Label("Jesse Menard-101363299 - Producer and lead programer work on levels,UI,Animation and Game play", "Consolas", 15, purple, glm::vec2(380.0f, 50.0f));
	m_pJesseLabel->SetParent(this);
	AddChild(m_pJesseLabel);

	m_pSamLabel = new Label("Samantha Green-101286952 - Art Director worked on sprites, level backgrounds , game concept", "Consolas", 15, purple, glm::vec2(365.0f, 80.0f));
	m_pSamLabel->SetParent(this);
	AddChild(m_pSamLabel);

	m_pTaejaunLabel = new Label("Taejaun Ayton-101188830 - General Programer work on sprites game balance, game concept, and scenes  ", "Consolas", 15, purple, glm::vec2(400.0f, 110.0f));
	m_pTaejaunLabel->SetParent(this);
	AddChild(m_pTaejaunLabel);

	m_pMigelLabel = new Label("Migel-101314253 - Audio Engineer worked on game audio and creating game scenes and game concept  ", "Consolas", 15, purple, glm::vec2(390.0f, 140.0f));
	m_pMigelLabel->SetParent(this);
	AddChild(m_pMigelLabel);

	m_pRestartButton = new Button("../Assets/textures/restartButton.png", "restartButton", GameObjectType::RESTART_BUTTON);
	m_pRestartButton->GetTransform()->position = glm::vec2(640.0f, 600.0f);
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

void CreditsScene::GUI_Function()
{
}
