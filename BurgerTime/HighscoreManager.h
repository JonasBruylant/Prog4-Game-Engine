#pragma once
#include "Singleton.h"
#include <string>
#include <vector>


namespace dae {
	class HighscoreDisplayComponent;
class HighscoreManager final : public Singleton<HighscoreManager>
{
public:
	void Init(const std::string& dataPath);
	void Quit();


	void AddToTotalScore(int score) { m_TotalScore += score; }
	void ResetTotalScore() { m_TotalScore = 0; }

	int GetTotalScore() const { return m_TotalScore; }
	std::vector<int>& GetHighScores() { return m_HighScores; }

	void InsertFinalScore();

	void SetHighScoreDisplayComp(HighscoreDisplayComponent* pHighscoreDisplayComp);
	HighscoreDisplayComponent* GetHighScoreDisplayComp();

	void WriteAwayHighScore();
private:
	friend class Singleton<HighscoreManager>;
	HighscoreManager() = default;
	std::string m_dataPath;

	int m_TotalScore{0};
	std::vector<int> m_HighScores{};

	HighscoreDisplayComponent* m_pHighscoreDisplayComponent;
};
}

