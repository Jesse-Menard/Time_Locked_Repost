#pragma once
#ifndef __LEVEL3__
#define __LEVEL3__

#include "Scene.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Background.h"
#include <vector>
#include <array>

#include "BoomerangEnemy.h"
#include "Bullet.h"
#include "CrossBowEnemy.h"
#include "Level3Boss.h"

class Level3 : public Scene
{
public:
	Level3();
	~Level3() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;

	int m_timer3;

private:
	// IMGUI Function
	void GUI_Function();
	bool m_debugView;
	
	glm::vec2 m_mousePosition;
	 
	std::vector<Background*> m_pBackgrounds;
	Player* m_pPlayer{};
	Level3Boss* m_pBoss;
	std::vector<BoomerangEnemy*> m_pBoomerangs;
	std::vector<CrossBowEnemy*> m_pCrossBows;

	std::vector<Bullet*> m_pBullets;
	
	void Collisions() const;
	void Deletion();
	void Spawning();
	void Pushing() const;

	// UI Items
	Label* m_pKillCounter{};
	Label* m_pTimeCounter{};

	// Input Control
	int m_pCurrentInputType{};
	glm::vec2 GetPlayerInput();
	void GetKeyboardInput();

	// Scene variables
	bool m_bossFight;
	int m_killCtr;
	const int m_killLimit{};
	int m_enemySpawnCounter;
	int m_accelerationCtr = 0;
	float m_flashAlpha = 0.0f;

	template <typename T>
	void m_addGameObject(T*& object, glm::vec2 position = glm::vec2(0.0f, 0.0f), int layer = 0, bool enabled = true);
};

#endif /* defined (__PLAY_SCENE__) */