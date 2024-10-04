#include "DefeatScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

EndScene::EndScene()
{
	EndScene::Start();
}

EndScene::~EndScene()
= default;

void EndScene::Draw()
{
	DrawDisplayList();
}

void EndScene::Update()
{
	UpdateDisplayList();
}

void EndScene::Clean()
{
	RemoveAllChildren();
}

void EndScene::HandleEvents()
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

	//if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	//{
	//	SoundManager::Instance().StopMusic(0);
	//	Game::Instance().ChangeSceneState(SceneState::PLAY);
	//}
}

void EndScene::Start()
{
	const SDL_Color red = { 255, 0, 0, 255 };

	m_pBackground = new Background("../Assets/backgrounds/Defeat.png", "default");
	AddChild(m_pBackground);

	m_label = new Label("DEFEAT!", "Dock51", 75, red, glm::vec2(640.0f, 340.0f));
	m_label->SetParent(this);
	AddChild(m_label);

	m_label = new Label("You're history now.", "Consolas", 25, red, glm::vec2(640.0f, 400.0f));
	m_label->SetParent(this);
	AddChild(m_label);

	// Background Music
	SoundManager::Instance().Load("../Assets/audio/DefeatMusic.mp3", "Defeat", SoundType::SOUND_MUSIC);
	SoundManager::Instance().PlayMusic("Defeat");

	// Restart Button
	m_pRestartButton = new Button("../Assets/textures/restartButton.png", "restartButton", GameObjectType::RESTART_BUTTON);
	m_pRestartButton->GetTransform()->position = glm::vec2(640.0f, 520.0f);
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

	ImGuiWindowFrame::Instance().SetDefaultGuiFunction();
}