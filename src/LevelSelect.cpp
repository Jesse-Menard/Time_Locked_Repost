#include "LevelSelect.h"
#include "EventManager.h"
#include "Game.h"
#include "SoundManager.h"
#include "Label.h"

LevelSelect::LevelSelect()
{
	LevelSelect::Start();
}

LevelSelect::~LevelSelect()
= default;

void LevelSelect::Draw()
{
	DrawDisplayList();
}

void LevelSelect::Update()
{
	UpdateDisplayList();
}

void LevelSelect::Clean()
{
	RemoveAllChildren();
}

void LevelSelect::HandleEvents()
{
	EventManager::Instance().Update();

	// Keyboard Events
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		SoundManager::Instance().StopMusic(0);
		Game::Instance().Quit();
	}
}

void LevelSelect::Start()
{
	const SDL_Color blue = { 0, 255, 255, 255 };

	m_pBackground = new Background("../Assets/backgrounds/level0.png", "default");
	AddChild(m_pBackground);

	m_pLevelSelectLabel = new Label("SELECT LEVEL", "Dock51", 75, blue, glm::vec2(640.0f, 60.0f));
	m_pLevelSelectLabel->SetParent(this);
	AddChild(m_pLevelSelectLabel);


	// Level 1 Button
	m_pLevelOneButton = new Button("../Assets/textures/Level1Button.png",
		"Level1Button", GameObjectType::LEVEL_ONE_BUTTON, glm::vec2(0.0f, 0.0f), false);
	m_pLevelOneButton->GetTransform()->position = glm::vec2(60.0f, 150.0f + 130.0f);

	m_pLevelOneButton->AddEventListener(Event::CLICK, [&]()-> void
		{
			m_pLevelOneButton->SetActive(false);
			Game::Instance().ChangeSceneState(SceneState::LEVEL1);
		});

	m_pLevelOneButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
		{
			m_pLevelOneButton->SetAlpha(128);
		});

	m_pLevelOneButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
		{
			m_pLevelOneButton->SetAlpha(255);
		});
	AddChild(m_pLevelOneButton);

	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });

	//Level 2 Button
	m_pLevelTwoButton = new Button("../Assets/textures/Level2Button.png", "Level2Button",
		GameObjectType::LEVEL_TWO_BUTTON, glm::vec2(0.0f, 0.0f), false);
	m_pLevelTwoButton->GetTransform()->position = glm::vec2(60.0f, 205.0f + 130.0f);
	m_pLevelTwoButton->AddEventListener(Event::CLICK, [&]()-> void
		{
			m_pLevelTwoButton->SetActive(false);
			Game::Instance().ChangeSceneState(SceneState::LEVEL2);
		});

	m_pLevelTwoButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
		{
			m_pLevelTwoButton->SetAlpha(128);
		});

	m_pLevelTwoButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
		{
			m_pLevelTwoButton->SetAlpha(255);
		});
	AddChild(m_pLevelTwoButton);

	//Level 3 Button
	m_pLevelThreeButton = new Button("../Assets/textures/Level3Button.png",
		"Level3Button", GameObjectType::LEVEL_THREE_BUTTON, glm::vec2(0.0f, 0.0f), false);
	m_pLevelThreeButton->GetTransform()->position = glm::vec2(60.0f, 260.0f + 130.0f);
	m_pLevelThreeButton->AddEventListener(Event::CLICK, [&]()-> void
		{
			m_pLevelThreeButton->SetActive(false);
			Game::Instance().ChangeSceneState(SceneState::LEVEL3);
		});

	m_pLevelThreeButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
		{
			m_pLevelThreeButton->SetAlpha(128);
		});

	m_pLevelThreeButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
		{
			m_pLevelThreeButton->SetAlpha(255);
		});
	AddChild(m_pLevelThreeButton);

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

void LevelSelect::GUI_Function()
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

