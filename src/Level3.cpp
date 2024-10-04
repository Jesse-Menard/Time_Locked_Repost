#include "Level3.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"
#include "SoundManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

Level3::Level3() : m_timer3(0), m_debugView(false), m_bossFight(false), m_killCtr(0), m_killLimit(22), m_enemySpawnCounter(180)
{
	Level3::Start();
}

Level3::~Level3()
= default;

void Level3::Draw()
{
	DrawDisplayList();

	// Boss HP Bar
	if (m_bossFight)
	{
		// Boss hit effect (Yes this is messy, but it's faster/easier)
		const glm::vec4 red = glm::vec4(1, 0, 0, 1);
		static glm::vec4 display_colour = red;
		static int colour_ctr;
		static int last_health = m_pBoss->GetHealth();
		
		if (m_pBoss->GetHealth() != last_health)
			display_colour = glm::vec4(0, 0, 0, 1);
		
		last_health = m_pBoss->GetHealth();
		if (colour_ctr++ % 2 == 0)
		{
			if (display_colour.r <= red.r + 0.1f && display_colour.r >= red.r - 0.1f &&
				display_colour.g <= red.g + 0.1f && display_colour.g >= red.g - 0.1f &&
				display_colour.b <= red.b + 0.1f && display_colour.b >= red.b - 0.1f)
			{
				display_colour = red;
			}
			else
				display_colour += glm::vec4(0.2, 0, 0, 0);
		}
		
		if (m_pBoss->GetHealth() > 0)
		{
			Util::DrawFilledRect(glm::vec2(50, 20), (Config::SCREEN_WIDTH - 100) * m_pBoss->GetHealth() / m_pBoss->GetMaxHealth(), 25, display_colour);
			if (m_pBoss->GetShielding())
				Util::DrawFilledRect(glm::vec2(50, 20), (Config::SCREEN_WIDTH - 100) * m_pBoss->GetHealth() / m_pBoss->GetMaxHealth(), 25, glm::vec4(0.25, 1, 0.75, 1));
		}
		Util::DrawRect(glm::vec2(50, 20), (Config::SCREEN_WIDTH - 100), 25, glm::vec4(0, 0, 0, 1));
	}

	if (m_debugView)
	{
		if (m_bossFight)
		{
			Util::DrawRect(m_pBoss->GetTransform()->position - glm::vec2(m_pBoss->GetWidth() * 0.5, m_pBoss->GetHeight() * 0.5), m_pBoss->GetWidth(), m_pBoss->GetHeight());
			m_pBoss->DrawHitBox();
		}

		for (const auto bullet : m_pBullets)
		{
			Util::DrawRect(bullet->GetTransform()->position - glm::vec2(bullet->GetWidth() * 0.5, bullet->GetHeight() * 0.5), bullet->GetWidth(), bullet->GetHeight(), {1,0,0,1});
		}
		for (const auto crossbow : m_pCrossBows)
		{
			Util::DrawRect(crossbow->GetTransform()->position - glm::vec2(crossbow->GetWidth() * 0.5, crossbow->GetHeight() * 0.5), crossbow->GetWidth(), crossbow->GetHeight());
		}
		for (const auto boomerang : m_pBoomerangs)
		{
			boomerang->DrawHitBox();
			Util::DrawRect(boomerang->GetTransform()->position - glm::vec2(boomerang->GetWidth() * 0.5, boomerang->GetHeight() * 0.5), boomerang->GetWidth(), boomerang->GetHeight());
		}
		Util::DrawRect(m_pPlayer->GetTransform()->position - glm::vec2(m_pPlayer->GetWidth() * 0.5, m_pPlayer->GetHeight() * 0.5), m_pPlayer->GetWidth(), m_pPlayer->GetHeight());
	}

	if(m_pBoss->GetPhaseTransition() && m_accelerationCtr > 1080) // 18s
	{
		if (m_flashAlpha < 1)
			m_flashAlpha += 1.0f / 110.0f;
		if (m_flashAlpha > 1)
			m_flashAlpha = 1.0f;
		Util::DrawFilledRect({ 0.0f ,0.0f }, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, glm::vec4(1, 1, 1, m_flashAlpha));
	}

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void Level3::Update()
{
	UpdateDisplayList();
	Pushing();

	if (m_pBoss->GetHealth() > 0 && !m_pBoss->GetPhaseTransition() && !m_pBoss->GetPhaseTwo())
		Spawning();
	else
	{
		if (m_pBoss->m_spawnBullet)
		{
			m_pBullets.push_back(new Bullet(m_pBoss->GetTransform()->position, -m_pBoss->GetTargetDirection(), m_pBoss->GetWeaponType(), false));
			AddChild(m_pBullets.back());
			m_pBullets.push_back(new Bullet(m_pBoss->GetTransform()->position, glm::vec2( 0.5f * m_pBoss->GetTargetDirection().x - 0.866f * m_pBoss->GetTargetDirection().y, 0.866 * m_pBoss->GetTargetDirection().x + 0.5 * m_pBoss->GetTargetDirection().y), m_pBoss->GetWeaponType(), false));
			AddChild(m_pBullets.back());
			m_pBullets.push_back(new Bullet(m_pBoss->GetTransform()->position, glm::vec2(0.5f * m_pBoss->GetTargetDirection().x + 0.866f * m_pBoss->GetTargetDirection().y, -0.866 * m_pBoss->GetTargetDirection().x + 0.5 * m_pBoss->GetTargetDirection().y), m_pBoss->GetWeaponType(), false));
			AddChild(m_pBullets.back());
		}
	}

	if (m_pBoss->GetAccelerateMap())
	{
		if (Util::Distance({ Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 4 }, m_pBoss->GetTransform()->position) > 1.5f)
			m_pBoss->GetTransform()->position += Util::Normalize(glm::vec2( Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 4 ) - m_pBoss->GetTransform()->position) * (m_accelerationCtr < 120 ? 0.25f : m_accelerationCtr > 660 ? 1.0f : 0.75f);

		if (Util::Distance({ Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 4 * 3 }, m_pPlayer->GetTransform()->position) > 1.5f)
			m_pPlayer->GetTransform()->position += Util::Normalize(glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 4 * 3) - m_pPlayer->GetTransform()->position) * (m_accelerationCtr < 120 ? 0.25f : m_accelerationCtr > 660 ? 1.0f : 0.75f);

		static float transition_speed = 0.6f;
		int modulus;
		float multiplier = 1.035f;

		if (m_accelerationCtr < 120)
			modulus = 2;
		else if (m_accelerationCtr < 300)
			modulus = 15;
		else if (m_accelerationCtr < 660)
		{
			modulus = 9;
			multiplier = 0.977f;
		}
		else if (m_accelerationCtr < 840)
			modulus = 3;
		else
			modulus = 15;

		if (++m_accelerationCtr % modulus == 0)
			transition_speed *= multiplier;

		if (m_accelerationCtr == 1155)
			SoundManager::Instance().PlaySound("transition");

		for (const auto background : m_pBackgrounds)
		{
			background->GetTransform()->position.x += transition_speed;
		}

		if(m_accelerationCtr % 15 == 0 && m_pPlayer->GetHealth() != m_pPlayer->GetMaxHealth())
		{
			m_pPlayer->SetHealth(m_pPlayer->GetHealth() + 1);
		}
	}

	if(m_pBackgrounds.size() == 4 && m_pBoss->GetPhaseTwo())
	{
		RemoveChild(m_pBackgrounds[0]);
		m_pBackgrounds.erase(m_pBackgrounds.begin());
		m_pBackgrounds.shrink_to_fit();
		RemoveChild(m_pBackgrounds[0]);
		m_pBackgrounds.erase(m_pBackgrounds.begin());
		m_pBackgrounds.shrink_to_fit();
		RemoveChild(m_pBackgrounds[0]);
		m_pBackgrounds.erase(m_pBackgrounds.begin());
		m_pBackgrounds.shrink_to_fit();
		RemoveChild(m_pBackgrounds[0]);
		m_pBackgrounds.erase(m_pBackgrounds.begin());
		m_pBackgrounds.shrink_to_fit();

		m_pBackgrounds.push_back(new Background("../Assets/backgrounds/default.jpg", "space", false));
		AddChild(m_pBackgrounds.back());
		m_pBackgrounds.back()->GetTransform()->position = glm::vec2(0, 0);
	}

	// Boss stuff
	if (m_killCtr >= m_killLimit)
		m_bossFight = true;

	if (m_bossFight)
	{
		if(!m_pBoss->GetPhaseTwo())
		{
			SoundManager::Instance().PlayMusic("BossFight");
		}

		m_pBoss->SetEnabled(true);
		m_pBoss->SetTargetPosition(m_pPlayer->GetTransform()->position);
		
		// When the final boss dies, kill all cavemen
		if (m_pBoss->GetHealth() <= 0)
		{
			for (const auto crossbow : m_pCrossBows)
			{
				crossbow->SetHealth(0);
			}
			for (const auto boomerang : m_pBoomerangs)
			{
				boomerang->SetHealth(0);
			}
		}

		if(m_pBoss->GetIsMagnet())
		{
			if(m_pBoss->GetPhaseTwo())
			{
				for (int i = 0; i < m_pBullets.size(); ++i)
				{
					if(i != 0)
					{
						m_pBullets[i]->GetTransform()->position += Util::Normalize(m_pBoss->GetTransform()->position - m_pBullets[i]->GetTransform()->position) * m_pBoss->GetMagnetStrength() * 1.5f;
					}
				}
			}
			m_pPlayer->GetTransform()->position += Util::Normalize(m_pBoss->GetTransform()->position - m_pPlayer->GetTransform()->position) * m_pBoss->GetMagnetStrength();
		}


		if (m_pPlayer->GetTransform()->position.x < 0 + (m_pPlayer->GetWidth() / 2))
		{
			m_pPlayer->GetTransform()->position.x = (m_pPlayer->GetWidth() / 2);
		}
		else if (m_pPlayer->GetTransform()->position.x > 1280 - (m_pPlayer->GetWidth() / 2))
		{
			m_pPlayer->GetTransform()->position.x = 1280 - (m_pPlayer->GetWidth() / 2);
		}
		if (m_pPlayer->GetTransform()->position.y < 0 + (m_pPlayer->GetHeight() / 2))
		{
			m_pPlayer->GetTransform()->position.y = (m_pPlayer->GetHeight() / 2);
		}
		else if (m_pPlayer->GetTransform()->position.y > 720 - (m_pPlayer->GetHeight() / 2))
		{
			m_pPlayer->GetTransform()->position.y = 720 - (m_pPlayer->GetHeight() / 2);
		}
	}



	// Background movement code
	if(!m_pBoss->GetPhaseTwo())
	{
		if (m_pBackgrounds[0]->GetTransform()->position.x < -m_pBackgrounds[0]->GetWidth() && m_pBackgrounds[1]->GetTransform()->position.x < 0.0f)
		{

			m_pBackgrounds[0]->GetTransform()->position = glm::vec2(0.0f, m_pBackgrounds[0]->GetTransform()->position.y);
			m_pBackgrounds[1]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetWidth(), m_pBackgrounds[1]->GetTransform()->position.y);
			m_pBackgrounds[2]->GetTransform()->position = glm::vec2(0.0f, m_pBackgrounds[2]->GetTransform()->position.y);
			m_pBackgrounds[3]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetWidth(), m_pBackgrounds[3]->GetTransform()->position.y);
		}
		if (m_pBackgrounds[0]->GetTransform()->position.x > 0.0f && m_pBackgrounds[1]->GetTransform()->position.x > m_pBackgrounds[0]->GetWidth())
		{

			m_pBackgrounds[0]->GetTransform()->position = glm::vec2(-m_pBackgrounds[0]->GetWidth(), m_pBackgrounds[0]->GetTransform()->position.y);
			m_pBackgrounds[1]->GetTransform()->position = glm::vec2(0.0f, m_pBackgrounds[1]->GetTransform()->position.y);
			m_pBackgrounds[2]->GetTransform()->position = glm::vec2(-m_pBackgrounds[0]->GetWidth(), m_pBackgrounds[2]->GetTransform()->position.y);
			m_pBackgrounds[3]->GetTransform()->position = glm::vec2(0.0f, m_pBackgrounds[3]->GetTransform()->position.y);
		}
		if (m_pBackgrounds[0]->GetTransform()->position.y < -m_pBackgrounds[0]->GetHeight() && m_pBackgrounds[2]->GetTransform()->position.y < 0.0f)
		{
			m_pBackgrounds[0]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetTransform()->position.x, 0.0f);
			m_pBackgrounds[1]->GetTransform()->position = glm::vec2(m_pBackgrounds[1]->GetTransform()->position.x, 0.0f);
			m_pBackgrounds[2]->GetTransform()->position = glm::vec2(m_pBackgrounds[2]->GetTransform()->position.x, m_pBackgrounds[0]->GetHeight());
			m_pBackgrounds[3]->GetTransform()->position = glm::vec2(m_pBackgrounds[3]->GetTransform()->position.x, m_pBackgrounds[0]->GetHeight());
		}
		if (m_pBackgrounds[0]->GetTransform()->position.y > 0.0f && m_pBackgrounds[2]->GetTransform()->position.y > m_pBackgrounds[0]->GetHeight())
		{
			m_pBackgrounds[0]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetTransform()->position.x, -m_pBackgrounds[0]->GetHeight());
			m_pBackgrounds[1]->GetTransform()->position = glm::vec2(m_pBackgrounds[1]->GetTransform()->position.x, -m_pBackgrounds[0]->GetHeight());
			m_pBackgrounds[2]->GetTransform()->position = glm::vec2(m_pBackgrounds[2]->GetTransform()->position.x, 0.0f);
			m_pBackgrounds[3]->GetTransform()->position = glm::vec2(m_pBackgrounds[3]->GetTransform()->position.x, 0.0f);
		}
	}

	m_timer3 += 1;

	// UI stuff
	m_pKillCounter->SetText(m_bossFight ? " " : "Kills until Boss: " + std::to_string(m_killLimit - m_killCtr));

	Collisions();
	Deletion();
}

void Level3::Clean()
{
	RemoveAllChildren();
}

void Level3::HandleEvents()
{
	EventManager::Instance().Update();

	const float m = m_pBoss->GetAccelerateMap() ? 0.0f : m_pPlayer->GetIsAttacking() ? 2.0f : 5.0f;

	glm::vec2 direction = GetPlayerInput();
	glm::vec2 multiplier{ m, m };

	if (!m_bossFight && m_pPlayer->GetHealth() > 0) // Update GameObject, Background and Player positions
	{
		for (int i = 0; i < m_pBackgrounds.size(); i++)
		{
			m_pBackgrounds[i]->GetTransform()->position += Util::Normalize(direction) * multiplier; // update background positions
		}
		for (const auto boomerang : m_pBoomerangs)
		{
			boomerang->GetTransform()->position += Util::Normalize(direction) * multiplier; // update boomerang positions
		}
		for (const auto crossbow : m_pCrossBows)
		{
			crossbow->GetTransform()->position += Util::Normalize(direction) * multiplier; // update X-bow positions
		}
		for (const auto bullet : m_pBullets)
		{
			bullet->GetTransform()->position += Util::Normalize(direction) * multiplier; // update Bullet positions
		}
	}
	else
	{
		m_pPlayer->GetTransform()->position -= Util::Normalize(direction) * multiplier; // If in boss fight, lock everything and move the player
	}

	GetKeyboardInput();
}

void Level3::Start()
{
	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	// Background
	for (int i = 0; i < 4; i++)
	{
		m_pBackgrounds.push_back(new Background("../Assets/backgrounds/level3.png", "default"));
		AddChild(m_pBackgrounds[i], 0);
	}

	m_pBackgrounds[0]->GetTransform()->position = glm::vec2(0.0f, 0.0f);
	m_pBackgrounds[1]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetWidth(), 0.0f);
	m_pBackgrounds[2]->GetTransform()->position = glm::vec2(0.0f, m_pBackgrounds[0]->GetHeight());
	m_pBackgrounds[3]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetWidth(), m_pBackgrounds[0]->GetHeight());

	// Player Sprite
	m_addGameObject(m_pPlayer, glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2), 1);

	// Making bullet vector always have 1
	m_pBullets.push_back(new Bullet());

	// UI Labels
	m_pKillCounter = new Label("Kills until Boss: " + std::to_string(m_killLimit - m_killCtr), "Dock51", 25, { 0, 0, 0, 255 });
	m_pKillCounter->GetTransform()->position = glm::vec2(640, 15);
	AddChild(m_pKillCounter, 10);

	// Boomerang testing
	//	m_pBoomerangs.push_back(new BoomerangEnemy());
	//	m_pBoomerangs.back()->GetTransform()->position = glm::vec2(500, 200);
	//	AddChild(m_pBoomerangs.back(), 2);

	//	// Crossbow testing
	//	m_pCrossBows.push_back(new CrossBowEnemy());
	//	m_pCrossBows.back()->GetTransform()->position = glm::vec2(500, 200);
	//	AddChild(m_pCrossBows.back(), 2);

	// Boss testing
	m_addGameObject(m_pBoss, glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 4), 1, false);
	//m_bossFight = true;

	stats->SetLastLevel("Future");

	// Background Music
	SoundManager::Instance().PlayMusic("Map5");

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void Level3::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Time-Locked Survivors Debug Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
	
	static bool godMode = m_pPlayer->GetIsGod();
	if (ImGui::Checkbox("God Mode", &godMode))
	{
		m_pPlayer->SetIsGod(godMode);
	}

	ImGui::Separator();

	static bool toggleDebug = m_debugView;
	if(ImGui::Checkbox("Debug View                                      ", &toggleDebug)) // So GUI has room
	{
		m_debugView = toggleDebug;
	}

	ImGui::Separator();

	static bool toggleBoss = m_bossFight;
	if (ImGui::Checkbox("Toggle Boss Fight", &toggleBoss))
	{
		m_bossFight = toggleBoss;
		SoundManager::Instance().PlayMusic("BossFight");
	}

	if(ImGui::Button("Damage Boss", {200.0f, 75.0f}))
	{
		m_pBoss->TakeDamage(20);
	}
	
	ImGui::End();
}

void Level3::Collisions() const
{
	for (int i = 0; i < m_pBullets.size(); ++i)
	{
		if(m_pBullets[i]->GetIsPlayerOwned())
		{
			for (const auto crossbow : m_pCrossBows)
			{
				if (CollisionManager::AABBCheck(m_pBullets[i], crossbow))
				{
					if (i != 0)
					{
						if (crossbow->GetHitDelayCtr() % crossbow->GetHitDelayMax() == crossbow->GetHitDelayMax() - 1)
							stats->AddDamage(3);
						crossbow->TakeDamage(3);
					}
				}
			}
			for (const auto boomerang : m_pBoomerangs)
			{
				if (CollisionManager::AABBCheck(m_pBullets[i], boomerang))
				{
					if (i != 0)
					{
						if (boomerang->GetHitDelayCtr() % boomerang->GetHitDelayMax() == boomerang->GetHitDelayMax() - 1)
							stats->AddDamage(3);
						boomerang->TakeDamage(3);
					}
				}
			}
			if (!m_pBoss->GetShielding() && m_pBoss->IsEnabled() && !m_pBoss->GetPhaseTransition() && m_pBoss->GetHealth() > 0)
			{
				if (CollisionManager::AABBCheck(m_pBullets[i], m_pBoss))
				{
					if (i != 0)
					{
						if (m_pBoss->GetHitDelayCtr() % m_pBoss->GetHitDelayMax() == m_pBoss->GetHitDelayMax() - 1)
							stats->AddDamage(3);
						m_pBoss->TakeDamage(3);
					}
				}
			}
		}
		else
		{
			if (CollisionManager::AABBCheck(m_pBullets[i], m_pPlayer))
			{
				if(m_pBullets[i]->GetWeaponType() == WeaponType::LASER)
					m_pBullets[i]->m_duration = 0;

				if (m_pPlayer->GetHitDelayCtr() <= 0 && i != 0 && m_pPlayer->GetHealth() > 0)
				{
					m_pPlayer->ResetHitDelayCtr();
					if (m_pBullets[i]->GetWeaponType() == WeaponType::LASER)
						m_pPlayer->TakeDamage(4);
					else
						m_pPlayer->TakeDamage(6);
				}
			}
		}
	}

	for (const auto boomerang : m_pBoomerangs)
	{
		if(boomerang->GetHealth() > 0)
		{
			if (CollisionManager::AABBCheck(boomerang->GetHitBox(), m_pPlayer))
			{
				if (m_pPlayer->GetHitDelayCtr() <= 0 && m_pPlayer->GetHealth() > 0)
				{
					m_pPlayer->ResetHitDelayCtr();
					m_pPlayer->TakeDamage(6);
				}
			}
		}
	}
	if (m_bossFight && m_pBoss->GetHealth() > 0)
	{
		if (CollisionManager::AABBCheck(m_pBoss->GetHitBox(), m_pPlayer))
		{
			if (m_pPlayer->GetHitDelayCtr() <= 0 && m_pPlayer->GetHealth() > 0)
			{
				m_pPlayer->ResetHitDelayCtr();
				m_pBoss->GetPhaseTwo() ? m_pPlayer->TakeDamage(13) : m_pPlayer->TakeDamage(7);
			}
		}
	}
}

void Level3::Deletion()
{
	// Bullets
	for (int i = 0; i < m_pBullets.size(); ++i)
	{
		if (m_pBoss->GetHealth() <= 0)
		{
			if (m_pBullets[i]->GetWeaponType() == WeaponType::LASER || m_pBullets[i]->GetWeaponType() == WeaponType::FINAL_BOSS)
				m_pBullets[i]->m_duration = 0;
		}

		if (m_pBullets[i]->GetDuration() == 0)
		{
			RemoveChild(m_pBullets[i]);
			m_pBullets.erase(m_pBullets.begin() + i);
			m_pBullets.shrink_to_fit();
		}
	}

	// Crossbows
	for (int i = 0; i < m_pCrossBows.size(); ++i)
	{
		if (m_pCrossBows[i]->GetDeleteMe())
		{
			if (!m_pBoss->GetDeleteMe())
				m_killCtr++;
			RemoveChild(m_pCrossBows[i]);
			m_pCrossBows.erase(m_pCrossBows.begin() + i);
			m_pCrossBows.shrink_to_fit();
		}
	}

	// Boomerangs
	for (int i = 0; i < m_pBoomerangs.size(); ++i)
	{
		if (m_pBoomerangs[i]->GetDeleteMe())
		{
			if (!m_pBoss->GetDeleteMe())
				m_killCtr++;
			RemoveChild(m_pBoomerangs[i]);
			m_pBoomerangs.erase(m_pBoomerangs.begin() + i);
			m_pBoomerangs.shrink_to_fit();
		}
	}

	// Player
	if (m_pPlayer->GetHealth() <= 0)
	{
		m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_DEATH);
		if (m_pPlayer->GetAnimation("death").current_frame == m_pPlayer->GetAnimation("death").frames.size() - 1)
		{
			stats->SetKills(m_killCtr);
			SoundManager::Instance().StopMusic(0);
			Game::Instance().ChangeSceneState(SceneState::END);
			return;
		}
	}

	// Boss
	if (m_pBoss->GetDeleteMe())
	{
		m_killCtr++;
		stats->SetKills(m_killCtr);
		m_bossFight = false;
		RemoveChild(m_pBoss);
		stats->SetTimePlayed(m_timer3 / 60);
		Game::Instance().ChangeSceneState(SceneState::RESULTS);
	}
}

void Level3::Spawning()
{
	int crossbowSpawn = 300; // 5s
	int boomerangSpawn = 210; // 3.5s
	if(m_bossFight)
	{
		crossbowSpawn = crossbowSpawn * 2 + crossbowSpawn / 2; // 2.5
		boomerangSpawn = boomerangSpawn * 2 + boomerangSpawn / 2; // 2.5
	}
	if (m_pCrossBows.size() <= 5) 
	{
		if (++m_enemySpawnCounter % crossbowSpawn == 0)
		{
			m_pCrossBows.push_back(new CrossBowEnemy());
			AddChild(m_pCrossBows.back());
		}
	}
	if (m_pBoomerangs.size() <= 9) 
	{
		if (m_enemySpawnCounter % boomerangSpawn == 0)
		{
			m_pBoomerangs.push_back(new BoomerangEnemy());
			AddChild(m_pBoomerangs.back());
		}
	}
	if (m_enemySpawnCounter == crossbowSpawn * boomerangSpawn)
		m_enemySpawnCounter = 1;

	for (const auto crossbow : m_pCrossBows)
	{
		crossbow->SetTargetPosition(m_pPlayer->GetTransform()->position);
		if(crossbow->m_spawnBullet)
		{
			m_pBullets.push_back(new Bullet(crossbow->GetTransform()->position, crossbow->GetTargetDirection(), crossbow->GetWeaponType(), false));
			AddChild(m_pBullets.back());
		}
	}
	for (const auto boomerang : m_pBoomerangs)
	{
		boomerang->SetTargetPosition(m_pPlayer->GetTransform()->position);
	}
}

void Level3::Pushing() const
{
	glm::vec2 mtv;
	for (int i = 0; i < m_pCrossBows.size(); ++i)
	{
		for (int ii = 0; ii < m_pCrossBows.size(); ++ii)
		{
			if (ii != i)
				CollisionManager::CircleCollisionPush(m_pCrossBows[i], m_pCrossBows[ii], mtv);
		}
		for (int jj = 0; jj < m_pBoomerangs.size(); ++jj)
		{
			if (jj != i)
				CollisionManager::CircleCollisionPush(m_pCrossBows[i], m_pBoomerangs[jj], mtv);
		}
		CollisionManager::CircleCollisionPush(m_pPlayer, m_pCrossBows[i], mtv, true);
	}

	for (int i = 0; i < m_pBoomerangs.size(); ++i)
	{
		for (int ii = 0; ii < m_pBoomerangs.size(); ++ii)
		{
			if (ii != i)
				CollisionManager::CircleCollisionPush(m_pBoomerangs[i], m_pBoomerangs[ii], mtv);
		}
		for (int jj = 0; jj < m_pCrossBows.size(); ++jj)
		{
			if (jj != i)
				CollisionManager::CircleCollisionPush(m_pBoomerangs[i], m_pCrossBows[jj], mtv);
		}

		CollisionManager::CircleCollisionPush(m_pPlayer, m_pBoomerangs[i], mtv, true);
	}

	if (m_pBoss->IsEnabled())
	{
		for (int ii = 0; ii < m_pCrossBows.size(); ++ii)
		{
			CollisionManager::CircleCollisionPush(m_pBoss, m_pCrossBows[ii], mtv, true);
		}
		for (int jj = 0; jj < m_pBoomerangs.size(); ++jj)
		{
			CollisionManager::CircleCollisionPush(m_pBoss, m_pBoomerangs[jj], mtv, true);
		}
		CollisionManager::CircleCollisionPush(m_pBoss, m_pPlayer, mtv, true);
	}
}


glm::vec2 Level3::GetPlayerInput()
{
	float x = 0.0f;
	float y = 0.0f;
	static float xOld = 0.0f;
	static float yOld = 0.0f;
	switch (m_pCurrentInputType)
	{
	case static_cast<int>(InputType::KEYBOARD_MOUSE):

		if (EventManager::Instance().KeyPressed(SDL_SCANCODE_P) && !m_pBoss->GetPhaseTransition())
		{
			Game::Instance().TogglePause();
		}

		if (m_pPlayer->GetHealth() > 0)
		{
			if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
			{
				x = 1.0f;
				yOld = 0.0f;
				xOld = x;
			}
			else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
			{
				x = -1.0f;
				yOld = 0.0f;
				xOld = x;
			}
			if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_W))
			{
				y = 1.0f;
				xOld = 0.0f;
				yOld = y;
			}
			else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_S))
			{
				y = -1.0f;
				xOld = 0.0f;
				yOld = y;
			}

			if (!m_pPlayer->GetIsAttacking()) // Player movement direction input
			{
				if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_WALK_LEFT);
				}
				else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_WALK_RIGHT);
				}
				if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_W))
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_WALK_UP);
				}
				else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_S))
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_WALK_DOWN);
				}
				if (x == 0.f && y == 0.0f)
				{
					switch (static_cast<int>(xOld))
					{
					case 1:
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
						break;
					case -1:
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
						break;
					}
					switch (static_cast<int>(yOld))
					{
					case 1:
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_UP);
						break;
					case -1:
						m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_DOWN);
						break;
					}

				}
			}
			else // wait for player attack animation to finish then set IsAttacking to false
			{
				if (m_pPlayer->GetAnimation("attack_up").current_frame == m_pPlayer->GetAnimation("attack_up").frames.size() - 1)
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_WALK_UP);
					m_pPlayer->GetAnimation("attack_up").current_frame = 0;
					m_pPlayer->SetIsAttacking(false);
				}
				if (m_pPlayer->GetAnimation("attack_left").current_frame == m_pPlayer->GetAnimation("attack_left").frames.size() - 1)
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_WALK_LEFT);
					m_pPlayer->GetAnimation("attack_left").current_frame = 0;
					m_pPlayer->SetIsAttacking(false);
				}
				if (m_pPlayer->GetAnimation("attack_down").current_frame == m_pPlayer->GetAnimation("attack_down").frames.size() - 1)
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_WALK_DOWN);
					m_pPlayer->GetAnimation("attack_down").current_frame = 0;
					m_pPlayer->SetIsAttacking(false);
				}
				if (m_pPlayer->GetAnimation("attack_right").current_frame == m_pPlayer->GetAnimation("attack_right").frames.size() - 1)
				{
					m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_WALK_RIGHT);
					m_pPlayer->GetAnimation("attack_right").current_frame = 0;
					m_pPlayer->SetIsAttacking(false);
				}
			}
		}

		if (EventManager::Instance().KeyPressed(SDL_SCANCODE_SPACE) && !m_pBoss->GetPhaseTransition()) // attack based on direction
		{
			if (m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_WALK_UP || m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_IDLE_UP)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_UP);
				m_pPlayer->SetIsAttacking(true);
				m_pBullets.push_back(new Bullet(m_pPlayer->GetTransform()->position, -glm::vec2(0.0f, 1.0f), WeaponType::SPEAR, true));
				AddChild(m_pBullets[m_pBullets.size() - 1], 5);
			}
			else if (m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_WALK_LEFT || m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_IDLE_LEFT)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_LEFT);
				m_pPlayer->SetIsAttacking(true);
				m_pBullets.push_back(new Bullet(m_pPlayer->GetTransform()->position, -glm::vec2(1.0f, 0.0f), WeaponType::SPEAR, true));
				AddChild(m_pBullets[m_pBullets.size() - 1], 5);
			}
			else if (m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_WALK_DOWN || m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_IDLE_DOWN)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_DOWN);
				m_pPlayer->SetIsAttacking(true);
				m_pBullets.push_back(new Bullet(m_pPlayer->GetTransform()->position, -glm::vec2(0.0f, -1.0f), WeaponType::SPEAR, true));
				AddChild(m_pBullets[m_pBullets.size() - 1], 5);
			}
			else if (m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_WALK_RIGHT || m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_IDLE_RIGHT)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_RIGHT);
				m_pPlayer->SetIsAttacking(true);
				m_pBullets.push_back(new Bullet(m_pPlayer->GetTransform()->position, -glm::vec2(-1.0f, 0.0f), WeaponType::SPEAR, true));
				AddChild(m_pBullets[m_pBullets.size() - 1], 5);
			}
		}
		break;
	}

	return glm::vec2(x, y);
}

void Level3::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		SoundManager::Instance().StopMusic(0);
		Game::Instance().Quit();
	}
}

template <typename T>
void Level3::m_addGameObject(T*& object, glm::vec2 position, int layer, bool enabled)
{
	// Add the T Type object to the Scene
	if(typeid(object) != typeid(CaveMan*)) // To prevent double creation.. Assuming it matters
	{
		object = new T(); // instantiate an object of type given
		//std::cout << "object name?: " << typeid(object).name() << std::endl; 
	}
	object->GetTransform()->position = position;
	object->SetEnabled(enabled);
	AddChild(object, layer);
}