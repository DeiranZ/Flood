// Thanks to www.studyplan.dev

#pragma once
#include <vector>
#include <SDL.h>
#include "EventReceiver.h"


class Layer
{
public:
	// Returns true if the event was handled
	bool HandleEvent(const SDL_Event* Event)
	{
		for (const auto Handler : Subscribers)
		{
			if (Handler->HandleEvent(Event))
			{
				return true;
			}
		}

		return false;
	}

	void SubscribeToEvents(EventReceiver* Receiver)
	{
		Subscribers.push_back(Receiver);
	}

private:
	std::vector<EventReceiver*> Subscribers;
};