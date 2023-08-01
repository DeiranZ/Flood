#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <stack>
#include <map>
#include "Window.h"
#include "Layer.h"
#include "EventReceiver.h"
#include "UIButton.h"
#include "Application.h"
#include "Vector2.h"
#include "Square.h"
#include "Board.h"
#include "Color.h"


//Screen dimension constants
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 900;


const int paletteSize = 6;
Color palette[paletteSize] = {
	Color(90, 193, 225),
	Color(246, 241, 187),
	Color(230, 195, 71),
	Color(192, 106, 63),
	Color(99, 77, 72),
	Color(206, 33, 23)
};


Board GameBoard;
int ActiveColor = 0;
int GameDifficulty = 1;
std::default_random_engine Generator;
std::uniform_int_distribution<int> PaletteDistribution(0, paletteSize-1);

int CameraOffsetX = 0;
int CameraOffsetY = 0;
int CameraMovementSpeed = 5;
bool LMBDown = false;
Vector2 LastMousePos = { -1, -1 };

float Zoom = 1.0f;


void Close(Window* GameWindow)
{
	GameWindow->DestroyWindow();

	//Quit SDL subsystems
	SDL_Quit();
}


// Renders the game board in the GameWindow
void RenderBoard(Window* GameWindow)
{
	SDL_Renderer* renderer = GameWindow->GetRenderer();

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
	SDL_Rect bgRect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(renderer, &bgRect);

	for (int i = 0; i < GameBoard.squares.size(); i++)
	{
		Square* currentSquare = nullptr;
		currentSquare = &GameBoard.squares[i];
		currentSquare->visitedThisTurn = false;
		
		// A bit bigger so no seams are visible when zooming
		SDL_Rect renderRect{ (currentSquare->x + CameraOffsetX) * Zoom, (currentSquare->y + CameraOffsetY) * Zoom, currentSquare->size * Zoom + 0.1f, currentSquare->size * Zoom + 0.1f };
		
		SDL_SetRenderDrawColor(renderer, palette[currentSquare->color].r, palette[currentSquare->color].g, palette[currentSquare->color].b, 255);
		SDL_RenderFillRect(renderer, &renderRect);
	}
}


// Builds the board and renders the board in the GameWindow
void BuildBoard(Window* GameWindow)
{
	int boardSize = 5 * GameDifficulty;
	int squareSize = SCREEN_WIDTH / boardSize;

	GameBoard = Board(boardSize, squareSize);

	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			int randomColorIndex = PaletteDistribution(Generator);
			Color* randomColor = &palette[randomColorIndex];

			GameBoard.AddSquare(i, j, squareSize, randomColorIndex);
		}
	}

	ActiveColor = GameBoard.squares[0].color;

	RenderBoard(GameWindow);
}


// Propagates the color of index newColorIndex from coordinates ( i, j )
void PropagateColor(int i, int j, int newColorIndex)
{
	int oldColor = GameBoard.squares.at(i + j * GameBoard.boardSize).color;

	std::stack<Square*> stack;
	stack.push(&GameBoard.squares.at(i + j * GameBoard.boardSize));

	while (!stack.empty())
	{
		Square* current = stack.top();
		stack.pop();

		// Check if the currently checked square was already checked during this turn
		if (current->visitedThisTurn)
		{
			continue;
		}

		current->color = newColorIndex;


		std::vector<Square*> surround;
		GameBoard.GetSurroundingSquares(current->column, current->row, &surround);

		current->visitedThisTurn = true;

		for (Square* sq : surround)
		{
			if (!sq->visitedThisTurn && sq->color == oldColor)
			{
				stack.push(sq);
			}
		}
	}
}


// Chooses a color as active and propagates it
// Also checks if the game was won with that move
void ChooseColor(int index, Window* GameWindow)
{
	if (ActiveColor == index) return;

	ActiveColor = index;

	PropagateColor(0, 0, ActiveColor);

	// Check if won
	int totalSquares = GameBoard.boardSize * GameBoard.boardSize;
	int singleColoredSquares = 0;

	for (int i = 0; i < totalSquares; i++)
	{
		Square* sq = &GameBoard.squares[i];

		if (sq->color == ActiveColor) singleColoredSquares += 1;
	}

	if (singleColoredSquares == totalSquares)
	{
		std::cout << "You won!" << std::endl;

		GameDifficulty += 1;
		BuildBoard(GameWindow);
	}
}


int main(int argc, char* args[])
{
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event Event;

	Window GameWindow;
	Application App(&GameWindow);
	Layer UI;

	int buttonMargin = 20;
	int offset = -350;
	int buttonHeight = 100;
	int buttonWidth = 100;
	int vBoxHeight = GameWindow.GetWindowHeight() - 105;

	// Color 1 button
	SDL_Rect Color1Rect{ offset + GameWindow.GetWindowWidth()/2, vBoxHeight, buttonWidth, buttonHeight };
	SDL_Color Color1BG{ palette[0].r, palette[0].g, palette[0].b, 255 };
	SDL_Color Color1Hover{ palette[0].r - 30, palette[0].g - 30, palette[0].b - 30, 255 };
	UIButton Color1Button(&App, &Color1Rect, &Color1BG, &Color1Hover, [&GameWindow]() { ChooseColor(0, &GameWindow); RenderBoard(&GameWindow); GameWindow.RenderFrame();});
	UI.SubscribeToEvents(&Color1Button);

	// Color 2 button
	SDL_Rect Color2Rect{ offset + GameWindow.GetWindowWidth() / 2 + (buttonWidth + buttonMargin) * 1, vBoxHeight, buttonWidth, buttonHeight };
	SDL_Color Color2BG{ palette[1].r, palette[1].g, palette[1].b, 255 };
	SDL_Color Color2Hover{ palette[1].r - 30, palette[1].g - 30, palette[1].b - 30, 255 };
	UIButton Color2Button(&App, &Color2Rect, &Color2BG, &Color2Hover, [&GameWindow]() { ChooseColor(1, &GameWindow); RenderBoard(&GameWindow); GameWindow.RenderFrame(); });
	UI.SubscribeToEvents(&Color2Button);

	// Color 3 button
	SDL_Rect Color3Rect{ offset + GameWindow.GetWindowWidth() / 2 + (buttonWidth + buttonMargin) * 2, vBoxHeight, buttonWidth, buttonHeight };
	SDL_Color Color3BG{ palette[2].r, palette[2].g, palette[2].b, 255 };
	SDL_Color Color3Hover{ palette[2].r - 30, palette[2].g - 30, palette[2].b - 30, 255 };
	UIButton Color3Button(&App, &Color3Rect, &Color3BG, &Color3Hover, [&GameWindow]() { ChooseColor(2, &GameWindow); RenderBoard(&GameWindow); GameWindow.RenderFrame(); });
	UI.SubscribeToEvents(&Color3Button);

	// Color 4 button
	SDL_Rect Color4Rect{ offset + GameWindow.GetWindowWidth() / 2 + (buttonWidth + buttonMargin) * 3, vBoxHeight, buttonWidth, buttonHeight };
	SDL_Color Color4BG{ palette[3].r, palette[3].g, palette[3].b, 255 };
	SDL_Color Color4Hover{ palette[3].r - 30, palette[3].g - 30, palette[3].b - 30, 255 };
	UIButton Color4Button(&App, &Color4Rect, &Color4BG, &Color4Hover, [&GameWindow]() { ChooseColor(3, &GameWindow); RenderBoard(&GameWindow); GameWindow.RenderFrame(); });
	UI.SubscribeToEvents(&Color4Button);

	// Color 5 button
	SDL_Rect Color5Rect{ offset + GameWindow.GetWindowWidth() / 2 + (buttonWidth + buttonMargin) * 4, vBoxHeight, buttonWidth, buttonHeight };
	SDL_Color Color5BG{ palette[4].r, palette[4].g, palette[4].b, 255 };
	SDL_Color Color5Hover{ palette[4].r - 30, palette[4].g - 30, palette[4].b - 30, 255 };
	UIButton Color5Button(&App, &Color5Rect, &Color5BG, &Color5Hover, [&GameWindow]() { ChooseColor(4, &GameWindow); RenderBoard(&GameWindow); GameWindow.RenderFrame(); });
	UI.SubscribeToEvents(&Color5Button);

	// Color 6 button
	SDL_Rect Color6Rect{ offset + GameWindow.GetWindowWidth() / 2 + (buttonWidth + buttonMargin) * 5, vBoxHeight, buttonWidth, buttonHeight };
	SDL_Color Color6BG{ palette[5].r, palette[5].g, palette[5].b, 255 };
	SDL_Color Color6Hover{ palette[5].r - 30, palette[5].g - 30, palette[5].b - 10, 255 };
	UIButton Color6Button(&App, &Color6Rect, &Color6BG, &Color6Hover, [&GameWindow]() { ChooseColor(5, &GameWindow); RenderBoard(&GameWindow); GameWindow.RenderFrame(); });
	UI.SubscribeToEvents(&Color6Button);

	// Randomize the game
	Generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

	BuildBoard(&GameWindow);

	// Main game loop
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&Event) != 0)
		{
			//User requests quit
			if (Event.type == SDL_QUIT || (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE))
			{
				quit = true;
			}

			if (UI.HandleEvent(&Event))
			{
				continue;
			}

			// Camera movement

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_w)
			{
				CameraOffsetY += CameraMovementSpeed * Zoom;

				RenderBoard(&GameWindow);
			}

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_s)
			{
				CameraOffsetY -= CameraMovementSpeed * Zoom;

				RenderBoard(&GameWindow);
			}

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_d)
			{
				CameraOffsetX -= CameraMovementSpeed * Zoom;

				RenderBoard(&GameWindow);
			}

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_a)
			{
				CameraOffsetX += CameraMovementSpeed * Zoom;

				RenderBoard(&GameWindow);
			}

			// Color choosing

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_1)
			{
				ChooseColor(0, &GameWindow);

				RenderBoard(&GameWindow);
			}

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_2)
			{
				ChooseColor(1, &GameWindow);

				RenderBoard(&GameWindow);
			}

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_3)
			{
				ChooseColor(2, &GameWindow);

				RenderBoard(&GameWindow);
			}

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_4)
			{
				ChooseColor(3, &GameWindow);

				RenderBoard(&GameWindow);
			}

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_5)
			{
				ChooseColor(4, &GameWindow);

				RenderBoard(&GameWindow);
			}

			else if (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_6)
			{
				ChooseColor(5, &GameWindow);

				RenderBoard(&GameWindow);
			}

			// Zooming

			else if (Event.type == SDL_MOUSEWHEEL)
			{
				if (Event.wheel.y > 0)
				{
					Zoom += 0.1;
				}

				else if (Event.wheel.y < 0)
				{
					Zoom -= 0.1;
				}

				RenderBoard(&GameWindow);
			}
			
			// Camera dragging using mouse

			else if (Event.type == SDL_MOUSEBUTTONDOWN && Event.button.button == SDL_BUTTON_LEFT)
			{
				LMBDown = true;

				LastMousePos.x = Event.motion.x;
				LastMousePos.y = Event.motion.y;
			}

			else if (Event.type == SDL_MOUSEBUTTONUP && Event.button.button == SDL_BUTTON_LEFT)
			{
				LMBDown = false;

				LastMousePos.x = -1;
				LastMousePos.y = -1;
			}

			else if (Event.type == SDL_MOUSEMOTION && LMBDown)
			{
				CameraOffsetX -= (LastMousePos.x - Event.motion.x);
				CameraOffsetY -= (LastMousePos.y - Event.motion.y);

				LastMousePos.x = Event.motion.x;
				LastMousePos.y = Event.motion.y;

				RenderBoard(&GameWindow);
			}
		}

		// Update the UI

		Color1Button.Update();
		Color2Button.Update();
		Color3Button.Update();
		Color4Button.Update();
		Color5Button.Update();
		Color6Button.Update();
		GameWindow.RenderFrame();
	}
		
	//Free resources and close SDL
	Close(&GameWindow);

	return 0;
}
