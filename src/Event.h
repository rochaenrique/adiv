#pragma once

enum EventType
  {
	CheckPoint
  };

#define EVENT_TYPE(type) static EventType GetStaticType() { return EventType::type; } \
  EventType GetType() const override { return GetStaticType(); } \
  
class Event
{
public:
  virtual ~Event() = default;
  virtual EventType GetType() const = 0;
  bool IsHandled() { return m_Handled; }
  void SetHandled() { m_Handled = true; }
private: 
  bool m_Handled = false;
};

class CheckPointEvent : public Event
{
  EVENT_TYPE(CheckPoint);
};
