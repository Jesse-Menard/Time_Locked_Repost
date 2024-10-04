#include "Level1.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"
#include "SoundManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

Level1::Level1() : m_debugView(false), m_enemySpawnCounter(0), m_killCtr(0), m_killLimit(22) ,m_bossFight(false)
{
	Level1::Start();
}

Level1::~Level1()
= default;

void Level1::Draw()
{
	DrawDisplayList();

	// Boss specific stuff
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
			{
				display_colour += glm::vec4(0.2, 0, 0, 0);

			}
		}

		if (m_pBoss->GetHealth() > 0)
			Util::DrawFilledRect(glm::vec2(50, 20), (Config::SCREEN_WIDTH - 100) * m_pBoss->GetHealth() / 40, 25, display_colour);
		Util::DrawRect(glm::vec2(50, 20), (Config::SCREEN_WIDTH - 100), 25, glm::vec4(0, 0, 0, 1));
	}

	//	// Player HP bar
	//	Util::DrawFilledRect(glm::vec2((Config::SCREEN_WIDTH / 2) - m_pPlayer->GetHealth(), Config::SCREEN_HEIGHT - 40), m_pPlayer->GetHealth() * 2, 40, glm::vec4(0, 1, 0, 1));Util::DrawFilledRect(glm::vec2((Config::SCREEN_WIDTH / 2) - m_pPlayer->GetHealth(), Config::SCREEN_HEIGHT - 40), m_pPlayer->GetHealth() * 2, 40, glm::vec4(0, 1, 0, 1));
	//	Util::DrawRect(glm::vec2((Config::SCREEN_WIDTH / 2) - 100, Config::SCREEN_HEIGHT - 40), 200, 40, glm::vec4(0, 0, 0, 1));

	if (m_debugView)
	{
		Util::DrawRect(m_pPlayer->GetTransform()->position - glm::vec2(m_pPlayer->GetWidth() * 0.5, m_pPlayer->GetHeight() * 0.5), m_pPlayer->GetWidth(), m_pPlayer->GetHeight());
		if(m_pBoss->IsEnabled())
		{
			Util::DrawRect(m_pBoss->GetTransform()->position - glm::vec2(m_pBoss->GetWidth() * 0.5, m_pBoss->GetHeight() * 0.5), m_pBoss->GetWidth(), m_pBoss->GetHeight());
			m_pBoss->DrawHitBox();
		}

		for (const auto caveman : m_pCaveMan)
		{
			Util::DrawRect(caveman->GetTransform()->position -
				glm::vec2(caveman->GetWidth() * 0.5, caveman->GetHeight() * 0.5),
				caveman->GetWidth(), caveman->GetHeight());
		}

		for (const auto bullet : m_pBullets)
		{
			Util::DrawRect(bullet->GetTransform()->position -
				glm::vec2(bullet->GetWidth() * 0.5, bullet->GetHeight() * 0.5),
				bullet->GetWidth(), bullet->GetHeight(), {1, 0, 0, 1});
		}
	}

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void Level1::Update()
{
	UpdateDisplayList();

	glm::vec2 mtv;
	for (int i = 0; i < m_pCaveMan.size(); ++i)
	{
		for (int ii = 0; ii < m_pCaveMan.size(); ++ii)
		{
			if (ii != i)
				CollisionManager::CircleCollisionPush(m_pCaveMan[i], m_pCaveMan[ii], mtv);
		}

		if(m_pBoss->IsEnabled())
			CollisionManager::CircleCollisionPush(m_pBoss, m_pCaveMan[i], mtv, true);

		CollisionManager::CircleCollisionPush(m_pPlayer, m_pCaveMan[i], mtv);
	}

	if(m_pBoss->IsEnabled())
		CollisionManager::CircleCollisionPush(m_pBoss, m_pPlayer, mtv, true);

	if (m_pBoss->GetHealth() > 0)
		SpawnCaveManAndBullet();

	// Bullet Updates
	for (int i = 0; i < m_pBullets.size(); ++i)
	{
		// Hostile projectiles
		if (m_pBullets[i]->isSeeking())
		{
			if (!m_pBullets[i]->GetIsPlayerOwned())
				m_pBullets[i]->SetTargetPosition(m_pPlayer->GetTransform()->position);
		}
	}

	if (m_bossFight) 
	{
		m_pBoss->SetEnabled(true);
		m_pBoss->SetTargetPosition(m_pPlayer->GetTransform()->position);

		// Bullet spawning for Boss
		if (m_pBoss->m_spawnBullet)
		{
			m_pBullets.push_back(new Bullet(m_pBoss->GetTransform()->position, m_pBoss->GetTargetDirection(), m_pBoss->GetWeaponType(), false, true, 2.0f));
			AddChild(m_pBullets[m_pBullets.size() - 1], 3);
		}

		// When the final boss dies, kill all cavemen
		if (m_pBoss->GetHealth() <= 0)
		{
			for (const auto caveMan : m_pCaveMan)
			{
				caveMan->SetHealth(0);
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
	else // move player back to the center of the screen if bossFight is false
	{
		if (m_pPlayer->GetTransform()->position != glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT /2))
		{
			m_pPlayer->GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2);
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

	m_timer1 += 1;

	// UI stuff
	m_pKillCounter->SetText(m_bossFight ? " " : "Kills until Boss: " + std::to_string(m_killLimit - m_killCtr));

	//m_pTimeCounter->SetText("Time (seconds): " + std::to_string(m_timer1 / 60));

	Collisions();
	Deletion();
}

void Level1::Clean()
{
	RemoveAllChildren();
}

void Level1::HandleEvents()
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
			
		for (int i = 0; i < m_pCaveMan.size(); ++i)
		{
			m_pCaveMan[i]->GetTransform()->position += Util::Normalize(direction) * multiplier; // update caveman position
		}

		for (auto bullet : m_pBullets)
		{
			bullet->GetTransform()->position += Util::Normalize(direction) * multiplier; // update bullet position
		}
	}
	else
	{
		m_pPlayer->GetTransform()->position -= Util::Normalize(direction) * multiplier; // If in boss fight, lock everything and move the player
	}

	GetKeyboardInput();
}

void Level1::Start()
{
	m_timer1 = 0;

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	// Background
	for (int i = 0; i < m_pBackgrounds.size(); i++)
	{
		m_pBackgrounds[i] = new Background("../Assets/backgrounds/level1.png", "default");
		AddChild(m_pBackgrounds[i], 0);
	}

	m_pBackgrounds[0]->GetTransform()->position = glm::vec2(0.0f, 0.0f);
	m_pBackgrounds[1]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetWidth(), 0.0f);
	m_pBackgrounds[2]->GetTransform()->position = glm::vec2(0.0f, m_pBackgrounds[0]->GetHeight());
	m_pBackgrounds[3]->GetTransform()->position = glm::vec2(m_pBackgrounds[0]->GetWidth(), m_pBackgrounds[0]->GetHeight());

	// Player Sprite
	m_addGameObject(m_pPlayer, glm::vec2(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2), 1);

	// Boss
	m_addGameObject(m_pBoss, glm::vec2(Config::SCREEN_WIDTH * 0.5, 200), 4, false);

	//	// Player Label
	//	m_pPlayerHP = new Label("HEALTH", "Consolas");
	//	m_pPlayerHP->GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 660.0f);
	//	AddChild(m_pPlayerHP);

	// Making bullet vector always have 1
	m_pBullets.push_back(new Bullet());

	stats->SetLastLevel("Prehistoric");
	
	// UI Labels
	m_pKillCounter = new Label("Kills until Boss: " + std::to_string(m_killLimit - m_killCtr), "Dock51", 25, { 0, 0, 0, 255 });
	m_pKillCounter->GetTransform()->position = glm::vec2(640, 15);
	AddChild(m_pKillCounter, 10);

	//	m_pTimeCounter = new Label("Time (seconds): " + std::to_string(m_timer1 / 60), "Dock51", 25, { 0, 0, 255, 255 });
	//	m_pTimeCounter->GetTransform()->position = glm::vec2(640, 695);
	//	AddChild(m_pTimeCounter, 10);

	// Background Music
	SoundManager::Instance().PlayMusic("Map1");

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void Level1::GUI_Function()
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
	if(ImGui::Checkbox("Toggle Boss Fight", &toggleBoss))
	{
		m_bossFight = toggleBoss;
		SoundManager::Instance().PlayMusic("BossFight");
	}

	if(ImGui::Button("End Scene"))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
	
	ImGui::End();
}

void Level1::SpawnCaveManAndBullet()
{
	float counterLimit = 225; // Every 3.75s
	if (m_bossFight)
		counterLimit *= 2.9f; // (*= 2.5)
	if (m_pCaveMan.size() <= 13) 
	{
		if (m_enemySpawnCounter++ % static_cast<int>(counterLimit) == 0)
		{
			m_pCaveMan.push_back(new CaveMan());
			m_addGameObject(m_pCaveMan[m_pCaveMan.size() - 1], glm::vec2(rand() % Config::SCREEN_WIDTH, rand() % Config::SCREEN_HEIGHT), 1);
			m_pCaveMan.push_back(new CaveMan());
			m_addGameObject(m_pCaveMan[m_pCaveMan.size() - 1], glm::vec2(rand() % Config::SCREEN_WIDTH, rand() % Config::SCREEN_HEIGHT), 1);
		}
	}

	// Bullet spawning for CaveMan & CaveMan Updates
	for (int i = 0; i < m_pCaveMan.size(); ++i)
	{
		m_pCaveMan[i]->SetTargetPosition(m_pPlayer->GetTransform()->position);
		if (m_pCaveMan[i]->m_spawnBullet)
		{
			m_pBullets.push_back(new Bullet(m_pCaveMan[i]->GetTransform()->position, m_pCaveMan[i]->GetBulletDirection(), m_pCaveMan[i]->GetWeaponType(), false));
			AddChild(m_pBullets[m_pBullets.size() - 1], 3);
		}
	}
}

void Level1::Collisions()
{
	for (int i = 0; i < m_pBullets.size(); ++i)
	{
		// bug: if i = 0, CollisionManager doesn't update..
		// Hostile projectiles
		if (CollisionManager::AABBCheck(m_pBullets[i], m_pPlayer))
		{
			// Nested if because CollisionManager is lame
			if (m_pPlayer->GetHitDelayCtr() <= 0 && i != 0 && m_pPlayer->GetHealth() > 0 && !m_pBullets[i]->GetIsPlayerOwned())
			{
				m_pPlayer->ResetHitDelayCtr();
				m_pPlayer->TakeDamage(5);
			}
		}
		// Friendly projectiles
		if (m_pBullets[i]->GetIsPlayerOwned())
		{
			for (int j = 0; j < m_pCaveMan.size(); j++)
			{
				if (CollisionManager::AABBCheck(m_pBullets[i], m_pCaveMan[j]))
				{
					if (i != 0)
					{
						int test = m_pCaveMan[j]->GetHitDelayCtr() % m_pCaveMan[j]->GetHitDelayMax();
						if (m_pCaveMan[j]->GetHitDelayCtr() % m_pCaveMan[j]->GetHitDelayMax() == m_pCaveMan[j]->GetHitDelayMax() - 1)
							stats->AddDamage(3);
						m_pCaveMan[j]->TakeDamage(3);
					}
				}
			}
			if (CollisionManager::AABBCheck(m_pBullets[i], m_pBoss))
			{
				if (i != 0 && m_pBoss->IsEnabled())
				{
					if (m_pBoss->GetHitDelayCtr() % m_pBoss->GetHitDelayMax() == m_pBoss->GetHitDelayMax() - 1)
						stats->AddDamage(3);
					m_pBoss->TakeDamage(3);
				}
			}
		}
	}

	if (m_bossFight)
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

void Level1::Deletion()
{
	// Bullets
	for (int i = 0; i < m_pBullets.size(); ++i)
	{
		if (m_pBoss->GetHealth() <= 0)
		{
			if (m_pBullets[i]->GetWeaponType() == WeaponType::SPIKE_BALL);
				m_pBullets[i]->m_duration = 0;
		}

		if (m_pBullets[i]->GetDuration() == 0)
		{
			RemoveChild(m_pBullets[i]);
			m_pBullets.erase(m_pBullets.begin() + i);
			m_pBullets.shrink_to_fit();
		}
	}

	// CaveMan
	for (int i = 0; i < m_pCaveMan.size(); i++)
	{
		if (m_pCaveMan[i]->GetDeleteMe())
		{
			RemoveChild(m_pCaveMan[i]);
			m_pCaveMan.erase(m_pCaveMan.begin() + i);
			m_pCaveMan.shrink_to_fit();

			if (!m_pBoss->GetDeleteMe())
				m_killCtr++;
			if (m_killCtr == m_killLimit)
			{
				SoundManager::Instance().PlayMusic("BossFight");
				m_bossFight = true;
			}
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


	// Boss bullet

	// Boss
	if (m_pBoss->GetDeleteMe())
	{
		m_killCtr++;
		stats->SetKills(m_killCtr);
		m_bossFight = false;
		RemoveChild(m_pBoss);
		stats->SetTimePlayed(m_timer1 / 60);
		Game::Instance().ChangeSceneState(SceneState::RESULTS);
	}

}

glm::vec2 Level1::GetPlayerInput()
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

		if(m_pPlayer->GetHealth() > 0)
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
				AddChild(m_pBullets[m_pBullets.size() - 1], 3);
			}
			else if (m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_WALK_LEFT || m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_IDLE_LEFT)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_LEFT);
				m_pPlayer->SetIsAttacking(true);
				m_pBullets.push_back(new Bullet(m_pPlayer->GetTransform()->position, -glm::vec2(1.0f, 0.0f), WeaponType::SPEAR, true));
				AddChild(m_pBullets[m_pBullets.size() - 1], 3);
			}
			else if (m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_WALK_DOWN || m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_IDLE_DOWN)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_DOWN);
				m_pPlayer->SetIsAttacking(true);
				m_pBullets.push_back(new Bullet(m_pPlayer->GetTransform()->position, -glm::vec2(0.0f, -1.0f), WeaponType::SPEAR, true));
				AddChild(m_pBullets[m_pBullets.size() - 1], 3);
			}
			else if (m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_WALK_RIGHT || m_pPlayer->GetAnimationState() == PlayerAnimationState::PLAYER_IDLE_RIGHT)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_ATTACK_RIGHT);
				m_pPlayer->SetIsAttacking(true);
				m_pBullets.push_back(new Bullet(m_pPlayer->GetTransform()->position, -glm::vec2(-1.0f, 0.0f), WeaponType::SPEAR, true));
				AddChild(m_pBullets[m_pBullets.size() - 1], 3);
			}
		}
		break;
	}

	return glm::vec2(x, y);
}

void Level1::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		SoundManager::Instance().StopMusic(0);
		Game::Instance().Quit();
	}
}

template <typename T>
void Level1::m_addGameObject(T*& object, glm::vec2 position, int layer, bool enabled)
{
	// Add the T Type object to the Scene
	if(typeid(object) != typeid(CaveMan*)) // To prevent double creation.. Assuming it matters
	{
		object = new T(); // instantiate an object of type given
		//std::cout << "object name?: " << typeid(object).name() << std::endl; 
		object->GetTransform()->position = position;
	}
	object->SetEnabled(enabled);
	AddChild(object, layer);
}