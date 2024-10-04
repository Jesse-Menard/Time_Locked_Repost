#pragma once
#ifndef __LEVEL1__
#define __LEVEL1__

#include "Scene.h"
#include "CaveMan.h"
#include "Level1Boss.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Background.h"
#include <vector>
#include <array>
#include "ResScene.h"

#include "Bullet.h"

class Level1 : public Scene
{
public:
	Level1();
	~Level1() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;

	int m_timer1;

private:
	// IMGUI Function
	void GUI_Function();
	bool m_debugView;
	
	glm::vec2 m_mousePosition;
	 
	std::array<Background*, 4> m_pBackgrounds;
	Player* m_pPlayer{};
	Level1Boss* m_pBoss;
	std::vector<CaveMan*> m_pCaveMan;
	std::vector<Bullet*> m_pBullets;
	std::vector<Bullet*> m_pPlayerBullets;
	void SpawnCaveManAndBullet();
	void Collisions();
	void Deletion();

	// UI Items
	Label* m_pKillCounter{};
	//Label* m_pTimeCounter{};

	// Input Control
	int m_pCurrentInputType{};
	glm::vec2 GetPlayerInput();
	void GetKeyboardInput();

	int m_enemySpawnCounter;
	int m_killCtr;
	const int m_killLimit{};
	bool m_bossFight;
	
	template <typename T>
	void m_addGameObject(T*& object, glm::vec2 position = glm::vec2(0.0f, 0.0f), int layer = 0, bool enabled = true);
};

#endif /* defined (__PLAY_SCENE__) */