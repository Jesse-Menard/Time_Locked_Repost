#include "ResScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "Util.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

ResScene::ResScene()
{
	ResScene::Start();
}

ResScene::~ResScene()
= default;

void ResScene::Draw()
{
	DrawDisplayList();
	Util::DrawFilledRect(glm::vec2(Config::SCREEN_WIDTH - 1200, Config::SCREEN_HEIGHT - 600), 1100, 500, glm::vec4(1, 1, 1, 1));
	m_kills->Draw();
	m_totalKills->Draw();
	m_Damage->Draw();
	m_TotalDamage->Draw();
	m_level->Draw();
	m_TimePlayed->Draw();
}

void ResScene::Update()
{
	UpdateDisplayList();
}

void ResScene::Clean()
{
	RemoveAllChildren();
}

void ResScene::HandleEvents()
{
	EventManager::Instance().Update();

	// Keyboard Events
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		SoundManager::Instance().StopMusic(0);
		Game::Instance().Quit();
	}
}

void ResScene::Start()
{
	const SDL_Color blue = { 0, 255, 255, 255 };
	const SDL_Color black = { 0, 0, 0, 255 };

	m_pBackground = new Background("../Assets/backgrounds/level0.png", "default");
	AddChild(m_pBackground);

	m_results = new Label("Results!", "Dock51", 75, blue, glm::vec2(640.0f, 75.0f));
	m_results->SetParent(this);
	AddChild(m_results);
	m_level = new Label("Level completed: " + stats->GetLastLevel(), "Dock51", 45, black, glm::vec2(115.0f, 150.0f), 0, false);
	m_level->SetParent(this);
	AddChild(m_level);
	m_kills = new Label("Kills this round: " + std::to_string(stats->GetKills()), "Dock51", 45, black, glm::vec2(115.0f, 250.0f), 0, false);
	m_kills->SetParent(this);
	AddChild(m_kills);
	m_totalKills = new Label("Total Kills: " + std::to_string(stats->GetTotalKills()), "Dock51", 45, black, glm::vec2(115.0f, 310.0f), 0, false);
	m_totalKills->SetParent(this);
	AddChild(m_totalKills);
	m_Damage = new Label("Damage Dealt this round: " + std::to_string(stats->GetThisLevelDamage()), "Dock51", 45, black, glm::vec2(115.0f, 390.0f), 0, false);
	m_Damage->SetParent(this);
	AddChild(m_Damage);
	m_TotalDamage = new Label("Total Damage Dealt: " + std::to_string(stats->GetTotalDamage()), "Dock51", 45, black, glm::vec2(115.0f, 450.0f), 0, false);
	m_TotalDamage->SetParent(this);
	AddChild(m_TotalDamage);
	m_TimePlayed = new Label("Time Played (seconds): " + std::to_string(stats->GetTimePlayed()), "Dock51", 45, black, glm::vec2(115.0f, 550.0f), 0, false);
	m_TimePlayed->SetParent(this);
	AddChild(m_TimePlayed);

	m_pRestartButton = new Button("../Assets/textures/restartButton.png", "restartButton", GameObjectType::RESTART_BUTTON);
	m_pRestartButton->GetTransform()->position = glm::vec2(640.0f, 650.0f);
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
	m_pNextLevel = new Button("../Assets/textures/nextButton.png", "NextButton", GameObjectType::NEXT_BUTTON);
	m_pNextLevel->GetTransform()->position = glm::vec2(740.0f, 650.0f);
	m_pNextLevel->AddEventListener(Event::CLICK, [&]()-> void
		{
			m_pNextLevel->SetActive(false);
			if (Stats::GetLastLevel() == "Prehistoric")
			{
				Game::Instance().ChangeSceneState(SceneState::LEVEL2);
			}
			else if (Stats::GetLastLevel() == "Medieval")
			{
				Game::Instance().ChangeSceneState(SceneState::LEVEL3);
			}
			else if (Stats::GetLastLevel() == "Future")
			{
				Game::Instance().ChangeSceneState(SceneState::WIN);
			}
			else
			{
				Game::Instance().ChangeSceneState(SceneState::START);
			}
		});

	m_pNextLevel->AddEventListener(Event::MOUSE_OVER, [&]()->void
		{
			m_pNextLevel->SetAlpha(128);
		});

	m_pNextLevel->AddEventListener(Event::MOUSE_OUT, [&]()->void
		{
			m_pNextLevel->SetAlpha(255);
		});

	AddChild(m_pNextLevel);

	SoundManager::Instance().Load("../Assets/audio/VictoryMusic.mp3", "win", SoundType::SOUND_MUSIC);
	SoundManager::Instance().PlayMusic("win", 0);

	ImGuiWindowFrame::Instance().SetDefaultGuiFunction();
}

void ResScene::GUI_Function()
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
