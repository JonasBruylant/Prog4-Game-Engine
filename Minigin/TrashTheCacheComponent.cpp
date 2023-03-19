#include "TrashTheCacheComponent.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_plot.h"

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>

dae::TrashTheCacheComponent::TrashTheCacheComponent(std::weak_ptr<GameObject> pOwner) 
	: Component(pOwner)
{

}

void dae::TrashTheCacheComponent::RenderImGUI()
{
	DoExercise01();
	DoExercise02();
}

void dae::TrashTheCacheComponent::DoExercise01()
{
	ImGui::Begin("Exercise 01");
	ImGui::SliderInt("Samples", &m_sampleAmount, 10, 100);

	if (ImGui::Button("Trash the ints"))
		m_clickedEx01Button = true;

	if (m_clickedEx01Button && !m_isEx01Done)
	{
		DoEX01TrashingInts();
		m_isEx01Done = true;
	}

	if (m_isEx01Done)
	{
		ImGui::PlotConfig config;
		config.values.xs = m_stepsizes.data();
		config.values.ys = m_Ex01recordedTimesAverage.data();
		config.values.count = static_cast<int>(m_Ex01recordedTimesAverage.size());
		config.values.color = ImColor(1.f, 0.f, 0.f, 0.5f);
		config.scale.min = -1;
		config.scale.max = 40'000;
		config.tooltip.show = true;
		config.tooltip.format = "x=%.2f, y=%.2f";
		config.grid_x.show = true;
		config.grid_y.show = true;
		config.frame_size = ImVec2(200, 200);
		config.line_thickness = 2.f;
		
		ImGui::Plot("Trashed the ints", config);
	}

	ImGui::End();
}

void dae::TrashTheCacheComponent::DoExercise02()
{
	ImGui::Begin("Exercise 02");
	ImGui::SliderInt("Samples", &m_sampleAmount, 10, 100);

	if (ImGui::Button("Trash the Gameobjects"))
		m_clickedEx02Button = true;

	if (m_clickedEx02Button && !m_isEx02Done)
	{
		DoEX02TrashingGameObjects();
		m_isEx02Done = true;
	}

	if (m_isEx02Done)
	{
		ImGui::PlotConfig config;
		config.values.xs = m_stepsizes.data();
		config.values.ys = m_Ex02recordedTimesAverage.data();
		config.values.count = static_cast<int>(m_Ex02recordedTimesAverage.size());
		config.values.color = ImColor(1.f, 0.f, 0.f, 0.5f);
		config.scale.min = -1;
		config.scale.max = 40'000;
		config.tooltip.show = true;
		config.tooltip.format = "x=%.2f, y=%.2f";
		config.grid_x.show = true;
		config.grid_y.show = true;
		config.frame_size = ImVec2(200, 200);
		config.line_thickness = 2.f;

		ImGui::Plot("Trashed the GameObjects", config);
	}

	if (ImGui::Button("Trash the GameobjectAlts"))
		m_clickedEx03Button = true;

	if (m_clickedEx03Button && !m_isEx03Done)
	{
		DoEX03TrashingGameObjectAlts();
		m_isEx03Done = true;
	}

	if (m_isEx03Done)
	{
		ImGui::PlotConfig config;
		config.values.xs = m_stepsizes.data();
		config.values.ys = m_Ex03recordedTimesAverage.data();
		config.values.count = static_cast<int>(m_Ex03recordedTimesAverage.size());
		config.values.color = ImColor(1.f, 0.f, 0.f, 0.5f);
		config.scale.min = -1;
		config.scale.max = 40'000;
		config.tooltip.show = true;
		config.tooltip.format = "x=%.2f, y=%.2f";
		config.grid_x.show = true;
		config.grid_y.show = true;
		config.frame_size = ImVec2(200, 200);
		config.line_thickness = 2.f;

		ImGui::Plot("Trashed the GameObjectAlts", config);
	}

	ImGui::End();
}


void dae::TrashTheCacheComponent::DoEX01TrashingInts()
{
	unsigned int* intArry = new unsigned int[m_arrayAmount] {};
	for (int currentLoop = 0; currentLoop < m_sampleAmount; ++currentLoop)
	{
		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < m_arrayAmount; i += stepsize)
			{
				intArry[i] *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			//Add the elapsed time to the current stepsize
			m_Ex01recordedTimes[stepsize].push_back(static_cast<float>(elapsedTime));
		}
	}

	for (const auto& time : m_Ex01recordedTimes)
	{

		//For every stepsize : 1,2,4,8,... get the amount of elapsedTimes in the vector and remove the highest & lowest values.
		m_Ex01recordedTimesNoHighOrLow = RemoveHighestAndLowest(time.second);

		//Give the average time for that stepsize
		m_Ex01recordedTimesAverage.push_back(GetAverageTime(m_Ex01recordedTimesNoHighOrLow));


		//MicroSeconds divided by 1k become milliseconds.
		//std::cout << "operation took: " << averageTime / 1000.f << " milliseconds \n";
	}
	delete[] intArry;
}


void dae::TrashTheCacheComponent::DoEX02TrashingGameObjects()
{
	GameObject3D* gameObjectArray = new GameObject3D[m_arrayAmount]{};
	for (int currentLoop = 0; currentLoop < m_sampleAmount; ++currentLoop)
	{
		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < m_arrayAmount; i += stepsize)
			{
				gameObjectArray[i].ID *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			m_Ex02recordedTimes[stepsize].push_back(static_cast<float>(elapsedTime));
		}
	}

	for (const auto& time : m_Ex02recordedTimes)
	{
		//For every stepsize : 1,2,4,8,... get the amount of elapsedTimes in the vector and remove the highest & lowest values.
		m_Ex02recordedTimesNoHighOrLow = RemoveHighestAndLowest(time.second);

		//Give the average time for that stepsize
		m_Ex02recordedTimesAverage.push_back(GetAverageTime(m_Ex02recordedTimesNoHighOrLow));


		//MicroSeconds divided by 1k become milliseconds.
		//std::cout << "operation took: " << averageTime / 1000.f << " milliseconds \n";
	}
	delete[] gameObjectArray;
}

void dae::TrashTheCacheComponent::DoEX03TrashingGameObjectAlts()
{
	GameObject3DAlt* gameObjectArray = new GameObject3DAlt[m_arrayAmount]{};
	for (int currentLoop = 0; currentLoop < m_sampleAmount; ++currentLoop)
	{
		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < m_arrayAmount; i += stepsize)
			{
				gameObjectArray[i].ID *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			m_Ex03recordedTimes[stepsize].push_back(static_cast<float>(elapsedTime));
		}
	}

	for (const auto& time : m_Ex03recordedTimes)
	{
		//For every stepsize : 1,2,4,8,... get the amount of elapsedTimes in the vector and remove the highest & lowest values.
		m_Ex03recordedTimesNoHighOrLow = RemoveHighestAndLowest(time.second);

		//Give the average time for that stepsize
		m_Ex03recordedTimesAverage.push_back(GetAverageTime(m_Ex03recordedTimesNoHighOrLow));


		//MicroSeconds divided by 1k become milliseconds.
		//std::cout << "operation took: " << averageTime / 1000.f << " milliseconds \n";
	}
	delete[] gameObjectArray;
}

std::vector<float> dae::TrashTheCacheComponent::RemoveHighestAndLowest(const std::vector<float>& timedVector)
{
	//Gets the vector with the all the elapsed times and remove the highest and lowest
	std::vector<float> newVec = timedVector;


	std::sort(newVec.begin(), newVec.end());
	return std::vector<float>(&newVec[1], &newVec[newVec.size() - 1]);
}

float dae::TrashTheCacheComponent::GetAverageTime(const std::vector<float>& timedVector)
{
	auto vectorAmount = timedVector.size();
	return std::reduce(timedVector.begin(), timedVector.end()) / float(vectorAmount);
}

