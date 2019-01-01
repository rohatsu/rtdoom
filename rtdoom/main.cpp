#include "pch.h"

#include <SDL.h>
#include <SDL_render.h>

#include "GameLoop.h"

using namespace rtdoom;
using namespace std::string_literals;
using std::cout, std::endl, std::string;

void InitSDL(SDL_Renderer*& sdlRenderer, SDL_Window*& sdlWindow);
void DestroySDL(SDL_Renderer* sdlRenderer, SDL_Window* sdlWindow);

int main(int /*argc*/, char** /*argv*/)
{
	try
	{
		SDL_Renderer *sdlRenderer;
		SDL_Window *sdlWindow;
		InitSDL(sdlRenderer, sdlWindow);

		GameLoop gameLoop{ sdlRenderer };
		gameLoop.Start("..\\maps\\freedoom-e4m1"s);

		SDL_Event event;
		const static auto tickFrequency = SDL_GetPerformanceFrequency();
		auto tickCounter = SDL_GetPerformanceCounter();

		while (gameLoop.isRunning())
		{
			while (SDL_PollEvent(&event))
			{
				if ((SDL_QUIT == event.type) || (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
				{
					gameLoop.Stop();
					break;
				}

				if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					gameLoop.ResizeWindow(event.window.data1, event.window.data2);
				}

				if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0)
				{
					auto k = event.key;
					auto p = event.type == SDL_KEYDOWN;
					switch (k.keysym.sym)
					{
					case SDLK_UP:
						gameLoop.Move(p ? 1 : 0);
						break;
					case SDLK_DOWN:
						gameLoop.Move(p ? -1 : 0);
						break;
					case SDLK_LEFT:
						gameLoop.Rotate(p ? -1 : 0);
						break;
					case SDLK_RIGHT:
						gameLoop.Rotate(p ? 1 : 0);
						break;
					case SDLK_1:
						gameLoop.SetRenderingMode(ViewRenderer::RenderingMode::Wireframe);
						break;
					case SDLK_2:
						gameLoop.SetRenderingMode(ViewRenderer::RenderingMode::Solid);
						break;
					case SDLK_3:
						gameLoop.SetRenderingMode(ViewRenderer::RenderingMode::Textured);
						break;
					case SDLK_d:
						cout << "Player position: (" << gameLoop.Player().x << ", " << gameLoop.Player().y << ", " << gameLoop.Player().a << ")" << endl;
					default:
						break;
					}
				}
			}

			gameLoop.RenderFrame();
			SDL_RenderPresent(sdlRenderer);

			const auto nextCounter = SDL_GetPerformanceCounter();
			const auto seconds = (nextCounter - tickCounter) / static_cast<float>(tickFrequency);
			tickCounter = nextCounter;
			gameLoop.Tick(seconds);

#if _DEBUG
			cout << "Frame time: " << seconds * 1000.0 << "ms" << endl;
#endif
		}

		DestroySDL(sdlRenderer, sdlWindow);
	}
	catch (std::exception& ex)
	{
		cout << "Exception: " << ex.what() << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void InitSDL(SDL_Renderer*& sdlRenderer, SDL_Window*& sdlWindow)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		throw std::runtime_error("Unable to initialize SDL");
	}

	atexit(SDL_Quit);

	if constexpr (s_multisamplingLevel != 1.0f)
	{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	}

	sdlWindow = SDL_CreateWindow
	(
		"rtdoom",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		s_displayX,
		s_displayY,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	sdlRenderer = SDL_CreateRenderer
	(
		sdlWindow,
		-1,
		SDL_RENDERER_ACCELERATED
	);
}

void DestroySDL(SDL_Renderer* sdlRenderer, SDL_Window* sdlWindow)
{
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}