#pragma once
#include <memory>
#include <string>
#include "Event.h"

namespace dae {

	class GameObject;

	class Observer
	{
	public:
		Observer() = default;
	
		virtual ~Observer() = default;
		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) = delete;
	
		virtual void Notify(Event event) = 0;
	
	};

}