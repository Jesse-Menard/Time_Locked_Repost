#pragma once
#ifndef __UI_CONTROL__
#define __UI_CONTROL__

#include "DisplayObject.h"
#include <SDL.h>
#include "Event.h"
#include <unordered_map>
#include <functional>

class UIControl : public DisplayObject
{
public:
	UIControl();
	~UIControl() override;
	
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Clean() = 0;

	typedef std::function<void()> EventHandler;
	bool AddEventListener(Event event, const EventHandler& handler);

	EventHandler GetEventHandler(Event event);
	
protected:
	void OnMouseOver(bool centered = true);
	void OnMouseOut(bool centered = true);
	void OnLeftMouseButtonClick(bool centered = true);

private:
	bool CheckIfEventExists(Event id);

	// mouse events
	bool m_mouseOver;
	bool m_mouseOverActive;
	bool m_mouseOutActive;
	
	bool m_leftMouseButtonClicked;

	// event structure
	std::unordered_map<Event, EventHandler> m_events;
};

#endif /* defined (__UI_CONTROL__) */

