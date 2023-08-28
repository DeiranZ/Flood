#pragma once
#include <SDL.h>
#include "Application.h"
#include "EventReceiver.h"
#include <functional>

class UIButton : public EventReceiver
{
public:
	UIButton(Application* App, const SDL_Rect* Rect, const SDL_Color* BaseColor, const SDL_Color* HoverColor, std::function<void()> OnPressed) :
		SDLRenderer(App->GetWindowRenderer()),
		App(App),
		Rect(*(Rect)),
		BGColor(*(BaseColor)),
		HoverColor(*(HoverColor)),
		OnPressed(OnPressed)
	{
		Update();
	}

	bool HandleEvent(const SDL_Event* Event) override {
		if (
			Event->type == SDL_MOUSEBUTTONDOWN &&
			Event->button.button == SDL_BUTTON_LEFT &&
			isHovered
			)
		{
			OnPressed();
			return true;
		}
		else if (
			Event->type == SDL_MOUSEMOTION
			) [[likely]] {
				if (isHovered != IsWithinBounds(Event->motion.x, Event->motion.y))
				{
					isHovered = !isHovered;

					Update();
				}

				// Return true if hovering button, to cancel any input below ui
				return isHovered;
		}

		return false;
	}
	
	void Update()
	{
		SDL_Color actualColor { isHovered ? HoverColor : BGColor };

		// Draw Outline
		int outlineSize = 5;
		SDL_Rect outline{ Rect.x - outlineSize, Rect.y - outlineSize, Rect.w + outlineSize * 2, Rect.h + outlineSize * 2 };
		SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, actualColor.a);
		SDL_RenderFillRect(SDLRenderer, &outline);

		SDL_SetRenderDrawColor(SDLRenderer, actualColor.r, actualColor.g, actualColor.b, actualColor.a);
		SDL_RenderFillRect(SDLRenderer, &Rect);

	}
private:

	bool IsWithinBounds(int x, int y)
	{
		if (x < Rect.x) return false;

		if (x > Rect.x + Rect.w) return false;

		if (y < Rect.y) return false;

		if (y > Rect.y + Rect.h) return false;

		return true;
	}

private:
	bool isHovered { false };

	Application* App;
	SDL_Renderer* SDLRenderer;

	SDL_Color BGColor { 255, 50, 50, 255 };
	SDL_Color HoverColor { 50, 50, 255, 255 };
	SDL_Rect Rect{ 0, 0, 30, 30 };

	std::function<void()> OnPressed;
};