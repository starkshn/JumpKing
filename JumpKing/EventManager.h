#pragma once

class Object;

struct Event
{
	EVENT_TYPE	_eventType;
	DWORD_PTR	_lParam;
	DWORD_PTR	_rParam;
	Object*		p_obj;
};

class EventManager
{
	SINGLE(EventManager);

private:
	EventManager();
	~EventManager();

private:
	vector<Event> _vecEvents;
	vector<Object*> _vecDead;

public:
	void Update();

private:
	void ExcuteEvent(const Event& e);

public:
	void AddEvent(const Event& e)
	{
		_vecEvents.emplace_back(e);
	}
};

