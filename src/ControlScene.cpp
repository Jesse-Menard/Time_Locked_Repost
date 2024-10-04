#include "ControlScene.h"
#include "EventManager.h"
#include "Game.h"
#include "SoundManager.h"
#include "Label.h"

ControlScene::ControlScene()
{
	ControlScene::Start();
}

ControlScene::~ControlScene()
= default;

void ControlScene::Draw()
{
	DrawDisplayList();
}

void ControlScene::Update()
{
	UpdateDisplayList();
}

void ControlScene::Clean()
{
	RemoveAllChildren();
}

void ControlScene::HandleEvents()
{
	EventManager::Instance().Update();

	// Keyboard Events
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		SoundManager::Instance().StopMusic(0);
		Game::Instance().Quit();
	}
}

void ControlScene::Start()
{
	const SDL_Color blue = { 0, 255, 255, 255 };

	m_pBackground = new Background("../Assets/backgrounds/black.png", "black");
	AddChild(m_pBackground);

	m_pControlSceneLabel = new Label("CONTROLS", "Dock51", 75, blue, glm::vec2(640.0f, 60.0f));
	m_pControlSceneLabel->SetParent(this);
	AddChild(m_pControlSceneLabel);

	// Background Music
	//SoundManager::Instance().Load("../Assets/audio/Map0-TitleMusic.mp3", "Map0", SoundType::SOUND_MUSIC);
	//SoundManager::Instance().SetMusicVolume(16);
	//SoundManager::Instance().PlayMusic("Map0");

	//Back Button
	m_pBackButton = new Button("../Assets/textures/BackButton.png",
		"BackButton", GameObjectType::BACK_BUTTON, glm::vec2(0.0f, 0.0f), true);
	m_pBackButton->GetTransform()->position = glm::vec2(640.0f, 650.0f);
	m_pBackButton->AddEventListener(Event::CLICK, [&]()-> void
		{
			m_pBackButton->SetActive(false);
			Game::Instance().ChangeSceneState(SceneState::START);
		});

	m_pBackButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
		{
			m_pBackButton->SetAlpha(128);
		});

	m_pBackButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
		{
			m_pBackButton->SetAlpha(255);
		});
	AddChild(m_pBackButton);
}

void ControlScene::GUI_Function()
{
	ImGui::NewFrame();

	ImGui::Begin("Time-Locked Survivors Debug Menu - Level Selector", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if (ImGui::Button("Level 1"))
	{
		Game::Instance().ChangeSceneState(SceneState::LEVEL1);
	}

	ImGui::Separator();

	if (ImGui::Button("Level 2"))
	{
		Game::Instance().ChangeSceneState(SceneState::LEVEL2);
	}

	ImGui::Separator();

	if (ImGui::Button("Level 3"))
	{
		Game::Instance().ChangeSceneState(SceneState::LEVEL3);
	}

	ImGui::Separator();


	ImGui::End();
}
