#pragma once
#include "Observer.h"
#include "Event.h"
#include <vector>

namespace dae 
{

	class Subject final
	{
	public:
		Subject() = default;

		~Subject() = default;
		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;

		void AddObserver(Observer* observer)
		{
			auto it = std::find_if(m_observers.begin(), m_observers.end(),
				[&](Observer* currentObserver)
				{
					return currentObserver == observer;
				});

			if (it == m_observers.end())
				m_observers.push_back(observer);
		}

		void RemoveObserver(Observer* observer)
		{
			Observer* observerToRemove = observer;
			auto it = std::find(m_observers.begin(), m_observers.end(), observerToRemove);
			if (it != m_observers.end()) {
				m_observers.erase(it);
			}
		}

		void NotifyObservers(Event event)
		{
			for (auto& observer : m_observers)
				observer->Notify(event);
		}

	private:
		std::vector<Observer*> m_observers{};
	};
}
