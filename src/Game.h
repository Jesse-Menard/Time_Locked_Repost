#pragma once
#ifndef __GAME__
#define __GAME__

// Core Libraries
#include <iostream>
#include <string>
#include <vector>
#include "SceneState.h"

#include <SDL.h>

// Game Managers
#include "CollisionManager.h"

// Scenes
#include "StartScene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "DefeatScene.h"

#include "Config.h"

#include "ImGuiWindowFrame.h"

class Game
{
public:
	static Game& Instance()
	{
		static Game instance;
		return instance;
	}

	// simply set the isRunning variable to true
	void Init();
	bool Init(const char* title, int x, int y, int width, int height, bool fullscreen);

	// public life cycle functions
	void Render() const;
	void Update() const;
	void HandleEvents() const;
	void Clean() const;
	void Start();
	void Quit();
	void TogglePause();

	// getter and setter functions
	[[nodiscard]] bool IsPaused() const;
	[[nodiscard]] Scene* GetPauseScene() const; // function for main
	

	[[nodiscard]] glm::vec2 GetMousePosition() const;

	void SetFrames(Uint32 frames);
	[[nodiscard]] Uint32 GetFrames() const;

	[[nodiscard]] float GetDeltaTime() const;
	void SetDeltaTime(float time);

	[[nodiscard]] bool IsRunning() const;
	void ChangeSceneState(SceneState new_state);

	[[nodiscard]] SDL_Window* GetWindow() const;

private:
	Game();
	~Game();

	// game properties
	bool m_bRunning;
	bool m_bPaused;
	Uint32 m_frames;
	float m_deltaTime{};
	glm::vec2 m_mousePosition;

	// scene variables
	Scene* m_pCurrentScene;
	Scene* m_pPauseScene;
	SceneState m_currentSceneState;

	// storage structures
	std::shared_ptr<SDL_Window> m_pWindow;
};

#endif /* defined (__GAME__) */