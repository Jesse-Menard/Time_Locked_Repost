#include "PauseScene.h"

#include "EventManager.h"
#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

PauseScene::PauseScene() : m_controlsToggle(false)
{
	PauseScene::Start();
}

PauseScene::~PauseScene()
= default;

void PauseScene::Draw()
{
	// Draw pause menu
	Util::DrawFilledRect(glm::vec2(150.0f, 100.0f), Config::SCREEN_WIDTH - 300, Config::SCREEN_HEIGHT - 200, glm::vec4(0, 0, 0, 0.33));
	if(m_controlsToggle)
		TextureManager::Instance().Draw("black_small", glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2 - 50), 0, 255, true);
	m_pResumeButton->Draw();
	m_pControlsButton->Draw();
	m_label->Draw();
}

void PauseScene::Update()
{
	m_pResumeButton->Update();
	m_pControlsButton->Update();
}

void PauseScene::Clean()
{
	SoundManager::Instance().SetSoundVolume(24);
	SoundManager::Instance().SetMusicVolume(16);
	RemoveChild(m_label);
	RemoveChild(m_pResumeButton);
	RemoveChild(m_pControlsButton);
}

void PauseScene::HandleEvents()
{
	EventManager::Instance().Update();

	// Keyboard Events
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		SoundManager::Instance().StopMusic(0);
		Game::Instance().Quit();
	}
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_P))
	{
		SoundManager::Instance().SetMusicVolume(16);
		SoundManager::Instance().SetSoundVolume(24);

		Game::Instance().TogglePause();
	}
}

void PauseScene::Start()
{
	SoundManager::Instance().SetMusicVolume(8);
	SoundManager::Instance().SetSoundVolume(0);

	TextureManager::Instance().Load("../Assets/backgrounds/black_small.png", "black_small");


	m_label = new Label("PAUSED!", "Dock51", 60, {0, 0, 255, 255}, glm::vec2(640.0f, 75.0f));
	m_label->SetParent(this);
	AddChild(m_label, 20);

	// Restart Button
	m_pResumeButton = new Button("../Assets/textures/resumeButton.png", "resumeButton", GameObjectType::RESTART_BUTTON);
	m_pResumeButton->GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2 - 60.0f, 550.0f );
	m_pResumeButton->AddEventListener(Event::CLICK, [&]()-> void
	{
		m_pResumeButton->SetActive(false);
		Game::Instance().TogglePause();
	});

	m_pResumeButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
	{
		m_pResumeButton->SetAlpha(128);
	});

	m_pResumeButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
	{
		m_pResumeButton->SetAlpha(255);
	});

	m_pResumeButton->SetParent(this);
	AddChild(m_pResumeButton, 20);

	// Control Button
	m_pControlsButton = new Button("../Assets/textures/ControlsButton.png", "paused_controlButton", GameObjectType::RESTART_BUTTON);
	m_pControlsButton->GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2 + 60.0f, 550.0f);
	m_pControlsButton->AddEventListener(Event::CLICK, [&]()-> void
	{
		m_pControlsButton->SetActive(false);
		m_controlsToggle = !m_controlsToggle;
	});

	m_pControlsButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
	{
		m_pControlsButton->SetAlpha(128);
	});

	m_pControlsButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
	{
		m_pControlsButton->SetAlpha(255);
	});

	m_pControlsButton->SetParent(this);
	AddChild(m_pControlsButton, 20);

	ImGuiWindowFrame::Instance().SetDefaultGuiFunction();
}