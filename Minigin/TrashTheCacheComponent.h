#pragma once
#include "Component.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <numeric>
#include <math.h>
#include "vld.h"
#include <algorithm>

namespace dae {

	struct Transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};
	
	class GameObject3D
	{
	public:
		Transform transform{};
		int ID{};
	};
	
	class GameObject3DAlt
	{
	public:
		Transform* transform{};
		int ID{};
	};

	class TrashTheCacheComponent final : public Component
	{
	public:
		TrashTheCacheComponent(std::weak_ptr<GameObject> pOwner);
	
	
		virtual ~TrashTheCacheComponent() = default;
		TrashTheCacheComponent(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent(TrashTheCacheComponent&& other) = delete;
		TrashTheCacheComponent& operator=(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent& operator=(TrashTheCacheComponent&& other) = delete;
	
		void RenderImGUI() override;
	
	private:
		int m_sampleAmount = 10;
		const int m_arrayAmount = 10'000'000;

		bool m_clickedEx01Button = false;
		bool m_isEx01Done = false;

		bool m_clickedEx02Button = false;
		bool m_isEx02Done = false;

		bool m_clickedEx03Button = false;
		bool m_isEx03Done = false;

		std::vector<float> m_stepsizes{1,2,4,8,16,32,64,128,256,512,1024};

		std::map<int, std::vector<float>> m_Ex01recordedTimes;
		std::vector<float> m_Ex01recordedTimesNoHighOrLow;
		std::vector<float> m_Ex01recordedTimesAverage;

		std::map<int, std::vector<float>> m_Ex02recordedTimes;
		std::vector<float> m_Ex02recordedTimesNoHighOrLow;
		std::vector<float> m_Ex02recordedTimesAverage;

		std::map<int, std::vector<float>> m_Ex03recordedTimes;
		std::vector<float> m_Ex03recordedTimesNoHighOrLow;
		std::vector<float> m_Ex03recordedTimesAverage;

		void DoExercise01();
		void DoExercise02();
		void DoEX01TrashingInts();
		void DoEX02TrashingGameObjects();
		void DoEX03TrashingGameObjectAlts();
		

		std::vector<float> RemoveHighestAndLowest(std::vector<float>& timedVector);
		float GetAverageTime(const std::vector<float>& timedVector);
	};
}

