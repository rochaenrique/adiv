#pragma once
#include "Event.h"
#include <queue>
#include <functional>
#include <memory>

class EventManager
{
public:
  static EventManager& Get()
  {
	static EventManager instance;
	return instance;
  }

  template<typename T, typename... Args>
  void Emit(Args&&... args)
  {
	m_Events.push(std::make_unique<T>(std::forward<Args>(args)...));
  }

  void Dispatch()
  {
	assert(m_EventCallback && "No event callback");
	while (!m_Events.empty()) {
	  std::unique_ptr<Event> event = std::move(m_Events.front());
	  if (!event->IsHandled()) {
		bool handled = m_EventCallback(*event);
		if (handled) event->SetHandled();
	  }
	  m_Events.pop();
	}
  };

  void Flush(EventType type)
  {
	std::queue<std::unique_ptr<Event>> filtered;
	while (!m_Events.empty()) {
	  if (m_Events.front()->GetType() != type) 
		filtered.push(std::move(m_Events.front()));
	  m_Events.pop();
	}
	m_Events = std::move(filtered);
  }
  
  using EventCallback = std::function<bool(Event&)>;
  void SetEventCallback(const EventCallback& f)
  {
	m_EventCallback = f;
  }
  
private:
  EventManager() = default;
  ~EventManager() = default;
  
  std::queue<std::unique_ptr<Event>> m_Events {};
  EventCallback m_EventCallback;
};
