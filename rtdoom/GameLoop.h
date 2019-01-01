#pragma once

#include <SDL.h>
#include <SDL_render.h>

#include "rtdoom.h"
#include "Viewport.h"
#include "GameState.h"
#include "ViewRenderer.h"
#include "MapRenderer.h"

namespace rtdoom
{
	class GameLoop
	{
	protected:
		GameState m_gameState;
		ViewRenderer m_viewRenderer;
		Viewport m_playerViewport;
		MapRenderer m_mapRenderer;
		Viewport m_mapViewport;
		bool m_isRunning;
		int m_moveDirection;
		int m_rotateDirection;

		constexpr int ViewScale(int windowSize) const;
		constexpr int MapScale(int windowSize) const;

	public:
		void Start(const std::string& mapFolder);
		void Stop();
		bool isRunning() const { return m_isRunning; }

		void Move(int moveDirection);
		void Rotate(int rotateDirection);

		void RenderFrame();
		void Tick(float seconds);
		void ResizeWindow(int width, int height);
		void SetRenderingMode(ViewRenderer::RenderingMode renderingMode);

		const Thing& Player() const { return m_gameState.m_player; }

		GameLoop(SDL_Renderer* sdlRenderer);
		~GameLoop();
	};
}