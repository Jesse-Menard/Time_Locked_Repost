#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "ControlScene.h"

StartScene::StartScene()
{
	StartScene::Start();
}

StartScene::~StartScene()
= default;

void StartScene::Draw()
{
	DrawDisplayList();
}

void StartScene::Update()
{
	UpdateDisplayList();
}

void StartScene::Clean()
{
	RemoveAllChildren();
}

void StartScene::HandleEvents()
{
	EventManager::Instance().Update();

	// Keyboard Events
	if(EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		SoundManager::Instance().StopMusic(0);
		Game::Instance().Quit();
	}

	//if(EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	//{
	//	SoundManager::Instance().StopMusic(0);
	//	Game::Instance().ChangeSceneState(SceneState::PLAY);
	//}
}

void StartScene::Start()
{
	const SDL_Color blue = { 0, 255, 255, 255 };

	m_pBackground = new Background("../Assets/backgrounds/level0.png", "default");
	AddChild(m_pBackground);

	m_pStartLabel = new Label("TIME-LOCKED SURVIVOR", "Dock51", 75, blue, glm::vec2(640.0f, 60.0f));
	m_pStartLabel->SetParent(this);
	AddChild(m_pStartLabel);

	m_pInstructionsLabel = new Label("Can you survive history?", "Consolas", 25, blue, glm::vec2(640.0f, 120.0f));
	m_pInstructionsLabel->SetParent(this);
	AddChild(m_pInstructionsLabel);

	m_pInstructionsLabel = new Label("Puzzled Games", "Consolas", 15, blue, glm::vec2(640.0f, 10.0f));
	m_pInstructionsLabel->SetParent(this);
	AddChild(m_pInstructionsLabel);

	// Player Sprite
	//m_pPlayer = new Player();
	//m_pPlayer->GetTransform()->position = glm::vec2(640.0f, 420.0f);
	//AddChild(m_pPlayer);
	//m_playerFacingRight = true;

	// Background Music
	SoundManager::Instance().Load("../Assets/audio/BossMusic.mp3", "BossFight", SoundType::SOUND_MUSIC);
	SoundManager::Instance().Load("../Assets/audio/TowerKnightTheme.mp3", "phase2", SoundType::SOUND_MUSIC);
	SoundManager::Instance().Load("../Assets/audio/Map0-TitleMusic.mp3", "Map0", SoundType::SOUND_MUSIC);
	SoundManager::Instance().Load("../Assets/audio/Map1-PrehistoricMusic.mp3", "Map1", SoundType::SOUND_MUSIC);
	SoundManager::Instance().Load("../Assets/audio/Map2-MedievalMusic.mp3", "Map2", SoundType::SOUND_MUSIC);
	SoundManager::Instance().Load("../Assets/audio/Map5-FuturisticMusic.mp3", "Map5", SoundType::SOUND_MUSIC);
	SoundManager::Instance().SetMusicVolume(16);
	SoundManager::Instance().PlayMusic("Map0");


	// SFX
	SoundManager::Instance().AllocateChannels(128);

	SoundManager::Instance().Load("../Assets/sounds/Damaged2.wav", "damage", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/phase2transition.wav", "transition", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/KC-Edit Louder.wav", "bass", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/laser.wav", "laser", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/SpearSwing.wav", "spear", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/GlowSword.wav", "Gsword", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/SwordSwing.wav", "sword", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/RockThrow.wav", "throw", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/EnemyDeath.wav", "enemy_death", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/PlayerDeath.wav", "player_death", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/ArrowRelease.wav", "arrow", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/EquipPickup.wav", "pickup", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/sounds/SelectChime.wav", "select", SoundType::SOUND_SFX);
	SoundManager::Instance().SetSoundVolume(24);

	// Start Button
	m_pStartButton = new Button("../Assets/textures/startButton.png", "startButton",
		GameObjectType::START_BUTTON, glm::vec2(0.0f, 0.0f), false);
	m_pStartButton->GetTransform()->position = glm::vec2(60.0f, 150.0f + 130.0f);
	m_pStartButton->AddEventListener(Event::CLICK, [&]()-> void
	{
		m_pStartButton->SetActive(false);
		Game::Instance().ChangeSceneState(SceneState::LEVEL1);
	});
	m_pStartButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
	{
		m_pStartButton->SetAlpha(128);
	});
	m_pStartButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
	{
		m_pStartButton->SetAlpha(255);
	});
	AddChild(m_pStartButton);

	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });

	//Level Select Button
	m_pLevelSelectButton = new Button("../Assets/textures/LevelSelectButton.png","LevelSelectButton",
		GameObjectType::LEVEL_SELECT_BUTTON, glm::vec2(0.0f, 0.0f), false);
	m_pLevelSelectButton->GetTransform()->position = glm::vec2(60.0f, 260.0f + 130.0f);
	m_pLevelSelectButton->AddEventListener(Event::CLICK, [&]()-> void
	{
			m_pLevelSelectButton->SetActive(false);
			Game::Instance().ChangeSceneState(SceneState::LEVEL_SELECT);
	});
	m_pLevelSelectButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
	{
			m_pLevelSelectButton->SetAlpha(128);
	});
	m_pLevelSelectButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
	{
			m_pLevelSelectButton->SetAlpha(255);
	});
	AddChild(m_pLevelSelectButton);

	//Control Select Button
	m_pControlSelectButton = new Button("../Assets/textures/controlsButton.png",
		"controlsButton", GameObjectType::CONTROLS_BUTTON, glm::vec2(0.0f, 0.0f), false);
	m_pControlSelectButton->GetTransform()->position = glm::vec2(60.0f, 205.0f + 130.0f);
	m_pControlSelectButton->AddEventListener(Event::CLICK, [&]()-> void
	{
			m_pControlSelectButton->SetActive(false);
			Game::Instance().ChangeSceneState(SceneState::CONTROL_SCENE);
	});
	m_pControlSelectButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
	{
			m_pControlSelectButton->SetAlpha(128);
	});
	m_pControlSelectButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
	{
			m_pControlSelectButton->SetAlpha(255);
	});
	AddChild(m_pControlSelectButton);

	//Quit Button
	m_pExitButton = new Button("../Assets/textures/quitButton.png",
		"quitButton", GameObjectType::CONTROLS_BUTTON, glm::vec2(0.0f, 0.0f), true);
	m_pExitButton->GetTransform()->position = glm::vec2(640.0f, 650.0f);
	m_pExitButton->AddEventListener(Event::CLICK, [&]()-> void
		{
			m_pExitButton->SetActive(false);
			Game::Instance().Quit();
		});
	m_pExitButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
		{
			m_pExitButton->SetAlpha(128);
		});
	m_pExitButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
		{
			m_pExitButton->SetAlpha(255);
		});
	AddChild(m_pExitButton);
}

void StartScene::GUI_Function()
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

	if (ImGui::Button("Win"))
	{
		Game::Instance().ChangeSceneState(SceneState::WIN);
	}

	ImGui::Separator();

	if (ImGui::Button("End"))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}

	ImGui::Separator();

	if (ImGui::Button("Results"))
	{
		Game::Instance().ChangeSceneState(SceneState::RESULTS);
	}


	ImGui::End();
}
