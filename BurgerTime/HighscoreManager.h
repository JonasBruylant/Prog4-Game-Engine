#pragma once
#include "Singleton.h"
#include <string>
namespace dae {

class HighscoreManager final : public Singleton<HighscoreManager>
{
public:
	void Init(const std::string& dataPath);

	void AddToTotalScore(int score) { m_TotalScore += score; }
	void ResetTotalScore() { m_TotalScore = 0; }
	int GetTotalScore() const { return m_TotalScore; }
private:
	friend class Singleton<HighscoreManager>;
	HighscoreManager() = default;
	std::string m_dataPath;

	int m_TotalScore{0};
};
}

