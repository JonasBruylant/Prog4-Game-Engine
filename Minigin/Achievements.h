#pragma once
#include <stdint.h>
#include <isteamuserstats.h>
#include <steam_gameserver.h>
#include <vector>

namespace dae {
	#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }

	// Defining our achievements
	enum EAchievements
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_GAIN_500_POINTS = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};

	struct Achievement_t
	{
		Achievement_t(int ID, const char* IDname): 
			m_eAchievementID{ ID }, 
			m_pchAchievementID{ IDname },
			m_bAchieved{false}
		{
		};

		int m_eAchievementID;
		const char* m_pchAchievementID;
		char m_rgchName[128];
		char m_rgchDescription[256];
		bool m_bAchieved;
		int m_iIconImage;
	};

	class SteamAchievements final
	{

	public:
		//It originally took a pointer and a size, with a vector you need to only put in one variable since the vector can get it's own size whenever it needs to
		SteamAchievements(const std::vector<Achievement_t>& Achievements);
		~SteamAchievements();

		bool RequestStats();
		bool SetAchievement(const char* ID);

		STEAM_CALLBACK(SteamAchievements, OnUserStatsReceived, UserStatsReceived_t,
			m_CallbackUserStatsReceived);
		STEAM_CALLBACK(SteamAchievements, OnUserStatsStored, UserStatsStored_t,
			m_CallbackUserStatsStored);
		STEAM_CALLBACK(SteamAchievements, OnAchievementStored,
			UserAchievementStored_t, m_CallbackAchievementStored);


	private:
		uint64 m_iAppID; // Our current AppID
		std::vector<Achievement_t> m_pAchievements; // Achievements data
		int m_iNumAchievements; // The number of Achievements
		bool m_bInitialized; // Have we called Request stats and received the callback?
	};

}