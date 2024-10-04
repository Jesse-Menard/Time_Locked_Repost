#include "Level2.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"
#include "SoundManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

Level2::Level2() : m_debugView(false), m_bossFight(false), m_killCtr(0), m_killLimit(14), m_enemySpawnCounter(180)
{
	Level2::Start();
}

Level2::~Level2()
= default;

void Level2::Draw()
{
	DrawDisplayList();

	// Boss HP Bar
	if(m_bossFight)
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
			Util::DrawFilledRect(glm::vec2(50, 20), (Config::SCREEN_WIDTH - 100) * m_pBoss->GetHealth() / 40, 25, display_colour);
			if (m_pBoss->GetShielding())
				Util::DrawFilledRect(glm::vec2(50, 20), (Config::SCREEN_WIDTH - 100) * m_pBoss->GetHealth() / 40, 25, glm::vec4(0.25, 1, 0.75, 1));
		}
		Util::DrawRect(glm::vec2(50, 20), (Config::SCREEN_WIDTH - 100), 25, glm::vec4(0, 0, 0, 1));
	}

	//	// Player HP bar
	//	Util::DrawFilledRect(glm::vec2((Config::SCREEN_WIDTH / 2) - m_pPlayer->GetHealth(), Config::SCREEN_HEIGHT - 40), m_pPlayer->GetHealth() * 2, 40, glm::vec4(0, 1, 0, 1));Util::DrawFilledRect(glm::vec2((Config::SCREEN_WIDTH / 2) - m_pPlayer->GetHealth(), Config::SCREEN_HEIGHT - 40), m_pPlayer->GetHealth() * 2, 40, glm::vec4(0, 1, 0, 1));
	//	Util::DrawRect(glm::vec2((Config::SCREEN_WIDTH / 2) - 100, Config::SCREEN_HEIGHT - 40), 200, 40, glm::vec4(0, 0, 0, 1));

	if (m_debugView)
	{
		if (m_bossFight)
		{
			Util::DrawRect(m_pBoss->GetTransform()->position - glm::vec2(m_pBoss->GetWidth() * 0.5, m_pBoss->GetHeight() * 0.5), m_pBoss->GetWidth(), m_pBoss->GetHeight());
			m_pBoss->DrawHitBox();
		}

		Util::DrawRect(m_pPlayer->GetTransform()->position - glm::vec2(m_pPlayer->GetWidth() * 0.5, m_pPlayer->GetHeight() * 0.5), m_pPlayer->GetWidth(), m_pPlayer->GetHeight());

		for (const auto archer : m_pArchers)
		{
			Util::DrawRect(archer->GetTransform()->position - glm::vec2(archer->GetWidth() * 0.5, archer->GetHeight() * 0.5), archer->GetWidth(), archer->GetHeight());
		}
		for (const auto summoner : m_pSummoners)
		{
			Util::DrawRect(summoner->GetTransform()->position - glm::vec2(summoner->GetWidth() * 0.5, summoner->GetHeight() * 0.5), summoner->GetWidth(), summoner->GetHeight());
		}
		for (const auto skull : m_pSkulls)
		{
			Util::DrawRect(skull->GetTransform()->position - glm::vec2(skull->GetWidth() * 0.5, skull->GetHeight() * 0.5), skull->GetWidth(), skull->GetHeight(), glm::vec4(1, 0, 0, 1));
		}
		for (const auto bullet : m_pBullets)
		{
			Util::DrawRect(bullet->GetTransform()->position - glm::vec2(bullet->GetWidth() * 0.5, bullet->GetHeight() * 0.5), bullet->GetWidth(), bullet->GetHeight(), glm::vec4(1, 0, 0, 1));
		}
	}

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void Level2::Update()
{
	UpdateDisplayList();

	glm::vec2 mtv;
	for (int i = 0; i < m_pSummoners.size(); ++i)
	{
		for (int ii = 0; ii < m_pSummoners.size(); ++ii)
		{
			if (ii != i)
				CollisionManager::CircleCollisionPush(m_pSummoners[i], m_pSummoners[ii], mtv);
		}
		for (int jj = 0; jj < m_pArchers.size(); ++jj)
		{
			if(jj != i)
				CollisionManager::CircleCollisionPush(m_pSummoners[i], m_pArchers[jj], mtv);
		}
		for (int ii = 0; ii < m_pSkulls.size(); ++ii)
		{
			if (ii != i)
				CollisionManager::CircleCollisionPush(m_pSummoners[i], m_pSkulls[ii], mtv, true);
		}

		CollisionManager::CircleCollisionPush(m_pPlayer, m_pSummoners[i], mtv, true);
	}

	for (int i = 0; i < m_pArchers.size(); ++i)
	{
		for (int ii = 0; ii < m_pSummoners.size(); ++ii)
		{
			if (ii != i)
				CollisionManager::CircleCollisionPush(m_pArchers[i], m_pSummoners[ii], mtv);
		}
		for (int jj = 0; jj < m_pArchers.size(); ++jj)
		{
			if (jj != i)
				CollisionManager::CircleCollisionPush(m_pArchers[i], m_pArchers[jj], mtv);
		}
		for (int ii = 0; ii < m_pSkulls.size(); ++ii)
		{
			if (ii != i)
				CollisionManager::CircleCollisionPush(m_pArchers[i], m_pSkulls[ii], mtv, true);
		}

		CollisionManager::CircleCollisionPush(m_pPlayer, m_pArchers[i], mtv, true);
	}

	for (int i = 0; i < m_pSkulls.size(); ++i)
	{
		for (int ii = 0; ii < m_pSkulls.size(); ++ii)
		{
			if (ii != i)
				CollisionManager::CircleCollisionPush(m_pSkulls[i], m_pSkulls[ii], mtv);
		}

		CollisionManager::CircleCollisionPush(m_pPlayer, m_pSkulls[i], mtv, true);
	}

	if(m_pBoss->IsEnabled())
	{
		for (int ii = 0; ii < m_pSummoners.size(); ++ii)
		{
			CollisionManager::CircleCollisionPush(m_pBoss, m_pSummoners[ii], mtv, true);
		}
		for (int jj = 0; jj < m_pArchers.size(); ++jj)
		{
			CollisionManager::CircleCollisionPush(m_pBoss, m_pArchers[jj], mtv, true);
		}
		for (int ii = 0; ii < m_pSkulls.size(); ++ii)
		{
			CollisionManager::CircleCollisionPush(m_pBoss, m_pSkulls[ii], mtv, true);
		}
		CollisionManager::CircleCollisionPush(m_pBoss, m_pPlayer, mtv, true);
	}

	if(m_pBoss->GetHealth() > 0)
		Spawning();

	// Boss stuff
	if (m_killCtr >= m_killLimit)
		m_bossFight = true;

	if(m_bossFight)
	{
		SoundManager::Instance().PlayMusic("BossFight");

		m_pBoss->SetEnabled(true);
		m_pBoss->SetTargetPosition(m_pPlayer->GetTransform()->position);

		// When the final boss dies, kill all cavemen
		if (m_pBoss->GetHealth() <= 0)
		{
			for (const auto archer : m_pArchers)
			{
				archer->SetHealth(0);
			}
			for (const auto summoner : m_pSummoners)
			{
				summoner->SetHealth(0);
			}
			for (const auto skull : m_pSkulls)
			{
				skull->SetHealth(0);
			}
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

	// Hostile seeking bullets
	for (auto bullet : m_pBullets)
	{
		if(bullet->isSeeking() && !bullet->GetIsPlayerOwned())
		{
			bullet->SetTargetPosition(m_pPlayer->GetTransform()->position);
		}
	}

	// Background movement code
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

	m_timer2 += 1;

	// UI stuff
	m_pKillCounter->SetText(m_bossFight ? " " : "Kills until Boss: " + std::to_string(m_killLimit - m_killCtr));

	//m_pTimeCounter->SetText("Time (seconds): " + std::to_string(m_timer2 / 60));

	Collisions();
	Deletion();
}

void Level2::Clean()
{
	RemoveAllChildren();
}

void Level2::HandleEvents()
{
	EventManager::Instance().Update();

	const float m = m_pPlayer->GetIsAttacking() ? 2.0f : 5.0f;

	glm::vec2 direction = GetPlayerInput();
	glm::vec2 multiplier{ m, m };

	if (!m_bossFight && m_pPlayer->GetHealth() > 0) // Update GameObject, Background and Player positions
	{
		for (int i = 0; i < m_pBackgrounds.size(); i++)
		{
			m_pBackgrounds[i]->GetTransform()->position += Util::Normalize(direction) * multiplier; // update background positions
		}
		for (int i = 0; i < m_pArchers.size(); i++)
		{
			m_pArchers[i]->GetTransform()->position += Util::Normalize(direction) * multiplier; // update archer positions
		}
		for (int i = 0; i < m_pBullets.size(); i++)
		{
			m_pBullets[i]->GetTransform()->position += Util::Normalize(direction) * multiplier; // update bullet positions
		}
		for (const auto summoner : m_pSummoners)
		{
			summoner->GetTransform()->position += Util::Normalize(direction) * multiplier; // update summoner positions
		}
		for (auto skull : m_pSkulls)
		{
			skull->GetTransform()->position += Util::Normalize(direction) * multiplier; // update skull positions
		}
		for (auto particle : m_pParticles)
		{
			particle->GetTransform()->position += Util::Normalize(direction) * multiplier; // update particle positions
		}
	}
	else
	{
		m_pPlayer->GetTransform()->position -= Util::Normalize(direction) * multiplier; // If in boss fight, lock everything and move the player
	}

	GetKeyboardInput();
}

void Level2::Start()
{
	m_timer2 = 0;

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	// Background
	for (int i = 0; i < m_pBackgrounds.size(); i++)
	{
		m_pBackgrounds[i] = new Background("../Assets/backgrounds/level2.png", "default");
		AddChild(m_pBackgrounds[i], 0);
	}

	m_pBackgrounds[0]->GetTransform()->position = glm::vec2(0.0f, 0.0f);
	m_pBackgrounds[1]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetWidth(), 0.0f);
	m_pBackgrounds[2]->GetTransform()->position = glm::vec2(0.0f, m_pBackgrounds[0]->GetHeight());
	m_pBackgrounds[3]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetWidth(), m_pBackgrounds[0]->GetHeight());

	// Player Sprite
	m_addGameObject(m_pPlayer, glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2), 2);

	// Boss
	m_addGameObject(m_pBoss, glm::vec2(Config::SCREEN_WIDTH * 0.5, 200), 3, false);

	// Making bullet vector always have 1
	m_pBullets.push_back(new Bullet());

	//	// Player Label
	//	m_pPlayerHP = new Label("HEALTH", "Consolas");
	//	m_pPlayerHP->GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 660.0f);
	//	AddChild(m_pPlayerHP, 10);

	// UI Labels
	m_pKillCounter = new Label("Kills until Boss: " + std::to_string(m_killLimit - m_killCtr), "Dock51", 25, { 0, 0, 0, 255 });
	m_pKillCounter->GetTransform()->position = glm::vec2(640, 15);
	AddChild(m_pKillCounter, 10);

	//	m_pTimeCounter = new Label("Time (seconds): " + std::to_string(m_timer2 / 60), "Dock51", 25, { 0, 0, 255, 255 });
	//	m_pTimeCounter->GetTransform()->position = glm::vec2(640, 695);
	//	AddChild(m_pTimeCounter, 10);

	//	m_pTimer = new Label("Time: " + std::to_string(), "Consolas");
	//	m_pKillCounter->GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH - 50, 25);
	//	AddChild(m_pKillCounter);

	//	//	Summoner Testing
	//	m_pSummoners.push_back(new Summoner());
	//	m_pSummoners.back()->GetTransform()->position = glm::vec2(500, 200);
	//	AddChild(m_pSummoners.back());

	// Stats
	stats->SetLastLevel("Medieval");
	
	// Background Music
	SoundManager::Instance().PlayMusic("Map2");

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void Level2::GUI_Function()
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
	
	ImGui::End();
}

void Level2::Collisions()
{
	for (auto skull : m_pSkulls)
	{
		if(skull->GetHealth() > 0)
		{
			if(CollisionManager::AABBCheck(skull, m_pPlayer))
			{
				// Nested if because CollisionManager is lame
				if (m_pPlayer->GetHitDelayCtr() <= 0 && m_pPlayer->GetHealth() > 0)
				{
					m_pPlayer->ResetHitDelayCtr();
					m_pPlayer->TakeDamage(2);
				}
			}
		}
	}

	for (int i = 0; i < m_pBullets.size(); ++i)
	{
		// bug: if i = 0, CollisionManager doesn't update..
		// Hostile projectiles
		if (CollisionManager::AABBCheck(m_pBullets[i], m_pPlayer))
		{
			if (m_pBullets[i]->GetWeaponType() == WeaponType::ARROW)
				m_pBullets[i]->m_duration = 0;
			// Nested if because CollisionManager is lame
			if (m_pPlayer->GetHitDelayCtr() <= 0 && i != 0 && m_pPlayer->GetHealth() > 0 && !m_pBullets[i]->GetIsPlayerOwned())
			{
				m_pPlayer->ResetHitDelayCtr();
				if (m_pBullets[i]->GetWeaponType() == WeaponType::ARROW)
					m_pPlayer->TakeDamage(5);
				else
					m_pPlayer->TakeDamage(4);
			}
		}

		// Friendly projectiles
		if (m_pBullets[i]->GetIsPlayerOwned())
		{
			for (int j = 0; j < m_pArchers.size(); j++)
			{
				if (CollisionManager::AABBCheck(m_pBullets[i], m_pArchers[j]))
				{
					if (i != 0)
					{
						if(m_pArchers[j]->GetHitDelayCtr() % m_pArchers[j]->GetHitDelayMax() == m_pArchers[j]->GetHitDelayMax() - 1)
							stats->AddDamage(3);
						m_pArchers[j]->TakeDamage(3);
					}
				}
			}
			for (int j = 0; j < m_pSummoners.size(); j++)
			{
				if (CollisionManager::AABBCheck(m_pBullets[i], m_pSummoners[j]))
				{
					if (i != 0)
					{
						if (m_pSummoners[j]->GetHitDelayCtr() % m_pSummoners[j]->GetHitDelayMax() == m_pSummoners[j]->GetHitDelayMax() - 1)
							stats->AddDamage(3);
						m_pSummoners[j]->TakeDamage(3);
					}
				}
			}
			for (int j = 0; j < m_pSkulls.size(); j++)
			{
				if (CollisionManager::AABBCheck(m_pBullets[i], m_pSkulls[j]))
				{
					if (i != 0)
					{
						if (m_pSkulls[j]->GetHitDelayCtr() % m_pSkulls[j]->GetHitDelayMax() == m_pSkulls[j]->GetHitDelayMax() - 1)
							stats->AddDamage(3);
						m_pSkulls[j]->TakeDamage(3);
					}
				}
			}
			if(!m_pBoss->GetShielding() && m_pBoss->IsEnabled())
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
	}

	if (m_bossFight && m_pBoss->GetHealth() > 0)
	{
		if (CollisionManager::AABBCheck(m_pBoss->GetHitBox(), m_pPlayer))
		{
			if (m_pPlayer->GetHitDelayCtr() <= 0 && m_pPlayer->GetHealth() > 0)
			{
				m_pPlayer->ResetHitDelayCtr();
				m_pPlayer->TakeDamage(7);
			}
		}
	}
}

void Level2::Deletion()
{
	// Bullets
	for (int i = 0; i < m_pBullets.size(); ++i)
	{
		if(m_pBoss->GetHealth() <= 0)
		{
			if (m_pBullets[i]->GetWeaponType() == WeaponType::ARROW || m_pBullets[i]->GetWeaponType() == WeaponType::MAGIC_MISSILE)
				m_pBullets[i]->m_duration = 0;
		}

		if (m_pBullets[i]->GetDuration() == 0)
		{
			RemoveChild(m_pBullets[i]);
			m_pBullets.erase(m_pBullets.begin() + i);
			m_pBullets.shrink_to_fit();
		}
	}

	// Archers
	for (int i = 0; i < m_pArchers.size(); i++)
	{
		if (m_pArchers[i]->GetDeleteMe())
		{
			if (!m_pBoss->GetDeleteMe())
				m_killCtr++;
			RemoveChild(m_pArchers[i]);
			m_pArchers.erase(m_pArchers.begin() + i);
			m_pArchers.shrink_to_fit();
		}
	}

	// Summoners
	for (int i = 0; i < m_pSummoners.size(); i++)
	{
		if (m_pSummoners[i]->GetDeleteMe())
		{
			if (!m_pBoss->GetDeleteMe())
				m_killCtr++;
			RemoveChild(m_pSummoners[i]);
			m_pSummoners.erase(m_pSummoners.begin() + i);
			m_pSummoners.shrink_to_fit();
		}
	}

	// Particles
	for (int i = 0; i < m_pParticles.size(); ++i)
	{
		if(m_pParticles[i]->m_duration <= 0)
		{
			RemoveChild(m_pParticles[i]);
			m_pParticles.erase(m_pParticles.begin() + i);
			m_pParticles.shrink_to_fit();
		}
	}

	// Skulls
	for (int i = 0; i < m_pSkulls.size(); i++)
	{
		if (m_pSkulls[i]->GetDeleteMe())
		{
			RemoveChild(m_pSkulls[i]);
			m_pSkulls.erase(m_pSkulls.begin() + i);
			m_pSkulls.shrink_to_fit();
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
		stats->SetTimePlayed(m_timer2 / 60);
		Game::Instance().ChangeSceneState(SceneState::RESULTS);
	}
}

void Level2::Spawning()
{
	int archerSpawn = 270; // every 4.5s
	int SummonerSpawn = 390; // every 6.5s
	if(m_bossFight)
	{
		archerSpawn = archerSpawn * 4; // 
		SummonerSpawn = SummonerSpawn * 4; // 
	}
	if (m_pArchers.size() <= 7) 
	{
		if (m_enemySpawnCounter++ % archerSpawn == 0)
		{
			m_pArchers.push_back(new Archer());
			m_addGameObject(m_pArchers[m_pArchers.size() - 1], glm::vec2(0, 0), 2);//glm::vec2(rand() % Config::SCREEN_WIDTH, rand() % Config::SCREEN_HEIGHT), 2);
		}
	}
	if (m_pSummoners.size() <= 4) 
	{
		if (m_enemySpawnCounter % SummonerSpawn == 0)
		{
			m_pSummoners.push_back(new Summoner());
			m_addGameObject(m_pSummoners[m_pSummoners.size() - 1], glm::vec2(rand() % Config::SCREEN_WIDTH, rand() % Config::SCREEN_HEIGHT), 2);
		}
	}
	// Bullet spawning for Enemies
	for (int i = 0; i < m_pArchers.size(); i++)
	{
		m_pArchers[i]->SetTargetPosition(m_pPlayer->GetTransform()->position);
		if (m_pArchers[i]->m_spawnBullet)
		{
			m_pBullets.push_back(new Bullet(m_pArchers[i]->GetTransform()->position, m_pArchers[i]->GetTargetDirection(), m_pArchers[i]->GetWeaponType(), false));
			AddChild(m_pBullets[m_pBullets.size() - 1], 5);
		}
	}
	for (int i = 0; i < m_pSummoners.size(); i++)
	{
		m_pSummoners[i]->SetTargetPosition(m_pPlayer->GetTransform()->position);
		if (m_pSummoners[i]->m_spawnBullet)
		{
			m_pBullets.push_back(new Bullet(m_pSummoners[i]->GetTransform()->position, m_pSummoners[i]->GetTargetDirection(), m_pSummoners[i]->GetWeaponType(), false));
			AddChild(m_pBullets[m_pBullets.size() - 1], 5);
		}
		if (m_pSummoners[i]->m_summon && m_pSkulls.size() <= 8)
		{
			m_pSkulls.push_back(new Skull());
			m_pSkulls[m_pSkulls.size() - 1]->GetTransform()->position = glm::vec2(m_pSummoners[i]->GetTransform()->position.x - 40, m_pSummoners[i]->GetTransform()->position.y);
			AddChild(m_pSkulls[m_pSkulls.size() - 1], 1);
			m_pSkulls.push_back(new Skull());
			m_pSkulls[m_pSkulls.size() - 1]->GetTransform()->position = glm::vec2(m_pSummoners[i]->GetTransform()->position.x + 40, m_pSummoners[i]->GetTransform()->position.y);
			AddChild(m_pSkulls[m_pSkulls.size() - 1], 1);
		}
	}
	for (auto skull : m_pSkulls)
	{
		skull->SetTargetPosition(m_pPlayer->GetTransform()->position);
	}

	for (auto bullet : m_pBullets)
	{
		if (bullet->GetWeaponType() == WeaponType::MAGIC_MISSILE && bullet->GetParticleCtr() % 2 == 0)
		{
			glm::vec2 particle_position(bullet->GetTransform()->position.x - 7 + static_cast<float>(rand() % 14), bullet->GetTransform()->position.y - 7 + static_cast<float>(rand() % 14));
			m_pParticles.push_back(new Particle(glm::vec2(bullet->GetTransform()->position.x - 7 + static_cast<float>(rand() % 14), bullet->GetTransform()->position.y - 7 + static_cast<float>(rand() % 14)), 30));
			AddChild(m_pParticles[m_pParticles.size() - 1], 1);
		}
	}
}


glm::vec2 Level2::GetPlayerInput()
{
	float x = 0.0f;
	float y = 0.0f;
	static float xOld = 0.0f;
	static float yOld = 0.0f;
	switch (m_pCurrentInputType)
	{
	case static_cast<int>(InputType::KEYBOARD_MOUSE):

		if (EventManager::Instance().KeyPressed(SDL_SCANCODE_P))
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

		if (EventManager::Instance().KeyPressed(SDL_SCANCODE_SPACE)) // attack based on direction
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

void Level2::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		SoundManager::Instance().StopMusic(0);
		Game::Instance().Quit();
	}
}

template <typename T>
void Level2::m_addGameObject(T*& object, glm::vec2 position, int layer, bool enabled)
{
	// Add the T Type object to the Scene
	if(typeid(object) != typeid(Archer*) && typeid(object) != typeid(Summoner*)) // To prevent double creation.. Assuming it matters
	{
		object = new T(); // instantiate an object of type given
		std::cout << "object name?: " << typeid(object).name() << std::endl; 
		object->GetTransform()->position = position;
	}
	object->SetEnabled(enabled);
	AddChild(object, layer);
}