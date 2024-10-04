#pragma once
#ifndef __LEVEL2__
#define __LEVEL2__

#include "Scene.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Background.h"
#include <vector>
#include <array>
#include "ResScene.h"

#include "Archer.h"
#include "Summoner.h"
#include "Bullet.h"
#include "Skull.h"
#include "Level2Boss.h"

class Level2 : public Scene
{
public:
	Level2();
	~Level2() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;

	int m_timer2;

private:
	// IMGUI Function
	void GUI_Function();
	bool m_debugView;
	
	glm::vec2 m_mousePosition;

	// Game Objects
	std::array<Background*, 4> m_pBackgrounds;
	Player* m_pPlayer{};
	Level2Boss* m_pBoss{};
	std::vector<Archer*> m_pArchers;
	std::vector<Summoner*> m_pSummoners;
	std::vector<Skull*> m_pSkulls;
	std::vector<Bullet*> m_pBullets;
	std::vector<Particle*> m_pParticles;

	void Collisions();
	void Deletion();
	void Spawning();

	// UI Items
	Label* m_pKillCounter{};
	//Label* m_pTimeCounter{};

	// Input Control
	int m_pCurrentInputType{};
	glm::vec2 GetPlayerInput();
	void GetKeyboardInput();

	// Scene variables
	bool m_bossFight;
	int m_killCtr;
	const int m_killLimit{};
	int m_enemySpawnCounter;

	template <typename T>
	void m_addGameObject(T*& object, glm::vec2 position = glm::vec2(0.0f, 0.0f), int layer = 0, bool enabled = true);
};

#endif /* defined (__PLAY_SCENE__) */