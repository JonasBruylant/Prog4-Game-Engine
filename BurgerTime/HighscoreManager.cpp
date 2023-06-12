#include "HighscoreManager.h"
#include "JsonFileReader.h"
#include <algorithm>

void dae::HighscoreManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;
	m_HighScores = JsonFileReader::GetInstance().ReadHighscores(dataPath);
}

void dae::HighscoreManager::Quit()
{
	WriteAwayHighScore();
}

void dae::HighscoreManager::InsertFinalScore()
{
	m_HighScores.emplace_back(m_TotalScore);

	std::sort(m_HighScores.begin(), m_HighScores.end(), std::greater<int>());
}

void dae::HighscoreManager::SetHighScoreDisplayComp(HighscoreDisplayComponent* pHighscoreDisplayComp)
{
	m_pHighscoreDisplayComponent = pHighscoreDisplayComp;
}

dae::HighscoreDisplayComponent* dae::HighscoreManager::GetHighScoreDisplayComp()
{
	return m_pHighscoreDisplayComponent;
}

void dae::HighscoreManager::WriteAwayHighScore()
{
	JsonFileReader::GetInstance().WriteHighscores(m_dataPath, m_HighScores);
}
